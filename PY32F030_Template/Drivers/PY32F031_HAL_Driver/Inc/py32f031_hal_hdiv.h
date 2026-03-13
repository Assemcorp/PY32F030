/**
  ******************************************************************************
  * @file    py32f031_hal_hdiv.h
  * @author  MCU Application Team
  * @brief   Header file of HDIV HAL module.
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
#ifndef PY32F031_HAL_HDIV_H
#define PY32F031_HAL_HDIV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f031_hal_def.h"

/** @addtogroup PY32F031_HAL_Driver
  * @{
  */

/** @addtogroup HDIV
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup HDIV_Exported_Types HDIV Exported Types
  * @{
  */

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
  HAL_HDIV_STATE_RESET   = 0x00U,
  HAL_HDIV_STATE_READY   = 0x01U,
  HAL_HDIV_STATE_BUSY    = 0x02U, 
  HAL_HDIV_STATE_END     = 0x03U, 
  HAL_HDIV_STATE_ZERO    = 0x04U,
  HAL_HDIV_STATE_TIMEOUT = 0x05U
} HAL_HDIV_StateTypeDef;

/**
  * @brief  HDIV calculated value Structure definition
  */
typedef struct __HDIV_CalculatedTypeDef
{
  uint32_t    Sign;            /*!< Set division sign  */
                   
  int32_t     Dividend;        /*!< Dividend value     */
                   
  int32_t     Divisor;         /*!< Divisor value      */
    
  int32_t     Quotient;        /*!< Quotient value     */
                            
  int32_t     Remainder;       /*!< Remainder value    */
  
} HDIV_CalculatedTypeDef;

/**
  * @brief  HDIV handle Structure definition
  */
typedef struct __HDIV_HandleTypeDef
{
  HDIV_TypeDef        *Instance;              /*!< Register base address         */

  HAL_LockTypeDef    Lock;                   /*!< HDIV locking object            */
  
  __IO HAL_HDIV_StateTypeDef   State;         /*!< HDIV operation state           */
} HDIV_HandleTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup HDIV_Exported_Constants HDIV Exported Constants
  * @{
  */

/** @defgroup Sign mode Selection
  * @{
  */
#define HDIV_MODE_UNSIGNED             0x00000000U
#define HDIV_MODE_SIGNED               HDIV_SIGN_SIGN
/**
  * @}
  */
  
/**
  * @}
  */

/** @defgroup HDIV_Exported_Macros HDIV Exported Macros
  * @{
  */

/** @brief  Get quotient value.
  * @param  __HANDLE__ HDIV handle.
  * @retval None
  */
#define __HAL_HDIV_GET_QUOT(__HANDLE__)  (READ_REG((__HANDLE__)->Instance->QUOT))

/** @brief  Get remainder value.
  * @param  __HANDLE__ HDIV handle.
  * @retval None
  */
#define __HAL_HDIV_GET_REMD(__HANDLE__)  (READ_REG((__HANDLE__)->Instance->REMD))

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HDIV_Exported_Functions
  * @{
  */
/** @defgroup HDIV_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions.
 *
  * @{
  */
HAL_StatusTypeDef HAL_HDIV_Init(HDIV_HandleTypeDef *hdiv);
HAL_StatusTypeDef HAL_HDIV_DeInit(HDIV_HandleTypeDef *hdiv);
void HAL_HDIV_MspInit(HDIV_HandleTypeDef *hdiv);
void HAL_HDIV_MspDeInit(HDIV_HandleTypeDef *hdiv);
/**
  * @}
  */

/** @addtogroup HDIV_Exported_Functions_Group2 HDIV calculate functions
  *  @brief HDIV calculate functions
  * @{
  */
HAL_StatusTypeDef HAL_HDIV_Calculate(HDIV_HandleTypeDef *hdiv, HDIV_CalculatedTypeDef* Calculated);
/**
  * @}
  */

/** @defgroup HDIV_Exported_Functions_Group3 Peripheral Control functions 
  *  @brief   HDIV control functions 
  * @{
  */
HAL_HDIV_StateTypeDef HAL_HDIV_Get_State(HDIV_HandleTypeDef *hdiv);
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

#endif /* PY32F031_HAL_HDIV_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
