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
#define Delay             40*3

/* Private variables ---------------------------------------------------------*/
LPTIM_HandleTypeDef       LPTIMConf = {0};
EXTI_HandleTypeDef        ExtiHandle = {0};
EXTI_ConfigTypeDef        ExtiCfg = {0};
__IO uint32_t             RatioNops = 0;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_RCCOscConfig(void);
static void APP_DelayNops(uint32_t udelay);
static void APP_GpioConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */ 
  HAL_Init();
  
  /* Configure RCCOSC */
  APP_RCCOscConfig();
  
  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);
  
  APP_GpioConfig();
  
  /* Initialize Button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Initialize LPTIM */
  LPTIMConf.Instance = LPTIM1;                        /* LPTIM1 */
  LPTIMConf.Init.Prescaler = LPTIM_PRESCALER_DIV128;  /* DIV 128 */
  LPTIMConf.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE; /* UPDATE IMMEDIATE */
  /* Initialize LPTIM */
  if (HAL_LPTIM_Init(&LPTIMConf) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Configure Event Wakeup */
  ExtiCfg.Line = EXTI_LINE_29;
  ExtiCfg.Mode = EXTI_MODE_EVENT;
  HAL_EXTI_SetConfigLine(&ExtiHandle, &ExtiCfg);
 
  /* LED ON*/
  BSP_LED_On(LED_GREEN);
  
  /* Wait for Button */
  while (BSP_PB_GetState(BUTTON_USER) != 0)
  {
  }

  /* LED OFF */
  BSP_LED_Off(LED_GREEN);
  
  /* Calculate the value required for a delay of macro-defined(Delay) us*/
  RatioNops = Delay * (SystemCoreClock / 1000000U) / 4;

  while (1)
  {
    /* LPTIM must be disabled to restore internal state before next time enter stop mode */
    __HAL_LPTIM_DISABLE(&LPTIMConf);
    
    /* Wait at least three LSI times for the completion of the disable operation */
    APP_DelayNops(RatioNops); 
                               
    /* Suspend Systick */
    HAL_SuspendTick();
    
    /* Configure LPTIM for once mode and enable interrupt */
    HAL_LPTIM_SetOnce_Start_IT(&LPTIMConf, 51);
    
    /* Enter Stop Mode and Wakeup by WFE */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE); 
    
    /* Resume Systick */
    HAL_ResumeTick();

    /* The Autoreload match flag must be cleared before entering stop mode the next time */
    if(__HAL_LPTIM_GET_FLAG(&LPTIMConf, LPTIM_FLAG_ARRM) != RESET)
    {    
      __HAL_LPTIM_CLEAR_FLAG(&LPTIMConf, LPTIM_FLAG_ARRM);
      /* Toggle LED */
      BSP_LED_Toggle(LED_GREEN);
    }  
    
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
  }
}

/**
  * @brief   Clock configuration function
  * @param   None
  * @retval  None
  */
static void APP_RCCOscConfig(void)
{
  RCC_OscInitTypeDef OSCINIT = {0};
  RCC_PeriphCLKInitTypeDef LPTIM_RCC = {0};

  /* LSI clock configuration */
  OSCINIT.OscillatorType = RCC_OSCILLATORTYPE_LSI;  /* Set the oscillator type to LSI */
  OSCINIT.LSIState = RCC_LSI_ON;                    /* Enable LSI */
  OSCINIT.PLL.PLLState = RCC_PLL_NONE;              /* PLL configuration unchanged */
  /*OSCINIT.PLL.PLLSource = RCC_PLLSOURCE_HSI;*/
  /*OSCINIT.PLL.PLLMUL = RCC_PLL_MUL2;*/
  /* Clock initialization */
  if (HAL_RCC_OscConfig(&OSCINIT) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* LPTIM clock configuration */
  LPTIM_RCC.PeriphClockSelection = RCC_PERIPHCLK_LPTIM;     /* Select peripheral clock: LPTIM */
  LPTIM_RCC.LptimClockSelection = RCC_LPTIMCLKSOURCE_LSI;   /* Select LPTIM clock source: LSI */
  /* Peripheral clock initialization */
  if (HAL_RCCEx_PeriphCLKConfig(&LPTIM_RCC) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Enable LPTIM clock */
  __HAL_RCC_LPTIM_CLK_ENABLE();
}

/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
static void APP_GpioConfig(void)
{
  /* Configuration pins */
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();                  /* Enable the GPIO clock*/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;   /* GPIO mode is OutputPP */
  GPIO_InitStruct.Pull  = GPIO_PULLUP;           /* pull up */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  /* The speed is high */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  Delayed by NOPS
  * @param  None
  * @retval None
  */
static void APP_DelayNops(uint32_t Nops)
{
  for(uint32_t i=0; i<Nops;i++)
  {
    __NOP();
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
