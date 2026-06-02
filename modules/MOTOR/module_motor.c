/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-29 13:54:40
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 14:08:02
 * @FilePath: \rescue\modules\MOTOR\module_motor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "module_motor.h"
#include "module_config.h"
#include "tx_api.h"
#include "bsp_def.h"

#define LOG_LVL LOG_LVL_INFO
#define LOG_TAG "module_motor"
#include "ulog_def.h"

static TX_THREAD                  motor_thread;
APPS_STACK_SECTION static uint8_t motor_thread_stack[MOTOR_TASK_STACK_SIZE];
static TX_THREAD                  motor_decode_thread;
APPS_STACK_SECTION static uint8_t motor_decode_thread_stack[MOTOR_DECODE_TASK_STACK_SIZE];

void motor_task_entry(ULONG arg)
{
    (void)arg;

    while (1)
    {
        Motor_UpdateAll();
        tx_thread_sleep(2);
    }
}

void motor_decode_task_entry(ULONG arg)
{
    (void)arg;
    while (1)
    {
        Motor_FeedbackUpdateAll();
    }
}

void Module_Motor_init(void)
{
    UINT status = tx_thread_create(&motor_thread, "motor", motor_task_entry, 0, motor_thread_stack, MOTOR_TASK_STACK_SIZE, MOTOR_TASK_PRIORITY,
                                   MOTOR_TASK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        LOG_E("Failed to create motor thread");
    }
    status = tx_thread_create(&motor_decode_thread, "motor_decode", motor_decode_task_entry, 0, motor_decode_thread_stack,
                              MOTOR_DECODE_TASK_STACK_SIZE, MOTOR_DECODE_TASK_PRIORITY, MOTOR_DECODE_TASK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
    LOG_I("Motor module initialized");
}