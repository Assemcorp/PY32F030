/**
  ******************************************************************************
  * @file    py32f031_hal_lcd.h
  * @author  MCU Application Team
  * @brief   Header file of LCD Controller HAL module.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PY32F031_HAL_LCD_H
#define __PY32F031_HAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f031_hal_def.h"

/** @addtogroup PY32F031_HAL_Driver
  * @{
  */

/** @defgroup LCD LCD
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/** @defgroup LCD_Exported_Types LCD Exported Types
  * @{
  */

/** 
  * @brief LCD Init structure definition
  */
typedef struct
{
  uint32_t Contrast;        /*!< Configures the LCD Contrast.
                                 This parameter can be one value of @ref LCD_Contrast */
  uint32_t BiasSrc;         /*!< Configures the LCD Bias Source.
                                 This parameter can be one value of @ref LCD_BiasSource */
  uint32_t Duty;            /*!< Configures the LCD Duty.
                                 This parameter can be one value of @ref LCD_Duty */
  uint32_t Bias;            /*!< Configures the LCD Bias.
                                 This parameter can be one value of @ref LCD_Bias */
  uint32_t ScanFre;         /*!< Configures the LCD Scan Frequency.
                                 This parameter can be one value of @ref LCD_ScanFrequency */
  uint32_t Mode;            /*!< Configures the LCD Mode.
                                 This parameter can be one value of @ref LCD_Mode */
}LCD_InitTypeDef;

/** 
  * @brief HAL LCD State structures definition
  */ 
typedef enum
{
  HAL_LCD_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized */
  HAL_LCD_STATE_READY             = 0x01U,    /*!< Peripheral Initialized and ready for use */
  HAL_LCD_STATE_BUSY              = 0x02U,    /*!< an internal process is ongoing */
  HAL_LCD_STATE_TIMEOUT           = 0x03U,    /*!< Timeout state */
  HAL_LCD_STATE_ERROR             = 0x04U     /*!< Error */
}HAL_LCD_StateTypeDef;

/** 
  * @brief  LCD handle Structure definition
  */  
typedef struct
{
  LCD_TypeDef                   *Instance;  /* LCD registers base address */
  
  LCD_InitTypeDef               Init;       /* LCD communication parameters */

  HAL_LockTypeDef               Lock;       /* Locking object */

  __IO HAL_LCD_StateTypeDef     State;      /* LCD communication state */
  
  __IO uint32_t                 ErrorCode;  /* LCD Error code */
  
}LCD_HandleTypeDef;

/** 
  * @brief  LCD SEG COM port output enable configuration
  */
typedef struct
{
    uint32_t Seg0_13;
    union{
        uint32_t Seg14_17_Com0_7;
        struct
        {
            uint32_t reserved     :4;
            uint32_t Seg17Com4    :1;
            uint32_t Seg16Com5    :1;
            uint32_t Seg15Com6    :1;
            uint32_t Seg14Com7    :1;
            uint32_t Com0_3       :4;
        }SegComBit;
    }Seg14_17_Com0_7_t;
}LCD_SegComTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup LCD_Exported_Constants LCD Exported Constants
  * @{
  */

/** @defgroup LCD_Error_Code_definition LCD Error Code definition
  * @brief  LCD Error Code definition
  * @{
  */
#define HAL_LCD_ERROR_NONE       ((uint32_t)0x00U)    /*!< No error */
/**
  * @}
  */

/** @defgroup LCD_Contrast LCD Contrast
  * @{
  */

#define LCD_CONTRASTLEVEL_0               ((uint32_t)0x00000000U)
#define LCD_CONTRASTLEVEL_1               (LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_2               (LCD_CR0_CONTRAST_1)
#define LCD_CONTRASTLEVEL_3               (LCD_CR0_CONTRAST_1 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_4               (LCD_CR0_CONTRAST_2)
#define LCD_CONTRASTLEVEL_5               (LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_6               (LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_1)
#define LCD_CONTRASTLEVEL_7               (LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_1 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_8               (LCD_CR0_CONTRAST_3)
#define LCD_CONTRASTLEVEL_9               (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_10              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_1)
#define LCD_CONTRASTLEVEL_11              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_1 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_12              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_2)
#define LCD_CONTRASTLEVEL_13              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_0)
#define LCD_CONTRASTLEVEL_14              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_1)
#define LCD_CONTRASTLEVEL_15              (LCD_CR0_CONTRAST_3 | LCD_CR0_CONTRAST_2 | LCD_CR0_CONTRAST_1 | LCD_CR0_CONTRAST_0)

/**
  * @}
  */

/** @defgroup LCD_BiasSource LCD Bias Source
  * @{
  */

#define LCD_BIAS_SRC_IN_RES_HIGH_POWER     (LCD_CR0_BSEL_2 | LCD_CR0_BSEL_1) /* Internal resistance partial voltage, high power consumption mode */
#define LCD_BIAS_SRC_IN_RES_LOW_POWER      (LCD_CR0_BSEL_2)                  /* Internal resistance partial voltage, low power consumption mode */
#define LCD_BIAS_SRC_IN_RES_MID_POWER      (LCD_CR0_BSEL_1)                  /* Internal resistance partial voltage, medium power consumption mode */
#define LCD_BIAS_SRC_EXT_RES               ((uint32_t)0x00000000U)           /* External resistance mode */

/**
  * @}
  */

/** @defgroup LCD_Duty LCD Duty
  * @{
  */
  
#define LCD_DUTY_STATIC                 ((uint32_t)0x00000000U)                              /*!< Static duty */
#define LCD_DUTY_1_2                    (LCD_CR0_DUTY_0)                                     /*!< 1/2 duty    */
#define LCD_DUTY_1_3                    (LCD_CR0_DUTY_1)                                     /*!< 1/3 duty    */
#define LCD_DUTY_1_4                    ((LCD_CR0_DUTY_1 | LCD_CR0_DUTY_0))                  /*!< 1/4 duty    */
#define LCD_DUTY_1_6                    ((LCD_CR0_DUTY_2 | LCD_CR0_DUTY_0))                  /*!< 1/6 duty    */
#define LCD_DUTY_1_8                    ((LCD_CR0_DUTY_2 | LCD_CR0_DUTY_1 | LCD_CR0_DUTY_0)) /*!< 1/8 duty    */

/**
  * @}
  */ 

/** @defgroup LCD_Bias LCD Bias
  * @{
  */
  
#define LCD_BIAS_1_3                    ((uint32_t)0x00000000U) /*!< 1/3 Bias */
#define LCD_BIAS_1_2                    (LCD_CR0_BIAS)          /*!< 1/2 Bias */

/**
  * @}
  */

/** @defgroup LCD_ScanFrequency LCD Scan Frequency
  * @{
  */
  
#define LCD_SCAN_FRE_64HZ               ((uint32_t)0x00000000U)
#define LCD_SCAN_FRE_128HZ              (LCD_CR0_LCDCLK_0)
#define LCD_SCAN_FRE_256HZ              (LCD_CR0_LCDCLK_1)
#define LCD_SCAN_FRE_512HZ              (LCD_CR0_LCDCLK_1 | LCD_CR0_LCDCLK_0)

/**
  * @}
  */
  
/** @defgroup LCD_Mode LCD Mode
  * @{
  */
  
#define LCD_MODE_0                    ((uint32_t)0x00000000U)
#define LCD_MODE_1                    (LCD_CR1_MODE)

/**
  * @}
  */

/** @defgroup LCD_Interrupt LCD Interrupt
  * @{
  */

#define LCD_IT                      LCD_CR1_IE

/**
  * @}
  */

/** @defgroup LCD_Flag LCD Flag
  * @{
  */

#define LCD_FLAG_INTF               LCD_CR1_INTF

/**
  * @}
  */
  
/** @defgroup LCD_RAMRegister LCD RAMRegister
  * @{
  */

#define LCD_RAM_REGISTER0               ((uint32_t)0x00000000U) /*!< LCD RAM Register 0  */
#define LCD_RAM_REGISTER1               ((uint32_t)0x00000001U) /*!< LCD RAM Register 1  */
#define LCD_RAM_REGISTER2               ((uint32_t)0x00000002U) /*!< LCD RAM Register 2  */
#define LCD_RAM_REGISTER3               ((uint32_t)0x00000003U) /*!< LCD RAM Register 3  */
#define LCD_RAM_REGISTER4               ((uint32_t)0x00000004U) /*!< LCD RAM Register 4  */

/**
  * @}
  */
 
/**
  * @}
  */
 
/* Exported macro ------------------------------------------------------------*/

/** @defgroup LCD_Exported_Macros LCD Exported Macros
  * @{
  */

/** @brief Reset LCD handle state
  * @param  __HANDLE__ specifies the LCD Handle.
  * @retval None
  */
#define __HAL_LCD_RESET_HANDLE_STATE(__HANDLE__)  ((__HANDLE__)->State = HAL_LCD_STATE_RESET)

/** @brief  macros to enables or disables the LCD
  * @param  __HANDLE__ specifies the LCD Handle.  
  * @retval None
  */     
#define __HAL_LCD_ENABLE(__HANDLE__)              (SET_BIT((__HANDLE__)->Instance->CR0, LCD_CR0_EN))
#define __HAL_LCD_DISABLE(__HANDLE__)             (CLEAR_BIT((__HANDLE__)->Instance->CR0, LCD_CR0_EN))

/**
  * @brief  Macro to configure the LCD Contrast.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __CONTRAST__ specifies the LCD Contrast.
  *   This parameter can be one of the following values:
  *     @arg LCD_CONTRASTLEVEL_0
  *     @arg LCD_CONTRASTLEVEL_1
  *     @arg LCD_CONTRASTLEVEL_2
  *     @arg LCD_CONTRASTLEVEL_3
  *     @arg LCD_CONTRASTLEVEL_4
  *     @arg LCD_CONTRASTLEVEL_5
  *     @arg LCD_CONTRASTLEVEL_6
  *     @arg LCD_CONTRASTLEVEL_7
  *     @arg LCD_CONTRASTLEVEL_8
  *     @arg LCD_CONTRASTLEVEL_9
  *     @arg LCD_CONTRASTLEVEL_10
  *     @arg LCD_CONTRASTLEVEL_11
  *     @arg LCD_CONTRASTLEVEL_12
  *     @arg LCD_CONTRASTLEVEL_13
  *     @arg LCD_CONTRASTLEVEL_14
  *     @arg LCD_CONTRASTLEVEL_15
  * @retval None
  */
#define __HAL_LCD_CONTRAST_CONFIG(__HANDLE__, __CONTRAST__)    (MODIFY_REG((__HANDLE__)->Instance->CR0, LCD_CR0_CONTRAST, (__CONTRAST__)))

/** @brief  Enables or disables the specified LCD interrupt.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __INTERRUPT__ specifies the LCD interrupt source to be enabled or disabled.
  *          This parameter can be one of the following values:
  *     @arg LCD_IT
  * @retval None
  */
#define __HAL_LCD_ENABLE_IT(__HANDLE__, __INTERRUPT__)      (SET_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))
#define __HAL_LCD_DISABLE_IT(__HANDLE__, __INTERRUPT__)     (CLEAR_BIT((__HANDLE__)->Instance->CR1, (__INTERRUPT__)))

/** @brief  Checks whether the specified LCD interrupt is enabled or not.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __IT__ specifies the LCD interrupt source to check.
  *        This parameter can be one of the following values:
  *        @arg LCD_IT
  * @retval The state of __IT__ (TRUE or FALSE).
  */
#define __HAL_LCD_GET_IT_SOURCE(__HANDLE__, __IT__) ((((__HANDLE__)->Instance->CR1) & (__IT__)) == (__IT__))

/** @brief  Checks whether the specified LCD flag is set or not.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __FLAG__ specifies the flag to check.
  *        This parameter can be one of the following values:
  *        @arg LCD_FLAG_INTF
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_LCD_GET_FLAG(__HANDLE__, __FLAG__)    (((__HANDLE__)->Instance->CR1 & (__FLAG__)) == (__FLAG__))

/** @brief  Clears the specified LCD pending flag.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __FLAG__ specifies the flag to clear.
  *        This parameter can be any combination of the following values:
  *        @arg LCD_FLAG_INTF
  * @retval None
  */
#define __HAL_LCD_CLEAR_FLAG(__HANDLE__, __FLAG__)  (CLEAR_BIT((__HANDLE__)->Instance->INTCLR, (LCD_INTCLR_INTF_CLR)))

/** @brief  Enables or disables the LCD DMA.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @retval None
  */
#define __HAL_LCD_ENABLE_DMA_HARD(__HANDLE__)      (SET_BIT((__HANDLE__)->Instance->CR1, LCD_CR1_DMAEN))
#define __HAL_LCD_DISABLE_DMA_HARD(__HANDLE__)     (CLEAR_BIT((__HANDLE__)->Instance->CR1, LCD_CR1_DMAEN))

/** @brief  Enables or disables the LCD blink mode.
  * @param  __HANDLE__ specifies the LCD Handle.
  * @retval None
  */
#define __HAL_LCD_ENABLE_BLINK(__HANDLE__)      (SET_BIT((__HANDLE__)->Instance->CR1, LCD_CR1_BLINKEN))
#define __HAL_LCD_DISABLE_BLINK(__HANDLE__)     (CLEAR_BIT((__HANDLE__)->Instance->CR1, LCD_CR1_BLINKEN))

/** @brief  Configure the number of frames corresponding to LCD blink frequency and interrupt interval
  * @param  __HANDLE__ specifies the LCD Handle.
  * @param  __VALUE__ specifies the number of frames corresponding to LCD blink frequency and interrupt interval.
  * @retval None
  */
#define __HAL_LCD_BLINK_CONFIG(__HANDLE__, __VALUE__)   (MODIFY_REG((__HANDLE__)->Instance->CR1, LCD_CR1_BLINKCNT, (__VALUE__)))

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup LCD_Private_Macros LCD Private Macros
  * @{
  */

#define IS_LCD_CONTRAST(__CONTRAST__) (((__CONTRAST__) == LCD_CONTRASTLEVEL_0) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_1) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_2) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_3) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_4) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_5) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_6) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_7) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_8) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_9) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_10) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_11) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_12) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_13) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_14) || \
                                       ((__CONTRAST__) == LCD_CONTRASTLEVEL_15))

#define IS_LCD_BIAS_SRC(__SOURCE__)   (((__SOURCE__) == LCD_BIAS_SRC_IN_RES_HIGH_POWER) || \
                                       ((__SOURCE__) == LCD_BIAS_SRC_IN_RES_LOW_POWER) || \
                                       ((__SOURCE__) == LCD_BIAS_SRC_IN_RES_MID_POWER) || \
                                       ((__SOURCE__) == LCD_BIAS_SRC_EXT_RES))

#define IS_LCD_DUTY(__DUTY__)         (((__DUTY__) == LCD_DUTY_STATIC)  || \
                                       ((__DUTY__) == LCD_DUTY_1_2)     || \
                                       ((__DUTY__) == LCD_DUTY_1_3)     || \
                                       ((__DUTY__) == LCD_DUTY_1_4)     || \
                                       ((__DUTY__) == LCD_DUTY_1_6)     || \
                                       ((__DUTY__) == LCD_DUTY_1_8))

#define IS_LCD_BIAS(__BIAS__)         (((__BIAS__) == LCD_BIAS_1_3) || \
                                       ((__BIAS__) == LCD_BIAS_1_2))

#define IS_LCD_SCAN_FRE(__FRE__)      (((__FRE__) == LCD_SCAN_FRE_64HZ) || \
                                       ((__FRE__) == LCD_SCAN_FRE_128HZ) || \
                                       ((__FRE__) == LCD_SCAN_FRE_256HZ) || \
                                       ((__FRE__) == LCD_SCAN_FRE_512HZ))

#define IS_LCD_MODE(__MODE__)         (((__MODE__) == LCD_MODE_0) || \
                                       ((__MODE__) == LCD_MODE_1))

#define IS_LCD_RAM_REGISTER(__REGISTER__) (((__REGISTER__) == LCD_RAM_REGISTER0)  || \
                                           ((__REGISTER__) == LCD_RAM_REGISTER1)  || \
                                           ((__REGISTER__) == LCD_RAM_REGISTER2)  || \
                                           ((__REGISTER__) == LCD_RAM_REGISTER3)  || \
                                           ((__REGISTER__) == LCD_RAM_REGISTER4))


/**
  * @}
  */
  
/* Exported functions ------------------------------------------------------- */

/** @defgroup LCD_Exported_Functions LCD Exported Functions
  * @{
  */

/** @defgroup LCD_Exported_Functions_Group1 Initialization and de-initialization methods
  * @{
  */

/* Initialization/de-initialization methods  **********************************/
HAL_StatusTypeDef     HAL_LCD_DeInit(LCD_HandleTypeDef *hlcd);
HAL_StatusTypeDef     HAL_LCD_Init(LCD_HandleTypeDef *hlcd);
void                  HAL_LCD_MspInit(LCD_HandleTypeDef *hlcd);
void                  HAL_LCD_MspDeInit(LCD_HandleTypeDef *hlcd);

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group2 IO operation methods
  * @{
  */

/* IO operation methods *******************************************************/
HAL_StatusTypeDef     HAL_LCD_SetSegCom(LCD_HandleTypeDef *hlcd, LCD_SegComTypeDef *SegCom);
HAL_StatusTypeDef     HAL_LCD_Write(LCD_HandleTypeDef *hlcd, uint32_t RAMRegisterIndex, uint32_t Data);
HAL_StatusTypeDef     HAL_LCD_Clear(LCD_HandleTypeDef *hlcd);

void HAL_LCD_IRQHandler(LCD_HandleTypeDef *hlcd);
void HAL_LCD_IntCallback(LCD_HandleTypeDef *hlcd);

/**
  * @}
  */

/** @defgroup LCD_Exported_Functions_Group3 Peripheral State methods
  * @{
  */

/* Peripheral State methods  **************************************************/
HAL_LCD_StateTypeDef  HAL_LCD_GetState(LCD_HandleTypeDef *hlcd);
uint32_t              HAL_LCD_GetError(LCD_HandleTypeDef *hlcd);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __PY32F031_HAL_LCD_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
