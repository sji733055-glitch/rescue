#ifndef __BSP_SOFTSPI_H__
#define __BSP_SOFTSPI_H__
#include <stdint.h>
#include "tx_api.h"
#include "main.h"

typedef struct SOFT_SPI_Device SOFT_SPI_Device;

#define SOFT_SPI_BUS_NUM 2

/* 初始化配置结构体 */
typedef struct
{
    CHAR               *spi_name;

    GPIO_TypeDef       *sck_port;
    uint16_t            sck_pin;
    GPIO_TypeDef       *mosi_port;
    uint16_t            mosi_pin;
    GPIO_TypeDef       *miso_port;
    uint16_t            miso_pin;
    
    GPIO_TypeDef       *cs_port;
    uint16_t            cs_pin;
    uint32_t            delay_us;
} SOFT_SPI_Config;
/**
 * @brief  SPI设备初始化
 * @param  {SOFT_SPI_Config*}config
 * @retval {SOFT_SPI_Device*} 成功返回设备实例指针，失败返回NULL
 */
SOFT_SPI_Device *BSP_SoftSPI_Device_Init(SOFT_SPI_Config *config);
/**
 * @brief  线程安全的软件模拟 SPI 全双工多字节流传输函数（专为 PS2 手柄时序优化）
 * @note   1. 本函数支持全双工通信，在时钟低电平后半段（上升沿前）精确采样，并内置字节间喘息延时。
 * 2. 具备指针安全防护：tx_data 为 NULL 时默认盲发 0x00 提供时钟；rx_data 为 NULL 时丢弃接收数据。
 * 3. 自动识别上下文：在 ThreadX 任务中运行时自动加锁确保多线程安全，在中断/初始化中调用自动免锁。
 * @param  dev:     指向已通过 BSP_SoftSPI_Device_Init 注册的软件 SPI 设备句柄 (不透明指针)
 * @param  tx_data: 指向发送数据缓冲区的首地址。若仅需接收数据，可传入 NULL
 * @param  rx_data: 指向接收数据缓冲区的首地址。若仅需发送数据，可传入 NULL
 * @param  size:    本次通信连续传输的“数据字节总数（Bytes）”，必须与缓冲区开辟大小匹配
 * @param  timeout: 获取总线互斥锁的最大等待时间（单位: 硬件 Tick 数），若在中断中此参数失效
 * @retval 0:       传输成功完成
 * @retval 1:       传输失败（可能由于入参非法、未找到对应总线或 ThreadX 锁获取超时）
 */
uint8_t BSP_SoftSPI_TransmitReceive(SOFT_SPI_Device *dev, const uint8_t *tx_data, uint8_t *rx_data, uint16_t size, uint32_t timeout);
#endif /* __BSP_SOFTSPI_H__ */