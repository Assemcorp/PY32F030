/**
  ******************************************************************************
  * @file    py32f031_hal_cordic.c
  * @author  MCU Application Team
  * @brief   CORDIC HAL module driver.
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
#include "py32f0xx_hal.h"

#if defined(CORDIC)
#ifdef HAL_CORDIC_MODULE_ENABLED

/** @addtogroup PY32F031_HAL_Driver
  * @{
  */

/** @defgroup CORDIC CORDIC
  * @brief CORDIC HAL driver modules.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup CORDIC_Exported_Functions CORDIC Exported Functions
  * @{
  */

/** @defgroup CORDIC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions.
  *
@verbatim
  ==============================================================================
              ##### Initialization and de-initialization functions #####
  ==============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the CORDIC peripheral and the associated handle
      (+) DeInitialize the CORDIC peripheral
      (+) Initialize the CORDIC MSP (MCU Specific Package)
      (+) De-Initialize the CORDIC MSP

    [..]

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the CORDIC peripheral and the associated handle.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_Init(CORDIC_HandleTypeDef *hcordic)
{
  /* Check the CORDIC handle allocation */
  if (hcordic == NULL)
  {
    /* Return error status */
    return HAL_ERROR;
  }

  /* Check the instance */
  assert_param(IS_CORDIC_ALL_INSTANCE(hcordic->Instance));

  if (hcordic->State == HAL_CORDIC_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hcordic->Lock = HAL_UNLOCKED;

    /* Initialize the low level hardware */
    HAL_CORDIC_MspInit(hcordic);
  }

  /* Set CORDIC error code to none */
  hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

  /* Change CORDIC peripheral state */
  hcordic->State = HAL_CORDIC_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  DeInitialize the CORDIC peripheral.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_DeInit(CORDIC_HandleTypeDef *hcordic)
{
  /* Check the CORDIC handle allocation */
  if (hcordic == NULL)
  {
    /* Return error status */
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_CORDIC_ALL_INSTANCE(hcordic->Instance));

  /* Change CORDIC peripheral state */
  hcordic->State = HAL_CORDIC_STATE_BUSY;
  
  /* Force reset CORDIC */
  __HAL_RCC_CORDIC_FORCE_RESET();

  /* Release reset CORDIC */
  __HAL_RCC_CORDIC_RELEASE_RESET();

  /* De-Initialize the low level hardware: CLOCK, NVIC */
  HAL_CORDIC_MspDeInit(hcordic);

  /* Set CORDIC error code to none */
  hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

  /* Change CORDIC peripheral state */
  hcordic->State = HAL_CORDIC_STATE_RESET;

  /* Reset Lock */
  hcordic->Lock = HAL_UNLOCKED;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initialize the CORDIC MSP.
  * @param  hcordic CORDIC handle
  * @retval None
  */
__weak void HAL_CORDIC_MspInit(CORDIC_HandleTypeDef *hcordic)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcordic);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CORDIC_MspInit can be implemented in the user file
   */
}

/**
  * @brief  DeInitialize the CORDIC MSP.
  * @param  hcordic CORDIC handle
  * @retval None
  */
__weak void HAL_CORDIC_MspDeInit(CORDIC_HandleTypeDef *hcordic)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcordic);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CORDIC_MspDeInit can be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup CORDIC_Exported_Functions_Group2 Peripheral Control functions
  *  @brief    Control functions.
  *
  * @{
  */

/**
  * @brief  Configure the CORDIC processing according to the specified
            parameters in the CORDIC_ConfigTypeDef structure.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @param  sConfig pointer to a CORDIC_ConfigTypeDef structure that
  *         contains the CORDIC configuration information.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_Configure(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_CORDIC_FUNCTION(sConfig->Function));
  assert_param(IS_CORDIC_START_MODE(sConfig->StartMode));
  assert_param(IS_CORDIC_ITERATIONS(sConfig->Iterations));

  /* Check handle state is ready */
  if (hcordic->State == HAL_CORDIC_STATE_READY)
  {
    MODIFY_REG(hcordic->Instance->CR, (CORDIC_CR_CORDIC_MODE | CORDIC_CR_START_MODE | CORDIC_CR_ITERATION), \
               (sConfig->Function | sConfig->StartMode | sConfig->Iterations));

    if(sConfig->Function == CORDIC_FUNCTION_SIN_COS)
    {
      assert_param(IS_CORDIC_INSIZE(sConfig->InSize));
      assert_param(IS_CORDIC_OUTSIZE(sConfig->OutSize));
      /* Perform sin and cos operations */
      MODIFY_REG(hcordic->Instance->CR, (CORDIC_CR_ARGSIZE | CORDIC_CR_RESSIZE), \
               (sConfig->InSize | sConfig->OutSize));
    }
    else
    {
      assert_param(IS_CORDIC_ARCTAN_MOD_IN_OUT_SIZE(sConfig->ArctanModInOutSize));
      /* Perform arctan and mod operations */
      if(sConfig->ArctanModInOutSize == CORDIC_ARCTAN_MOD_IN_OUT_SIZE_NOT_24BITS)
      {
        assert_param(IS_CORDIC_INSIZE(sConfig->InSize));
        assert_param(IS_CORDIC_OUTSIZE(sConfig->OutSize));
        MODIFY_REG(hcordic->Instance->CR, (CORDIC_CR_ARGSIZE | CORDIC_CR_RESSIZE | CORDIC_CR_VECSIZE), \
               (sConfig->InSize | sConfig->OutSize | sConfig->ArctanModInOutSize));
      }
      else
      {
        MODIFY_REG(hcordic->Instance->CR, CORDIC_CR_VECSIZE, sConfig->ArctanModInOutSize);
      }
    }
  }
  else
  {
    /* Set CORDIC error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_NOT_READY;

    /* Return error status */
    status = HAL_ERROR;
  }

  /* Return function status */
  return status;
}

/**
  * @brief  Carry out data of CORDIC processing in polling mode,
  *         according to the existing CORDIC configuration.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  sConfig pointer to a CORDIC_ConfigTypeDef structure that
  *         contains the CORDIC configuration information.
  * @param  Calculated Pointing to CORDIC calculation value structure
  * @param  Timeout Specify Timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_Calculate(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated, uint32_t Timeout)
{
  uint32_t tickstart = 0u;
  
  if (Calculated == NULL)
  {
    /* Update the error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_PARAM;

    /* Return error status */
    return HAL_ERROR;
  }
  
  /* Check handle state is ready */
  if (hcordic->State == HAL_CORDIC_STATE_READY)
  {
    /* Reset CORDIC error code */
    hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_BUSY;
    
    /* Set input values for calculation */
    if (sConfig->Function == CORDIC_FUNCTION_SIN_COS)
    {
      WRITE_REG(hcordic->Instance->THETA, Calculated->Theta);
    }
    else
    {
      WRITE_REG(hcordic->Instance->X, Calculated->X);
      WRITE_REG(hcordic->Instance->Y, Calculated->Y);
    }
    
    /* Start calculation */
    if (sConfig->StartMode == CORDIC_STARTMODE_NOT_AUTO)
    {
      SET_BIT(hcordic->Instance->CR, CORDIC_CR_START);
    }
    
    /* Waiting for calculation to complete */
    tickstart = HAL_GetTick();
    while (READ_BIT(hcordic->Instance->SR, CORDIC_SR_CCFF) != CORDIC_SR_CCFF)
    {
      if ((HAL_GetTick() - tickstart) > Timeout)
      {
        /* Set CORDIC error code */
        hcordic->ErrorCode = HAL_CORDIC_ERROR_TIMEOUT;

        /* Change the CORDIC state */
        hcordic->State = HAL_CORDIC_STATE_READY;

        /* Return function status */
        return HAL_ERROR;
      }
    }
    
    /* Clear CCFF flag */
    CLEAR_BIT(hcordic->Instance->SR, CORDIC_SR_CCFF);
    
    /* Get calculation results */
    if (sConfig->Function == CORDIC_FUNCTION_SIN_COS)
    {
      Calculated->Sin = READ_REG(hcordic->Instance->SIN);
      Calculated->Cos = READ_REG(hcordic->Instance->COS);
    }
    else
    {
      Calculated->Mod = READ_REG(hcordic->Instance->MOD);
      Calculated->Arctan = READ_REG(hcordic->Instance->ARCTAN);
    }
    
    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_READY;
    
    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Set CORDIC error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_NOT_READY;

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Carry out data of CORDIC processing in interrupt mode,
  *         according to the existing CORDIC configuration.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  sConfig pointer to a CORDIC_ConfigTypeDef structure that
  *         contains the CORDIC configuration information.
  * @param  Calculated Pointing to CORDIC calculation value structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_Calculate_IT(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated)
{
  if (Calculated == NULL)
  {
    /* Update the error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_PARAM;

    /* Return error status */
    return HAL_ERROR;
  }
  
  /* Check handle state is ready */
  if (hcordic->State == HAL_CORDIC_STATE_READY)
  {
    /* Reset CORDIC error code */
    hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_BUSY;
    
    /* Enable interrupts */
    __HAL_CORDIC_ENABLE_IT(hcordic, CORDIC_IT_CORDIC_INT);
    
    __HAL_CORDIC_ENABLE_IT_MASK(hcordic, CORDIC_IT_CORDIC_INT_MASK);
    __HAL_CORDIC_ENABLE_IT_MASK(hcordic, CORDIC_IT_CORDIC_ERROR_INT_MASK);
    
    
    /* Set input values for calculation */
    if (sConfig->Function == CORDIC_FUNCTION_SIN_COS)
    {
      WRITE_REG(hcordic->Instance->THETA, Calculated->Theta);
    }
    else
    {
      __HAL_CORDIC_ENABLE_IT_MASK(hcordic, CORDIC_IT_ARCTAN_MOD_OV_MASK);
      WRITE_REG(hcordic->Instance->X, Calculated->X);
      WRITE_REG(hcordic->Instance->Y, Calculated->Y);
    }
    
    /* Start calculation */
    if (sConfig->StartMode == CORDIC_STARTMODE_NOT_AUTO)
    {
      SET_BIT(hcordic->Instance->CR, CORDIC_CR_START);
    }
    
    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Set CORDIC error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_NOT_READY;

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Square root function of polling method.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  SqrtCalculated Pointing to SQRT calculation value structure.
  * @param  Timeout Specify Timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_SQRT_Calculate(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated, uint32_t Timeout)
{
  uint32_t tickstart = 0u;
  
  if (SqrtCalculated == NULL)
  {
    /* Update the error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_PARAM;

    /* Return error status */
    return HAL_ERROR;
  }
  
  /* Check handle state is ready */
  if (hcordic->State == HAL_CORDIC_STATE_READY)
  {
    /* Reset CORDIC error code */
    hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_BUSY;

    /* Write the radicand */
    WRITE_REG(hcordic->Instance->DSP_RAD, SqrtCalculated->Rad);
    
    /* Waiting for calculation to complete */
    tickstart = HAL_GetTick();
    while (READ_BIT(hcordic->Instance->SR, CORDIC_SR_SCFF) != CORDIC_SR_SCFF)
    {
      if ((HAL_GetTick() - tickstart) > Timeout)
      {
        /* Set CORDIC error code */
        hcordic->ErrorCode = HAL_CORDIC_ERROR_TIMEOUT;

        /* Change the CORDIC state */
        hcordic->State = HAL_CORDIC_STATE_READY;

        /* Return function status */
        return HAL_ERROR;
      }
    }
    
    /* Clear SCFF flag */
    CLEAR_BIT(hcordic->Instance->SR, CORDIC_SR_SCFF);
    
    /* Get calculation results */
    SqrtCalculated->SqrtValue = (uint16_t)READ_REG(hcordic->Instance->DSP_SQRT);
    
    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_READY;
    
    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Set CORDIC error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_NOT_READY;

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Square Root Function of Interrupt Method.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  SqrtCalculated Pointing to SQRT calculation value structure.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CORDIC_SQRT_Calculate_IT(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated)
{
  if (SqrtCalculated == NULL)
  {
    /* Update the error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_PARAM;

    /* Return error status */
    return HAL_ERROR;
  }
  
  /* Check handle state is ready */
  if (hcordic->State == HAL_CORDIC_STATE_READY)
  {
    /* Reset CORDIC error code */
    hcordic->ErrorCode = HAL_CORDIC_ERROR_NONE;

    /* Change the CORDIC state */
    hcordic->State = HAL_CORDIC_STATE_BUSY;
    
    /* Enable interrupts */
    __HAL_CORDIC_ENABLE_IT(hcordic, CORDIC_IT_SQRT_INT);
    
    __HAL_CORDIC_ENABLE_IT_MASK(hcordic, CORDIC_IT_SQRT_INT_MASK);

    /* Write the radicand */
    WRITE_REG(hcordic->Instance->DSP_RAD, SqrtCalculated->Rad);
    
    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Set CORDIC error code */
    hcordic->ErrorCode |= HAL_CORDIC_ERROR_NOT_READY;

    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @}
  */

/** @defgroup CORDIC_Exported_Functions_Group3 Callback functions
  *  @brief    Callback functions.
  *
  * @{
  */

/**
  * @brief  CORDIC calculate complete callback.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @retval None
  */
__weak void HAL_CORDIC_CalculateCpltCallback(CORDIC_HandleTypeDef *hcordic)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcordic);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_CORDIC_CalculateCpltCallback can be implemented in the user file
   */
}

/**
  * @brief  CORDIC error callback.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @retval None
  */
__weak void HAL_CORDIC_ErrorCallback(CORDIC_HandleTypeDef *hcordic)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcordic);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_CORDIC_ErrorCallback can be implemented in the user file
   */
}

/**
  * @brief  SQRT calculate complete callback.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @retval None
  */
__weak void HAL_CORDIC_SQRT_CalculateCpltCallback(CORDIC_HandleTypeDef *hcordic)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcordic);

  /* NOTE : This function should not be modified; when the callback is needed,
            the HAL_CORDIC_SQRT_CalculateCpltCallback can be implemented in the user file
   */
}

/**
  * @}
  */


/** @defgroup CORDIC_Exported_Functions_Group4 IRQ handler management
  *  @brief    IRQ handler.
  *
  * @{
  */

/**
  * @brief  Handle CORDIC interrupt request.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  sConfig pointer to a CORDIC_ConfigTypeDef structure that
  *         contains the CORDIC configuration information.
  * @param  Calculated Pointing to CORDIC calculation value structure
  * @retval None
  */
void HAL_CORDIC_IRQHandler(CORDIC_HandleTypeDef *hcordic, CORDIC_ConfigTypeDef *sConfig, \
                                        CORDIC_CalculatedTypeDef *Calculated)
{
  if (__HAL_CORDIC_GET_IT(hcordic, CORDIC_IT_CORDIC_INT) != RESET)
  {
    /* CORDIC calculation completed and processed */
    if ((__HAL_CORDIC_GET_FLAG(hcordic, CORDIC_FLAG_CCFF) != RESET) && (__HAL_CORDIC_GET_IT_MASK(hcordic, CORDIC_IT_CORDIC_INT_MASK) != RESET))
    {
      /* Clear CCFF flag */
      __HAL_CORDIC_CLEAR_FLAG(hcordic, CORDIC_FLAG_CCFF);
      
      /* Get calculation results */
      if (sConfig->Function == CORDIC_FUNCTION_SIN_COS)
      {
        Calculated->Sin = READ_REG(hcordic->Instance->SIN);
        Calculated->Cos = READ_REG(hcordic->Instance->COS);
      }
      else
      {
        Calculated->Mod = READ_REG(hcordic->Instance->MOD);
        Calculated->Arctan = READ_REG(hcordic->Instance->ARCTAN);
      }
      
      /* Change the CORDIC state */
      hcordic->State = HAL_CORDIC_STATE_READY;
      
      /* Call legacy weak callback */
      HAL_CORDIC_CalculateCpltCallback(hcordic);
    }
    
    /* CORDIC input data error interrupt */
    if ((__HAL_CORDIC_GET_FLAG(hcordic, CORDIC_FLAG_CCEF) != RESET) && (__HAL_CORDIC_GET_IT_MASK(hcordic, CORDIC_IT_CORDIC_ERROR_INT_MASK) != RESET))
    {
      /* Clear CCEF flag */
      __HAL_CORDIC_CLEAR_FLAG(hcordic, CORDIC_FLAG_CCEF);
      
      /* Change the CORDIC state */
      hcordic->State = HAL_CORDIC_STATE_READY;
      
      /* Update the error code */
      hcordic->ErrorCode |= HAL_CORDIC_ERROR_CCEF;
      
      /* Call legacy weak callback */
      HAL_CORDIC_ErrorCallback(hcordic);
    }
    
    /* Arctan and mod function calculation overflow interrupt */
    if ((__HAL_CORDIC_GET_FLAG(hcordic, CORDIC_FLAG_ACEF) != RESET) && (__HAL_CORDIC_GET_IT_MASK(hcordic, CORDIC_IT_ARCTAN_MOD_OV_MASK) != RESET))
    {
      /* Clear CCEF flag */
      __HAL_CORDIC_CLEAR_FLAG(hcordic, CORDIC_FLAG_ACEF);
      
      /* Change the CORDIC state */
      hcordic->State = HAL_CORDIC_STATE_READY;
      
      /* Update the error code */
      hcordic->ErrorCode |= HAL_CORDIC_ERROR_ACEF;
      
      /* Call legacy weak callback */
      HAL_CORDIC_ErrorCallback(hcordic);
    }

    /* Disble interupt*/
    __HAL_CORDIC_DISABLE_IT(hcordic, CORDIC_IT_CORDIC_INT);
    
    __HAL_CORDIC_DISABLE_IT_MASK(hcordic, CORDIC_IT_CORDIC_INT_MASK);
    __HAL_CORDIC_DISABLE_IT_MASK(hcordic, CORDIC_IT_CORDIC_ERROR_INT_MASK);
    __HAL_CORDIC_DISABLE_IT_MASK(hcordic, CORDIC_IT_ARCTAN_MOD_OV_MASK);
  }
}

/**
  * @brief  Handle SQRT interrupt request.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module.
  * @param  SqrtCalculated Pointing to SQRT calculation value structure.
  * @retval None
  */
void HAL_CORDIC_SQRT_IRQHandler(CORDIC_HandleTypeDef *hcordic,\
                                       CORDIC_SQRT_CalculatedTypeDef *SqrtCalculated)
{
  if (__HAL_CORDIC_GET_IT(hcordic, CORDIC_IT_SQRT_INT) != RESET)
  {
    /* CORDIC calculation completed and processed */
    if ((__HAL_CORDIC_GET_FLAG(hcordic, CORDIC_FLAG_SCFF) != RESET) && (__HAL_CORDIC_GET_IT_MASK(hcordic, CORDIC_IT_SQRT_INT_MASK) != RESET))
    {
      /* Clear CCFF flag */
      __HAL_CORDIC_CLEAR_FLAG(hcordic, CORDIC_FLAG_SCFF);
      
      /* Get calculation results */
      SqrtCalculated->SqrtValue = (uint16_t)READ_REG(hcordic->Instance->DSP_SQRT);
      
      /* Change the CORDIC state */
      hcordic->State = HAL_CORDIC_STATE_READY;
      
      /* Call legacy weak callback */
      HAL_CORDIC_SQRT_CalculateCpltCallback(hcordic);
    }
    
    /* Disble interupt*/
    __HAL_CORDIC_DISABLE_IT(hcordic, CORDIC_IT_SQRT_INT);
    
    __HAL_CORDIC_DISABLE_IT_MASK(hcordic, CORDIC_IT_SQRT_INT_MASK);
  }
}

/**
  * @}
  */

/** @defgroup CORDIC_Exported_Functions_Group5 Peripheral State functions
  *  @brief   Peripheral State functions.
  *
@verbatim
  ==============================================================================
                      ##### Peripheral State functions #####
  ==============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Return the CORDIC handle state.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @retval HAL state
  */
HAL_CORDIC_StateTypeDef HAL_CORDIC_GetState(CORDIC_HandleTypeDef *hcordic)
{
  /* Return CORDIC handle state */
  return hcordic->State;
}

/**
  * @brief  Return the CORDIC peripheral error.
  * @param  hcordic pointer to a CORDIC_HandleTypeDef structure that contains
  *         the configuration information for CORDIC module
  * @note   The returned error is a bit-map combination of possible errors
  * @retval Error bit-map
  */
uint32_t HAL_CORDIC_GetError(CORDIC_HandleTypeDef *hcordic)
{
  /* Return CORDIC error code */
  return hcordic->ErrorCode;
}

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
#endif /* HAL_CORDIC_MODULE_ENABLED */
#endif /* CORDIC */
