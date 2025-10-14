/**
 * @file            kim_ch32v_delay.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Kim Library to offer delay_xxx [CH32V]
 * 
 * @version         0.0.1 ( 0001L )
 *                  (match with ch32vxxx.h)
 * 
 * @date            2025-09-06
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       This project complies with the MIT License.
 *                  Refer to the LICENCE in root for more details.
 */
# include <stdint.h>

# include "ch32v20x.h"
// # include "ch32v30x.h"
// # include "ch32v10x.h"


static inline void Kim_SysTick_UpCountDiv8NoInterrupt_Init(void)
{
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
}

static inline uint32_t Kim_GetTick(void)
{
    return (uint32_t)(SysTick->CNT / (SystemCoreClock / 8000));
}

static inline void Kim_Delay_Init(void)
{
    Kim_SysTick_UpCountDiv8NoInterrupt_Init();
}

static inline void Kim_Delay_ms(uint32_t time_ms)
{
    uint64_t cmp = time_ms * (SystemCoreClock / 8000);
    uint64_t stamp = SysTick->CNT;
    while(SysTick->CNT - stamp < cmp);
}

static inline void Kim_Delay_us(uint32_t time_us)
{
    uint64_t cmp = time_us * (SystemCoreClock / 8000000);
    uint64_t stamp = SysTick->CNT;
    while(SysTick->CNT - stamp < cmp);
}

