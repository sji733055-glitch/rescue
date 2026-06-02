/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-27 19:55:55
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 20:02:17
 * @FilePath: \rescue\modules\REMOTE\module_remote.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "module_remote.h"
#include "tx_api.h"
#include "module_config.h"
#include "bsp_def.h"

#include "string.h"

#include "ps2.h"
#define LOG_TAG "module_remote"
#define LOG_LVL LOG_LVL_INFO
#include "ulog_def.h"

static Remote_data_t              remote_data;
static TX_THREAD                  remote_decode_task;
APPS_STACK_SECTION static uint8_t remote_decode_stack[REMOTE_TASK_STACK_SIZE];
static volatile bool              g_initialized; /* 模块初始化完成标志 */

static void remote_task_entry(ULONG arg)
{
    (void)arg;

    while (1)
    {
        PS2_decode(&remote_data);
        tx_thread_sleep(2);
    }
}

void Module_Remote_init(void)
{
    if (g_initialized) return;

    memset(&remote_data, 0, sizeof(Remote_data_t));

    if (PS2_Device_Init() != TX_SUCCESS)
    {
        LOG_E("module_ps2 init failed");
    }

    UINT status = tx_thread_create(&remote_decode_task, "remote_task", remote_task_entry, 0, remote_decode_stack, REMOTE_TASK_STACK_SIZE,
                                   REMOTE_TASK_PRIORITY, REMOTE_TASK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        LOG_E("Failed to create remote task");
        return;
    }
    g_initialized = true;
    LOG_I("remote_task init successfully!");
}

Remote_data_t *Module_Get_Data(void)
{
    if (!g_initialized) return NULL;

    return &remote_data;
}

uint16_t Module_Get_Channels(uint8_t ch)
{
    if(!g_initialized || ch<1 || ch>3) return 0;

    return remote_data.channels[ch-1];
}