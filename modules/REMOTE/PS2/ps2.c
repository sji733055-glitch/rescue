/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-27 18:34:09
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 13:58:46
 * @FilePath: \rescue\modules\REMOTE\PS2\ps2.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-27 18:34:09
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-27 19:26:03
 * @FilePath: \rescue\modules\REMOTE\module_ps2.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ps2.h"
#include "bsp_softspi.h"
#include "module_remote.h"
#include "gpio.h"
#define LOG_TAG "remote_ps2"
#define LOG_LVL LOG_LVL_WARNING
#include "ulog_def.h"

static uint8_t          PS2_cmd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static SOFT_SPI_Device *SPI_device;
static bool             PS2_intialized_flag;

int8_t PS2_Device_Init()
{
    SOFT_SPI_Config config = {.spi_name  = "hspi1",
                              .cs_port   = PS2_CS_GPIO_Port,
                              .cs_pin    = PS2_CS_Pin,
                              .mosi_port = PS2_MOSI_GPIO_Port,
                              .mosi_pin  = PS2_MOSI_Pin,
                              .miso_port = PS2_MISO_GPIO_Port,
                              .miso_pin  = PS2_MISO_Pin,
                              .sck_port  = PS2_CLK_GPIO_Port,
                              .sck_pin   = PS2_CLK_Pin,
                              .delay_us  = 10};
    SPI_device             = BSP_SoftSPI_Device_Init(&config);
    if (SPI_device == NULL)
    {
        LOG_E("SPI Device init failed");
        return -1;
    }
    PS2_intialized_flag = true;
    LOG_I("PS2 Device init success");
    return 0;
}

void PS2_decode(Remote_data_t *data)
{
    static uint8_t rx_buf[9];
    if (BSP_SoftSPI_TransmitReceive(SPI_device, PS2_cmd, rx_buf, 9, 100) != 0)
    {
        return; // 传输失败
    }

    if (rx_buf[2] == 0x5A)
    {
        data->channels[0] = rx_buf[3];
        data->channels[1] = rx_buf[4];
    }
}
