/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main program body
  * @author  AssemCorp Application Team - [emre.karabek@assemcorp.com]
  ******************************************************************************
  */
#include "main.h"
#include "init.h"

int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* System clock configuration */
  APP_SystemClockConfig();
  /* Initialize peripherals */
  APP_GpioConfig();

  /* infinite loop */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    HAL_Delay(1000);
  }
}
