# API

[English](./API.md)
[中文](./API_zh.md)

---

## Naming Conventions

- All open functions start with `SimpleButton_` (or a user-defined prefix)

- All private functions start with `simpleButton_Private_`

- All open macros start with `SIMPLEBTN__`

- All private macros start with `SIMPLEBTN_`

- Header file macros and version-related macros start with `SIMPLEBUTTON_`

- All private variables start with `simpleButton_`

- All private custom types start with `simpleButton_Type_` and end with `_t`

- All open custom types start with `SimpleButton_Type_` and end with `_t`

- All state machine enums start with `simpleButton_State_`


## Public Macros

1. `SIMPLEBTN__CREATE(GPIOx_BASE, GPIO_Pin_x, EXTI_Trigger_x, __name)`
    - **Function**: Create a button statically and name it `__name` (as an input parameter).
    - **Parameters**:
        - `GPIOx_BASE`: The address of the GPIOX pin to which the button is connected, where X can be A/B/C/D...
        - `GPIO_Pin_x`: The macro for the connected pin of the button, for example, in the HAL library it is GPIO_PIN_X, where X can be 0-15.
        - `EXTI_Trigger_x`: Whether to trigger the button on the rising edge or the falling edge.
        - `__name`: The name of the button.
    - **Internal Principle**: This macro generates a structure variable as the button object, and defines an open initialization function and two private processing functions. The initialization function copies the two private functions to the function pointer variable of the button object, and calls them as "methods".
    - **Other**: Initialize using `prefix/namespace` + `__name` + `_Init`, and call using `prefix/namespace` + `__name`. The `prefix/namespace` defaults to `SimpleButton_`. 

2. `SIMPLEBTN__DECLARE(__name)`
    - **Function**: Declare a button.
    - **Parameters**:
        - `__name`: The name of the button. This name must have been created by `SIMPLEBTN__CREATE()`. 

3. `SIMPLEBTN__START_LOWPOWER(...) `
    - **Function**: Check whether all buttons are in the idle state (Wait For Interrupt). If so, call `SIMPLEBTN_FUNC_START_LOW_POWER()` to enter the low-power mode. `SIMPLEBTN_FUNC_START_LOW_POWER()` is an interface implemented by the user in `simple_button_config.h`.
    - **Parameters**:
    - `__VA_ARGS__`: The number of parameters is variable (1 to 32), and all button objects (whether static or dynamic buttons need to be passed in).

## Public Functions

1. **SimpleButton_DynamicButton_Init**

```c
SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Init(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_GPIOBase_t    GPIO_Base,
    simpleButton_Type_GPIOPin_t     GPIO_Pin,
    simpleButton_Type_GPIOPinVal_t  normalPinVal
);
```

- **Function**: Initializes a dynamic button. Before this, you need to create a variable of type `SimpleButton_Type_DynamicBtn_t` as the dynamic button object. This object can be dynamically created or statically created, and can be global or local. Dynamic buttons rely on polling instead of relying on EXTI like static buttons, and are very flexible.
- **Parameters**:
    - `self`: Address of the dynamic button object
    - `GPIO_Base`: Address of the GPIOX pin to which the button is connected, X can be A/B/C/D...
    - `GPIO_Pin`: Macro for the connected pin of the button, for example, in the HAL library it is GPIO_PIN_X, X can be 0 to 15
    - `normalPinVal`: The pin level when the button is not pressed, high level is 1, low level is 0 

2. **SimpleButton_DynamicButton_Handler**

```c
SIMPLEBTN_C_API void
SimpleButton_DynamicButton_Handler(
    SimpleButton_Type_DynamicBtn_t* const self,
    simpleButton_Type_ShortPushCallBack_t shortPushCallBack,
    simpleButton_Type_LongPushCallBack_t longPushCallBack,
    simpleButton_Type_RepeatPushCallBack_t repeatPushCallBack
);
```

- **Function**: Poll the button status and process the status.
- **Parameters**:
    - `self`: Address of the dynamic button object
    - `shortPushCallBack`: Callback function for short press
    - `longPushCallBack`: Callback function for long press
    - `repeatPushCallBack`: Callback function for double-click/multi-click
