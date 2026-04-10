/**
  ******************************************************************************
  * @file    init.c
  * @brief   Initialization functions body
  * @author  AssemCorp Application Team - [emre.karabek@assemcorp.com]
  ******************************************************************************
  */
#include "init.h"
#include "main.h"
#include "py32f031_hal_tim.h"
#include "py32f031_hal_gpio_ex.h"
#include "py32f031_hal_adc.h"
#include "py32f031_hal_uart.h"

TIM_HandleTypeDef  htim1;
ADC_HandleTypeDef  hadc;
UART_HandleTypeDef UartHandle;
void Error_Handler(void);

void APP_ClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSISYS;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) { Error_Handler(); }
}

void APP_GpioConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Port A Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA8, PA9, PA10 as Output Push-Pull */
  GPIO_InitStruct.Pin   = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void APP_LCD_GpioConfig(void)
{
#ifdef USE_LCD
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* Enable GPIO Clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  /* Configure LCD Pins */
#ifdef LCD_RS_PIN
  GPIO_InitStruct.Pin = LCD_RS_PIN; HAL_GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStruct);
#endif
#ifdef LCD_EN_PIN
  GPIO_InitStruct.Pin = LCD_EN_PIN; HAL_GPIO_Init(LCD_EN_GPIO_PORT, &GPIO_InitStruct);
#endif
  
#ifdef LCD_D4_PIN
  GPIO_InitStruct.Pin = LCD_D4_PIN; HAL_GPIO_Init(LCD_D4_GPIO_PORT, &GPIO_InitStruct);
#endif
#ifdef LCD_D5_PIN
  GPIO_InitStruct.Pin = LCD_D5_PIN; HAL_GPIO_Init(LCD_D5_GPIO_PORT, &GPIO_InitStruct);
#endif
#ifdef LCD_D6_PIN
  GPIO_InitStruct.Pin = LCD_D6_PIN; HAL_GPIO_Init(LCD_D6_GPIO_PORT, &GPIO_InitStruct);
#endif
#ifdef LCD_D7_PIN
  GPIO_InitStruct.Pin = LCD_D7_PIN; HAL_GPIO_Init(LCD_D7_GPIO_PORT, &GPIO_InitStruct);
#endif
#endif
}


/**
 * @brief  Configures LED pins as output.
 */
void APP_GpioConfig_Output(GPIO_TypeDef *port, uint16_t pin)
{
  if (port == GPIOB)
    __HAL_RCC_GPIOB_CLK_ENABLE();
  if (port == GPIOA)
    __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/**
 * @brief  Configures button pins as input.
 */
void APP_GpioConfig_Input(GPIO_TypeDef *port, uint16_t pin)
{
  if (port == GPIOB)
    __HAL_RCC_GPIOB_CLK_ENABLE();
  if (port == GPIOA)
    __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/**
 * @brief  Turns on the Output pin(s).
 */
void OUTPUT_ON(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
 * @brief  Turns off the Output pin(s).
 */
void OUTPUT_OFF(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
 * @brief  Reads the Input state.
 * @retval 1 if high, 0 otherwise
 */
uint8_t INPUT_READ(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

/**
 * @brief  Toggles the specified Output pin with a 1000ms delay.
 */
void OUTPUT_TOGGLE(GPIO_TypeDef *port, uint16_t pin)
{
  HAL_GPIO_TogglePin(port, pin);
  HAL_Delay(1000);
}

/**
 * @brief  Initialises ADC1, channel IN0 (PA0), single conversion mode.
 */
void APP_AdcConfig(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA0 as analog input */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin  = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  hadc.Instance                   = ADC1;
  hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution            = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait      = DISABLE;
  hadc.Init.ContinuousConvMode    = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc) != HAL_OK) { Error_Handler(); }

  sConfig.Channel      = ADC_CHANNEL_0;   /* PA0 */
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) { Error_Handler(); }

  /* Run self-calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK) { Error_Handler(); }
}

/**
 * @brief  Initialises TIM1 as a 1 kHz base timer (1 ms tick at 24 MHz).
 */
void APP_TimerConfig(void)
{
  __HAL_RCC_TIM1_CLK_ENABLE();

  htim1.Instance               = TIM1;
  htim1.Init.Prescaler         = 24 - 1;      /* 24 MHz / 24 = 1 MHz */
  htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim1.Init.Period            = 1000 - 1;    /* 1 MHz / 1000 = 1 kHz (1 ms) */
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK) { Error_Handler(); }
}

/**
 * @brief  Initialises TIM1 CH1 (PA8) as PWM output, 1 kHz, 50% duty cycle.
 */
void APP_PwmConfig(void)
{
  /* Reuse timer base — call APP_TimerConfig first or configure here */
  __HAL_RCC_TIM1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* PA8 → TIM1_CH1 alternate function */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin       = GPIO_PIN_8;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  htim1.Instance               = TIM1;
  htim1.Init.Prescaler         = 24 - 1;
  htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim1.Init.Period            = 1000 - 1;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) { Error_Handler(); }

  TIM_OC_InitTypeDef sConfigOC = {0};
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.Pulse        = 500;           /* 50 % duty cycle */
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) { Error_Handler(); }
}

/**
 * @brief  Initialises USART1 at 115200 8N1 on PA9 (TX) / PA10 (RX).
 */
void APP_UartConfig(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();

  /* PA9 → USART1_TX,  PA10 → USART1_RX */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  UartHandle.Instance          = USART1;
  UartHandle.Init.BaudRate     = 115200;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&UartHandle) != HAL_OK) { Error_Handler(); }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}