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
ADC_HandleTypeDef             AdcHandle;
ADC_ChannelConfTypeDef        sConfig;
volatile uint16_t             aADCxConvertedData;
volatile uint32_t             VrefBuf_Value;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_ADCConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */ 
  HAL_Init();
  
  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  /* Initialize UART */
  BSP_USART_Config();

  /* Configure ADC */
  APP_ADCConfig();
 
  while (1)
  {   
    HAL_ADC_Start(&AdcHandle);                             /* Start ADC Conversion */
    HAL_ADC_PollForConversion(&AdcHandle, 1000000);        /* Polling for ADC Conversion */
    aADCxConvertedData = HAL_ADC_GetValue(&AdcHandle);     /* Get ADC Value */
    VrefBuf_Value = (4095 * 1200) / aADCxConvertedData;    /* Calculate the data */
    
    /* print VrefBuf value */
    printf("VrefBuf:%umV\r\n",(unsigned int)VrefBuf_Value);
    
    HAL_Delay(1000);
  }
}

/**
  * @brief  ADC configuration function
  * @param  None
  * @retval None
  */
static void APP_ADCConfig(void)
{
  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV8;                /* Set ADC clock*/
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;                      /* 12-bit resolution for converted data */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;                     /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;              /* Scan sequence direction: forward */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;                     /* Single Conversion */
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;                                  /* Auto-delayed conversion feature enable */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                                 /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                      /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;           /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)                                         /* ADC initialization */
  {
    APP_ErrorHandler();
  }
  
  /* Configure ADC VrefBuf 2.048V */
  HAL_ADC_ConfigVrefBuf(&AdcHandle,ADC_VREFBUF_2P048V);
  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)                           /* ADC Calibration */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_1;                             
  sConfig.Channel      = ADC_CHANNEL_VREFINT;     
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
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
