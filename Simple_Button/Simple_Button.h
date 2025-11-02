/**
 * @file            Simple_Button.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Header file of Simple-Button. This file contains
 *                  necessary declarations of functions.
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
 *                  <https://github.com/Kim-J-Smith/Simple-Button>
 */
#ifndef     SIMPLEBUTTON_H__
#define     SIMPLEBUTTON_H__    1019L

/* Incldue the config file of Simple_Button and check the version */
#include    "simple_button_config.h"
#if ( SIMPLEBUTTON_CONFIG_H__ != SIMPLEBUTTON_H__ )
 #warning [Simple-Button] : the version of "simple_button_config.h"\
 is different from it of "Simple_Button.h".
#endif /* SIMPLEBUTTON_CONFIG_H__ != SIMPLEBUTTON_H__ */

/* ============== Private-Use Macro / Type / Function ================= */

/* Compatible with previous versions */
#ifndef SIMPLEBTN_FUNC_GET_TICK_FromISR
 #define SIMPLEBTN_FUNC_GET_TICK_FromISR()  SIMPLEBTN_FUNC_GET_TICK()
#endif /* SIMPLEBTN_FUNC_GET_TICK_FromISR */

/* Macro for bit-field */
#define SIMPLEBTN_BITFIELD(type)                        uint32_t

/* Macro for debug */
#if SIMPLEBTN_MODE_ENABLE_DEBUG == 1
 #define SIMPLEBTN_DEBUG
#endif /* SIMPLEBTN_MODE_ENABLE_DEBUG == 1 */

/* Macro for multi-threads */
#if SIMPLEBTN_MODE_ENABLE_MULTI_THREADS == 1
 #define SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M() SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN()
 #define SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M() SIMPLEBTN_FUNC_CRITICAL_SECTION_END()
#else
 #define SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M()
 #define SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M()
#endif /* SIMPLEBTN_MODE_ENABLE_MULTI_THREADS == 1 */

/* Macro for C API */
#ifdef __cplusplus
 #define SIMPLEBTN_C_API extern "C"
#else
 #define SIMPLEBTN_C_API
#endif

/* Macro for force-inline */
#if !defined(SIMPLEBTN_FORCE_INLINE)
 #if defined(__GNUC__) || defined(__clang__)
  #define SIMPLEBTN_FORCE_INLINE static inline __attribute__((always_inline))
 #elif defined(_MSC_VER) || defined(__CC_ARM)
  #define SIMPLEBTN_FORCE_INLINE static __forceinline
 #else
  #define SIMPLEBTN_FORCE_INLINE static inline
 #endif
#endif /* !defined(SIMPLEBTN_FORCE_INLINE) */

/* Macro to connect 2 macro */
#define SIMPLEBTN_CONNECT2(a, b) SIMPLEBTN_CONNECT2_1(a, b)
#define SIMPLEBTN_CONNECT2_1(a, b) SIMPLEBTN_CONNECT2_2(a, b)
#define SIMPLEBTN_CONNECT2_2(a, b) SIMPLEBTN_CONNECT2_3(a, b)
#define SIMPLEBTN_CONNECT2_3(a, b) SIMPLEBTN_CONNECT2_4(a, b)
#define SIMPLEBTN_CONNECT2_4(a, b) SIMPLEBTN_CONNECT2_5(a, b)
#define SIMPLEBTN_CONNECT2_5(a, b) a ## b
/* Macro to connect 3 macro */
#define SIMPLEBTN_CONNECT3(a, b, c) SIMPLEBTN_CONNECT3_1(a, b, c)
#define SIMPLEBTN_CONNECT3_1(a, b, c) SIMPLEBTN_CONNECT3_2(a, b, c)
#define SIMPLEBTN_CONNECT3_2(a, b, c) SIMPLEBTN_CONNECT3_3(a, b, c)
#define SIMPLEBTN_CONNECT3_3(a, b, c) SIMPLEBTN_CONNECT3_4(a, b, c)
#define SIMPLEBTN_CONNECT3_4(a, b, c) SIMPLEBTN_CONNECT3_5(a, b, c)
#define SIMPLEBTN_CONNECT3_5(a, b, c) a ## b ## c

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)

/* Macro to count args - 1 */
 #define SIMPLEBTN_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8,  \
    _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,      \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N
/* Macro to count args - 2 */
 #define SIMPLEBTN_COUNT_ARGS(...)         \
    SIMPLEBTN_COUNT_ARGS_IMPL(__VA_ARGS__, \
        32, 31, 30, 29, 28, 27, 26, 25, \
        24, 23, 22, 21, 20, 19, 18, 17, \
        16, 15, 14, 13, 12, 11, 10, 9,  \
        8,  7,  6,  5,  4,  3,  2,  1)

#endif /* >= C99 or C++11 */

/* Short-Push callback function pointer Type */
typedef void (* simpleButton_Type_ShortPushCallBack_t)(void);

/* Long-Push callback function pointer Type */
#if SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH == 0
 typedef void (* simpleButton_Type_LongPushCallBack_t)(void);
#else
 typedef void (* simpleButton_Type_LongPushCallBack_t)(uint32_t longPushTime);
#endif /* SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH == 0 */

/* Repeat-Push callback function pointer Type */
#if SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0
 typedef void (* simpleButton_Type_RepeatPushCallBack_t)(void);
#else
 typedef void (* simpleButton_Type_RepeatPushCallBack_t)(uint8_t repeatCount);
#endif /* SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0 */

/* Combination-Push callback function pointer Type */
#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0
 typedef void (* simpleButton_Type_CombinationPushCallBack_t)(void);
#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

typedef void (* simpleButton_Type_AsynchronousHandler_t)(
            simpleButton_Type_ShortPushCallBack_t shortPushCallBack,
            simpleButton_Type_LongPushCallBack_t longPushCallBack,
            simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack
        );

typedef void (* simpleButton_Type_InterruptHandler_t)(void);

/**
 * @enum    simpleButton_Type_ButtonState_t
 * 
 * @brief   Enum for button state.
 */
typedef enum simpleButton_Type_ButtonState_t {
    simpleButton_State_Wait_For_Interrupt = 0,

    simpleButton_State_Push_Delay,

    simpleButton_State_Wait_For_End,

    simpleButton_State_Wait_For_Repeat,

    simpleButton_State_Single_Push,

    simpleButton_State_Repeat_Push,

    simpleButton_State_Release_Delay,

    simpleButton_State_Cool_Down,

#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0

    simpleButton_State_Combination_Push,

    simpleButton_State_Combination_WaitForEnd,

    simpleButton_State_Combination_Release,

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

#if SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0

    simpleButton_State_Hold_Push,

    simpleButton_State_Hold_Release,

#endif /* SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0 */

} simpleButton_Type_ButtonState_t;

/* struct for button private status. */
typedef struct simpleButton_Type_PrivateBtnStatus_t {

    uint32_t                        timeStamp_loop; /* used in while loop */

    volatile uint32_t               timeStamp_interrupt; /* used in interrupt */

    volatile SIMPLEBTN_BITFIELD (simpleButton_Type_ButtonState_t) state : 8;

    uint8_t                         push_time;

} simpleButton_Type_PrivateBtnStatus_t;

/* struct for combination status and config. */
#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0

 struct simpleButton_Type_Button_t; /* just declare */
 typedef struct simpleButton_Type_CmbBtnConfig_t {

    volatile struct simpleButton_Type_PrivateBtnStatus_t* previousButton;

    volatile simpleButton_Type_CombinationPushCallBack_t callBack;

 } simpleButton_Type_CmbBtnConfig_t;

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

/* struct for public status and config. */
typedef struct simpleButton_Type_PublicBtnStatus_t {

#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0
    simpleButton_Type_CmbBtnConfig_t combinationConfig;
#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

#if SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0

 #if SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0

    uint32_t                        holdPushMinTime;

 #endif /* SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0 */

    uint16_t                        longPushMinTime;
    uint16_t                        coolDownTime;
    uint16_t                        repeatWindowTime;

#endif /* SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0 */
} simpleButton_Type_PublicBtnStatus_t;

/* struct for public method. */
typedef struct simpleButton_Type_ButtonMethod_t {

    simpleButton_Type_AsynchronousHandler_t asynchronousHandler;

    simpleButton_Type_InterruptHandler_t interruptHandler;

} simpleButton_Type_ButtonMethod_t;

/**
 * @struct      simpleButton_Type_Button_t
 * 
 * @brief       This struct record the button status and config.
 *              User should not change the value in "Private" section,
 *              unless you know what you are doing.
 */
typedef struct simpleButton_Type_Button_t {

    simpleButton_Type_ButtonMethod_t Methods;

    simpleButton_Type_PublicBtnStatus_t Public;

    simpleButton_Type_PrivateBtnStatus_t Private;

} simpleButton_Type_Button_t;

/**
 * @typedef     SimpleButton_Type_DynamicBtn_t
 * 
 * @brief       This is a public type for user to define a button dynamically.
 * 
 * @note        These buttons are not triggered by EXTI but polling.
 *              User should use `SimpleButton_DynamicBtn_xxx` to deal with these buttons.
 * 
 *              see <../README.md/#dynamic-button> for more details.
 */
typedef struct SimpleButton_Type_DynamicBtn_t {

    simpleButton_Type_GPIOBase_t    GPIO_Base;

    simpleButton_Type_GPIOPin_t     GPIO_Pin;

    simpleButton_Type_GPIOPinVal_t  normalPinVal;

    simpleButton_Type_PrivateBtnStatus_t Private;

    simpleButton_Type_PublicBtnStatus_t Public;
    
} SimpleButton_Type_DynamicBtn_t;

/* Init the Button.Public */
SIMPLEBTN_C_API void simpleButton_Private_InitStructPublic(
    simpleButton_Type_PublicBtnStatus_t* self_public
);

/* Init the Button.Private */
SIMPLEBTN_C_API void simpleButton_Private_InitStructPrivate(
    simpleButton_Type_PrivateBtnStatus_t* self_private
);

/* Init the Button.Methods */
SIMPLEBTN_C_API void simpleButton_Private_InitStructMethods(
    simpleButton_Type_ButtonMethod_t* self_methods,
    simpleButton_Type_AsynchronousHandler_t asynchronousHandler,
    simpleButton_Type_InterruptHandler_t interruptHandler
);


SIMPLEBTN_C_API void simpleButton_Private_InitStruct(
    simpleButton_Type_Button_t* self,
    simpleButton_Type_AsynchronousHandler_t asynchronousHandler,
    simpleButton_Type_InterruptHandler_t interruptHandler
);

/**
 * @brief           Initilize the button.
 * 
 * @param[inout]    self - Pointer to the button object.
 * @param[in]       GPIOX_BASE - The base address of the GPIO port 
 *                  connected to the button.
 * @param[in]       GPIO_Pin_X - The GPIO Pin number connected to the button.
 * @param[in]       EXTI_Trigger_X - The EXTI Trigger of the Pin.
 * @param[in]       asynchronousHandler - Function pointer of asynchronous handler.
 * @param[in]       interruptHandler - Function pointer of interrupt handler.
 * 
 * @return          None
 */
SIMPLEBTN_FORCE_INLINE void simpleButton_Private_InitButton(
    simpleButton_Type_Button_t* self,
    simpleButton_Type_GPIOBase_t GPIOX_BASE,
    simpleButton_Type_GPIOPin_t GPIO_Pin_X,
    simpleButton_Type_EXTITrigger_t EXTI_Trigger_X,
    simpleButton_Type_AsynchronousHandler_t asynchronousHandler,
    simpleButton_Type_InterruptHandler_t interruptHandler
) {

    simpleButton_Private_InitStruct(
        self,
        asynchronousHandler,
        interruptHandler
    );

    SIMPLEBTN_FUNC_INIT_EXTI(
        GPIOX_BASE,
        GPIO_Pin_X,
        EXTI_Trigger_X
    );

}


SIMPLEBTN_C_API void simpleButton_Private_InterruptHandler(
    simpleButton_Type_PrivateBtnStatus_t* self_private
);


SIMPLEBTN_C_API void
simpleButton_Private_AsynchronousHandler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val,
    simpleButton_Type_ShortPushCallBack_t shortPushCB,
    simpleButton_Type_LongPushCallBack_t longPushCB,
    simpleButton_Type_RepeatPushCallBack_t repeatPushCB
);


#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)

SIMPLEBTN_FORCE_INLINE uint32_t simpleButton_Private_IsIdle(const simpleButton_Type_PrivateBtnStatus_t* self_private) {
    return (self_private->state == simpleButton_State_Wait_For_Interrupt);
}

#define SIMPLEBTN_ISIDLE_1(Btn)         simpleButton_Private_IsIdle( &((Btn).Private) )

#define SIMPLEBTN_ISIDLE_2(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_1(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_3(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_2(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_4(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_3(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_5(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_4(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_6(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_5(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_7(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_6(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_8(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_7(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_9(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_8(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_10(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_9(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_11(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_10(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_12(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_11(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_13(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_12(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_14(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_13(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_15(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_14(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_16(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_15(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_17(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_16(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_18(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_17(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_19(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_18(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_20(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_19(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_21(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_20(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_22(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_21(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_23(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_22(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_24(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_23(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_25(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_24(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_26(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_25(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_27(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_26(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_28(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_27(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_29(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_28(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_30(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_29(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_31(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_30(__VA_ARGS__))

#define SIMPLEBTN_ISIDLE_32(Btn, ...)    (SIMPLEBTN_ISIDLE_1(Btn) & SIMPLEBTN_ISIDLE_31(__VA_ARGS__))

#define SIMPLEBTN_ALLISIDLE(...)    \
    SIMPLEBTN_CONNECT2(SIMPLEBTN_ISIDLE_, SIMPLEBTN_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

/**
 * @brief   Start low power if all of the buttons are idle.
 * @param   __VA_ARGS__ - button object. (passing 1 ~ 32 parameters is OK)
 * @return  None
 */
#define SIMPLEBTN__START_LOWPOWER(...)  \
    do {                                            \
        SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN();    \
        if (SIMPLEBTN_ALLISIDLE(__VA_ARGS__)) {     \
            SIMPLEBTN_FUNC_START_LOW_POWER();       \
        }                                           \
        SIMPLEBTN_FUNC_CRITICAL_SECTION_END();      \
    } while (0)

#endif /* >= C99 or C++11 */


/**
 * @def         SIMPLEBTN__CREATE
 * @brief       Creat a button.
 * @param[in]   GPIOx_BASE - The address of GPIO port connected to button.
 * @param[in]   GPIO_Pin_x - The number of the GPIO pin connected to button.
 * @param[in]   EXTI_Trigger_x - The EXTI trigger.
 * @param[in]   __name - The name of the button.
 * @attention   EXTI_Trigger_x must choose between Raising and Falling.
 */
#define SIMPLEBTN__CREATE(GPIOx_BASE, GPIO_Pin_x, EXTI_Trigger_x, __name)       \
    simpleButton_Type_Button_t                                                  \
    SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name) = {0};                      \
                                                                                \
    static void                                                                 \
    SIMPLEBTN_CONNECT2(simpleButton_Private_AsyHandler_, __name)(               \
        simpleButton_Type_ShortPushCallBack_t  shortPushCallBack,               \
        simpleButton_Type_LongPushCallBack_t   longPushCallBack,                \
        simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack               \
    ) {                                                                         \
        const simpleButton_Type_GPIOPinVal_t normalPinVal =                     \
            (simpleButton_Type_GPIOPinVal_t)                                    \
            (((EXTI_Trigger_x) == SIMPLEBTN_EXTI_TRIGGER_FALLING) ? 1 : 0);     \
                                                                                \
        simpleButton_Private_AsynchronousHandler(                               \
            &(SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name).Private),         \
            &(SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name).Public),          \
            (GPIOx_BASE),                                                       \
            (GPIO_Pin_x),                                                       \
            normalPinVal,                                                       \
            shortPushCallBack,                                                  \
            longPushCallBack,                                                   \
            repeatPushCallBack                                                  \
        );                                                                      \
    }                                                                           \
                                                                                \
    static void                                                                 \
    SIMPLEBTN_CONNECT2(simpleButton_Private_ITHandler_, __name)(void) {         \
        simpleButton_Private_InterruptHandler(                                  \
            &(SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name).Private)          \
        );                                                                      \
    }                                                                           \
                                                                                \
    SIMPLEBTN_C_API void                                                        \
    SIMPLEBTN_CONNECT3(SIMPLEBTN_NAMESPACE, __name, _Init)(void) {              \
        simpleButton_Private_InitButton(                                        \
            &(SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name)),                 \
            (GPIOx_BASE),                                                       \
            (GPIO_Pin_x),                                                       \
            (EXTI_Trigger_x),                                                   \
            &(SIMPLEBTN_CONNECT2(simpleButton_Private_AsyHandler_, __name)),    \
            &(SIMPLEBTN_CONNECT2(simpleButton_Private_ITHandler_, __name))      \
        );                                                                      \
    }


/**
 * @def         SIMPLEBTN__DECLARE
 * @param[in]   __name - The name of button.
 * @attention   The name of button must be the same as it is when created.
 */
#define SIMPLEBTN__DECLARE(__name)                                              \
    extern simpleButton_Type_Button_t                                           \
    SIMPLEBTN_CONNECT2(SIMPLEBTN_NAMESPACE, __name);                            \
    SIMPLEBTN_C_API void                                                        \
    SIMPLEBTN_CONNECT3(SIMPLEBTN_NAMESPACE, __name, _Init)(void);


#if ( SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 )

/**
 * @def             SIMPLEBTN__CMBBTN_SETCALLBACK
 * @brief           Set callback function for @b Combination-Button.
 * 
 * @param[in]       preButton - The `previous button` of @b Combination-Button.
 *                  `previous button` is the button that is pressed first.
 * @param[inout]    nextButton - The `next button` of @b Combination-Button.
 *                  `previous button` is the button that is pressed second.
 * @param[in]       callback - The callback function of @b Combination-Button.
 * 
 * @attention       Make sure the macro `SIMPLEBTN_MODE_ENABLE_COMBINATION` is defined as 1.
 */
 #define SIMPLEBTN__CMBBTN_SETCALLBACK(preButton, nextButton, callback)                  \
    do {                                                                                \
        (nextButton).Public.combinationConfig.previousButton = &((preButton).Private);  \
        (nextButton).Public.combinationConfig.callBack = callback;                      \
    } while(0)

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Init(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_GPIOBase_t    GPIO_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin,
    simpleButton_Type_GPIOPinVal_t  inactiveLevel
);

SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Handler(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_ShortPushCallBack_t shortPushCallBack,
    simpleButton_Type_LongPushCallBack_t longPushCallBack,
    simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack
);

#endif /* SIMPLEBUTTON_H__ */
