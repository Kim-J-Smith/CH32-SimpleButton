/**
 * @file            sBtn_ch32_tick.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Header file to declare ch32 tick APIs.
 * 
 * @version         0.2.1 ( 0011L )
 * 
 * @date            2025-10-03
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       SPDX-License-Identifier: MIT
 *                  Refer to the LICENCE file in root for more details.
 *                  <https://github.com/Kim-J-Smith/CH32-SimpleButton>
 */
#ifndef     SIMPLEBUTTON_CH32_TICK_H__
#define     SIMPLEBUTTON_CH32_TICK_H__      0011L

#include    <stdint.h>
#include    "debug.h"
// # include "ch32v10x.h"
// # include "ch32v20x.h"
// # include "ch32v30x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void HAL_InitTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SIMPLEBUTTON_CH32_TICK_H__ */
