/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-30 11:36:36
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-06-02 19:35:33
 * @FilePath: \rescue\APP\car_control.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "car_control.h"
#include "motor_DC.h"
#include "module_remote.h"
#include "module_motor.h"
#include "app_config.h"
#include "bsp_def.h"
#include "tx_api.h"
#include "tim.h"
#define LOG_TAG "app_control"
#define LOG_LVL LOG_LVL_INFO
#include "ulog_def.h"

static TX_THREAD                  control_thread;
APPS_STACK_SECTION static uint8_t control_task_stack[CONTROL_TASK_STACK_SIZE];

static DC_Motor_t *Motor_list[4];

static void control_task_entry(ULONG thread_input)
{
    while (1)
    {
        uint8_t current_channel = Module_Get_Channels(1);
        if (current_channel == 0xFF)
        {
            DC_Motor_control(&Motor_list[0]->base, 0);
            DC_Motor_control(&Motor_list[1]->base, 0);
            DC_Motor_control(&Motor_list[2]->base, 0);
            DC_Motor_control(&Motor_list[3]->base, 0);
        }
        else if (current_channel == 0xBF)
        {
            DC_Motor_control(&Motor_list[0]->base, -500);
            DC_Motor_control(&Motor_list[1]->base, 500);
            DC_Motor_control(&Motor_list[2]->base, 500);
            DC_Motor_control(&Motor_list[3]->base, -500);
        }
        else if (current_channel == 0xEF)
        {
            DC_Motor_control(&Motor_list[0]->base, 500);
            DC_Motor_control(&Motor_list[1]->base, -500);
            DC_Motor_control(&Motor_list[2]->base, -500);
            DC_Motor_control(&Motor_list[3]->base, 500);
        }

        tx_thread_sleep(2);
    }
}
static void chassis_init()
{
    Motor_Init_Config_s motor_2_config = {
        .transport_config.pwm = {.htim = &htim2, .Channel = TIM_CHANNEL_3, .Mode = PWM_MODE_IT, .dutyx10 = 0},
        .transport            = MOTOR_TRANSPORT_PWM,
        .motor_init_info      = {.motor_type = DC_Gear_MOTOR},

    };
    Motor_list[0] = Motor_DC_Init(&motor_2_config, Motor_2BIN1_GPIO_Port, Motor_2BIN1_Pin, Motor_2BIN2_GPIO_Port, Motor_2BIN2_Pin);

    Motor_Init_Config_s motor_1_config = {
        .transport_config.pwm = {.htim = &htim2, .Channel = TIM_CHANNEL_1, .Mode = PWM_MODE_IT, .dutyx10 = 0},
        .transport            = MOTOR_TRANSPORT_PWM,
        .motor_init_info      = {.motor_type = DC_Gear_MOTOR},

    };
    Motor_list[1] = Motor_DC_Init(&motor_1_config, Motor_2AIN1_GPIO_Port, Motor_2AIN1_Pin, Motor_2AIN2_GPIO_Port, Motor_2AIN2_Pin);


    Motor_Init_Config_s motor_3_config = {
        .transport_config.pwm = {.htim = &htim8, .Channel = TIM_CHANNEL_1, .Mode = PWM_MODE_IT, .dutyx10 = 0},
        .transport            = MOTOR_TRANSPORT_PWM,
        .motor_init_info      = {.motor_type = DC_Gear_MOTOR},

    };
    Motor_list[2] = Motor_DC_Init(&motor_3_config, Motor_1_AIN1_GPIO_Port, Motor_1_AIN1_Pin, Motor_1_AIN2_GPIO_Port, Motor_1_AIN2_Pin);

    Motor_Init_Config_s motor_4_config = {
        .transport_config.pwm = {.htim = &htim8, .Channel = TIM_CHANNEL_3, .Mode = PWM_MODE_IT, .dutyx10 = 0},
        .transport            = MOTOR_TRANSPORT_PWM,
        .motor_init_info      = {.motor_type = DC_Gear_MOTOR},

    };
    Motor_list[3] = Motor_DC_Init(&motor_4_config, Motor_1BIN1_GPIO_Port, Motor_1BIN1_Pin, Motor_1BIN2_GPIO_Port, Motor_1BIN2_Pin);
}

void control_init()
{
    chassis_init();
    UINT status = tx_thread_create(&control_thread, "control_task", control_task_entry, 0, control_task_stack, CONTROL_TASK_STACK_SIZE,
                                   CONTROL_TASK_PRIORITY, CONTROL_TASK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        LOG_E("control_task created failed");
        return;
    }
}