/**
 * @file            sBtn_ch32_tick.c
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Header file to define ch32 tick APIs.
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
#include    "sBtn_ch32_tick.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* WEAK */
#if defined(__GNUC__) || defined(__clang__)
 #define WEAK   __attribute__((weak))
#elif defined(__CC_ARM)
 #define WEAK   __weak
#else 
 #define WEAk   
#endif /* defined(__GNUC__) || defined(__clang__) */

#if defined(__riscv)

 /* This CH32 chip may use Qingke core with RISCV Arch */

 /* static global variable */
 static uint32_t g_systick_div = 0;

 #define SIMPLEBTN_SYSTICK_NOT_INIT 0

 #define SIMPLEBTN_SYSTICK_IS_INIT  0x55

 static uint8_t g_systick_is_init = SIMPLEBTN_SYSTICK_NOT_INIT;

 /**
  * @brief  This function configures the source of the time base.
  * 
  * @note   The function is declared as WEAK to be overwritten  in case of other
  *         implementation  in user file.
  * 
  * @return None 
  */
 WEAK void HAL_InitTick(void)
 {
    if (g_systick_is_init == SIMPLEBTN_SYSTICK_IS_INIT) {
        return; /* has been initialized */
    }

#ifndef __CH32V10x_H
    /* SysTick configure */
    const uint32_t SysTick_Msk_Mode = (uint32_t)(1U << 4);
    const uint32_t SysTick_Msk_STCLK = (uint32_t)(1U << 2);
    const uint32_t SysTick_Msk_SWIE = (uint32_t)(1U << 31);

    const uint32_t SysTick_Msk_Init = (uint32_t)(1U << 5);
    const uint32_t SysTick_Msk_Enable = (uint32_t)(1U << 0);

    /* use Mode-Up Clock-Div8 SWInterrupt-Disable */
    SysTick->CTLR &= ~(SysTick_Msk_Mode | SysTick_Msk_STCLK | SysTick_Msk_SWIE);

    /* Init and Enable */
    SysTick->CTLR |= SysTick_Msk_Init | SysTick_Msk_Enable;

    /* Init the global var */
    g_systick_div = (SystemCoreClock / (SysTick_Msk_STCLK ? (8000) : (1000)));
#else
    /* SysTick configure */
    const uint32_t SysTick_Msk_Enable_with_8Div = (uint32_t)(1U << 0);
    SysTick->CTLR |= SysTick_Msk_Enable_with_8Div;

    /* Init the global var */
    g_systick_div = SystemCoreClock / 8000;
#endif /* __CH32V10x_H */

    /* global var */
    g_systick_is_init = SIMPLEBTN_SYSTICK_IS_INIT;
 }

 /**
  * @brief  Provides a tick value in millisecond.
  * 
  * @note   The function is declared as WEAK to be overwritten  in case of other
  *         implementation  in user file.
  * 
  * @return tick value.
  */
 WEAK uint32_t HAL_GetTick(void)
 {
#ifndef __CH32V10x_H
    return (uint32_t) (SysTick->CNT / g_systick_div);
#else
    volatile uint64_t* P_CNT = (uint64_t*) &(SysTick->CNTL0);
    return (uint32_t) ( (*P_CNT) / g_systick_div);
#endif /* __CH32V10x_H */
 }

 WEAK void HAL_Delay(uint32_t Delay)
 {
    uint32_t current = HAL_GetTick();
    while (HAL_GetTick() - current < Delay);
 }

#endif /* defined(__riscv) */


#ifdef __cplusplus
}
#endif /* __cplusplus */
