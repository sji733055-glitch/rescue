/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-29 14:26:35
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 19:54:37
 * @FilePath: \rescue\modules\MOTOR\DC_Gear\motor_DC.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "motor_DC.h"
#include "bsp_def.h"
#include "bsp_gpio.h"
#include "bsp_dwt.h"
#include "string.h"
#include "user_lib.h"
#include "stdlib.h"
#include "gpio.h"

#define LOG_TAG "motor_DC"
#define LOG_LVL LOG_LVL_INFO
#include "ulog_def.h"

static void Motor_DC_feedbackUpdate(Motor_Base *base)
{
    DC_Motor_t *motor = (DC_Motor_t *)base;

    base->dt = BSP_DWT_GetDeltaT(&base->DWT_CNT);

    int32_t current_cnt = motor->encoder_cnt;

    int32_t delta_cnt       = current_cnt - motor->last_encoder_cnt;
    motor->last_encoder_cnt = current_cnt;

    const float PULSES_PER_REV = 448.8f;

    float raw_speed_rpm = ((float)delta_cnt / PULSES_PER_REV) / base->dt * 60.0f;

    /* 4. 一阶低通滤波器
     * 针对 1:20.4 减速比在中低速下脉冲数稀疏的特性进行数学去噪 */
    const float ALPHA       = 0.18f;
    base->measure.speed_rpm = (ALPHA * raw_speed_rpm) + ((1.0f - ALPHA) * base->measure.speed_rpm);

    // 换算标准物理角速度 (rad/s)
    base->measure.speed_rad = base->measure.speed_rpm * (2.0f * 3.1415926f / 60.0f);
}

// static void DC_Motor_EXTI_callback(void *arg)
// {
//     DC_Motor_t *Motor = (DC_Motor_t *)arg;
//     if (!Motor) return;

//     uint8_t a_val = HAL_GPIO_ReadPin(Motor->exti_port, Motor->exti_pin);
//     uint8_t b_val = HAL_GPIO_ReadPin(Motor->gpio_port, Motor->gpio_pin);

//     if (a_val ^ b_val)
//         Motor->encoder_cnt++;
//     else
//         Motor->encoder_cnt--;
// }
// static float CalculatePIDOutput(DC_Motor_t *motor)
// {
//     float pid_measure, pid_ref;

//     pid_ref = motor->base.controller.ref;
//     if (motor->base.setting.motor_reverse_flag == 1) pid_ref *= -1;

//     /* 位置环 */
//     if (motor->base.setting.loop_type & ANGLE_LOOP)
//     {
//         pid_measure = (motor->base.setting.angle_feedback_source == 1 && motor->base.controller.other_angle_feedback_ptr)
//                           ? *motor->base.controller.other_angle_feedback_ptr
//                           : motor->base.measure.total_angle;

//         if (motor->base.setting.feedback_reverse_flag == 1) pid_measure *= -1;
//         pid_ref = PIDCalculate(&motor->base.controller.angle_PID, pid_measure, pid_ref);
//     }

//     /* 速度环 */
//     if (motor->base.setting.loop_type & SPEED_LOOP)
//     {
//         pid_measure = (motor->base.setting.speed_feedback_source == 1 && motor->base.controller.other_speed_feedback_ptr)
//                           ? *motor->base.controller.other_speed_feedback_ptr
//                           : motor->base.measure.speed_rad;

//         if (motor->base.setting.feedback_reverse_flag == 1) pid_measure *= -1;
//         pid_ref = PIDCalculate(&motor->base.controller.speed_PID, pid_measure, pid_ref);
//     }

//     return pid_ref;
// }

static void DC_ControlAndSend(Motor_Base *base)
{
    DC_Motor_t *motor = (DC_Motor_t *)base;
    if (!motor) return;
    if (!base->setting.enableflag) return;

    // float torque = CalculatePIDOutput(motor);

    int32_t output = (int32_t)motor->base.controller.output;

    VAL_LIMIT(output, 1000, -1000);

    if (output > 0)
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_RESET);
    }
    else if (output < 0)
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_RESET);
    }
    BSP_PWM_SetDutyCycle(motor->pwm_device, abs(output));
}

DC_Motor_t *Motor_DC_Init(Motor_Init_Config_s *config, GPIO_TypeDef *in1_port, uint16_t in1_pin, GPIO_TypeDef *in2_port, uint16_t in2_pin)
//   GPIO_TypeDef *exti_port, uint16_t exti_pin, GPIO_TypeDef *gpio_port, uint16_t gpio_pin)
{
    DC_Motor_t *motor = NULL;
    BSP_MEM_ALLOC_WAIT(motor, sizeof(DC_Motor_t), TX_NO_WAIT);
    if (!motor)
    {
        LOG_E("Fail to allocate memory for DC motor");
    }
    memset(motor, 0, sizeof(*motor));

    motor->base.info      = config->motor_init_info;
    motor->base.setting   = config->setting_init_config;
    motor->base.transport = config->transport;
    motor->base.type      = config->motor_init_info.motor_type;
    motor->in1_pin_port   = in1_port;
    motor->in1_pin        = in1_pin;
    motor->in2_pin_port   = in2_port;
    motor->in2_pin        = in2_pin;
    // motor->exti_port      = exti_port;
    // motor->exti_pin       = exti_pin;
    // motor->gpio_port      = gpio_port;
    // motor->gpio_pin       = gpio_pin;
    motor->encoder_cnt  = 0;
    PWM_Device *pwm_dev = BSP_PWM_Device_Init(&config->transport_config.pwm);

    if (!pwm_dev)
    {
        LOG_E("Failed to initialize PWM device");
        BSP_MEM_FREE(motor);
        return NULL;
    }
    // BSP_GPIO_EXTI_Register(motor->exti_pin, DC_Motor_EXTI_callback, motor);

    motor->pwm_device         = pwm_dev;
    motor->base.transport_dev = pwm_dev;

    BSP_PWM_Start(pwm_dev, NULL, 0);
    motor->base.ControlAndSend = DC_ControlAndSend;
    motor->base.FeedbackUpdate = Motor_DC_feedbackUpdate;
    Motor_Register(&motor->base);
    return motor;
}
void DC_Motor_control(Motor_Base *base, int16_t duty)
{
    DC_Motor_t *motor = (DC_Motor_t *)base;

    uint16_t abs_duty;
    if (duty > 0)
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_RESET);
        abs_duty = duty;
    }
    else if (duty < 0)
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_SET);
        abs_duty = (uint16_t)-duty;
    }
    else
    {
        HAL_GPIO_WritePin(motor->in1_pin_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_pin_port, motor->in2_pin, GPIO_PIN_RESET);
        abs_duty = 0;
    }
    BSP_PWM_SetDutyCycle(motor->pwm_device, abs_duty);
}
void Motor_DC_Start(DC_Motor_t *motor) { motor->base.setting.enableflag = 1; }

void Motor_DC_Stop(DC_Motor_t *motor) { motor->base.setting.enableflag = 0; }

void Motor_DC_SetRef(DC_Motor_t *motor, float ref) { motor->base.controller.ref = ref; }
