/**
  ******************************************************************************
  * @file    py32f031_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief Initialize global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief Initialize LCD MSP
  */
void HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable LSI */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI; /* Select LSI oscillator */
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;                   /* Enable LSI */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;             /* Keep PLL configuration unchanged */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Select LSI as the clock source for LCD */
  HAL_RCCEx_SetLSCSource(RCC_LSCSOURCE_LSI);
  
  /* Enable LCD clock */
  __HAL_RCC_LCD_CLK_ENABLE();
  
  /* Enable LCD pin clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Enable SYSCFG clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  
  /* Configure LCD pins */
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  GPIO_InitStructure.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* COM0 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* COM1 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* COM2 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* COM3 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG0 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);/* SEG1 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG2 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG3 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG4 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG5 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG6 */
  
  GPIO_InitStructure.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);/* SEG7 */
  
  /* IO PORTA and PORTB PAD_ANA2 enable */
  HAL_SYSCFG_EnableGPIOAnalog2(GPIOA, \
     GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
     GPIO_PIN_8 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | \
     GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);
  HAL_SYSCFG_EnableGPIOAnalog2(GPIOB, GPIO_PIN_2);
}
/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
