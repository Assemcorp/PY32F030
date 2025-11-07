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
TIM_HandleTypeDef             TimHandle;
TIM_MasterConfigTypeDef       sMasterConfig;
ADC_ChannelConfTypeDef        sConfig;
volatile uint16_t             aADCxConvertedData;
ADC_AnalogWDGConfTypeDef      ADCAnalogWDGConfig;

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
  
  /* Configure Timer */
  APP_TimConfig();
  
  HAL_ADC_Start_IT(&AdcHandle);
 
  while (1)
  {   

  }
}

/**
  * @brief  ADC Analog watchdog callback
  * @param  hadc：ADC handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  BSP_LED_On(LED_GREEN);
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
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;                                 /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                                 /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_TRGO;            /* Trig of conversion start done by external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;         /* Trigger on rising edges */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  
  if(HAL_ADC_Init(&AdcHandle) != HAL_OK)                                         /* ADC initialization */
  {
    APP_ErrorHandler();
  }
    
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)                           /* ADC Calibration */
  {
    APP_ErrorHandler();
  }
  
  sConfig.Rank         = ADC_REGULAR_RANK_1;                             
  sConfig.Channel      = ADC_CHANNEL_4;     
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  
  if(HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
  
  ADCAnalogWDGConfig.Channel=ADC_CHANNEL_4;
  ADCAnalogWDGConfig.HighThreshold=0x800;
  ADCAnalogWDGConfig.ITMode=ENABLE;
  ADCAnalogWDGConfig.LowThreshold=0x0;
  ADCAnalogWDGConfig.WatchdogMode=ADC_ANALOGWATCHDOG_SINGLE_REG;
  /* ADC Analog Watchdog config */
  if(HAL_ADC_AnalogWDGConfig(&AdcHandle, &ADCAnalogWDGConfig) != HAL_OK)  
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
