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
DMA_HandleTypeDef             HdmaCh1;
TIM_HandleTypeDef             TimHandle;
TIM_MasterConfigTypeDef       sMasterConfig;
uint32_t                      gADCxConvertedData[4];
ADC_ChannelConfTypeDef        sConfig;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_ADCConfig(void);
static void APP_TimConfig(void);


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
  
  HAL_ADC_Start_DMA(&AdcHandle,gADCxConvertedData,4);    
  
  /* TIM configuration */
  APP_TimConfig();
  
  while (1)
  {   
    BSP_LED_Toggle(LED_GREEN);
    while(!__HAL_DMA_GET_FLAG(&HdmaCh1, DMA_ISR_TCIF1));                               
    
    /* Clear DMA channel 1 transmission completion flag */
    __HAL_DMA_CLEAR_FLAG(&HdmaCh1, DMA_IFCR_CTCIF1);       
    printf("Channel4: %u \r\n", (unsigned int)gADCxConvertedData[0]);
    printf("Channel5: %u \r\n", (unsigned int)gADCxConvertedData[1]);
    printf("Channel6: %u \r\n", (unsigned int)gADCxConvertedData[2]);
    printf("Channel7: %u \r\n", (unsigned int)gADCxConvertedData[3]);   
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
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;                                  /* Auto-delayed conversion feature enabled */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                                 /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO;            /* Set the external trigger for conversion start event to TIM1_TRGO */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;         /* Trigger on rising edges */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.DMAContinuousRequests = ENABLE;                                  /* DMA Circle */
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)                                         /* ADC initialization */
  {
    APP_ErrorHandler();
  }
  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)                           /* ADC Calibration */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_1;                             
  sConfig.Channel      = ADC_CHANNEL_4;     
  sConfig.SamplingTime= ADC_SAMPLETIME_239CYCLES_5;  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_2;                             
  sConfig.Channel      = ADC_CHANNEL_5;     
  sConfig.SamplingTime= ADC_SAMPLETIME_239CYCLES_5;  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_3;                             
  sConfig.Channel      = ADC_CHANNEL_6;     
  sConfig.SamplingTime= ADC_SAMPLETIME_239CYCLES_5;  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_4;                             
  sConfig.Channel      = ADC_CHANNEL_7;     
  sConfig.SamplingTime= ADC_SAMPLETIME_239CYCLES_5;  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  TIM Configuration Function
  * @param  None
  * @retval None
  */
static void APP_TimConfig(void)
{
  TimHandle.Instance = TIM1;                                          /* TIM1 */
  TimHandle.Init.Period            = 8000 - 1;                        /* Period = 8000-1 */
  TimHandle.Init.Prescaler         = 1000 - 1;                        /* Prescaler = 1000-1 */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;          /* Clock does not divide frequency */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;              /* count up */
  TimHandle.Init.RepetitionCounter = 0;                               /* no-repeat */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  /* Automatic reloading of registers without buffering */
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)                        /* Initialize TIM1 */
  {
    APP_ErrorHandler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;                /* Select Update Event as Trigger Source */
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;        /* Master/Slave mode has no effect */
  HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig);  /* Configure TIM1*/
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)                       /* TIM1 start */
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
