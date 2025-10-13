# Derivative Project Specification

[English](./Derivative-Project-Specification.md)
[中文](./Derivative-Project-Specification_zh.md)

---

## Introduction

- In order to simplify and standardize the development process of Derivative open source projects, we define the **Derivative Project Specification**, hereinafter referred to as **specification**.

- This specification is a recommendation, not a requirement. To comply with [LICENSE](../LICENSE) to use the project is your right.

## File Management practices

- The file structure of the project (some files are reserved for future use) is as follows:

```
.
|
+-- docs/ # Documentation
|
+-- examples/ # Example code
|
+-- Simple_Button/ # main code of the project
|
+--.gitattributes # Help display files
|
+-- LICENSE # Open source license
|
+-- README.md/README_zh.md # Project description in English and Chinese
```

- Derivative projects are advised to keep all files except `examples/` for the project.

- where `README.md` and `README_zh.md` are recommended to be moved to `docs/`, and the root directory provides your projects `README.md`.

- It is recommended to change the `LICENSE` filename to `LICENSE-original` and add your `LICENSE` to the root directory.

- For files in the `Simple_Button/` directory, it is recommended that you only modify `simple_button_config.h`. If you have another extension, you can create a new file in this directory.

- When modifying `simple_button_config.h`, you can add your project version number macro. Macro names are recommended to start with `SIMPLEBUTTON_` and end with `VERSION`.

## Function Naming Conventions

- It is recommended to follow [naming conventions](./API.md#naming-conventions).
