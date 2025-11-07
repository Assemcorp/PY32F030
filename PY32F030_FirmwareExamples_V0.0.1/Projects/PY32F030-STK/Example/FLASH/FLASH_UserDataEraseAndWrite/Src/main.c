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
#define FLASH_USERDATA_START_ADDR     FLASH_USERDATA_BASE

/* Private variables ---------------------------------------------------------*/
const uint32_t DATA[32] =
{
  0x11111111, 0x23456789, 0x3456789A, 0x456789AB, 0x56789ABC, 0x6789ABCD, 0x789ABCDE, 0x89ABCDEF,
  0x9ABCDEF0, 0xABCDEF01, 0xBCDEF012, 0xCDEF0123, 0xDEF01234, 0xEF012345, 0xF0123456, 0x01234567,
  0x01010101, 0x23456789, 0x3456789A, 0x456789AB, 0x56789ABC, 0x6789ABCD, 0x789ABCDE, 0x89ABCDEF,
  0x9ABCDEF0, 0xABCDEF01, 0xBCDEF012, 0xCDEF0123, 0xDEF01234, 0xEF012345, 0xF0123456, 0x01234567
};

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_UserDataErase(void);
static void APP_UserDataProgram(void);
static void APP_UserDataBlank(void);
static void APP_UserDataVerify(void);

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

  /* Initialize Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* Initialize LED */
  BSP_LED_Init(LED_GREEN); 

  /* Wait for the button to be pressed */
  while (BSP_PB_GetState(BUTTON_USER)) 
  {
  }

  /* Unlock Flash */
  HAL_FLASH_Unlock();

  /* Erase UserData */
  APP_UserDataErase();

  /* Check UserData */
  APP_UserDataBlank();

  /* Program UserData */
  APP_UserDataProgram();

  /* Lock Flash */
  HAL_FLASH_Lock();

  /* Verify UserData */
  APP_UserDataVerify();

  BSP_LED_On(LED_GREEN);

  while (1)
  {
  }
}

/**
  * @brief  UserData Erase
  * @param  None
  * @retval None
  */
static void APP_UserDataErase(void)
{
  uint32_t UserDataError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct = {0};

  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_USERDATAERASE;      /* UserData Erase */
  EraseInitStruct.UserDataAddress = FLASH_USERDATA_START_ADDR;      /* Erase Start Address */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &UserDataError) != HAL_OK)    /* UserData Erase */
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  UserData Program
  * @param  None
  * @retval None
  */
static void APP_UserDataProgram(void)
{
  uint32_t flash_program_start = FLASH_USERDATA_START_ADDR ;                /* userdata program start address */
  uint32_t *src = (uint32_t *)DATA;                                     /* Program data */

  if (HAL_FLASH_UserDataProgram(flash_program_start, src) != HAL_OK)/* Program */
  {
    APP_ErrorHandler();
  }   
}

/**
  * @brief  UserData Check
  * @param  None
  * @retval None
  */
static void APP_UserDataBlank(void)
{
  uint32_t addr = 0;

  while (addr < sizeof(DATA))
  {
    if (0xFFFFFFFF != HW32_REG(FLASH_USERDATA_START_ADDR + addr))
    {
      APP_ErrorHandler();
    }
    addr += 4;
  }
}

/**
  * @brief  UserData Verify
  * @param  None
  * @retval None
  */
static void APP_UserDataVerify(void)
{
  uint32_t addr = 0;

  while (addr < sizeof(DATA))
  {
    if (DATA[addr / 4] != HW32_REG(FLASH_USERDATA_START_ADDR + addr))
    {
      APP_ErrorHandler();
    }
    addr += 4;
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
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* Close HSE*/
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Close LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* Close LSE */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* Close PLL */
  /*RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_NONE;*/
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
