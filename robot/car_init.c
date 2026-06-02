/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-30 10:50:00
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 19:27:06
 * @FilePath: \rescue\robot\car_init.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "car_init.h"
#include "tx_api.h"
#include "utils_init.h"
#include "bsp_init.h"
#include "module_init.h"
#include "bsp_def.h"
#include "app_init.h"

#define LOG_LVL LOG_LVL_INFO
#define LOG_TAG "Car Init"
#include "ulog_def.h"


#define TX_APP_MEM_POOL_SIZE (20 * 1024) // 20KB 应用内存池大小
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
TX_BYTE_POOL tx_app_byte_pool;

void car_init()
{   
    if (tx_byte_pool_create(&tx_app_byte_pool, "Tx App memory pool", tx_byte_pool_buffer, TX_APP_MEM_POOL_SIZE) != TX_SUCCESS)
    {
        LOG_E("Failed to create byte pool");
        while (1)
        {
        };
    }


    UTILS_Init();
    BSP_Init();
    MODULE_Init();
    app_init();
    LOG_E("CAR Init finish");
}

