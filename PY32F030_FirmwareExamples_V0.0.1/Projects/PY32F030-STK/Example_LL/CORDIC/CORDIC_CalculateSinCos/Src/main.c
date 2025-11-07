/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
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
#include "py32f031xx_ll_Start_Kit.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define PI   3.14159265358979323846
#define THETA (PI/6.0)
#define POWER_2_31 2147483648.0  /* To the 31st power of 2 */

/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_ConfigCordic(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval int
  */
int main(void)
{
  int32_t sinResult = 0;
  int32_t cosResult = 0;
  
  /* Enable SYSCFG and PWR clock */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Configure system clock */
  APP_SystemClockConfig();

  /* Initialize USART (for printf use) */
  BSP_USART_Config();

  /* Configure CORDIC */
  APP_ConfigCordic();
  
  /* Set input THETA */
  LL_CORDIC_SetTheta(CORDIC, (int32_t)((THETA / PI) * POWER_2_31));
  
  /* Start calculation */
  LL_CORDIC_SetStart(CORDIC);
  
  /* Waiting for calculation to complete */
  while (LL_CORDIC_IsActiveFlag_CCFF(CORDIC) == 1)
  {
  }
  
  /* Clear calculation completion flag */
  LL_CORDIC_ClearFlag_CCFF(CORDIC);
  
  /* Get calculation results */
  sinResult = LL_CORDIC_GetSin(CORDIC);
  cosResult = LL_CORDIC_GetCos(CORDIC);
  
  /* Print calculate value */
  printf("sin value = %lf\r\n", (sinResult / POWER_2_31));
  printf("cos value = %lf\r\n", (cosResult / POWER_2_31));

  while (1)
  {
    
  }
}

/**
  * @brief  Configure CORDIC.
  * @param  None
  * @retval None
  */
static void APP_ConfigCordic(void)
{
  /* Enable CORDIC clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CORDIC);
  
  /* Set iterations */
  LL_CORDIC_SetIterations(CORDIC, 24);
  
  /* Set function */
  LL_CORDIC_SetFunction(CORDIC, LL_CORDIC_FUNCTION_SIN_COS);
  
  /* Set start mode */
  LL_CORDIC_SetStartMode(CORDIC, LL_CORDIC_STARTMODE_NOT_AUTO);
  
  /* Set input data width */
  LL_CORDIC_SetInSize(CORDIC, LL_CORDIC_INSIZE_32BITS);
  
  /* Set output data width */
  LL_CORDIC_SetOutSize(CORDIC, LL_CORDIC_OUTSIZE_32BITS);
}

/**
  * @brief  Configure system clock
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  /* Enable HSI */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Set AHB prescaler: HCLK = SYSCLK */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Select HSISYS as system clock source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /* Set APB prescaler: PCLK = HCLK */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(8000000);

  /* Update the SystemCoreClock global variable(which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(8000000);
}

/**
  * @brief  Error executing function.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Users can add their own printing information as needed,
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
