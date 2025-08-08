/*******************************************************************************
 * @file    main.c
 * @author  MCU Application Team
 * @author  AssemCorp - Emre Karabek - Field Application Engineer
 * @date    2025-June
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
#define TRUE 1
#define FALSE 0
#define RESET 0

/* 7-Segment Display Pin Definitions */
// Segment pins
#define SEG_A_PIN                    GPIO_PIN_5    // A5
#define SEG_B_PIN                    GPIO_PIN_0    // B0
#define SEG_C_PIN                    GPIO_PIN_1    // B1
#define SEG_D_PIN                    GPIO_PIN_15   // A15
#define SEG_E_PIN                    GPIO_PIN_12   // A12
#define SEG_F_PIN                    GPIO_PIN_6    // A6
#define SEG_G_PIN                    GPIO_PIN_3    // A3
#define SEG_DP_PIN                   GPIO_PIN_4    // A4

// Digit select pins
#define DIG1_PIN                     GPIO_PIN_11   // A11
#define DIG2_PIN                     GPIO_PIN_7    // A7

// Port definitions
#define SEG_PORTA_PINS               (SEG_A_PIN | SEG_D_PIN | SEG_E_PIN | SEG_F_PIN | SEG_G_PIN | SEG_DP_PIN | DIG1_PIN | DIG2_PIN)
#define SEG_PORTB_PINS               (SEG_B_PIN | SEG_C_PIN)
/* Private variables ---------------------------------------------------------*/
uint8_t running = TRUE;
uint16_t mainMenu;
volatile uint8_t display_counter = 0;  // 0-99 counter
volatile uint8_t digit_select = 0;     // Digit multiplexing
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_GpioConfig(void);
static void APP_SystemClock_Config(void);
void APP_GpioConfig_Led(GPIO_TypeDef *port, uint16_t pin);
void APP_GpioConfig_Button(GPIO_TypeDef *port, uint16_t pin);
void APP_7SegmentConfig(void);
void LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t Button_Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Display_Number(uint8_t number);
void Display_Digit(uint8_t digit, uint8_t position);
void Clear_Display(void);
/**
 * @brief  Main program.
 * @retval int
 */
int main(void)
{
  /* Reset all peripherals, initialize the Systick. */
  HAL_Init();

  /* Configure the system clock to use external 24MHz oscillator */
  APP_SystemClock_Config();

  /* Initialize GPIO */
  APP_GpioConfig_Led(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
  APP_GpioConfig_Button(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
  
  /* Initialize 7-Segment Display */
  APP_7SegmentConfig();
  Clear_Display();

  static uint32_t delay_ms = 250; // Initial delay time

  while (1)
  {
    static uint32_t button_last_tick = 0;
    static uint32_t last_display_update = 0;
    static uint32_t last_counter_update = 0;
    #define BUTTON_DEBOUNCE_DELAY 200
    uint32_t now = HAL_GetTick();

    /* Fast display multiplexing - every 5ms */
    if (now - last_display_update >= 5) {
      Display_Number(display_counter);
      last_display_update = now;
    }

    if ((now - button_last_tick) > BUTTON_DEBOUNCE_DELAY)
    {
      if (!(Button_Read(GPIOA, GPIO_PIN_1))) // A1 Button - Start
      {
        running = TRUE;
        button_last_tick = now;
      }
      else if (!(Button_Read(GPIOA, GPIO_PIN_2))) // A2 Button - Stop
      {
        running = FALSE;
        button_last_tick = now;
      }
    }

    mainMenu++;

    static uint32_t last_tick = 0;
    static uint8_t led_state = 0;

    uint32_t current_tick = HAL_GetTick();

    if (running)
    {
      /* Update counter every 300ms */
      if (current_tick - last_counter_update >= 300) {
        display_counter++;
        if (display_counter > 99) {
          display_counter = 0;  // Reset to 0 after 99
        }
        last_counter_update = current_tick;
      }
      
      // Toggle LEDs at adjustable speed
      if ((current_tick - last_tick) >= delay_ms)
      {
        last_tick = current_tick;
        led_state = !led_state;
        if (led_state)
        {
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
        }
      }
    }
    else
    {
      // Short wait for faster button control
      // Turn off LEDs
      LED_Off(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    }
  }
}

/**
 * @brief  System Clock Configuration with External 24MHz Oscillator
 * @param  None
 * @retval None
 */
static void APP_SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure the main PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    /* Enable HSE */
  RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;               /* HSE frequency range */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;             /* Don't use PLL, use HSE directly */
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Select HSE as system clock source and configure HCLK, PCLK1 prescalers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;      /* Use HSE as system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         /* HCLK = SYSCLK = 24MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;          /* PCLK1 = HCLK = 24MHz */

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
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
/**
 * @brief  Configures LED pins as output.
 */
void APP_GpioConfig_Led(GPIO_TypeDef *port, uint16_t pin)
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
void APP_GpioConfig_Button(GPIO_TypeDef *port, uint16_t pin)
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
 * @brief  7-Segment Display GPIO configuration
 * @param  None
 * @retval None
 */
void APP_7SegmentConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* Enable GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure GPIOA pins (A, D, E, F, G, DP, DIG1, DIG2) */
  GPIO_InitStruct.Pin = SEG_PORTA_PINS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Configure GPIOB pins (B, C) */
  GPIO_InitStruct.Pin = SEG_PORTB_PINS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  Display a number on 7-segment display (0-99)
 * @param  number: Number to display (0-99)
 * @retval None
 */
void Display_Number(uint8_t number)
{
  uint8_t tens = number / 10;
  uint8_t units = number % 10;
  
  /* Alternate between digits for multiplexing */
  if (digit_select == 0) {
    Display_Digit(units, 1);  // Display units digit on DIG1
    digit_select = 1;
  } else {
    Display_Digit(tens, 2);   // Display tens digit on DIG2
    digit_select = 0;
  }
}

/**
 * @brief  Display a digit on specified position
 * @param  digit: Digit to display (0-9)
 * @param  position: Position (1 = units/birler, 2 = tens/onlar)
 * @retval None
 */
void Display_Digit(uint8_t digit, uint8_t position)
{
  /* 7-segment patterns for digits 0-9 (Common Cathode)
     Bit order: G F E D C B A */
  const uint8_t digit_patterns[10] = {
    0b0111111,  // 0
    0b0000110,  // 1
    0b1011011,  // 2
    0b1001111,  // 3
    0b1100110,  // 4
    0b1101101,  // 5
    0b1111101,  // 6
    0b0000111,  // 7
    0b1111111,  // 8
    0b1101111   // 9
  };
  
  uint8_t pattern = digit_patterns[digit % 10];
  
  /* Clear all digit selects first */
  HAL_GPIO_WritePin(GPIOA, DIG1_PIN | DIG2_PIN, GPIO_PIN_RESET);
  
  /* Set segment states */
  HAL_GPIO_WritePin(GPIOA, SEG_A_PIN, (pattern & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, SEG_B_PIN, (pattern & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, SEG_C_PIN, (pattern & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, SEG_D_PIN, (pattern & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, SEG_E_PIN, (pattern & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, SEG_F_PIN, (pattern & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, SEG_G_PIN, (pattern & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  
  /* DP is always off */
  HAL_GPIO_WritePin(GPIOA, SEG_DP_PIN, GPIO_PIN_RESET);
  
  /* Select appropriate digit */
  if (position == 1) {
    HAL_GPIO_WritePin(GPIOA, DIG1_PIN, GPIO_PIN_SET);  // Enable units digit (birler basamağı)
  } else if (position == 2) {
    HAL_GPIO_WritePin(GPIOA, DIG2_PIN, GPIO_PIN_SET);  // Enable tens digit (onlar basamağı)
  }
}

/**
 * @brief  Clear 7-segment display
 * @param  None
 * @retval None
 */
void Clear_Display(void)
{
  /* Turn off all segments */
  HAL_GPIO_WritePin(GPIOA, SEG_A_PIN | SEG_D_PIN | SEG_E_PIN | SEG_F_PIN | SEG_G_PIN | SEG_DP_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, SEG_B_PIN | SEG_C_PIN, GPIO_PIN_RESET);
  
  /* Turn off digit selects */
  HAL_GPIO_WritePin(GPIOA, DIG1_PIN | DIG2_PIN, GPIO_PIN_RESET);
}

/**
 * @brief  Turns on the LED(s).
 */
void LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
 * @brief  Turns off the LED(s).
 */
void LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/**
 * @brief  Reads the button state.
 * @retval 1 if pressed, 0 otherwise
 */
uint8_t Button_Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET) ? 1 : 0;
}
#ifdef USE_FULL_ASSERT
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
