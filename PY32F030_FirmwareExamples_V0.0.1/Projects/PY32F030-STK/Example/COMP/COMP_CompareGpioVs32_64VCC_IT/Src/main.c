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

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef  hcomp1;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();   

  /* System clock configuration */
  APP_SystemClockConfig();   
  
  BSP_LED_Init(LED_GREEN);  

  hcomp1.Instance = COMP1;                                              /* COMP1 */
  hcomp1.Init.InputMinus      = COMP_INPUT_MINUS_IO6;                   /* Negative input is Vrefcmp  */
  hcomp1.Init.InputPlus       = COMP_INPUT_PLUS_IO2;                    /* Positive input selection is PA1 */
  hcomp1.Init.OutputPol       = COMP_OUTPUTPOL_NONINVERTED;             /* COMP1 polarity Sexual selection is not reverse */
  hcomp1.Init.Mode            = COMP_POWERMODE_HIGHSPEED;               /* COMP1 power consumption mode is selected as High speed mode */
  hcomp1.Init.Hysteresis      = COMP_HYSTERESIS_DISABLE;                /* Hysteresis function off */
  hcomp1.Init.WindowMode      = COMP_WINDOWMODE_DISABLE;                /* Window Mode Off */
  hcomp1.Init.TriggerMode     = COMP_TRIGGERMODE_IT_RISING_FALLING;     /* COMP1 external Trigger on Rising/Falling */
  hcomp1.Init.VrefDiv         = COMP_VREFCMP_DIV_32_64VREFCMP;          /* Vrefcmp div 32/64 */
  hcomp1.Init.VrefSrc         = COMP_VREFCMP_SOURCE_VCC;                /* Vrefcmp select VCC */
  hcomp1.Init.DigitalFilter   = 0;                                      /* Disable DigitalFilter */

  /* COMP1 initialization */
  if (HAL_COMP_Init(&hcomp1) != HAL_OK)                                 
  {
    APP_ErrorHandler();
  }
  /* COMP1 Start */
  HAL_COMP_Start(&hcomp1); 

  while (1)
  {
  }
}

/**
  * @brief  Comparator interrupt callback function
  * @param  hcomp：COMP Handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  if(HAL_COMP_GetOutputLevel(&hcomp1) == 1)
  {
    BSP_LED_On(LED_GREEN);
  }
  else
  {
    BSP_LED_Off(LED_GREEN);
  }  
}

/**
  * @brief  System clock configuration function
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillator HSE, HSI, LSI, LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI 1 frequency division */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;  /* Configure HSI clock 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* Close HSE */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Close LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* Close LSE */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* Close PLL */
  /*RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;*/
  /*RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;*/
  /* Configure oscillator */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure clock HCLK, SYSCLK, PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSISYS; /* Select HSISYS as the system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB clock 1 division */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB clock 1 division */
  /* Configure clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
