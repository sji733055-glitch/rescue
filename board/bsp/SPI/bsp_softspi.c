/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-26 20:02:16
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-27 16:02:18
 * @FilePath: \rescue\board\bsp\SPI\bsp_softspi.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "bsp_softspi.h"
#include "bsp_dwt.h"
#include "bsp_def.h"
#include "tx_api.h"
#include "string.h"
#include "gpio.h"
#define LOG_TAG "bsp_spi"
#define LOG_LVL LOG_LVL_WARNING
#include "ulog_def.h"

/* 软件 SPI 设备结构体 */
typedef struct SOFT_SPI_Device
{
    CHAR                   *spi_name;
    GPIO_TypeDef           *cs_port;  // CS 引脚端口
    uint16_t                cs_pin;   // CS 引脚
    float                   delay_us; // 模拟时钟半周期的延时(微秒), 决定通信速率
    struct SOFT_SPI_Device *next;     // 链表指针
} SOFT_SPI_Device;

/* 软件 SPI 总线结构体 */
typedef struct
{
    CHAR         *spi_name;
    GPIO_TypeDef *sck_port;  // SCK 引脚端口
    uint16_t      sck_pin;   // SCK 引脚
    GPIO_TypeDef *mosi_port; // MOSI 引脚端口
    uint16_t      mosi_pin;  // MOSI 引脚
    GPIO_TypeDef *miso_port; // MISO 引脚端口
    uint16_t      miso_pin;  // MISO 引脚

    SOFT_SPI_Device *devices_list; // 挂在该总线上的设备链表
    TX_MUTEX         bus_mutex;    // ThreadX 互斥锁，保证多线程安全
    uint8_t          initialized;  // 总线是否已初始化
} SOFT_SPI_Bus;

static SOFT_SPI_Bus soft_spi_bus_table[SOFT_SPI_BUS_NUM];

/*内部辅助函数*/

void mosi_write(SOFT_SPI_Bus *bus, GPIO_PinState PinState) { HAL_GPIO_WritePin(bus->mosi_port, bus->mosi_pin, PinState); }

uint8_t miso_read(SOFT_SPI_Bus *bus) { return (HAL_GPIO_ReadPin(bus->miso_port, bus->miso_pin) == GPIO_PIN_SET); }

void sck_write(SOFT_SPI_Bus *bus, GPIO_PinState PinState) { HAL_GPIO_WritePin(bus->sck_port, bus->sck_pin, PinState); }

void spi_cs_low(SOFT_SPI_Device *dev)
{
    if (dev->cs_port != 0 && dev->cs_pin != 0)
    {
        HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    }
}
void spi_cs_high(SOFT_SPI_Device *dev)
{
    if (dev->cs_port != 0 && dev->cs_pin != 0)
    {
        HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief 根据引脚组合查找已经注册的软件总线
 */
static SOFT_SPI_Bus *find_soft_bus(CHAR *spi_name)
{
    for (int i = 0; i < SOFT_SPI_BUS_NUM; i++)
    {
        if (soft_spi_bus_table[i].initialized && soft_spi_bus_table[i].spi_name == spi_name)
        {
            return &soft_spi_bus_table[i];
        }
    }
    return NULL;
}

static SOFT_SPI_Bus *transfer_begin(SOFT_SPI_Device *dev, uint32_t timeout)
{
    if (dev == NULL) return NULL;

    SOFT_SPI_Bus *bus = find_soft_bus(dev->spi_name);
    if (bus == NULL)
    {
        LOG_E("Bus not found for hspi=%s", dev->spi_name);
        return NULL;
    }
    /* 仅在任务上下文中获取互斥锁 */
    if (tx_thread_identify() != NULL)
    {
        if (tx_mutex_get(&bus->bus_mutex, timeout) != TX_SUCCESS)
        {
            LOG_E("Failed to acquire bus mutex");
            return NULL;
        }
    }

    /* 2. 拉低片选，开启通信 */
    spi_cs_low(dev);
    BSP_DWT_Delay(dev->delay_us);
    return bus;
}

/**
 * @brief 传输后：拉高 CS → 释放总线锁
 */
static void transfer_end(SOFT_SPI_Bus *bus, SOFT_SPI_Device *dev)
{
    spi_cs_high(dev);
    if (tx_thread_identify() != NULL && bus != NULL)
    {
        tx_mutex_put(&bus->bus_mutex);
    }
}

static uint8_t transfer_once(SOFT_SPI_Bus *bus, SOFT_SPI_Device *dev, const uint8_t *tx_data, uint8_t *rx_data, uint16_t size)
{
    for (uint16_t byte_idx = 0; byte_idx < size; byte_idx++)
    {
        uint8_t byte_to_send    = 0;
        uint8_t byte_to_receive = 0;

        if (tx_data != NULL)
        {
            byte_to_send = tx_data[byte_idx];
        }

        for (int i = 0; i < 8; i++)
        {
            /*写入数据*/
            if (byte_to_send & (1 << i))
            {
                mosi_write(bus, GPIO_PIN_SET);
            }
            else
            {
                mosi_write(bus, GPIO_PIN_RESET);
            }
            sck_write(bus, GPIO_PIN_SET);

            BSP_DWT_Delay(dev->delay_us);

            sck_write(bus, GPIO_PIN_RESET);

            BSP_DWT_Delay(dev->delay_us);

            sck_write(bus, GPIO_PIN_SET);
            /*读取数据*/
            if (miso_read(bus))
            {
                byte_to_receive |= (1 << i);
            }
        }

        if (rx_data != NULL)
        {
            rx_data[byte_idx] = byte_to_receive;
        }

        BSP_DWT_Delay(16);
    }

    return 0; // 成功
}
/**
 * @brief 设备注册初始化（自动构建总线与多设备链表）
 */
SOFT_SPI_Device *BSP_SoftSPI_Device_Init(SOFT_SPI_Config *config)
{
    if (config == NULL) return NULL;
    CHAR         *spi_name = config->spi_name;
    SOFT_SPI_Bus *bus      = find_soft_bus(config->spi_name);

    /* 1. 如果该引脚组合对应的总线还没建立，则建立新总线 */
    if (bus == NULL)
    {
        for (int i = 0; i < SOFT_SPI_BUS_NUM; i++)
        {
            if (soft_spi_bus_table[i].initialized == 0)
            {
                bus = &soft_spi_bus_table[i];
                break;
            }
        }
    }
    if (bus == NULL)
    {
        LOG_E("No free bus slot (max %d)", SOFT_SPI_BUS_NUM);
        return NULL; // 没有空闲的总线槽位了
    }

    /* 首次建立总线，初始化总线互斥锁 */
    if (bus->initialized == 0)
    {
        bus->spi_name     = config->spi_name;
        bus->sck_port     = (GPIO_TypeDef *)config->sck_port;
        bus->sck_pin      = config->sck_pin;
        bus->mosi_port    = (GPIO_TypeDef *)config->mosi_port;
        bus->mosi_pin     = config->mosi_pin;
        bus->miso_port    = (GPIO_TypeDef *)config->miso_port;
        bus->miso_pin     = config->miso_pin;
        bus->devices_list = NULL;

        if (tx_mutex_create(&bus->bus_mutex, "soft_spi_mtx", TX_INHERIT) != TX_SUCCESS)
        {
            return NULL;
        }

        // 确保时钟线初始状态为高
        sck_write(bus, GPIO_PIN_SET);
        bus->initialized = 1;
        LOG_I("Bus %d initialized (hspi=%s)", (int)(bus - soft_spi_bus_table), spi_name);
    } 

    /* 2. 为新设备分配内存 */
    SOFT_SPI_Device *dev = malloc(sizeof(SOFT_SPI_Device));
    if (dev == NULL) return NULL;
    dev->spi_name = config->spi_name;
    dev->cs_port  = (GPIO_TypeDef *)config->cs_port;
    dev->cs_pin   = config->cs_pin;
    dev->delay_us = (config->delay_us == 0) ? 10 : config->delay_us; // 默认10us延迟

    // 确保片选引脚初始状态为高（不选中）
    spi_cs_high(dev);
    /* 3. 将设备挂载到该总线的设备链表中 */
    dev->next         = bus->devices_list;
    bus->devices_list = dev;

    return dev;
}

/**
 * @brief 线程安全的软件 SPI 复合传输函数
 */
uint8_t BSP_SoftSPI_TransmitReceive(SOFT_SPI_Device *dev, const uint8_t *tx_data, uint8_t *rx_data, uint16_t size, uint32_t timeout)
{
    if (!dev || !tx_data || !rx_data || size == 0) return 1;

    SOFT_SPI_Bus *bus = transfer_begin(dev, timeout);
    if (bus == NULL) return 1;

    uint8_t ret = transfer_once(bus, dev, tx_data, rx_data, size);
    transfer_end(bus, dev);
    return ret;
}