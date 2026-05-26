#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include <stdint.h>

/* 最大EXTI回调数量 */
#define MAX_EXTI_CALLBACKS 8

/**
 * @brief 注册外部中断回调函数
 * @param pin GPIO引脚号 (0-15)
 * @param callback 回调函数
 * @return uint8_t 已注册回调的索引，失败返回0xFF
 */
uint8_t BSP_GPIO_EXTI_Register(uint16_t pin, void (*callback)(void));

/**
 * @brief 注销外部中断回调函数
 * @param index BSP_GPIO_EXTI_Register返回的回调索引
 */
void BSP_GPIO_EXTI_Unregister(uint8_t index);

#endif // _BSP_GPIO_H_
