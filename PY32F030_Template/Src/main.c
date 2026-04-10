/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main program body
  * @author  AssemCorp Application Team - [emre.karabek@assemcorp.com]
  ******************************************************************************
  */
#include "main.h"
#include "init.h"
#include "Puya_Lcd/lcd.h"
#include "py32f031_hal_tim.h"

int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* System clock configuration */
  APP_ClockConfig();

  /* Initialize peripherals */
#ifdef USE_GPIO
  APP_GpioConfig_Output(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
#endif

#ifdef USE_LCD
  APP_LCD_GpioConfig();
  LCD_Init();
#endif

#ifdef USE_UART
  APP_UartConfig();
#endif

#ifdef USE_ADC
  APP_AdcConfig();
#endif

#ifdef USE_TIMER
  APP_TimerConfig();
  HAL_TIM_Base_Start(&htim1);
#endif

#ifdef USE_PWM
  APP_PwmConfig();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
#endif

#ifdef USE_INPUT
  APP_GpioConfig_Input(GPIOA, GPIO_PIN_0);   /* Example: button on PA0 */
#endif

  /* infinite loop */
  while (1)
  {

#ifdef USE_UART
    /* Transmit a periodic status message over UART */
    {
      const char *msg = "PUYA Running\r\n";
      HAL_UART_Transmit(&UartHandle, (uint8_t *)msg, 14, 100);
    }
#endif

#ifdef USE_ADC
    /* Start ADC conversion and read result */
    {
      uint32_t adcValue = 0;
      HAL_ADC_Start(&hadc);
      if (HAL_ADC_PollForConversion(&hadc, 10) == HAL_OK)
      {
        adcValue = HAL_ADC_GetValue(&hadc);
      }
      HAL_ADC_Stop(&hadc);
#ifdef USE_LCD
      {
        char buf[17];
        sprintf(buf, "ADC: %4lu       ", (unsigned long)adcValue);
        LCD_SetCursor(1, 0);
        LCD_Print(buf);
      }
#endif
    }
#endif

#ifdef USE_TIMER
    /* Timer is running in background — add timer-based logic here */
    (void)__HAL_TIM_GET_COUNTER(&htim1);  /* Read counter if needed */
#endif

#ifdef USE_PWM
    /* Adjust PWM duty cycle — example: ramp from 0 to ARR */
    {
      static uint32_t duty = 0;
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
      duty = (duty >= htim1.Init.Period) ? 0 : duty + 100;
    }
#endif

#ifdef USE_INPUT
    /* Read button on PA0 and mirror state to PA8 */
    if (INPUT_READ(GPIOA, GPIO_PIN_0))
    {
      OUTPUT_ON(GPIOA, GPIO_PIN_8);
    }
    else
    {
      OUTPUT_OFF(GPIOA, GPIO_PIN_8);
    }
#endif

#ifdef USE_LCD
    LCD_SetCursor(0, 0);
    LCD_Print("   ASSEMCORP    ");
    LCD_SetCursor(1, 0);
    LCD_Print("PUYA PIN CONFIG.");
#endif

#ifdef USE_OUTPUT
    OUTPUT_TOGGLE(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
#endif
    HAL_Delay(500);
  }
}
