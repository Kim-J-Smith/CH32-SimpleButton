# Derivative Project Specification

[English](./Derivative-Project-Specification.md)
[中文](./Derivative-Project-Specification_zh.md)

---

## 简介

- 为了简化、规范化衍生开源项目的开发流程，我们定义了《**衍生项目规范（Derivative Project Specification）**》，以下简称**规范**。

- 这个规范只是建议，而非强制要求。以遵循[LICENSE](../LICENSE)的方式使用项目是您的权利。

## 文件管理规范

- 本项目的文件结构（有些文件是保留各未来使用的）如下：

```
.
|
+-- docs/  # 文档资料
|
+-- examples/  # 示例代码
|
+-- Simple_Button/  # 项目主要代码
|
+-- .gitattributes  # 辅助显示文件
|
+-- LICENSE  # 开源协议
|
+-- README.md/README_zh.md  # 中英文项目说明
```

- 衍生项目建议保留本项目除`examples/`外的全部文件。

- 其中`README.md`与`README_zh.md`建议转移至`docs/`当中，根目录提供您的项目的`README.md`。

- `LICENSE`文件名建议修改为`LICENSE-original`，并在根目录添加您的`LICENSE`。

- 对于`Simple_Button/`目录中的文件，建议您只修改`simple_button_config.h`。如果有别的拓展，可以在该目录下新建文件。

- 修改`simple_button_config.h`时，您可以加入您的项目版本管理号宏，宏命名建议以`SIMPLEBUTTON_`开头，以`VERSION`结尾。

## 函数命名规范

- 建议遵循[命名规则](./API_zh.md#命名规则)。
