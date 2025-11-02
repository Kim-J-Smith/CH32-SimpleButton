# Config Guide

[English](./config_guide.md)
[中文](./config_guide_zh.md)

---

## 目录

- [简介](#简介)

- [自定义选项](#自定义选项)
    1. [Head-File](#自定义选项-head-file)
    2. [Simple-Button-Type](#自定义选项-simple-button-type)
    3. [Other-Functions](#自定义选项-other-functions)
    4. [Time-Set](#自定义选项-time-set)
    5. [Mode-Set](#自定义选项-mode-set)
    6. [Namespace](#自定义选项-namespace)
    7. [Initialization-Function](#自定义选项-initialization-function)

---

## 简介

- 在`simple_button_config.h`文件里，我们提供了丰富的可定制选项。

- 本文档旨在说明这些选项的**默认状态**与一些**配置建议**。

## 自定义选项

### 自定义选项 Head-File

```c
/** @b ================================================================ **/
/** @b Head-File */

# include "xxx.h"
```

- 头文件根据芯片而定。如果你使用的是stm32f103c8t6，并使用HAL库编程，就使用`"stm32f1xx_hal.h"`，又或者直接使用`"main.h"`。

### 自定义选项 Simple-Button-Type

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

- 根据具体的芯片调整这些类型。

- 将`SIMPLEBTN_EXTI_TRIGGER_FALLING`定义为具体芯片的`EXTI_Trigger_Falling`。

### 自定义选项 Other-Functions

```c
/** @b ================================================================ **/
/** @b Other-Functions */

// ...
```

- 这是一系列需要被实现的函数接口，下方将一一解释其含义，以及在何处被使用：

1. `SIMPLEBTN_FUNC_READ_PIN(GPIOX_Base, Pin)`
    - 这是一个读取引脚状态的函数接口，在STM32-HAL中常常被实现为`HAL_GPIO_ReadPin((GPIO_TypeDef*)(GPIOX_Base), Pin)`。此处的`GPIOX_Base`参数只是GPIOX的地址，需要转化类型才能使用。
    - 这个函数将在需要读取引脚电平状态的函数，例如异步处理函数中被调用。

2. `SIMPLEBTN_FUNC_GET_TICK()`
    - 这是一个获取时间戳的函数接口。
    - 这个接口返回的时间戳应当是`uint32_t`类型的值，并且它的值应当每毫秒增加1。
    - 这个函数在STM32-HAL中常常被实现为`HAL_GetTick()`。
    - 这个函数在所有需要获取时间戳的函数，例如中断处理函数与异步处理函数中被调用。

2. `SIMPLEBTN_FUNC_GET_TICK_FromISR()`
    - 中断服务函数中专用的获取时间戳函数。
    - 通常情况下与`SIMPLEBTN_FUNC_GET_TICK()`一致。
    - 如果使用FreeRTOS，这个宏应该被定义为 `xTaskGetTickCountFromISR()`。

3. `SIMPLEBTN_FUNC_PANIC(Cause, ErrorNum, etc)`
    - 这是一个仅在调试模式（`SIMPLEBTN_MODE_ENABLE_DEBUG`被定义为1时）中被调用的函数。
    - 只有在调试模式下发生异常情况时，该函数才会被调用。
    - `Cause`是一段字符串，描述异常原因， `ErrorNum`是异常编号， `etc`是其他信息。
    - 这个函数在STM32-HAL中常常被实现为`do { Error_Handler(); } while (1)`。相当于弃用所有参数。

4. `SIMPLEBTN_FUNC_CRITICAL_SECTION_BEGIN()`
    - 这个函数用于开启临界区保护，防止临界区数据被其他线程/中断篡改。
    - 被用于在临界区前调用。
    - 常被实现为`__disable_irq()`，即“关中断”。

5. `SIMPLEBTN_FUNC_CRITICAL_SECTION_END()`
    - 这个函数用于关闭临界区保护。
    - 被用于在临界区后调用。
    - 常被实现为`__enable_irq()`，即“开中断”。

6. `SIMPLEBTN_FUNC_START_LOW_POWER()`
    - 这个函数接口应当被实现为一个“进入低功耗状态”的函数，通常被实现为`__WFI()`。
    - 按键按下的外部中断能够唤醒CPU，从低功耗模式脱离。
    - 这个函数会被`SIMPLEBTN__START_LOWPOWER(...)`调用：这个可变参数宏函数会在确定所有传入的按键对象都处于空闲状态时调用`SIMPLEBTN_FUNC_START_LOW_POWER()`。

### 自定义选项 Time-Set

```c
/** @b ================================================================ **/
/** @b Time-Set */
```

- 在处理按键的过程中，存在许多“判定时间”。例如“长按最小时间”，超过这个时间的按下事件将被认定为“长按”；又比如“多击窗口时间”，在这个窗口时间内再次按下就能触发双击或让多击计数加一。

- 下方一一介绍这些判定时间的用处（所有时间单位为`ms`）：

1. `SIMPLEBTN_TIME_PUSH_DELAY`：按键按下后的消抖延迟时间。

2. `SIMPLEBTN_TIME_RELEASE_DELAY`：按键释放后的消抖时间。

3. `SIMPLEBTN_TIME_LONG_PUSH_MIN`：长按最小时间，超过这个时间的按键事件才被判定为长按。但优先级低于多击。

4. `SIMPLEBTN_TIME_REPEAT_WINDOW`：单击/双击窗口时间，在该窗口期内再次按下，将会触发双击回调函数（或者如果您开启了计数多击功能，这会让多击计数加一）。

5. `SIMPLEBTN_TIME_COOL_DOWN`：冷却时间。按键触发功能后到再次能够被触发的等待时间。

6. `SIMPLEBTN_TIME_HOLD_PUSH_MIN`：长按保持最小时间。这个时间在开启长按保持功能后生效，长按超过这个时间后会持续触发长按回调函数。

7. `SIMPLEBTN_TIME_HOLD_INTERVAL`：长按保持时，触发回调函数的间隔。这个时间在开启长按保持功能后生效。

8. `SIMPLEBTN_TIME__TIMEOUT_NORMAL`：普通长按安全时限。长按超过这个时间按键会强制回到初始状态。

9. `SIMPLEBTN_TIME__TIMEOUT_COMBINATION`：组合键前驱按键长按安全时限。组合键前驱按键长按超过这个时间按键会强制回到初始状态。

### 自定义选项 Mode-Set

```c
    // Enable debug mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_DEBUG                     1
```

- `SIMPLEBTN_MODE_ENABLE_DEBUG` 当它被定义为1时，**调试模式**将被开启。

```c
    // Enable combination mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COMBINATION               1
```

- `SIMPLEBTN_MODE_ENABLE_COMBINATION` 当它被定义为1时，**组合键**将被开启。

```c
    // Enable timer long-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH           1
```

- `SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH` 当它被定义为1时，**计时长按**将被开启。

```c
    // Enable counter repeat-push mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_COUNTER_REPEAT_PUSH       1
```

- `SIMPLEBTN_MODE_ENABLE_TIMER_LONG_PUSH` 当它被定义为1时，**计数多击**将被开启。

```c
    // Enable only-use-default-time mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME         0
```

- `SIMPLEBTN_MODE_ENABLE_ADJUSTABLE_TIME` 当它被定义为1时，**可调时间**将被开启。即用户可以自定义每个按键的判定时间。

```c
    // Enable multi-threads mode(enable this only when you do use multi-thread) if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_MULTI_THREADS             1
```

- `SIMPLEBTN_MODE_ENABLE_MULTI_THREADS` 当它被定义为1时，**多线程安全模式**将被开启。会对临界区进行保护。

```c
    // Enable long-push-hold mode if this macro is defined as 1.
#define SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD             1
```

- `SIMPLEBTN_MODE_ENABLE_LONGPUSH_HOLD` 当它被定义为1时，**长按保持**将被开启。不建议与组合键同时使用。如果同时使用，建议将组合键的前置按键的`Public.holdPushMinTime`设置得大一些，防止长按保持抢先触发而导致的组合键失效。

### 自定义选项 Namespace

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

- 您可以使用任意的前缀名称适配您的项目。

- 这个改动会影响您创建出来的函数的初始化和调用（不包括动态按键）。

- 假设您在`SIMPLEBTN__CREATE()`中定义按键的名字为`name`。原本您需要使用`SimpleButton_name_Init()`来初始化按键，再使用`SimpleButton_name.Methos`来调用处理函数；而如果您将`SIMPLEBTN_NAMESPACE`定义为`SB_`，您就需要改用`SB_name_Init()`来初始化按键，再使用`SB_name.Methos`来调用处理函数。

- `SIMPLEBTN_NAMESPACE`可以定义为空。

### 自定义选项 Initialization-Function

```c
/** @b ================================================================ **/
/** @b Initialization-Function */

/* This macro just forward the parameter to another function */
#define SIMPLEBTN_FUNC_INIT_EXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X) \
    simpleButton_Private_InitEXTI(GPIOX_Base, GPIO_Pin_X, EXTI_Trigger_X)
```

- 其中`simpleButton_Private_InitEXTI()`应当在下方`OTHER LOCAL-PLATFORM CUSTOMIZATION`处被实现为`static inline`函数，方便编译器内联优化并进行常量传播优化。

- 我们提供了一个框架如下，您只需补全即可（`SIMPLEBTN_FORCE_INLINE`会尽量要求编译器内联）：

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
