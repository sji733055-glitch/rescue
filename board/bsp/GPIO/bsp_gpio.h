/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-26 15:41:06
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-26 16:58:39
 * @FilePath: \rescue\board\bsp\GPIO\bsp_gpio.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
