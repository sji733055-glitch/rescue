#ifndef __MOTOR_DC_H__
#define __MOTOR_DC_H__

#include "motor_base.h"
#include "bsp_pwm.h"
#include "user_lib.h"
#include "gpio.h"
#include <stdint.h>

typedef struct {
    Motor_Base base;
    PWM_Device *pwm_device;

    GPIO_TypeDef *in1_pin_port;
    uint16_t  in1_pin;

    GPIO_TypeDef *in2_pin_port;
    uint16_t   in2_pin;

    GPIO_TypeDef *exti_port;
    uint16_t  exti_pin;

    GPIO_TypeDef *gpio_port;
    uint16_t gpio_pin;

    volatile int32_t encoder_cnt;
    int32_t last_encoder_cnt;

    first_order_filter_type_t filter;
    
} DC_Motor_t;

DC_Motor_t *Motor_DC_Init(Motor_Init_Config_s *config, GPIO_TypeDef *in1_port, uint16_t in1_pin, GPIO_TypeDef *in2_port, uint16_t in2_pin);
void Motor_DC_Start(DC_Motor_t *motor);
void DC_Motor_control(Motor_Base *base,int16_t duty);

void Motor_DC_Stop(DC_Motor_t *motor);

/*ref单位(rad/s)*/
void Motor_DC_SetRef(DC_Motor_t *motor, float ref);

#endif /* __MOTOR_DC_H__ */