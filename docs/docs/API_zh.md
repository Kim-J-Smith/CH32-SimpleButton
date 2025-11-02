# API

[English](./API.md)
[中文](./API_zh.md)

---

## 命名规则

- 所有开放函数使用 `SimpleButton_` (或用户自定义 前缀) 开头

- 所有私有函数使用 `simpleButton_Private_` 开头

- 所有开放的宏使用 `SIMPLEBTN__` 开头

- 所有私有的宏使用 `SIMPLEBTN_` 开头

- 头文件宏与版本相关宏使用 `SIMPLEBUTTON_` 开头

- 所有私有的变量使用 `simpleButton_` 开头

- 所有私有的自定义的类型使用 `simpleButton_Type_` 开头，且以 `_t` 结尾

- 所有开放的自定义的类型使用 `SimpleButton_Type_` 开头，且以 `_t` 结尾

- 所有状态机枚举使用 `simpleButton_State_` 开头


## 开放宏使用详解

1. `SIMPLEBTN__CREATE(GPIOx_BASE, GPIO_Pin_x, EXTI_Trigger_x, __name)`
    - **功能**：静态地创建一个按键，并将其命名为`__name`（传入参数）。
    - **参数**：
        - `GPIOx_BASE`：按键所连接的引脚的GPIOX的地址，X可以是A/B/C/D ...
        - `GPIO_Pin_x`：按键所连接的引脚的宏，例如HAL库是GPIO_PIN_X，X可以是0~15
        - `EXTI_Trigger_x`：上升沿触发按键或下降沿触发按键
        - `__name`：按键的名称。
    - **内部原理**：该宏会生成一个结构体变量作为按键对象，并配套定义一个开放的初始化函数和两个私有的处理函数。初始化函数会将两个私有函数复制到按键对象的函数指针变量上，作为“方法”被调用。
    - **其他**：使用`前缀/命名空间` + `__name` + `_Init`初始化，使用`前缀/命名空间` + `__name`调用。`前缀/命名空间`默认为`SimpleButton_`。

2. `SIMPLEBTN__DECLARE(__name)`
    - **功能**：声明一个按键。
    - **参数**：
        - `__name`：按键的名称。这个名称必须是`SIMPLEBTN__CREATE()`创建过的。

3. `SIMPLEBTN__START_LOWPOWER(...)`
    - **功能**：判断所有按键是否都处于空闲状态(Wait For Interrupt)，如果是，则调用`SIMPLEBTN_FUNC_START_LOW_POWER()`进入低功耗模式。`SIMPLEBTN_FUNC_START_LOW_POWER()`是用户在`simple_button_config.h`中实现的接口。
    - **参数**：
        - `__VA_ARGS__`：参数的个数是可变的（1~32），需要传入所有按键对象（不论是静态还是动态按键都需要传入）。

4. `SIMPLEBTN__CMBBTN_SETCALLBACK(preButton, nextButton, callback)`
    - **功能**：配置组合键的回调函数。
    - **参数**：
        - `preButton`：组合键中，先按下的按键。
        - `nextButton`：组合键中，后按下的按键。
        - `callback`：依次按下按键后，触发的回调函数。

## 开放函数使用详解

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

- **功能**：初始化一个动态按键。在此之前您需要创建一个`SimpleButton_Type_DynamicBtn_t`类型的变量作为动态按键对象。这个对象可以是动态创建的、也可以是静态创建的，可以是全局的、也可以是局部的。动态按键依赖轮询而非像静态按键那样依赖EXTI，十分灵活。
- **参数**：
    - `self`：动态按键对象的地址
    - `GPIO_Base`：按键所连接的引脚的GPIOX的地址，X可以是A/B/C/D ...
    - `GPIO_Pin`：按键所连接的引脚的宏，例如HAL库是GPIO_PIN_X，X可以是0~15
    - `normalPinVal`：按键未被按下时的引脚电平，高电平为1，低电平为0

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

- **功能**：轮询按键状态，并对状态进行处理。
- **参数**：
    - `self`：动态按键对象的地址
    - `shortPushCallBack`：短按回调函数
    - `longPushCallBack`：长按回调函数
    - `repeatPushCallBack`：双击/多击回调函数
