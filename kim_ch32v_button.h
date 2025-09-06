/**
 * @file            kim_ch32v_button.h
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           Kim Library to offer a template for button [CH32V]
 * 
 * @version         0.0.2 ( 0002L )
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

#if         ( !defined(KIM_CH32V_BUTTON_H) ) && defined(__riscv)
#define     KIM_CH32V_BUTTON_H      0002L

/* ================================================================================ */
/**    @b ATTENTION Kim_Button use SysTick, which conflicts with with Delay_xxx    **/
/* ================================================================================ */

/* ============ Users can customize these by themselves ============ */

/** @p ------------------------------------------------------------- */
/** @b HEADER-FILES */

/***** @headerfile Select one of the header files given below as needed *****/
# include "ch32v20x.h"
// # include "ch32v30x.h"
// # include "ch32v10x.h"

/** @p ------------------------------------------------------------- */
/** @b TIMES */

/***** time config *****/
/* calculate the tick with the time */
#define KIM_BUTTON_TIME_MS(_xx_ms)                  (1 * (uint32_t)(_xx_ms))

#define KIM_BUTTON_PUSH_DELAY_TIME                  KIM_BUTTON_TIME_MS(40)          /* 40 ms */
#define KIM_BUTTON_REPEAT_PUSH_MAX_TIME             KIM_BUTTON_TIME_MS(300)         /* 300 ms */
#define KIM_BUTTON_LONG_PUSH_MIN_TIME               KIM_BUTTON_TIME_MS(1000)        /* 1000 ms */
#define KIM_BUTTON_RELEASE_DELAY_TIME               KIM_BUTTON_TIME_MS(40)          /* 40 ms */
#define KIM_BUTTON_COOL_DOWN_TIME                   KIM_BUTTON_TIME_MS(0)           /* 0 ms */
#define KIM_BUTTON_SAFE_PUSH_MAX_TIME               KIM_BUTTON_TIME_MS(60000)       /* 1 min */

/** @p ------------------------------------------------------------- */
/** @b NVIC-PRIORITY */

/***** NVIC Priority config *****/
#define KIM_BUTTON_NVIC_EXTI_PreemptionPriority     0
#define KIM_BUTTON_NVIC_EXTI_SubPriority            0

/** @p ------------------------------------------------------------- */
/** @b ENABLE-DISABLE-OPTIONS */

/* If this macro is 1, then the TIME above cannot be configured separately for each button */
#define KIM_BUTTON_ONLY_USE_DEFAULT_TIME            0

/***** Macro for use debug mode *****/
#define KIM_BUTTON_USE_DEBUG_MODE                   0   /* 1 --> use debug mode */

/***** Macro for noinline state machine(Kim_Button_PrivateUse_AsynchronousHandler) function *****/
#define KIM_BUTTON_NO_INLINE_STATE_MACHINE          0

/***** Macro to enable different long push time *****/
#define KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH  0

/***** Macro to enable button combination *****/
#define KIM_BUTTON_ENABLE_BUTTON_COMBINATION        0

/***** Macro to enable button repeat(2 ~ 7) *****/
#define KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT        0

/** @p ------------------------------------------------------------- */
/** @b FUNCTIONS-HOOKS */

/***** Critical Zone *****/
/* define follow macro when multi-thread */
#define KIM_BUTTON_CRITICAL_ZONE_BEGIN()            do { __disable_irq(); } while(0U)
#define KIM_BUTTON_CRITICAL_ZONE_END()              do { __enable_irq(); } while(0U)

/* define follow macro any time */
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN()     do { __disable_irq(); } while(0U)
#define KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END()       do { __enable_irq(); } while(0U)

/***** Macro for debug hook *****/
/* ... can be your function ... */
#define KIM_BUTTON_DEBUG_ERROR_HOOK()               do { while(1) {} } while(0U)

/***** Macro for get tick *****/
#define KIM_BUTTON_GET_TICK()                      ( (uint32_t)(SysTick->CNT / (SystemCoreClock / 8000)) )

/***** Macro for GPIO read pin *****/
#define KIM_BUTTON_READ_PIN(GPIOx_BASE, PIN)        GPIO_ReadInputDataBit((GPIO_TypeDef*)(GPIOx_BASE), PIN)

/***** Macro for replace Delay_xxx *****/
#if (!defined(KIM_DELAY_MS)) && (!defined(KIM_DELAY_US))
    #define KIM_DELAY_MS(time_ms)                                   \
        do {                                                        \
            uint64_t cmp = time_ms * (SystemCoreClock / 8000);      \
            uint64_t stamp = SysTick->CNT;                          \
            while(SysTick->CNT - stamp < cmp);                      \
        } while(0U)
    #define KIM_DELAY_US(time_us)                                   \
        do {                                                        \
            uint64_t cmp = time_us * (SystemCoreClock / 8000000);   \
            uint64_t stamp = SysTick->CNT;                          \
            while(SysTick->CNT - stamp < cmp);                      \
        } while(0U)
#endif /* delay */

/** @p ------------------------------------------------------------- */
/** @b NAMESPACE-NAME-PREFIX */

/***** @namespace Name Prefix *****/
/** If you change this macro, you need to use `new_prefix + Init_ + button_name()`      **
 ** to initialize the button, and use `new_prefix + button_name` struct to use method.  **
 **                                                                                     **
 ** @example                                                                            **
 **     #define KIM_BUTTON_NAME_PREFIX         KEY_                                     **
 **     KIM_BUTTON__REGISTER(..., ..., ..., THE_NAME)                                   **
 **                                                                                     **
 **     Then I need to use `KEY_Init_THE_NAME()` to initialize the button, and use      **
 **     `KEY_THE_NAME.method_asynchronous_handler(..., ..., ...)` and                   **
 **     `KEY_THE_NAME.method_interrupt_handler()`                                       **/
#define KIM_BUTTON_NAME_PREFIX                      Kim_Button_

/* ====================== Customization END ======================== */



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus enum & struct */

/* The enum is for State Machine. */
enum Kim_Button_State {
    Kim_Button_State_Wait_For_Interrupt = 0,

    Kim_Button_State_Push_Delay,

    Kim_Button_State_Wait_For_End,

    Kim_Button_State_Wait_For_Repeat,

    Kim_Button_State_Single_Push,

    Kim_Button_State_Repeat_Push,

    Kim_Button_State_Release_Delay,

    Kim_Button_State_Cool_Down,

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    Kim_Button_State_Combination_Push,

    Kim_Button_State_Combination_WaitForEnd,

    Kim_Button_State_Combination_Release,

#endif /* KIM_BUTTON_ENABLE_BUTTON_COMBINATION */
};

#ifndef ENUM_BITFIELD
    /* Used in struct bit field */
    #define ENUM_BITFIELD(type)     unsigned int 
#endif /* ENUM_BITFIELD */

typedef void (*Kim_Button_ShortPushCallBack_t)(void);

#if KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH == 0
    typedef void (*Kim_Button_LongPushCallBack_t)(void);
#else
    typedef void (*Kim_Button_LongPushCallBack_t)(uint32_t long_push_tick);
#endif /* different long push time */

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
    typedef void (*Kim_Button_RepeatPushCallBack_t)(void);
#else
    typedef void (*Kim_Button_RepeatPushCallBack_t)(uint8_t repeat_time);
#endif /* repeat more */

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0
    typedef void (*Kim_Button_CombinationCallBack_t)(void);
#endif /* button combination */

/* This struct is for status and behaviour of each button. */
struct Kim_Button_TypeDef {
    /** @p [private] This member variable will be changed only in interrupt service routine. */
    volatile uint32_t                               private_time_stamp_interrupt;

    /** @p [private] This member variable will be changed only in while loop. */
    uint32_t                                        private_time_stamp_loop;

    /** @b [public] Method for asynchronous handler. Use this method in while loop. */
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    );

    /** @b [public] Method for handler in interrupt (EXTI). Use this method in IT service routine. */
    void (* method_interrupt_handler) (void);

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    /** @b [public] Record the combination [before] button pointer. */
    volatile struct Kim_Button_TypeDef*             public_comb_before_button;

    /** @b [public] Record the combination button callback function.
     * (This callback function will be called when button-[This] is released 
     * and button-[before] is in the pressed state.) */
    volatile Kim_Button_CombinationCallBack_t       public_comb_callback;

#endif /* button combination */

#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0

    /** @b [public] This member variable is used to record the long-push time. */
    uint16_t                                        public_long_push_min_time;

    /** @b [public] This member variable is used to record the cool-down time. */
    uint16_t                                        public_cool_down_time;

    /** @b [public] This member variable is used to record the double push max time. */
    uint16_t                                        public_repeat_push_max_time;

#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */

    /** @p [private] This member variable is used to record the state of each button. */
    volatile ENUM_BITFIELD (enum Kim_Button_State)  private_state : 4;

    /** @p [private] This member variable is used to record how many times you push the button(0 ~ 7). */
    uint8_t                                         private_push_time : 3;

    /** @p [private] This member variable is used to record the state of initialization. */
    uint8_t                                         private_is_init : 1;
};

#ifdef __cplusplus
}
#endif /* __cplusplus enum & struct */


/* ======= [private use] Some private-use functions [static] ======= */

/* Macro for judge the EXTI Trigger */
#define KIM_BUTTON_JUDGE_TRIGGER(EXTI_TRIGGER_X)        \
    ((EXTI_TRIGGER_X == EXTI_Trigger_Rising_Falling) ? -1 : 1)

/* Macro for judge GPIOx_BASE and GPIO_PIN_X */
#if defined(GPIOF_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE)    ((GPIOx_BASE) == GPIOF_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_F */

#if defined(GPIOG_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE)    ((GPIOx_BASE) == GPIOG_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_G */

#if defined(GPIOH_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE)    ((GPIOx_BASE) == GPIOH_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_H */

#if defined(GPIOI_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE)    ((GPIOx_BASE) == GPIOI_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_I */

#if defined(GPIOJ_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE)    ((GPIOx_BASE) == GPIOJ_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_J */

#if defined(GPIOK_BASE)
 #define KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)    ((GPIOx_BASE) == GPIOK_BASE)
#else
 #define KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)    (0)
#endif /* KIM_BUTTON_JUDGE_GPIO_PIN_K */

#define KIM_BUTTON_JUDGE_GPIO_PIN(GPIOx_BASE, PIN)      \
    ( ((                                                \
        (GPIOx_BASE) == GPIOA_BASE ||                   \
        (GPIOx_BASE) == GPIOB_BASE ||                   \
        (GPIOx_BASE) == GPIOC_BASE ||                   \
        (GPIOx_BASE) == GPIOD_BASE ||                   \
        (GPIOx_BASE) == GPIOE_BASE ||                   \
        KIM_BUTTON_JUDGE_GPIO_PIN_F(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_G(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_H(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_I(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_J(GPIOx_BASE) ||      \
        KIM_BUTTON_JUDGE_GPIO_PIN_K(GPIOx_BASE)         \
    ) && (                                              \
        (PIN) == GPIO_Pin_0 || (PIN) == GPIO_Pin_1 ||   \
        (PIN) == GPIO_Pin_2 || (PIN) == GPIO_Pin_3 ||   \
        (PIN) == GPIO_Pin_4 || (PIN) == GPIO_Pin_5 ||   \
        (PIN) == GPIO_Pin_6 || (PIN) == GPIO_Pin_7 ||   \
        (PIN) == GPIO_Pin_8 || (PIN) == GPIO_Pin_9 ||   \
        (PIN) == GPIO_Pin_10 || (PIN) == GPIO_Pin_11 || \
        (PIN) == GPIO_Pin_12 || (PIN) == GPIO_Pin_13 || \
        (PIN) == GPIO_Pin_14 || (PIN) == GPIO_Pin_15    \
    )) ? 1 : -1 )

/* Macro for using functin with security */
#define KIM_BUTTON_SAFE_CALLBACK(_callback)         \
    do { if((_callback) != ((void*)0)) { _callback(); } } while(0U)

/* Macro for C API */
#ifdef __cplusplus
    #define KIM_BUTTON_C_API    extern "C" 
#else 
    #define KIM_BUTTON_C_API
#endif /* __cplusplus */

/* Macro for Force Inline of private-use functions */
#if defined(__GNUC__)
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static inline __attribute__((always_inline)) 
#elif defined(__CC_ARM)
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static __forceinline 
#else
    #define KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE    static inline
#endif /* FORCE_INLINE */

/* Macro for suggest inline of private-use functions */
#if !(defined(KIM_BUTTON_DEBUG))
 #if KIM_BUTTON_NO_INLINE_STATE_MACHINE == 0
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static inline
 #else
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static
 #endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */
#else
 #if defined(__GNUC__)
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static __attribute__((noinline)) \
                                                        __attribute__((optimize("O1"))) __attribute__((unused))
 #elif defined(__CC_ARM)
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static __attribute__((noinline))
 #else
    #define KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE      static 
 #endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */
#endif /* KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE */

/* Macro for debug */
#if KIM_BUTTON_USE_DEBUG_MODE != 0
    #define KIM_BUTTON_DEBUG
#endif

/* Macro to connect macro */
#define KIM_BUTTON_CONNECT(_a, _b)          KIM_BUTTON_CONNECT_1(_a, _b)
#define KIM_BUTTON_CONNECT_1(_a, _b)        KIM_BUTTON_CONNECT_2(_a, _b)
#define KIM_BUTTON_CONNECT_2(_a, _b)        _a ## _b

#define KIM_BUTTON_CONNECT3(_a, _b, _c)     KIM_BUTTON_CONNECT3_1(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_1(_a, _b, _c)   KIM_BUTTON_CONNECT3_2(_a, _b, _c)
#define KIM_BUTTON_CONNECT3_2(_a, _b, _c)   _a ## _b ## _c

/**
 * @p               [private-use]
 * @brief           Initialize the struct self.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       method_asynchronous_handler - function pointer to the method named 
 *                  `Kim_Button_Asynchronous_Handler_ ## __name` [static].
 * @param[in]       method_interrupt_handler - function pointer to the method named 
 *                  `Kim_Button_Handler_During_IT_ ## __name` [static].
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitStructSelf(
    struct Kim_Button_TypeDef* const self,
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    ),
    void (* method_interrupt_handler) (void)
)
{
    /* Initialize only once */
    self->private_is_init = 1;

    /* Initialize the member variables */
    self->private_push_time = 0;
    self->private_time_stamp_loop = 0;

    /* volatile variables */
    KIM_BUTTON_CRITICAL_ZONE_BEGIN();
    self->private_state = Kim_Button_State_Wait_For_Interrupt;
    self->private_time_stamp_interrupt = 0;
    KIM_BUTTON_CRITICAL_ZONE_END();

    /* public variables */
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
    self->public_long_push_min_time = KIM_BUTTON_LONG_PUSH_MIN_TIME;
    self->public_cool_down_time = KIM_BUTTON_COOL_DOWN_TIME;
    self->public_repeat_push_max_time = KIM_BUTTON_REPEAT_PUSH_MAX_TIME;
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */

    /* Initialize the public method */
    self->method_asynchronous_handler = method_asynchronous_handler;
    self->method_interrupt_handler = method_interrupt_handler;

    /* combination button */
#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    self->public_comb_before_button = 0;
    self->public_comb_callback = 0;

#endif /* combination button */

}

/**
 * @p               [private-use]
 * @brief           Initialize the SysTick.
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitSysTick(void)
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

/**
 * @p               [private-use]
 * @brief           Initialize the EXTI.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitEXTI(
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const EXTITrigger_TypeDef exti_trigger_x
)
{

    /* Initialize the GPIOx Clock */
    GPIO_InitTypeDef gpio_config;    
    uint8_t PortSource;         
    uint8_t PinSource;          
    uint32_t EXTI_Line;         
    EXTI_InitTypeDef exti_config;    
    NVIC_InitTypeDef nvic_config;
                
    /* static constexpr assign values to variables */                                               \
    switch(gpiox_base)
    {         
    case GPIOA_BASE:            
        PortSource = GPIO_PortSourceGPIOA;                                                          \
        break;
    case GPIOB_BASE:            
        PortSource = GPIO_PortSourceGPIOB;                                                          \
        break;
    case GPIOC_BASE:            
        PortSource = GPIO_PortSourceGPIOC;                                                          \
        break;
    case GPIOD_BASE:            
        PortSource = GPIO_PortSourceGPIOD;                                                          \
        break;
    case GPIOE_BASE:            
        PortSource = GPIO_PortSourceGPIOE;                                                          \
        break;
    default:  
        /* ... error handler ... */  
        break;
    }         
                
    switch(gpio_pin_x)
    {         
    case GPIO_Pin_0:            
        PinSource = GPIO_PinSource0; 
        EXTI_Line = EXTI_Line0; 
        nvic_config.NVIC_IRQChannel = EXTI0_IRQn;                                                   \
        break;
    case GPIO_Pin_1:            
        PinSource = GPIO_PinSource1; 
        EXTI_Line = EXTI_Line1; 
        nvic_config.NVIC_IRQChannel = EXTI1_IRQn;                                                   \
        break;
    case GPIO_Pin_2:            
        PinSource = GPIO_PinSource2; 
        EXTI_Line = EXTI_Line2; 
        nvic_config.NVIC_IRQChannel = EXTI2_IRQn;                                                   \
        break;
    case GPIO_Pin_3:            
        PinSource = GPIO_PinSource3; 
        EXTI_Line = EXTI_Line3; 
        nvic_config.NVIC_IRQChannel = EXTI3_IRQn;                                                   \
        break;
    case GPIO_Pin_4:            
        PinSource = GPIO_PinSource4; 
        EXTI_Line = EXTI_Line4; 
        nvic_config.NVIC_IRQChannel = EXTI4_IRQn;                                                   \
        break;
    case GPIO_Pin_5:            
        PinSource = GPIO_PinSource5; 
        EXTI_Line = EXTI_Line5; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;                                                 \
        break;
    case GPIO_Pin_6:            
        PinSource = GPIO_PinSource6; 
        EXTI_Line = EXTI_Line6; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;                                                 \
        break;
    case GPIO_Pin_7:            
        PinSource = GPIO_PinSource7; 
        EXTI_Line = EXTI_Line7; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;                                                 \
        break;
    case GPIO_Pin_8:            
        PinSource = GPIO_PinSource8; 
        EXTI_Line = EXTI_Line8; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;                                                 \
        break;
    case GPIO_Pin_9:            
        PinSource = GPIO_PinSource9; 
        EXTI_Line = EXTI_Line9; 
        nvic_config.NVIC_IRQChannel = EXTI9_5_IRQn;                                                 \
        break;
    case GPIO_Pin_10:           
        PinSource = GPIO_PinSource10;
        EXTI_Line = EXTI_Line10;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    case GPIO_Pin_11:           
        PinSource = GPIO_PinSource11;
        EXTI_Line = EXTI_Line11;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    case GPIO_Pin_12:           
        PinSource = GPIO_PinSource12;
        EXTI_Line = EXTI_Line12;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    case GPIO_Pin_13:           
        PinSource = GPIO_PinSource13;
        EXTI_Line = EXTI_Line13;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    case GPIO_Pin_14:           
        PinSource = GPIO_PinSource14;
        EXTI_Line = EXTI_Line14;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    case GPIO_Pin_15:           
        PinSource = GPIO_PinSource15;
        EXTI_Line = EXTI_Line15;
        nvic_config.NVIC_IRQChannel = EXTI15_10_IRQn;                                               \
        break;
    default:  
        /* ... error handler ... */
        break;
    }

    /* GPIO & AFIO Init */
    RCC_APB2PeriphClockCmd( ((gpiox_base == GPIOA_BASE)
        * RCC_APB2Periph_GPIOA + (gpiox_base == GPIOB_BASE)
        * RCC_APB2Periph_GPIOB + (gpiox_base == GPIOC_BASE)
        * RCC_APB2Periph_GPIOC + (gpiox_base == GPIOD_BASE)
        * RCC_APB2Periph_GPIOD + (gpiox_base == GPIOE_BASE)
        * RCC_APB2Periph_GPIOE), ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    gpio_config.GPIO_Pin    = gpio_pin_x;
    gpio_config.GPIO_Speed  = GPIO_Speed_2MHz;
    gpio_config.GPIO_Mode   = (GPIOMode_TypeDef)
        ((exti_trigger_x == EXTI_Trigger_Falling)
        * GPIO_Mode_IPU + (exti_trigger_x == EXTI_Trigger_Rising)
        * GPIO_Mode_IPD);
    GPIO_Init((GPIO_TypeDef*)gpiox_base, &gpio_config);

    GPIO_EXTILineConfig(PortSource, PinSource);
    exti_config.EXTI_Line = EXTI_Line;
    exti_config.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_config.EXTI_Trigger = exti_trigger_x;
    exti_config.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_config);

    nvic_config.NVIC_IRQChannelPreemptionPriority = KIM_BUTTON_NVIC_EXTI_PreemptionPriority;
    nvic_config.NVIC_IRQChannelSubPriority = KIM_BUTTON_NVIC_EXTI_PreemptionPriority;
    nvic_config.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_config);
}

/** 
 * @p               [private-use]
 * @brief           Init the Button, including the GPIO, EXTI and NVIC.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param[in]       method_asynchronous_handler - function pointer to the method named 
 *                  `Kim_Button_Asynchronous_Handler_ ## __name` [static].
 * @param[in]       method_interrupt_handler - function pointer to the method named 
 *                  `Kim_Button_Handler_During_IT_ ## __name` [static].
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_InitButton(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const EXTITrigger_TypeDef exti_trigger_x,
    void (* method_asynchronous_handler) (
        Kim_Button_ShortPushCallBack_t short_push_callback,
        Kim_Button_LongPushCallBack_t long_push_callback,
        Kim_Button_RepeatPushCallBack_t repeat_push_callback
    ),
    void (* method_interrupt_handler) (void)
)
{
    /* Check whether it is first time to init */
    if(self->private_is_init != 0) {
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        return;
#endif /* DEBUG MODE */
    }

    /* Initialize the struct self */
    Kim_Button_PrivateUse_InitStructSelf(
        self,
        method_asynchronous_handler,
        method_interrupt_handler
    );

    /* Initialize the SysTick (include SysTick's NVIC) */
    Kim_Button_PrivateUse_InitSysTick();

    /* Initialize the EXTI (include EXTI's NVIC) */
    Kim_Button_PrivateUse_InitEXTI(gpiox_base, gpio_pin_x, exti_trigger_x);
}

/**
 * @p               [private-use]
 * @brief           Change the status of each button when during the EXTI interrupt
 *                  service routine.
 * @param[inout]    self - pointer to self struct.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_ITHandler(
    struct Kim_Button_TypeDef* const self
)
{
    if( ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Repeat
        || ((enum Kim_Button_State)self->private_state) == Kim_Button_State_Wait_For_Interrupt) 
    {
        self->private_time_stamp_interrupt = KIM_BUTTON_GET_TICK();
        self->private_state = Kim_Button_State_Push_Delay;
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFIHandler(void)
{
    /* do nothing */
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StatePushDelayHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt 
        > KIM_BUTTON_PUSH_DELAY_TIME)
    {
        /* Check the GPIO Pin again */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            if(self->private_push_time == 0) {
                self->private_state = Kim_Button_State_Wait_For_Interrupt;
            } else {
                self->private_state = Kim_Button_State_Wait_For_Repeat;
            }
        } else {
            self->private_state = Kim_Button_State_Wait_For_End;
        }
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFEHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
    {
        self->private_state = Kim_Button_State_Release_Delay;
    }
    else if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt 
        > KIM_BUTTON_SAFE_PUSH_MAX_TIME)
    {
#if defined(KIM_BUTTON_DEBUG)
        KIM_BUTTON_DEBUG_ERROR_HOOK();
#else
        self->private_push_time = 0;
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
#endif /* debug mode */
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateRepeatPushHandler(
    struct Kim_Button_TypeDef* const self,
    Kim_Button_RepeatPushCallBack_t repeat_push_callback
)
{
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
    KIM_BUTTON_SAFE_CALLBACK(repeat_push_callback);
#else
    if(repeat_push_callback != 0) {
        repeat_push_callback(self->private_push_time);
    }
#endif /* more repeat */
    
    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateWFRHandler(
    struct Kim_Button_TypeDef* const self
)
{
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

    KIM_BUTTON_ALWAYS_CRITICAL_ZONE_BEGIN(); /* ALWAYS critical zone begin */
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_repeat_push_max_time
#else
        > KIM_BUTTON_REPEAT_PUSH_MAX_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    {
#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0
        self->private_state = Kim_Button_State_Single_Push;
#else
        if(self->private_push_time == 1) {
            self->private_state = Kim_Button_State_Single_Push;
        } else {
            self->private_state = Kim_Button_State_Repeat_Push;
        }
#endif /* more repeat */
    }
    KIM_BUTTON_ALWAYS_CRITICAL_ZONE_END(); /* ALWAYS critical zone end */

    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateSiglePushHandler(
    struct Kim_Button_TypeDef* const self,
    Kim_Button_ShortPushCallBack_t short_push_callback,
    Kim_Button_LongPushCallBack_t long_push_callback
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_long_push_min_time
#else
        > KIM_BUTTON_LONG_PUSH_MIN_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    { 
#if KIM_BUTTON_ENABLE_DIFFERENT_TIME_LONG_PUSH == 0
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        KIM_BUTTON_SAFE_CALLBACK(long_push_callback);
#else
        uint32_t long_push_tick = KIM_BUTTON_GET_TICK() - self->private_time_stamp_interrupt;
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        if(long_push_callback != ((void*)0)) {
            long_push_callback(long_push_tick); 
        }
#endif /* different long push time */
    } else {
        KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */
        KIM_BUTTON_SAFE_CALLBACK(short_push_callback);
    }
    
    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateReleaseDelayHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
        > KIM_BUTTON_RELEASE_DELAY_TIME)
    {
        /* Delay time is over */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            self->private_push_time ++;
            self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();

#if KIM_BUTTON_ENABLE_BUTTON_MORE_REPEAT == 0 /* more repeat */
            self->private_state = (self->private_push_time == 1)
                ? Kim_Button_State_Wait_For_Repeat : Kim_Button_State_Repeat_Push;
#else
            self->private_state = (self->private_push_time < 7)
                ? Kim_Button_State_Wait_For_Repeat : Kim_Button_State_Repeat_Push;
#endif /* more repeat */

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0 /* combination button */

            if(self->public_comb_before_button == 0 || self->public_comb_callback == 0)
            {
                return;
            }
            /* check button-[before] */
            if(self->public_comb_before_button->private_state != Kim_Button_State_Combination_WaitForEnd)
            {
                if(self->public_comb_before_button->private_state != Kim_Button_State_Wait_For_End) {
                    return;
                } else {
                    self->public_comb_before_button->private_state = Kim_Button_State_Combination_WaitForEnd;
                }
            }
            self->private_state = Kim_Button_State_Combination_Push;
#endif /* combination button */

        } else {
            self->private_state = Kim_Button_State_Wait_For_End;
        }
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCoolDownHandler(
    struct Kim_Button_TypeDef* const self
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
#if KIM_BUTTON_ONLY_USE_DEFAULT_TIME == 0
        > (uint32_t)self->public_cool_down_time
#else
        > KIM_BUTTON_COOL_DOWN_TIME
#endif /* KIM_BUTTON_ONLY_USE_DEFAULT_TIME */
    )
    {
        self->private_state = Kim_Button_State_Wait_For_Interrupt;
    }
}



#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationPushHandler(
    struct Kim_Button_TypeDef* const self
)
{
    Kim_Button_CombinationCallBack_t tmp_comb_callback;
    tmp_comb_callback = (Kim_Button_CombinationCallBack_t)self->public_comb_callback;
    KIM_BUTTON_CRITICAL_ZONE_END(); /* Critical Zone End */

    KIM_BUTTON_SAFE_CALLBACK(tmp_comb_callback);

    KIM_BUTTON_CRITICAL_ZONE_BEGIN(); /* Critical Zone Begin */
    self->private_push_time = 0;
    self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
    self->private_state = Kim_Button_State_Cool_Down;
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationWFEHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
    {
        self->private_state = Kim_Button_State_Combination_Release;
    }
}

KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateCombinationReleaseHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val
)
{
    if(KIM_BUTTON_GET_TICK() - self->private_time_stamp_loop 
        > KIM_BUTTON_RELEASE_DELAY_TIME)
    {
        /* check again */
        if(KIM_BUTTON_READ_PIN(gpiox_base, gpio_pin_x) == Normal_Bit_Val)
        {
            self->private_push_time = 0;
            self->private_time_stamp_loop = KIM_BUTTON_GET_TICK();
            self->private_state = Kim_Button_State_Cool_Down;
        } else {
            self->private_state = Kim_Button_State_Combination_WaitForEnd;
        }         
    }
}

#endif /* KIM_BUTTON_ENABLE_BUTTON_COMBINATION */



KIM_BUTTON_PRIVATE_FUNC_FORCE_INLINE void Kim_Button_PrivateUse_StateDefaultHandler(void)
{

#if defined(KIM_BUTTON_DEBUG)
    KIM_BUTTON_DEBUG_ERROR_HOOK();
#endif /* DEBUG MODE */

}

/**
 * @p               [private-use]
 * @brief           Asynchronously call the callback func by the status of each button.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       gpiox_base - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param[in]       gpio_pin_x - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param[in]       exti_trigger_x - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param[in]       short_push_callback - callback function for short push.
 * @param[in]       long_push_callback - callback function for long push.
 * @param[in]       repeat_push_callback - callback function for double push.
 * @return          None
 */
KIM_BUTTON_PRIVATE_FUNC_SUGGEST_INLINE void Kim_Button_PrivateUse_AsynchronousHandler(
    struct Kim_Button_TypeDef* const self,
    const uint32_t gpiox_base,
    const uint16_t gpio_pin_x,
    const uint8_t Normal_Bit_Val,
    Kim_Button_ShortPushCallBack_t short_push_callback,
    Kim_Button_LongPushCallBack_t long_push_callback,
    Kim_Button_RepeatPushCallBack_t repeat_push_callback
)
{
    /* Critical Zone Begin */
    KIM_BUTTON_CRITICAL_ZONE_BEGIN();

    switch ((enum Kim_Button_State)self->private_state) 
    {

    case Kim_Button_State_Wait_For_Interrupt:
        Kim_Button_PrivateUse_StateWFIHandler();
        break;

    case Kim_Button_State_Push_Delay:
        Kim_Button_PrivateUse_StatePushDelayHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Wait_For_End:
        Kim_Button_PrivateUse_StateWFEHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Repeat_Push:
        Kim_Button_PrivateUse_StateRepeatPushHandler(self, repeat_push_callback);
        break;

    case Kim_Button_State_Wait_For_Repeat:
        Kim_Button_PrivateUse_StateWFRHandler(self);
        break;

    case Kim_Button_State_Single_Push:
        Kim_Button_PrivateUse_StateSiglePushHandler(self, short_push_callback, long_push_callback);
        break;

    case Kim_Button_State_Release_Delay:
        Kim_Button_PrivateUse_StateReleaseDelayHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Cool_Down:
        Kim_Button_PrivateUse_StateCoolDownHandler(self);
        break;

#if KIM_BUTTON_ENABLE_BUTTON_COMBINATION != 0

    case Kim_Button_State_Combination_Push:
        Kim_Button_PrivateUse_StateCombinationPushHandler(self);
        break;

    case Kim_Button_State_Combination_WaitForEnd:
        Kim_Button_PrivateUse_StateCombinationWFEHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

    case Kim_Button_State_Combination_Release:
        Kim_Button_PrivateUse_StateCombinationReleaseHandler(self, gpiox_base, gpio_pin_x, Normal_Bit_Val);
        break;

#endif /* button combination */

    default:
        Kim_Button_PrivateUse_StateDefaultHandler();
        break;

    }

    /* Critical Zone End */
    KIM_BUTTON_CRITICAL_ZONE_END(); 
}

/* ================ Private-use functions END ====================== */



/**
 * @def         KIM_BUTTON__REGISTER
 * @brief       This is a template for generating code of button.
 *              It will generate 3 functions, including:
 *      @arg `Kim_Button_Init_ ## __name` [extern "C" public-use].
 *      @arg `Kim_Button_Asynchronous_Handler_ ## __name` [static private-use].
 *      @arg `Kim_Button_Handler_During_IT_ ## __name` [static private-use].
 * @param       GPIOx_BASE - can be GPIOA_BASE / GPIOB_BASE / GPIOC_BASE / ...
 * @param       GPIO_PIN_X - can be GPIO_PIN_0 / GPIO_PIN_1 / ... / GPIO_PIN_15.
 * @param       EXTI_TRIGGER_X - can be EXTI_TRIGGER_RISING or EXTI_TRIGGER_FALLING.
 * @param       __name - the name of your button. can be any string.
 * @attention   @p Error(array[-1]) will occur:
 *              1. If EXTI_TRIGGER_X is set as EXTI_TRIGGER_RISING_FALLING or EXTI_TRIGGER_NONE.
 *              2. If GPIOx_BASE is invalid.
 *              3. If GPIO_PIN_X is invalid.
 */
#define KIM_BUTTON__REGISTER(GPIOx_BASE, GPIO_PIN_X, EXTI_TRIGGER_X, __name)    \
    struct Kim_Button_TypeDef                                                   \
        KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX,__name) = {0};                \
                                                                                \
    static void                                                                 \
    KIM_BUTTON_CONNECT(Kim_Button_Asynchronous_Handler_, __name)(               \
        Kim_Button_ShortPushCallBack_t short_push_callback,                     \
        Kim_Button_LongPushCallBack_t long_push_callback,                       \
        Kim_Button_RepeatPushCallBack_t repeat_push_callback                    \
    )                                                                           \
    {                                                                           \
        static const uint8_t Normal_Bit_Val =                                   \
            (EXTI_TRIGGER_X == EXTI_Trigger_Rising) ? 0 : 1;                    \
                                                                                \
        Kim_Button_PrivateUse_AsynchronousHandler(                              \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name)),              \
            GPIOx_BASE,                                                         \
            GPIO_PIN_X,                                                         \
            Normal_Bit_Val,                                                     \
            short_push_callback,                                                \
            long_push_callback,                                                 \
            repeat_push_callback                                                \
        );                                                                      \
    }                                                                           \
                                                                                \
    static void                                                                 \
    KIM_BUTTON_CONNECT(Kim_Button_Handler_During_IT_, __name)(void)             \
    {                                                                           \
        Kim_Button_PrivateUse_ITHandler(                                        \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name))               \
        );                                                                      \
    }                                                                           \
                                                                                \
    KIM_BUTTON_C_API void                                                       \
    KIM_BUTTON_CONNECT3(KIM_BUTTON_NAME_PREFIX, Init_, __name) (void)           \
    {                                                                           \
        /* Assert the EXTI Trigger (use array[-1] to raise error) */            \
        static const char                                                       \
            assert_exti_trg[KIM_BUTTON_JUDGE_TRIGGER(EXTI_TRIGGER_X)] = {0};    \
        (void)assert_exti_trg;                                                  \
                                                                                \
        /* Assert the GPIO_BASE and PIN (use array[-1] to raise error) */       \
        static const char                                                       \
            assert_gpio_pin[                                                    \
                KIM_BUTTON_JUDGE_GPIO_PIN(GPIOx_BASE, GPIO_PIN_X)               \
            ] = {0};                                                            \
        (void)assert_gpio_pin;                                                  \
                                                                                \
        Kim_Button_PrivateUse_InitButton(                                       \
            &(KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name)),              \
            GPIOx_BASE,                                                         \
            GPIO_PIN_X,                                                         \
            EXTI_TRIGGER_X,                                                     \
            &(Kim_Button_Asynchronous_Handler_ ## __name),                      \
            &(Kim_Button_Handler_During_IT_ ## __name)                          \
        );                                                                      \
    }
    
/**
 * @def         KIM_BUTTON__DECLARE
 * @brief       You can use `KIM_BUTTON__REGISTER` to generate code in one file.
 *              And then, you need to use `KIM_BUTTON__DECLARE` to declare them in
 *              another file before you want to use them.
 * @param       __name - the name of your button. remain the same as it in `KIM_BUTTON__REGISTER`.
 */
#define KIM_BUTTON__DECLARE(__name)                                                         \
    extern struct Kim_Button_TypeDef KIM_BUTTON_CONNECT(KIM_BUTTON_NAME_PREFIX, __name);    \
    KIM_BUTTON_C_API void KIM_BUTTON_CONNECT3(KIM_BUTTON_NAME_PREFIX, Init_, __name)(void);


#endif /* KIM_CH32V_BUTTON_H */
