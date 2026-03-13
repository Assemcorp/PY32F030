/**
  ******************************************************************************
  * @file    py32f031_hal_hdiv.c
  * @author  MCU Application Team
  * @brief   HDIV HAL module driver.
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

/** @addtogroup PY32F031_HAL_Driver
  * @{
  */

/** @defgroup HDIV HDIV
  * @brief    HDIV HAL module driver.
  * @{
  */

#ifdef HAL_HDIV_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup HDIV_Private_Constants HDIV Private Constants
  * @{
  */
#define HDIV_TIMEOUT_VALUE          (2U)    /* 2 ms (minimum Tick + 1) */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @defgroup HDIV_Exported_Functions HDIV Exported Functions
  * @{
  */

/** @defgroup HDIV_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions.
 *
  * @{
  */

/**
  * @brief  Initialize the HDIV according to the specified
  *         parameters in the HDIV_InitTypeDef and create the associated handle.
  * @param  hdiv HDIV handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HDIV_Init(HDIV_HandleTypeDef *hdiv)
{
  /* Check the HDIV handle allocation */
  if (hdiv == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_HDIV_ALL_INSTANCE(hdiv->Instance));

  if (hdiv->State == HAL_HDIV_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hdiv->Lock = HAL_UNLOCKED;
    /* Init the low level hardware */
    HAL_HDIV_MspInit(hdiv);
  }

  /* Change HDIV peripheral state */
  hdiv->State = HAL_HDIV_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  DeInitialize the HDIV peripheral.
  * @param  hdiv HDIV handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HDIV_DeInit(HDIV_HandleTypeDef *hdiv)
{
  /* Check the HDIV handle allocation */
  if (hdiv == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_HDIV_ALL_INSTANCE(hdiv->Instance));

  /* Check the HDIV peripheral state */
  if (hdiv->State == HAL_HDIV_STATE_BUSY)
  {
    return HAL_BUSY;
  }

  /* Change HDIV peripheral state */
  hdiv->State = HAL_HDIV_STATE_BUSY;

  /* Force reset HDIV */
  __HAL_RCC_HDIV_FORCE_RESET();

  /* Release reset HDIV */
  __HAL_RCC_HDIV_RELEASE_RESET();

  /* DeInit the low level hardware */
  HAL_HDIV_MspDeInit(hdiv);

  /* Change DIV peripheral state */
  hdiv->State = HAL_HDIV_STATE_RESET;

  /* Process unlocked */
  __HAL_UNLOCK(hdiv);

  /* Return function status */
  return HAL_OK;

}

/**
  * @brief  Initializes the HDIV MSP.
  * @param  hdiv HDIV handle
  * @retval None
  */
void HAL_HDIV_MspInit(HDIV_HandleTypeDef *hdiv)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdiv);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HDIV_MspInit can be implemented in the user file
   */
}

/**
  * @brief  DeInitialize the HDIV MSP.
  * @param  hdiv HDIV handle
  * @retval None
  */
void HAL_HDIV_MspDeInit(HDIV_HandleTypeDef *hdiv)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdiv);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_HDIV_MspDeInit can be implemented in the user file
   */
}

/**
  * @}
  */

/** @addtogroup HDIV_Exported_Functions_Group2 HDIV calculate functions
  *  @brief HDIV calculate functions
  * @{
  */

/**
  * @brief  Calculate the division result.
  * @param  hdiv HDIV handle
  * @param  Calculated Pointing to HDIV calculation value structure
  * @retval HAL_State.
  */
HAL_StatusTypeDef HAL_HDIV_Calculate(HDIV_HandleTypeDef *hdiv, HDIV_CalculatedTypeDef* Calculated)
{
  uint32_t tickstart;

  /* Check the HDIV handle allocation */
  if (hdiv == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_HDIV_ALL_INSTANCE(hdiv->Instance));

  MODIFY_REG(hdiv->Instance->SIGN, HDIV_SIGN_SIGN, (Calculated->Sign));

  WRITE_REG(hdiv->Instance->DEND, (Calculated-> Dividend));

  WRITE_REG(hdiv->Instance->SOR, (Calculated-> Divisor));

  if(READ_BIT(hdiv->Instance->STAT, HDIV_STAT_ZERO) != 0)
  {
    hdiv->State = HAL_HDIV_STATE_ZERO;
    return HAL_ERROR;
  }

  tickstart = HAL_GetTick();

  while (READ_BIT(hdiv->Instance->STAT, HDIV_STAT_END) != HDIV_STAT_END)
  {
    if ((HAL_GetTick() - tickstart) > HDIV_TIMEOUT_VALUE)
    {
      hdiv->State = HAL_HDIV_STATE_TIMEOUT;
      return HAL_TIMEOUT;
    }
  }

  hdiv->State = HAL_HDIV_STATE_END;

  Calculated->Quotient = __HAL_HDIV_GET_QUOT(hdiv);

  Calculated->Remainder = __HAL_HDIV_GET_REMD(hdiv);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HDIV_Exported_Functions_Group3 Peripheral Control functions 
  *  @brief   HDIV control functions 
  * @{
  */

/**
  * @brief  Get HDIV State.
  * @param  hdiv HDIV handle
  * @retval HDIV State.
  */
HAL_HDIV_StateTypeDef HAL_HDIV_Get_State(HDIV_HandleTypeDef *hdiv)
{
  return hdiv->State;
}

/**
  * @}
  */

/**
  * @}
  */


#endif /* HAL_HDIV_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
