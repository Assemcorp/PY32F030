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
HDIV_CalculatedTypeDef Calculatervalue;
HDIV_HandleTypeDef HDivhandle;
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();
  
  /* Initialize debugging serial port (used by printf) */
  BSP_USART_Config();
  
  /* Enable DIV */
  __HAL_RCC_HDIV_CLK_ENABLE();
  
  HDivhandle.Instance       = HDIV;                 /* Divider base address        */
  Calculatervalue.Sign      = HDIV_MODE_UNSIGNED;   /* Unsigned division          */
  Calculatervalue.Dividend  = 1917887483;           /* Dividend 1,917,887,483 */
  Calculatervalue.Divisor   = 9597;                 /* Divisor 9,597           */

  /* Start Divider */
  HAL_HDIV_Calculate(&HDivhandle, &Calculatervalue);
  
  /* Print division results */
  printf("Quotient:%d\r\nRemainder:%d\r\n", (int)Calculatervalue.Quotient, (int)Calculatervalue.Remainder);
  
  /* infinite loop */
  while (1)
  {
  }
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
