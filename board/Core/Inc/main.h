/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Motor_2PWMA_Pin GPIO_PIN_0
#define Motor_2PWMA_GPIO_Port GPIOA
#define Motor_2PWMB_Pin GPIO_PIN_2
#define Motor_2PWMB_GPIO_Port GPIOA
#define KEY_Pin GPIO_PIN_6
#define KEY_GPIO_Port GPIOA
#define KEY_EXTI_IRQn EXTI9_5_IRQn
#define MPU_INT_Pin GPIO_PIN_10
#define MPU_INT_GPIO_Port GPIOB
#define MPU_INT_EXTI_IRQn EXTI15_10_IRQn
#define Motor_2AIN2_Pin GPIO_PIN_12
#define Motor_2AIN2_GPIO_Port GPIOB
#define Motor_2AIN1_Pin GPIO_PIN_13
#define Motor_2AIN1_GPIO_Port GPIOB
#define Motor_2BIN1_Pin GPIO_PIN_14
#define Motor_2BIN1_GPIO_Port GPIOB
#define Motor_2BIN2_Pin GPIO_PIN_15
#define Motor_2BIN2_GPIO_Port GPIOB
#define Motor_1_AIN2_Pin GPIO_PIN_9
#define Motor_1_AIN2_GPIO_Port GPIOD
#define Motor_1_AIN1_Pin GPIO_PIN_11
#define Motor_1_AIN1_GPIO_Port GPIOD
#define Motor_1BIN1_Pin GPIO_PIN_13
#define Motor_1BIN1_GPIO_Port GPIOD
#define Motor_1BIN2_Pin GPIO_PIN_15
#define Motor_1BIN2_GPIO_Port GPIOD
#define Motor_1PWMA_Pin GPIO_PIN_6
#define Motor_1PWMA_GPIO_Port GPIOC
#define Servo1_Pin GPIO_PIN_7
#define Servo1_GPIO_Port GPIOC
#define Motor_1PWMB_Pin GPIO_PIN_8
#define Motor_1PWMB_GPIO_Port GPIOC
#define MPU_SCL_Pin GPIO_PIN_8
#define MPU_SCL_GPIO_Port GPIOA
#define Servo2_Pin GPIO_PIN_4
#define Servo2_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_9
#define MPU_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
