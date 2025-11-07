/**
  ******************************************************************************
  * @file    py32f031_hal_cordic.h
  * @author  MCU Application Team
  * @brief   This file contains all the functions prototypes for the CORDIC
             firmware library.
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
#ifndef PY32F031_HAL_CORDIC_H
#define PY32F031_HAL_CORDIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f031_hal_def.h"

#if defined(CORDIC)
/** @addtogroup PY32F031_HAL_Driver
  * @{
  */

/** @addtogroup CORDIC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup CORDIC_Exported_Types CORDIC Exported Types
  * @{
  */

/**
  * @brief  CORDIC HAL State Structure definition
  */
typedef enum
{
  HAL_CORDIC_STATE_RESET     = 0x00U,  /*!< CORDIC not yet initialized or disabled */
  HAL_CORDIC_STATE_READY     = 0x01U,  /*!< CORDIC initialized and ready for use   */
  HAL_CORDIC_STATE_BUSY      = 0x02U,  /*!< CORDIC internal process is ongoing     */
  HAL_CORDIC_STATE_ERROR     = 0x03U   /*!< CORDIC error state                     */
} HAL_CORDIC_StateTypeDef;

/**
  * @brief  CORDIC Handle Structure definition
  */
typedef struct
{
  CORDIC_TypeDef                *Instance;   /*!< Register base address */

  HAL_LockTypeDef               Lock;        /*!< CORDIC locking object */

  __IO HAL_CORDIC_StateTypeDef  State;       /*!< CORDIC state */

  __IO uint32_t                 ErrorCode;   /*!< CORDIC peripheral error code
                                                  This parameter can be a value of @ref CORDIC_Error_Code */

} CORDIC_HandleTypeDef;

/**
  * @brief  CORDIC Config Structure definition
  */
typedef struct
{
  uint32_t   Function;                 /*!< Function
                                            This parameter can be a value of @ref CORDIC_Function */
                              
  uint32_t   InSize;                   /*!< Width of input data
                                            This parameter can be a value of @ref CORDIC_In_Size */
                              
  uint32_t   OutSize;                  /*!< Width of output data
                                            This parameter can be a value of @ref CORDIC_Out_Size */
  
  uint32_t   ArctanModInOutSize;       /*!< Arctan and mod data bit width
                                            Note: When performing arctan and mod operations, when ArctanModInOutSize
                                                  is configured to 24 bits, the Insize and OutSize members of the current
                                                  structure are "not functional"; When ArctanModInOutSize is configured to
                                                  be non 24 bit, the Inseze and OutSize members of the current structure
                                                  'take effect'.
                                            This parameter can be a value of @ref CORDIC_Arctan_Mod_In_Out_Size */

  uint32_t   StartMode;                /*!< Start Mode
                                            This parameter can be a value of @ref CORDIC_Start_Mode */

  uint32_t   Iterations;               /*!< Iterations
                                            This parameter must be a number between Min_Data = 1 and Max_Data = 24 */

} CORDIC_ConfigTypeDef;

/**
  * @brief  CORDIC Calculated value Structure definition
  */
typedef struct
{
  int32_t Theta;                      /*!< Input angle */
  
  int32_t X;                          /*!< Input coordinate X */
  
  int32_t Y;                          /*!< Input coordinate Y */
  
  int32_t Sin;                        /*!< SIN calculation result output */
  
  int32_t Cos;                        /*!< COS calculation result output */
  
  int32_t Mod;                        /*!< MOD calculation result output */
  
  int32_t Arctan;                     /*!< ARCTAN calculation result output */

} CORDIC_CalculatedTypeDef;

/**
  * @brief  SQRT Calculated value Structure definition
  */
typedef struct
{
  uint32_t Rad;                        /*!< radicand */
  
  uint16_t SqrtValue;                  /*!< Square Root Result */

} CORDIC_SQRT_CalculatedTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup CORDIC_Exported_Constants CORDIC Exported Constants
  * @{
  */

/** @defgroup CORDIC_Error_Code CORDIC Error code
  * @{
  */
#define HAL_CORDIC_ERROR_NONE              ((uint32_t)0x00000000U)   /*!< No error                */
#define HAL_CORDIC_ERROR_PARAM             ((uint32_t)0x00000001U)   /*!< Wrong parameter error   */
#define HAL_CORDIC_ERROR_NOT_READY         ((uint32_t)0x00000002U)   /*!< Peripheral not ready    */
#define HAL_CORDIC_ERROR_TIMEOUT           ((uint32_t)0x00000004U)   /*!< Timeout error           */
#define HAL_CORDIC_ERROR_CCEF              ((uint32_t)0x00000008U)   /*!< CORDIC input data error error */
#define HAL_CORDIC_ERROR_ACEF              ((uint32_t)0x00000010U)   /*!< CORDIC input data error error */
/**
  * @}
  */

/** @defgroup CORDIC_Function CORDIC Function
  * @{
  */
#define CORDIC_FUNCTION_ARCTAN_MOD   (0x00000000U)               /*!< Arctan and Mod operation */
#define CORDIC_FUNCTION_SIN_COS      (CORDIC_CR_CORDIC_MODE)     /*!< Cosine and Sine operation*/
/**
  * @}
  */

/** @defgroup CORDIC_In_Size CORDIC input data size
  * @{
  */
#define CORDIC_INSIZE_32BITS       (0x00000000U)             /*!< 32 bits input data size (Q1.31 format) */
#define CORDIC_INSIZE_16BITS       CORDIC_CR_ARGSIZE         /*!< 16 bits input data size (Q1.15 format) */
/**
  * @}
  */

/** @defgroup CORDIC_Out_Size CORDIC Results Size
  * @{
  */
#define CORDIC_OUTSIZE_32BITS      (0x00000000U)             /*!< 32 bits output data size (Q1.31 format) */
#define CORDIC_OUTSIZE_16BITS      CORDIC_CR_RESSIZE         /*!< 16 bits output data size (Q1.15 format) */
/**
  * @}
  */

/** @defgroup CORDIC_Arctan_Mod_In_Out_Size CORDIC Arctan Mod In Out Size
  * @{
  */
#define CORDIC_ARCTAN_MOD_IN_OUT_SIZE_NOT_24BITS      (0x00000000U)             /*!< Determined by the RESSIZE and ARGSIZE registers */
#define CORDIC_ARCTAN_MOD_IN_OUT_SIZE_24BITS          CORDIC_CR_VECSIZE         /*!< Arctan/mod input and output data with a bit width of 24 bits */
/**
  * @}
  */

/** @defgroup CORDIC_Start_Mode CORDIC Start Mode
  * @{
  */
#define CORDIC_STARTMODE_AUTO      (0x00000000U)             /*!< Start calculation after performing write operations on registers CORDIC_THETA, CORDIC_X,CORDIC_Y */
#define CORDIC_STARTMODE_NOT_AUTO  CORDIC_CR_START_MODE      /*!< Calculate when the position of the START register is 1 */
/**
  * @}
  */
/** @defgroup CORDIC_Interrupts_Enable CORDIC Interrupts Enable bit
  * @{
  */
#define CORDIC_IT_CORDIC_INT              CORDIC_CR_CORDIC_INT
#define CORDIC_IT_CORDIC_INT_MASK         CORDIC_CR_CORDIC_INT_MASK
#define CORDIC_IT_CORDIC_ERROR_INT_MASK   CORDIC_CR_CORDIC_ERROR_INT_MASK
#define CORDIC_IT_ARCTAN_MOD_OV_MASK      CORDIC_CR_ARCTAN_MOD_OV_MASK

#define CORDIC_IT_SQRT_INT                CORDIC_CR_SQRT_INT
#define CORDIC_IT_SQRT_INT_MASK           CORDIC_CR_SQRT_INT_MASK
/**
  * @}
  */

/** @defgroup CORDIC_Flags  CORDIC status flags
  * @{
  */
#define CORDIC_FLAG_CCFF           CORDIC_SR_CCFF
#define CORDIC_FLAG_CCEF           CORDIC_SR_CCEF
#define CORDIC_FLAG_ACEF           CORDIC_SR_ACEF
#define CORDIC_FLAG_BSY            CORDIC_SR_BSY

#define CORDIC_FLAG_SCFF           CORDIC_SR_SCFF
/**
  * @}
  */

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/** @defgroup CORDIC_Exported_Macros CORDIC Exported Macros
  * @{
  */
/** @brief  Reset CORDIC handle state.
  * @param  __HANDLE__ CORDIC handle
  * @retval None
  */
#define __HAL_CORDIC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_CORDIC_STATE_RESET)

/**
  * @brief  Enable the CORDIC interrupt
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC Interrupt.
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT
  *            @arg @ref CORDIC_IT_SQRT_INT
  * @retval None
  */
#define __HAL_CORDIC_ENABLE_IT(__HANDLE__, __INTERRUPT__)                     \
  (((__HANDLE__)->Instance->CR) |= (__INTERRUPT__))

/**
  * @brief  Disable the CORDIC interrupt
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC Interrupt.
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT
  *            @arg @ref CORDIC_IT_SQRT_INT
  * @retval None
  */
#define __HAL_CORDIC_DISABLE_IT(__HANDLE__, __INTERRUPT__)                    \
  (((__HANDLE__)->Instance->CR) &= ~(__INTERRUPT__))

/** @brief  Check whether the specified CORDIC interrupt is enabled or not.
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC interrupt to check
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT 
  *            @arg @ref CORDIC_IT_SQRT_INT 
  * @retval FlagStatus
  */
#define __HAL_CORDIC_GET_IT(__HANDLE__, __INTERRUPT__)                 \
  ((((__HANDLE__)->Instance->CR) & (__INTERRUPT__)) == (__INTERRUPT__))

/**
  * @brief  Enable the CORDIC interrupt mask
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC Interrupt.
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT_MASK 
  *            @arg @ref CORDIC_IT_CORDIC_ERROR_INT_MASK 
  *            @arg @ref CORDIC_IT_ARCTAN_MOD_OV_MASK
  *            @arg @ref CORDIC_IT_SQRT_INT_MASK 
  * @retval None
  */
#define __HAL_CORDIC_ENABLE_IT_MASK(__HANDLE__, __INTERRUPT__)                     \
  (((__HANDLE__)->Instance->CR) &= ~(__INTERRUPT__))

/**
  * @brief  Disable the CORDIC interrupt mask
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC Interrupt.
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT_MASK 
  *            @arg @ref CORDIC_IT_CORDIC_ERROR_INT_MASK 
  *            @arg @ref CORDIC_IT_ARCTAN_MOD_OV_MASK
  *            @arg @ref CORDIC_IT_SQRT_INT_MASK 
  * @retval None
  */
#define __HAL_CORDIC_DISABLE_IT_MASK(__HANDLE__, __INTERRUPT__)                    \
  (((__HANDLE__)->Instance->CR) |= (__INTERRUPT__))

/** @brief  Check whether the specified CORDIC interrupt mask is enabled or not.
  * @param  __HANDLE__ CORDIC handle.
  * @param  __INTERRUPT__ CORDIC interrupt mask to check
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_IT_CORDIC_INT_MASK 
  *            @arg @ref CORDIC_IT_CORDIC_ERROR_INT_MASK 
  *            @arg @ref CORDIC_IT_ARCTAN_MOD_OV_MASK
  *            @arg @ref CORDIC_IT_SQRT_INT_MASK 
  * @retval FlagStatus
  */
#define __HAL_CORDIC_GET_IT_MASK(__HANDLE__, __INTERRUPT__)                 \
  ((((__HANDLE__)->Instance->CR) & (__INTERRUPT__)) == 0x0U)


/** @brief  Check whether the specified CORDIC status flag is set or not.
  * @param  __HANDLE__ CORDIC handle.
  * @param  __FLAG__ CORDIC flag to check
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_FLAG_CCFF
  *            @arg @ref CORDIC_FLAG_CCEF
  *            @arg @ref CORDIC_FLAG_ACEF
  *            @arg @ref CORDIC_FLAG_BSY
  *            @arg @ref CORDIC_FLAG_SCFF
  * @retval SET (flag is set) or RESET (flag is reset)
  */
#define __HAL_CORDIC_GET_FLAG(__HANDLE__, __FLAG__)                           \
  ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

/** @brief  Clear specified CORDIC status flag.
            flag can be cleared.
  * @param  __HANDLE__ CORDIC handle.
  * @param  __FLAG__ CORDIC flag to clear
  *         This parameter can be one of the following values:
  *            @arg @ref CORDIC_FLAG_CCFF
  *            @arg @ref CORDIC_FLAG_CCEF
  *            @arg @ref CORDIC_FLAG_ACEF
  *            @arg @ref CORDIC_FLAG_SCFF
  * @retval None
  */
#define __HAL_CORDIC_CLEAR_FLAG(__HANDLE__, __FLAG__)                           \
  (((__HANDLE__)->Instance->SR) &= ~(__FLAG__))


/**
  * @}
  */

/* Private macros --------------------------------------------------------*/
/** @defgroup  CORDIC_Private_Macros   CORDIC Private Macros
  * @{
  */
#define IS_CORDIC_FUNCTION(__FUNCTION__) (((__FUNCTION__) == CORDIC_FUNCTION_ARCTAN_MOD) || \
                                          ((__FUNCTION__) == CORDIC_FUNCTION_SIN_COS))
                                          
#define IS_CORDIC_START_MODE(__MODE__) (((__MODE__) == CORDIC_STARTMODE_AUTO) || \
                                        ((__MODE__) == CORDIC_STARTMODE_NOT_AUTO))
                                        
#define IS_CORDIC_ITERATIONS(__ITERATIONS__) (((__ITERATIONS__) >= 1U) && \
                                              ((__ITERATIONS__) <= 24u))
                                              
#define IS_CORDIC_INSIZE(__SIZE__) (((__SIZE__) == CORDIC_INSIZE_32BITS) || \
                                    ((__SIZE__) == CORDIC_INSIZE_16BITS))
                                    
#define IS_CORDIC_OUTSIZE(__SIZE__) (((__SIZE__) == CORDIC_OUTSIZE_32BITS) || \
                                     ((__SIZE__) == CORDIC_OUTSIZE_16BITS))
                                     
#define IS_CORDIC_ARCTAN_MOD_IN_OUT_SIZE(__SIZE__) (((__SIZE__) == CORDIC_ARCTAN_MOD_IN_OUT_SIZE_NOT_24BITS) || \
                                                    ((__SIZE__) == CORDIC_ARCTAN_MOD_IN_OUT_SIZE_24BITS))
/**
  * @}
  */

/** @addtogroup CORDIC_Exported_Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */

/** @addtogroup CORDIC_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions ******************************/
HAL_StatusTypeDef HAL_CORDIC_Init(CORDIC_HandleTypeDef *hcordic);
HAL_StatusTypeDef HAL_CORDIC_DeInit(CORDIC_HandleTypeDef *hcordic);
void HAL_CORDIC_MspInit(CORDIC_HandleTypeDef *hcordic);
void HAL_CORDIC_MspDeInit(CORDIC_HandleTypeDef *hcordic);
/**
  * @}
  */

/** @addtogroup CORDIC_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_CORDIC_Configure(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig);
HAL_StatusTypeDef HAL_CORDIC_Calculate(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated, uint32_t Timeout);
HAL_StatusTypeDef HAL_CORDIC_Calculate_IT(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated);
HAL_StatusTypeDef HAL_CORDIC_SQRT_Calculate(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated, uint32_t Timeout);
HAL_StatusTypeDef HAL_CORDIC_SQRT_Calculate_IT(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated);
/**
  * @}
  */

/** @addtogroup CORDIC_Exported_Functions_Group3
  * @{
  */
/* Callback functions *********************************************************/
void HAL_CORDIC_CalculateCpltCallback(CORDIC_HandleTypeDef *hcordic);
void HAL_CORDIC_ErrorCallback(CORDIC_HandleTypeDef *hcordic);
void HAL_CORDIC_SQRT_CalculateCpltCallback(CORDIC_HandleTypeDef *hcordic);
/**
  * @}
  */

/** @addtogroup CORDIC_Exported_Functions_Group4
  * @{
  */
/* IRQ handler management *****************************************************/
void HAL_CORDIC_IRQHandler(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated);
void HAL_CORDIC_SQRT_IRQHandler(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated);
/**
  * @}
  */

/** @addtogroup CORDIC_Exported_Functions_Group5
  * @{
  */
/* Peripheral State functions *************************************************/
HAL_CORDIC_StateTypeDef HAL_CORDIC_GetState(CORDIC_HandleTypeDef *hcordic);
uint32_t HAL_CORDIC_GetError(CORDIC_HandleTypeDef *hcordic);
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

#endif /* CORDIC */

#ifdef __cplusplus
}
#endif

#endif /* PY32F031_HAL_CORDIC_H */
