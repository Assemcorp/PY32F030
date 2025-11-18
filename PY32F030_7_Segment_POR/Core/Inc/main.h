/*
 * main.h
 *
 *  Created on: Dec 9, 2023
 *      Author: Saranga
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "py32f0xx_hal.h"
#include "py32f030xx_Start_Kit.h"

/* Optional POR timeout override (ms) */
#ifndef POR_TIMEOUT_MS
#define POR_TIMEOUT_MS 3000
#endif

/* Function prototypes */
void APP_ErrorHandler(void);
void APP_Trigger_WDT_Reset(void);

#endif /* INC_MAIN_H_ */

