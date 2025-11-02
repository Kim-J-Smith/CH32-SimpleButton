/**
 * @file            simple_button_config.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           The configuration file of the Simple_Button file.
 * 
 * @version         0.8.x
 * 
 * @date            2025-10-03
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       SPDX-License-Identifier: MIT
 *                  Refer to the LICENCE file in root for more details.
 * 
 *                  <https://github.com/Kim-J-Smith/Simple-Button>
 *                  <https://github.com/Kim-J-Smith/CH32-SimpleButton>
 */
#ifndef     SIMPLEBUTTON_CONFIG_H__
#define     SIMPLEBUTTON_CONFIG_H__     1019L
#include <stdint.h>

/** @p ================================================================ **/
/* ====================== CUSTOMIZATION BEGIN ========================== */


/*************************************************************************
 * @b Contents
 * 
 * 1. @e Head-File (Your head file depending on your chip)
 * 
 * 2. @e Simple-Button-Type (The Types used by Simple-Button)
 * 
 * 3. @e Other-Functions (Functions for read-pin, debug, low-power, ...)
 * 
 * 4. @e Time-Set (To set the adjustable time, such as repeat-push window. 
 *                  All these times will be set as default times, 
 *                  which can still be changed for each button.)
 * 
 * 5. @e Mode-Set (The mode of button, such as combination / timer-long-push / ...)
 * 
 * 6. @e Namespace (The namespace of Simple-Button public functions and variables)
 * 
 * 7. @e Initialization-Function (To init the GPIO, EXTI, NVIC/PFIC, etc)
 * 
 * 8. @e NVIC-EXTI-Priority (To specify the Priority of button)
 * 
 *************************************************************************
 */

/** @b ================================================================ **/
/** @b Head-File */
#include "sBtn_ch32_tick.h"

/** @b ================================================================ **/
/** @b Simple-Button-Type */

// The type of GPIOX_BASE. (X can be A / B / C ...)
typedef uint32_t            simpleButton_Type_GPIOBase_t;
// The type of GPIO_PIN_X. (X can be 0 / 1 / 2 / 3 / ...)
typedef uint16_t            simpleButton_Type_GPIOPin_t;
// The type of Value of Pin. (The value usually be 0 or 1)
typedef uint8_t             simpleButton_Type_GPIOPinVal_t;
// The type of EXTI_TRIGGER_X. (X can be FALLING / RAISING / ...)
typedef EXTITrigger_TypeDef simpleButton_Type_EXTITrigger_t;

#define SIMPLEBTN_EXTI_TRIGGER_FALLING      EXTI_Trigger_Falling

/** @b ================================================================ **/
/** @b Other-Functions */

#define SIMPLEBTN_FUNC_READ_PIN(GPIOX_Base, Pin) \
    GPIO_ReadInputDataBit((GPIO_TypeDef*)GPIOX_Base, Pin)

#define SIMPLEBTN_FUNC_GET_TICK() \
    HAL_GetTick() // or xTaskGetTickCount() if you use FreeRTOS

#define SIMPLEBTN_FUNC_GET_TICK_FromISR() \
    HAL_GetTick() // or xTaskGetTickCountFromISR() if you use FreeRTOS

#define SIMPLEBTN_FUNC_PANIC(Cause, ErrorNum, etc) \
    do { simpleButton_debug_panic(Cause); } while(0) /* only used in DEBUG mode */

#define SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN() \
    __disable_irq()

#define SIMPLEBTN_FUNC_CRITICAL_SECTION_END() \
    __enable_irq()

#define SIMPLEBTN_FUNC_START_LOW_POWER() \
    simpleButton_start_low_power()

/** @b ================================================================ **/
/** @b Time-Set */

    // The delay time after you push the button.
#define SIMPLEBTN_TIME_PUSH_DELAY                       30
    // The delay time after you release the button.
#define SIMPLEBTN_TIME_RELEASE_DELAY                    30
    // The minimum time to push the button for long-push.
#define SIMPLEBTN_TIME_LONG_PUSH_MIN                    1000
    // The window time for repeat-push.
#define SIMPLEBTN_TIME_REPEAT_WINDOW                    300
    // The cool-down time after you release the button.
#define SIMPLEBTN_TIME_COOL_DOWN                        0
    // The minimum time to push the button for long-push-hold.
#define SIMPLEBTN_TIME_HOLD_PUSH_MIN                    SIMPLEBTN_TIME_LONG_PUSH_MIN
    // The interval time to call callback function in hold-long-push mode.
#define SIMPLEBTN_TIME_HOLD_INTERVAL                    200
    // The timeout time for normal long button.
#define SIMPLEBTN_TIME__TIMEOUT_NORMAL                  (1000 * 120)
    // The timeout time for combination long button.
#define SIMPLEBTN_TIME__TIMEOUT_COMBINATION             (1000 * 240)

/** @b ================================================================ **/
/** @b Mode-Set */

    // Enable debug mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_DEBUG                     0
    // Enable combination mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COMBINATION               0
    // Enable timer long-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH           0
    // Enable counter repeat-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH       0
    // Enable adjustable mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME           0
    // Enable multi-threads mode(enable this only when you do use multi-thread) if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_MULTI_THREADS             0
    // Enable long-push-hold mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD             0

/** @b ================================================================ **/
/** @b Namespace */

/**
 * @brief   The namespace of Simple-Button public functions and variables.
 * 
 * @note    This namespace can be changed to avoid name conflicts.
 *          Default: SimpleButton_
 */
#define SIMPLEBTN_NAMESPACE                             SimpleButton_

/** @b ================================================================ **/
/** @b Initialization-Function */

/* This macro just forward the parameter to another function */
#define SIMPLEBTN_FUNC_INIT_EXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X) \
    simpleButton_Private_InitEXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X)


/** @b ================================================================ **/
/** @b NVIC-EXTI-Priority */

#define SIMPLEBTN_NVIC_EXTI_PreemptionPriority      0

#define SIMPLEBTN_NVIC_EXTI_SubPriority             0

/* ====================== CUSTOMIZATION END ============================ */
/** @p ================================================================ **/


// Macro for force-inline
#if defined(__GNUC__) || defined(__clang__)
 #define SIMPLEBTN_FORCE_INLINE static inline __attribute__((always_inline))
#elif defined(_MSC_VER) || defined(__CC_ARM)
 #define SIMPLEBTN_FORCE_INLINE static __forceinline
#else
 #define SIMPLEBTN_FORCE_INLINE static inline
#endif

/* ================ OTHER LOCAL-PLATFORM CUSTOMIZATION ================= */

static inline void simpleButton_debug_panic(const char* cause)
{
    /* deal with the error in debug mode */
    (void)cause;
    while(1);
}

// Init the EXTI
SIMPLEBTN_FORCE_INLINE
void simpleButton_Private_InitEXTI(
    simpleButton_Type_GPIOBase_t    GPIOX_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin_X,
    simpleButton_Type_EXTITrigger_t EXTI_Trigger_X
) {

    /* Initialize the GPIOx Clock */
    GPIO_InitTypeDef gpio_config;
    EXTI_InitTypeDef exti_config;    
    NVIC_InitTypeDef nvic_config;
    uint8_t PortSource;         
    uint8_t PinSource;          
    uint32_t EXTI_Line;
    uint32_t RCC_GPIOX;
                
    /* static constexpr assign values to variables */
    switch(GPIOX_Base)
    {
#if defined(GPIOA_BASE) && defined(RCC_APB2Periph_GPIOA)
    case GPIOA_BASE:
        PortSource = GPIO_PortSourceGPIOA;
        RCC_GPIOX = RCC_APB2Periph_GPIOA;
        break;
#endif /* defined(GPIOA) */
#if defined(GPIOB_BASE) && defined(RCC_APB2Periph_GPIOB)
    case GPIOB_BASE:
        PortSource = GPIO_PortSourceGPIOB;
        RCC_GPIOX = RCC_APB2Periph_GPIOB;
        break;
#endif /* defined(GPIOB) */
#if defined(GPIOC_BASE) && defined(RCC_APB2Periph_GPIOC)
    case GPIOC_BASE:
        PortSource = GPIO_PortSourceGPIOC;
        RCC_GPIOX = RCC_APB2Periph_GPIOC;
        break;
#endif /* defined(GPIOC) */
#if defined(GPIOD_BASE) && defined(RCC_APB2Periph_GPIOD)
    case GPIOD_BASE:
        PortSource = GPIO_PortSourceGPIOD;
        RCC_GPIOX = RCC_APB2Periph_GPIOD;
        break;
#endif /* defined(GPIOD) */
#if defined(GPIOE_BASE) && defined(RCC_APB2Periph_GPIOE)
    case GPIOE_BASE:
        PortSource = GPIO_PortSourceGPIOE;
        RCC_GPIOX = RCC_APB2Periph_GPIOE;
        break;
#endif /* defined(GPIOE) */
#if defined(GPIOF_BASE) && defined(RCC_APB2Periph_GPIOF)
    case GPIOF_BASE:
        PortSource = GPIO_PortSourceGPIOF;
        RCC_GPIOX = RCC_APB2Periph_GPIOF;
        break;
#endif /* defined(GPIOF_BASE) */
#if defined(GPIOG_BASE) && defined(RCC_APB2Periph_GPIOG)
    case GPIOG_BASE:
        PortSource = GPIO_PortSourceGPIOG;
        RCC_GPIOX = RCC_APB2Periph_GPIOG;
        break;
#endif /* defined(GPIOG_BASE) */
#if defined(GPIOH_BASE) && defined(RCC_APB2Periph_GPIOH)
    case GPIOH_BASE:
        PortSource = GPIO_PortSourceGPIOH;
        RCC_GPIOX = RCC_APB2Periph_GPIOH;
        break;
#endif /* defined(GPIOH_BASE) */
#if defined(GPIOI_BASE) && defined(RCC_APB2Periph_GPIOI)
    case GPIOI_BASE:
        PortSource = GPIO_PortSourceGPIOI;
        RCC_GPIOX = RCC_APB2Periph_GPIOI;
        break;
#endif /* defined(GPIOI_BASE) */
#if defined(GPIOJ_BASE) && defined(RCC_APB2Periph_GPIOJ)
    case GPIOJ_BASE:
        PortSource = GPIO_PortSourceGPIOJ;
        RCC_GPIOX = RCC_APB2Periph_GPIOJ;
        break;
#endif /* defined(GPIOJ_BASE) */
#if defined(GPIOK_BASE) && defined(RCC_APB2Periph_GPIOK)
    case GPIOK_BASE:
        PortSource = GPIO_PortSourceGPIOK;
        RCC_GPIOX = RCC_APB2Periph_GPIOK;
        break;
#endif /* defined(GPIOK_BASE) */
#if defined(GPIOL_BASE) && defined(RCC_APB2Periph_GPIOL)
    case GPIOL_BASE:
        PortSource = GPIO_PortSourceGPIOL;
        RCC_GPIOX = RCC_APB2Periph_GPIOL;
        break;
#endif /* defined(GPIOL_BASE) */
#if defined(GPIOM_BASE) && defined(RCC_APB2Periph_GPIOM)
    case GPIOM_BASE:
        PortSource = GPIO_PortSourceGPIOM;
        RCC_GPIOX = RCC_APB2Periph_GPIOM;
        break;
#endif /* defined(GPIOM_BASE) */
#if defined(GPION_BASE) && defined(RCC_APB2Periph_GPION)
    case GPION_BASE:
        PortSource = GPIO_PortSourceGPION;
        RCC_GPIOX = RCC_APB2Periph_GPION;
        break;
#endif /* defined(GPION_BASE) */
    default:
        SIMPLEBTN_FUNC_PANIC("unexpected GPIO port", , );
        break;
    }

    switch(GPIO_Pin_X)
    {

#if defined(__CH32V00x_H) // CH32V003
    case GPIO_Pin_0:            
        PinSource = GPIO_PinSource0; 
        EXTI_Line = EXTI_Line0; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_1:            
        PinSource = GPIO_PinSource1; 
        EXTI_Line = EXTI_Line1; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_2:            
        PinSource = GPIO_PinSource2; 
        EXTI_Line = EXTI_Line2; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_3:            
        PinSource = GPIO_PinSource3; 
        EXTI_Line = EXTI_Line3; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_4:            
        PinSource = GPIO_PinSource4; 
        EXTI_Line = EXTI_Line4; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_5:            
        PinSource = GPIO_PinSource5; 
        EXTI_Line = EXTI_Line5; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_6:            
        PinSource = GPIO_PinSource6; 
        EXTI_Line = EXTI_Line6; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
    case GPIO_Pin_7:            
        PinSource = GPIO_PinSource7; 
        EXTI_Line = EXTI_Line7; 
        nvic_config.NVIC_IRQChannel = EXTI7_0_IRQn;
        break;
#else
    case GPIO_Pin_0:            
        PinSource = GPIO_PinSource0; 
        EXTI_Line = EXTI_Line0; 
        nvic_config.NVIC_IRQChannel = EXTI0_IRQn;
        break;
    case GPIO_Pin_1:            
        PinSource = GPIO_PinSource1; 
        EXTI_Line = EXTI_Line1; 
        nvic_config.NVIC_IRQChannel = EXTI1_IRQn;
        break;
    case GPIO_Pin_2:            
        PinSource = GPIO_PinSource2; 
        EXTI_Line = EXTI_Line2; 
        nvic_config.NVIC_IRQChannel = EXTI2_IRQn;
        break;
    case GPIO_Pin_3:            
        PinSource = GPIO_PinSource3; 
        EXTI_Line = EXTI_Line3; 
        nvic_config.NVIC_IRQChannel = EXTI3_IRQn;
        break;
    case GPIO_Pin_4:            
        PinSource = GPIO_PinSource4; 
        EXTI_Line = EXTI_Line4; 
        nvic_config.NVIC_IRQChannel = EXTI4_IRQn;
        break;
    case GPIO_Pin_5:            
        PinSource = GPIO_PinSource5; 
        EXTI_Line = EXTI_Line5; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case GPIO_Pin_6:            
        PinSource = GPIO_PinSource6; 
        EXTI_Line = EXTI_Line6; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case GPIO_Pin_7:            
        PinSource = GPIO_PinSource7; 
        EXTI_Line = EXTI_Line7; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case GPIO_Pin_8:            
        PinSource = GPIO_PinSource8; 
        EXTI_Line = EXTI_Line8; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case GPIO_Pin_9:            
        PinSource = GPIO_PinSource9; 
        EXTI_Line = EXTI_Line9; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case GPIO_Pin_10:           
        PinSource = GPIO_PinSource10;
        EXTI_Line = EXTI_Line10;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    case GPIO_Pin_11:           
        PinSource = GPIO_PinSource11;
        EXTI_Line = EXTI_Line11;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    case GPIO_Pin_12:           
        PinSource = GPIO_PinSource12;
        EXTI_Line = EXTI_Line12;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    case GPIO_Pin_13:           
        PinSource = GPIO_PinSource13;
        EXTI_Line = EXTI_Line13;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    case GPIO_Pin_14:           
        PinSource = GPIO_PinSource14;
        EXTI_Line = EXTI_Line14;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    case GPIO_Pin_15:           
        PinSource = GPIO_PinSource15;
        EXTI_Line = EXTI_Line15;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
#endif

    default:  
        SIMPLEBTN_FUNC_PANIC("unexpected GPIO pin", , );
        break;
    } // end switch

    /* GPIO & AFIO Init */
    RCC_APB2PeriphClockCmd(RCC_GPIOX, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    gpio_config.GPIO_Pin    = GPIO_Pin_X;
    gpio_config.GPIO_Speed  = GPIO_Speed_2MHz;
    gpio_config.GPIO_Mode   = (GPIOMode_TypeDef)
        ((EXTI_Trigger_X == EXTI_Trigger_Falling)
        * GPIO_Mode_IPU + (EXTI_Trigger_X == EXTI_Trigger_Rising)
        * GPIO_Mode_IPD);
    GPIO_Init((GPIO_TypeDef*)GPIOX_Base, &gpio_config);

    GPIO_EXTILineConfig(PortSource, PinSource);
    exti_config.EXTI_Line = EXTI_Line;
    exti_config.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_config.EXTI_Trigger = EXTI_Trigger_X;
    exti_config.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_config);

    nvic_config.NVIC_IRQChannelPreemptionPriority = SIMPLEBTN_NVIC_EXTI_PreemptionPriority;
    nvic_config.NVIC_IRQChannelSubPriority = SIMPLEBTN_NVIC_EXTI_SubPriority;
    nvic_config.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_config);
}

// Start the low-power mode. (SLEEP)
SIMPLEBTN_FORCE_INLINE void simpleButton_start_low_power(void)
{
    /* We will disable gloabal irq before go into low power mode.
     * So, use follow 2 line code can make EXTI-pending be able to wake up CPU.
     */
    const uint32_t Mask_IRQ_CanWake_WithoutGlobalEnable = 1 << 4;
    NVIC->SCTLR |= Mask_IRQ_CanWake_WithoutGlobalEnable;

    __WFE();

    /* Use 'nop' to wait the CPU becoming stable */
    __NOP();
    __NOP();
    __NOP();
}

/* ================================ END ================================ */

#endif /* SIMPLEBUTTON_CONFIG_H__ */
