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
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_McoGpioConfig(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval int
  */
int main(void)
{
  /* Initialize GPIO output */
  APP_McoGpioConfig();

  /* MCO (Microcontroller Clock Output) clock and divider initialization */
  LL_RCC_ConfigMCO(LL_RCC_MCOSOURCE_SYSCLK, LL_RCC_MCO_DIV_1);
  
  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);
  
  /* Initialize button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* Wait for button press */
  while (BSP_PB_GetState(BUTTON_KEY) == 1);
  
  /* Initialize clock, configure system clock as LSE */
  APP_SystemClockConfig();

  while (1)
  {
    /* Toggle LED */
    BSP_LED_Toggle(LED_GREEN);
    
    /* Delay for 500ms */
    LL_mDelay(500);
  }
}

/**
  * @brief  Configure system clock
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  /* Enable and initialize LSE */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();
  while(LL_PWR_IsEnabledBkUpAccess() == 0)
  {
  }
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_HIGH);
  LL_RCC_LSE_Enable();
  while (LL_RCC_LSE_IsReady() != 1)
  {
  }
  LL_PWR_DisableBkUpAccess();
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  /* Set AHB prescaler */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Configure LSE as system clock and initialize */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_LSE);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_LSE)
  {
  }

  /* Set APB prescaler: PCLK = HCLK */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(LSE_VALUE);

  /* Update the SystemCoreClock global variable(which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(LSE_VALUE);

}

/**
  * @brief  Configure PA08 as MCO alternate function
  * @param  None
  * @retval None
  */
static void APP_McoGpioConfig(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Enable GPIOA clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* Configure PA08 as alternate function and set it as MCO output pin */
  /* Select pin 8 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  /* Set mode as alternate function */
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  /* Select alternate function 5 (AF5) */
  GPIO_InitStruct.Alternate = LL_GPIO_AF5_MCO;
  /* Set output speed as very high frequency */
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  /* Set output type as push-pull */
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  /* Set no pull-up/pull-down */
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

  /* Initialize GPIOA */
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
