# Config Guide

[English](./config_guide.md)
[中文](./config_guide_zh.md)

---

## Table of Contents
- [Introduction](#introduction)
- [Customization Options](#customization-options)
  1. [Head-File](#customization-options-head-file)
  2. [Simple-Button-Type](#customization-options-simple-button-type)
  3. [Other-Functions](#custom-options-other-functions)
  4. [Time-Set](#custom-options-time-set)
  5. [Mode-Set](#custom-options-mode-set)
  6. [Namespace](#custom-options-namespace)
  7. [Initialization-Function](#custom-option-initialization-function)

---

## Introduction
- In the `simple_button_config.h` file, we provide a variety of customizable options.
- This document aims to explain the **default states** of these options and some **configuration suggestions**.

## Customization Options

### Customization Options Head-File

```c
/** @b ================================================================
 **/ 
/** @b Head-File */
#include "xxx.h"
```

- The header file depends on the chip. If you are using the stm32f103c8t6 and programming with the HAL library, use `"stm32f1xx_hal.h"`, or directly use `"main.h"`.

### Customization Options Simple-Button-Type

```c
/** @b ================================================================ **/
/** @b Simple-Button-Type */

// The type of GPIOX_BASE. (X can be A / B / C ...)
typedef uint32_t            simpleButton_Type_GPIOBase_t;
// The type of GPIO_PIN_X. (X can be 0 / 1 / 2 / 3 / ...)
typedef uint16_t            simpleButton_Type_GPIOPin_t;
// The type of Value of Pin. (The value usually be 0 or 1)
typedef uint8_t             simpleButton_Type_GPIOPinVal_t;
// The type of EXTI_TRIGGER_X. (X can be FALLING / RAISING / ...)
typedef uint32_t            simpleButton_Type_EXTITrigger_t;

#define SIMPLEBTN_EXTI_TRIGGER_FALLING      /* for example: EXTI_TRIGGER_FALLING */
```

- Adjust these types according to the specific chip.

- Define `SIMPLEBTN_EXTI_TRIGGER_FALLING` as the specific chip's `EXTI_Trigger_Falling`.

### Custom Options Other-Functions

```c
/** @b ================================================================ **/
/** @b Other-Functions */

// ...
```

- This is a series of function interfaces that need to be implemented. The following will explain their meanings and where they are used:

1. `SIMPLEBTN_FUNC_READ_PIN(GPIOX_Base, Pin)`
   - This is a function interface for reading the pin state. In STM32-HAL, it is often implemented as `HAL_GPIO_ReadPin((GPIO_TypeDef*)(GPIOX_Base), Pin)`. Here, the `GPIOX_Base` parameter is just the address of GPIOX and needs to be type-cast to be used.
   - This function will be called in functions that need to read the pin level state, such as asynchronous processing functions.

2. `SIMPLEBTN_FUNC_GET_TICK()`
   - This is a function interface for obtaining the timestamp.
   - The timestamp returned by this interface should be a `uint32_t` type value and should increase by 1 every millisecond.
   - This function is often implemented as `HAL_GetTick()` in STM32-HAL.
   - This function is called in all functions that need to obtain the timestamp, such as interrupt handling functions and asynchronous processing functions.

3. `SIMPLEBTN_FUNC_PANIC(Cause, ErrorNum, etc)`
   - This is a function that is only called in debug mode (when `SIMPLEBTN_MODE_ENABLE_DEBUG` is defined as 1).
   - This function is only called when an abnormal situation occurs in debug mode.
   - `Cause` is a string describing the cause of the exception, `ErrorNum` is the exception number, and `etc` is other information.
   - This function is often implemented as `do { Error_Handler(); } while (1)` in STM32-HAL, which is equivalent to discarding all parameters.

4. `SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN()`
   - This function is used to start critical section protection to prevent critical section data from being modified by other threads/interrupts.
   - It is used before entering the critical section.
   - It is often implemented as `__disable_irq()`, which means "disable interrupts". 5. `SIMPLEBTN_FUNC_CRITICAL_SECTION_END()` - This function is used to close the critical section protection. - It is called after the critical section. - It is often implemented as `__enable_irq()`, which means "enable interrupts".

6. `SIMPLEBTN_FUNC_START_LOW_POWER()` - This function interface should be implemented as a function to enter low power mode, usually implemented as `__WFI()`. - The external interrupt of the button press can wake up the CPU and exit the low power mode. - This function is called by `SIMPLEBTN__START_LOWPOWER(...)`: this variable parameter macro function will call `SIMPLEBTN_FUNC_START_LOW_POWER()` when it is determined that all the passed button objects are in an idle state.

### Custom Options Time-Set

```c
/** @b ================================================================ **/
/** @b Time-Set */
```

- During the processing of the button, there are many "determination times". For example, the "minimum long press time", if the press event exceeds this time, it will be recognized as a long press; another example is the "multi-click window time", if the button is pressed again within this window, it will trigger the double-click callback function (or if you have enabled the multi-click counting function, it will increase the multi-click count by one).

- The following introduces the uses of these determination times (all time units are `ms`):

1. `SIMPLEBTN_TIME_PUSH_DELAY`: Debounce delay time after the button is pressed.

2. `SIMPLEBTN_TIME_RELEASE_DELAY`: Debounce time after the button is released.

3. `SIMPLEBTN_TIME_LONG_PUSH_MIN`: Minimum long press time. A press event exceeding this time will be recognized as a long press. However, it has a lower priority than multi-clicks.

4. `SIMPLEBTN_TIME_REPEAT_WINDOW`: Single-click/double-click window time. If the button is pressed again within this window, it will trigger the double-click callback function (or if you have enabled the multi-click counting function, it will increase the multi-click count by one).

5. `SIMPLEBTN_TIME_COOL_DOWN`: Cool-down time. The waiting time from when the button triggers a function to when it can be triggered again.

6. `SIMPLEBTN_TIME_HOLD_PUSH_MIN`: Minimum long press hold time. This time takes effect when the long press hold function is enabled. If the long press exceeds this time, the long press callback function will be continuously triggered.

7. `SIMPLEBTN_TIME_HOLD_INTERVAL`: Interval for triggering the callback function during long press hold. This time takes effect when the long press hold function is enabled.

8. `SIMPLEBTN_TIME__TIMEOUT_NORMAL`: Normal long press safety time limit. If the long press exceeds this time, the button will be forced back to the initial state.

9. `SIMPLEBTN_TIME__TIMEOUT_COMBINATION`: Safety time limit for the long press of the preceding button in a combination button. If the preceding button of a combination button is held down for longer than this time, the button will forcibly return to its initial state.

### Custom Options Mode-Set

```c
// Enable debug mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_DEBUG                     1
```
- `SIMPLEBTN_MODE_ENABLE_DEBUG`: When defined as 1, **debug mode** will be enabled.

```c
// Enable combination mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COMBINATION               1
```
- `SIMPLEBTN_MODE_ENABLE_COMBINATION`: When defined as 1, **combination buttons** will be enabled.

```c
// Enable timer long-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH           1
```
- `SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH`: When defined as 1, **timer long press** will be enabled.

```c
// Enable counter repeat-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH       1
```
- `SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH`: When defined as 1, **counter repeat press** will be enabled.

```c
// Enable only-use-default-time mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME         0
```
- `SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME`: When defined as 1, **adjustable time** will be enabled. That is, users will be able to customize the determination time for each button. 

```c
// If this macro is defined as 1, multi-thread mode will be enabled (enable this only when you do use multi-thread).
#define SIMPLEBTN_MODE_ENABLE_MULTI_THREADS 1
```

- `SIMPLEBTN_MODE_ENABLE_MULTI_THREADS` When it is defined as 1, the **multi-thread safety mode** will be enabled. Critical sections will be protected.

```c
// Enable long-push-hold mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD             1
```

- When `SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD` is defined as 1, **long press hold** will be enabled. It is not recommended to use it simultaneously with combination buttons. If used together, it is suggested to set the `Public.holdPushMinTime` of the leading button of the combination buttons to a larger value to prevent the long press hold from triggering first and causing the combination buttons to fail.

### Custom Options Namespace 

```c
/** @b ================================================================ **/
/** @b Namespace */


/**
* @brief   The namespace of Simple-Button public functions and variables.
*
* @note    This namespace can be changed to avoid name conflicts.
*          Default: SimpleButton_
*/
#define SIMPLEBTN_NAMESPACE                             SimpleButton_
```

- You can use any prefix name to suit your project. 

- This change will affect the initialization and invocation of the functions you create (excluding dynamic buttons). 

- Suppose you define the name of the button as `name` in `SIMPLEBTN__CREATE()`. Originally, you need to use `SimpleButton_name_Init()` to initialize the button and then use `SimpleButton_name.Methos` to call the processing function; but if you define `SIMPLEBTN_NAMESPACE` as `SB_`, you need to use `SB_name_Init()` to initialize the button and then use `SB_name.Methos` to call the processing function. 

- `SIMPLEBTN_NAMESPACE` can be defined as empty. 

### Custom Option Initialization-Function 

```c
/** @b ================================================================ **/
/** @b Initialization-Function */


/* This macro just forward the parameter to another function */
#define SIMPLEBTN_FUNC_INIT_EXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X) \
simpleButton_Private_InitEXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X)
```

- Among them, `simpleButton_Private_InitEXTI()` should be implemented as a `static inline` function at the `OTHER LOCAL-PLATFORM CUSTOMIZATION` section below, facilitating inlining optimization by the compiler and constant propagation optimization. 

- We provide a framework as follows. You just need to fill in the blanks (`SIMPLEBTN_FORCE_INLINE` will try to force the compiler to inline). 

```c
SIMPLEBTN_FORCE_INLINE
void simpleButton_Private_InitEXTI(
    simpleButton_Type_GPIOBase_t    GPIOX_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin_X,
    simpleButton_Type_EXTITrigger_t EXTI_Trigger_X
) {
    /* Write your code to initialize the GPIO/EXTI/NVIC ... */

    /* (void)xxx is used to suppress warning: "unused variables" */
    (void)GPIOX_Base;
    (void)GPIO_Pin_X;
    (void)EXTI_Trigger_X;
}
```
