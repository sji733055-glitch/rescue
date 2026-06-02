/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-29 10:32:43
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 14:05:59
 * @FilePath: \rescue\modules\MOTOR\motor_base.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "motor_base.h"

static Motor_Base *g_motor_list = NULL;

void Motor_Register(Motor_Base *motor)
{
    if (motor == NULL) return;
    motor->next = g_motor_list;
    g_motor_list = motor;
}

void Motor_UpdateAll(void)
{
    for (Motor_Base *motor = g_motor_list; motor; motor = motor->next)
    {
        if (motor->ControlAndSend) motor->ControlAndSend(motor);
    }
}

void Motor_FeedbackUpdateAll(void)
{
    for (Motor_Base *motor = g_motor_list; motor; motor = motor->next)
    {
        if (motor->FeedbackUpdate) motor->FeedbackUpdate(motor);
    }
}
