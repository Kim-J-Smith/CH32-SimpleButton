# CH32-SimpleButton

VERSION 0.0.2

**Simple** and tiny CH32 key(button) frame, compatible with the CH32 Standard library, which offer **short-press/long-press/repeat-press/combination-press** for each button, non-blocking.

一个单文件的CH32按键框架，**5行代码**完成按键部署，适配CH32 标准库，支持每个按键独立的 **短按/长按/多击/组合键**，采用外部中断加循环内异步处理，非阻塞状态机。

---

* 基于[STM32-SimpleButton](https://github.com/Kim-J-Smith/STM32-SimpleButton)的0.1.8版本

* 作为项目[STM32-SimpleButton](https://github.com/Kim-J-Smith/STM32-SimpleButton)针对CH32V系列芯片的深度定制版本，定制方法遵循[Developer-Guide](https://github.com/Kim-J-Smith/STM32-SimpleButton/blob/main/Developer_Guide.md)。

* 【注意】该项目会与ch32v系列自带的Delay_xxx()函数冲突，项目提供了 **KIM_DELAY_MS()** 和 **KIM_DELAY_US()** 宏函数来代替
