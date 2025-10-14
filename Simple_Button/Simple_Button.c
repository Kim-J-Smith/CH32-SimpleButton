/**
 * @file            Simple_Button.c
 * 
 * @author          Kim-J-Smith
 * 
 * @brief           State Machine and dynamic-button functions
 * 
 * @version         0.7.3 ( 0014L )
 * 
 * @date            2025-10-03
 * 
 * @attention       Copyright (c) 2025 Kim-J-Smith.
 *                  All rights reserved.
 * 
 * @copyright       This project complies with the MIT License.
 *                  Refer to the LICENCE file in root for more details.
 *                  <https://github.com/Kim-J-Smith/Simple-Button>
 */
#include    "Simple_Button.h"
#if SIMPLEBUTTON_H__ != 0014L
 #warning [Simple-Button]: There may be something wrong with the version.
#endif /* SIMPLEBTN_C_API */


SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateWaitForInterrupt_Handler(void) {
    /* Do nothing */
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StatePushDelay_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt <= SIMPLEBTN_TIME_PUSH_DELAY) {
        return; /* still need wait */
    }

    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) != normal_pin_val) {
        self_private->state = simpleButton_State_Wait_For_End;
    } else {
        if (self_private->push_time == 0) {
            self_private->state = simpleButton_State_Wait_For_Interrupt;
        } else {
            self_private->state = simpleButton_State_Wait_For_Repeat;
        }
    }
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateWaitForEnd_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
        self_private->state = simpleButton_State_Release_Delay;
    } 
#if SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0
 #if SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0
    else if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > self_public->holdPushMinTime)
 #else
    else if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > SIMPLEBTN_TIME_HOLD_PUSH_MIN)
 #endif /* SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0 */
    {
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
        self_private->state = simpleButton_State_Hold_Push;
    }
#endif /* SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0 */
    else if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > SIMPLEBTN_TIME__TIMEOUT_NORMAL) {

#if defined(SIMPLEBTN_DEBUG)
        SIMPLEBTN_FUNC_PANIC("normal long push time out", , );
#else
        self_private->push_time = 0;
        self_private->state = simpleButton_State_Wait_For_Interrupt;
#endif /* defined(SIMPLEBTN_DEBUG) */

    }
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateWaitForRepeat_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN(); /* begin always critical section */

#if SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop > (uint32_t)self_public->repeatWindowTime)
#else
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop > SIMPLEBTN_TIME_REPEAT_WINDOW)
#endif /* SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0 */
    {

#if SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0
        self_private->state = simpleButton_State_Single_Push;
#else
        if (self_private->push_time == 1) {
            self_private->state = simpleButton_State_Single_Push;
        } else {
            self_private->state = simpleButton_State_Repeat_Push;
        }
#endif /* SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0 */

    } // end if

    SIMPLEBTN_FUNC_CRITICAL_SECTION_END(); /* end always critical section */
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
}

// a helper function
SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_Do_LongPush(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_LongPushCallBack_t longPushCallBack
) {

#if SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH == 0
    (void)self;
    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */
    if (longPushCallBack != 0) {
        longPushCallBack();
    }
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
#else
    uint32_t longPushTime = SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt;
    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */
    if (longPushCallBack != 0) {
        longPushCallBack(longPushTime);
    }
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
#endif /* SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH == 0 */

}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_Do_ShortPush(
    simpleButton_Type_ShortPushCallBack_t shortPushCallBack
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */
    if (shortPushCallBack != 0) {
        shortPushCallBack();
    }
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateSinglePush_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public,
    simpleButton_Type_ShortPushCallBack_t shortPushCallBack,
    simpleButton_Type_LongPushCallBack_t longPushCallBack
) {
    
#if SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > (uint32_t)self_public->longPushMinTime)
#else
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > SIMPLEBTN_TIME_LONG_PUSH_MIN)
#endif /* SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0 */
    {
        simpleButton_Private_Do_LongPush(self_private, longPushCallBack);
    } else {
        simpleButton_Private_Do_ShortPush(shortPushCallBack);
    }

    self_private->push_time = 0;
    self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
    self_private->state = simpleButton_State_Cool_Down;
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateRepeatPush_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */

    if (repeatPushCallBack != 0) {

#if SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0
        repeatPushCallBack();
#else
        repeatPushCallBack(self_private->push_time);
#endif /* SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0 */

    } // end if

    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
    self_private->push_time = 0;
    self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
    self_private->state = simpleButton_State_Cool_Down;
}

// a helper function
SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_CmbBtnAfterReleaseOK(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public
) {
#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0

    if (self_public->combinationConfig.callBack == 0 || self_public->combinationConfig.previousButton == 0) {
        return; /* user didn't use combination button */
    }

    /* prev-button should be [Combination]_WaitForEnd. */
    if (self_public->combinationConfig.previousButton->Private.state != simpleButton_State_Combination_WaitForEnd) {
        if (self_public->combinationConfig.previousButton->Private.state != simpleButton_State_Wait_For_End) {
            return; /* prev-button isn't [Combination]_WaitForEnd. */
        } else {
            self_public->combinationConfig.previousButton->Private.state = simpleButton_State_Combination_WaitForEnd;
        }
    }

    self_private->state = simpleButton_State_Combination_Push;

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */
}

// a helper function
SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_ReleaseOK(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public
) {
    self_private->push_time ++;
    self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();

    /* enable counter-repeat-push or not */
#if SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0
    self_private->state = (self_private->push_time == 1)
        ? simpleButton_State_Wait_For_Repeat : simpleButton_State_Repeat_Push;
#else
    self_private->state = (self_private->push_time < 0xFF)
        ? simpleButton_State_Wait_For_Repeat : simpleButton_State_Repeat_Push;
#endif /* SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH == 0 */

    simpleButton_Private_CmbBtnAfterReleaseOK(self_private, self_public);
}


SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateReleaseDelay_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop <= SIMPLEBTN_TIME_RELEASE_DELAY) {
        return; /* still need wait */
    }

    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        simpleButton_Private_ReleaseOK(self_private, self_public);
    } else {
        self_private->state = simpleButton_State_Wait_For_End;
    }
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateCoolDown_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public
) {
#if SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop > self_public->coolDownTime)
#else
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop > SIMPLEBTN_TIME_COOL_DOWN)
#endif /* SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME != 0 */
    {
        self_private->state = simpleButton_State_Wait_For_Interrupt;
    }

}

#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0
SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateCombinationPush_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    simpleButton_Type_PublicBtnStatus_t* const self_public
) {
    simpleButton_Type_CombinationPushCallBack_t cmbCallBack;
    cmbCallBack = self_public->combinationConfig.callBack;

    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */

    if (cmbCallBack != 0) {
        cmbCallBack();
    }

    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */
    self_private->push_time = 0;
    self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
    self_private->state = simpleButton_State_Cool_Down;
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateCombinationWaitForEnd_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        self_private->state = simpleButton_State_Combination_Release;
    } else if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_interrupt > SIMPLEBTN_TIME__TIMEOUT_COMBINATION) {

#if defined(SIMPLEBTN_DEBUG)
        SIMPLEBTN_FUNC_PANIC("combination long push time out", , );
#else
        self_private->push_time = 0;
        self_private->state = simpleButton_State_Wait_For_Interrupt;
#endif /* defined(SIMPLEBTN_DEBUG) */

    }
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateCombinationRelease_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop <= SIMPLEBTN_TIME_RELEASE_DELAY) {
        return; /* still need wait */
    }

    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        self_private->push_time = 0;
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
        self_private->state = simpleButton_State_Cool_Down;
    } else {
        self_private->state = simpleButton_State_Combination_WaitForEnd;
    }
}

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */


#if SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateHoldPush_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val,
    const simpleButton_Type_LongPushCallBack_t longPushCallBack
) {
    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
        self_private->state = simpleButton_State_Hold_Release;
    }

    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop > SIMPLEBTN_TIME_HOLD_INTERVAL) {
        simpleButton_Private_Do_LongPush(self_private, longPushCallBack);
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
    }
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateHoldRelease_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private,
    const simpleButton_Type_GPIOBase_t gpiox_base,
    const simpleButton_Type_GPIOPin_t  gpio_pin_x,
    const simpleButton_Type_GPIOPinVal_t normal_pin_val
) {
    if (SIMPLEBTN_FUNC_GET_TICK() - self_private->timeStamp_loop <= SIMPLEBTN_TIME_RELEASE_DELAY) {
        return; /* still need wait */
    }

    if (SIMPLEBTN_FUNC_READ_PIN(gpiox_base, gpio_pin_x) == normal_pin_val) {
        self_private->push_time = 0;
        self_private->timeStamp_loop = SIMPLEBTN_FUNC_GET_TICK();
        self_private->state = simpleButton_State_Cool_Down;
    } else {
        self_private->state = simpleButton_State_Hold_Push;
    }
}

#endif /* SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0 */


SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_StateDefault_Handler(
    simpleButton_Type_PrivateBtnStatus_t* const self_private
) {

#if defined(SIMPLEBTN_DEBUG)
    (void)self_private;
    SIMPLEBTN_FUNC_PANIC("invalid button state", , );
#else
    self_private->push_time = 0;
    self_private->state = simpleButton_State_Wait_For_Interrupt;
#endif /* defined(SIMPLEBTN_DEBUG) */

}

/**
 * @brief           Asynchronously call the callback function in while loop.
 * @param[inout]    self - pointer to self struct.
 * @param[in]       gpiox_base - Address of GPIO port connected to the button.
 * @param[in]       gpio_pin_x - GPIO pin number connected to the button.
 * @param[in]       normal_pin_val - Normal(didn't push) pin value of button pin. (can be 1 or 0)
 * @param[in]       shortPushCB - callback function for short push.
 * @param[in]       longPushCB - callback function for long push.
 * @param[in]       repeatPushCB - callback function for repeat push.
 * @return          None
 */
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
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN_M(); /* begin multi-thread critical section */

    switch ( (simpleButton_Type_ButtonState_t)(self_private->state) ) {
    case simpleButton_State_Wait_For_Interrupt: {
        simpleButton_Private_StateWaitForInterrupt_Handler();
        break;
    }

    case simpleButton_State_Push_Delay: {
        simpleButton_Private_StatePushDelay_Handler(self_private, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    }

    case simpleButton_State_Wait_For_End: {
        simpleButton_Private_StateWaitForEnd_Handler(self_private, self_public, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    }

    case simpleButton_State_Wait_For_Repeat: {
        simpleButton_Private_StateWaitForRepeat_Handler(self_private, self_public);
        break;
    }

    case simpleButton_State_Single_Push: {
        simpleButton_Private_StateSinglePush_Handler(self_private, self_public, shortPushCB, longPushCB);
        break;
    }

    case simpleButton_State_Repeat_Push: {
        simpleButton_Private_StateRepeatPush_Handler(self_private, repeatPushCB);
        break;
    }

    case simpleButton_State_Release_Delay: {
        simpleButton_Private_StateReleaseDelay_Handler(self_private, self_public, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    }

    case simpleButton_State_Cool_Down: {
        simpleButton_Private_StateCoolDown_Handler(self_private, self_public);
        break;
    }

#if SIMPLEBTN_MODE_ENABLE_COMBINATION != 0

    case simpleButton_State_Combination_Push: {
        simpleButton_Private_StateCombinationPush_Handler(self_private, self_public);
        break;
    }

    case simpleButton_State_Combination_WaitForEnd: {
        simpleButton_Private_StateCombinationWaitForEnd_Handler(self_private, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    }

    case simpleButton_State_Combination_Release: {
        simpleButton_Private_StateCombinationRelease_Handler(self_private, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    }

#endif /* SIMPLEBTN_MODE_ENABLE_COMBINATION != 0 */

#if SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0

    case simpleButton_State_Hold_Push: {
        simpleButton_Private_StateHoldPush_Handler(self_private, gpiox_base, gpio_pin_x, normal_pin_val, longPushCB);
        break;
    }

    case simpleButton_State_Hold_Release: {
        simpleButton_Private_StateHoldRelease_Handler(self_private, gpiox_base, gpio_pin_x, normal_pin_val);
        break;
    } 

#endif /* SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD != 0 */

    default: {
        simpleButton_Private_StateDefault_Handler(self_private);
        break;
    }

    } // end switch

    SIMPLEBTN_FUNC_CRITICAL_SECTION_END_M(); /* end multi-thread critical section */
}

/**
 * @brief           Initialize the dynamic-button.
 * 
 * @param[inout]    self - The pointer of dynamic-button object.
 * @param[in]       GPIO_Base - The base address of the GPIO port 
 *                  connected to the button.
 * @param[in]       GPIO_Pin - The GPIO Pin number connected to the button.
 * @param[in]       normalPinVal - Normal(didn't push) pin value of button pin. (can be 1 or 0)
 * 
 * @return          None
 */
SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Init(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_GPIOBase_t    GPIO_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin,
    simpleButton_Type_GPIOPinVal_t  normalPinVal
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN(); /* critical section begin */

    self->GPIO_Base = GPIO_Base;
    self->GPIO_Pin = GPIO_Pin;
    self->normalPinVal = normalPinVal;

    simpleButton_Private_InitStructPrivate(&(self->Private));

    simpleButton_Private_InitStructPublic(&(self->Public));

    SIMPLEBTN_FUNC_CRITICAL_SECTION_END(); /* critical section end */
}

SIMPLEBTN_FORCE_INLINE void
simpleButton_Private_DynamicBtn_CheckState(
    SimpleButton_Type_DynamicBtn_t* const self
) {
    SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN(); /* critical section begin */

    if (
        (simpleButton_Type_ButtonState_t)(self->Private.state) != simpleButton_State_Wait_For_Interrupt
        && (simpleButton_Type_ButtonState_t)(self->Private.state) != simpleButton_State_Wait_For_Repeat
    ) {
        return; /* WFI or WFR is needed */
    }

    if (SIMPLEBTN_FUNC_READ_PIN(self->GPIO_Base, self->GPIO_Pin) == self->normalPinVal) {
        return; /* didn't be pushed */
    }

    self->Private.timeStamp_interrupt = SIMPLEBTN_FUNC_GET_TICK();
    self->Private.state = simpleButton_State_Push_Delay;

    SIMPLEBTN_FUNC_CRITICAL_SECTION_END(); /* critical section end */
}

/**
 * @brief           Handler of dynamic-button.
 * 
 * @param[inout]    self - The pointer of dynamic-button object.
 * @param[in]       shortPushCallBack - callback function for short push.
 * @param[in]       longPushCallBack - callback function for long push.
 * @param[in]       repeatPushCallBack - callback function for repeat push.
 * 
 * @return          None
 */
SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Handler(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_ShortPushCallBack_t shortPushCallBack,
    simpleButton_Type_LongPushCallBack_t longPushCallBack,
    simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack
) {

    simpleButton_Private_DynamicBtn_CheckState(self);

    simpleButton_Private_AsynchronousHandler(
        &(self->Private),
        &(self->Public),
        self->GPIO_Base,
        self->GPIO_Pin,
        self->normalPinVal,
        shortPushCallBack,
        longPushCallBack,
        repeatPushCallBack
    );
}
