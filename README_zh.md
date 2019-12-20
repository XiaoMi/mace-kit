MACE-Kit: a Machine Learning Toolkit Powered by MACE
====================================================

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)

近年来，随着机器学习技术的不断发展，人工智能应用实际落地变得越来越普遍。

对于人工智能应用开发者，基于性能出色的机器学习框架开发应用，
可以大大减少开发工作量，提升工作效率。

因此，**Mace-Kit**提供了一套易于上手的模型工具库，可以帮助开发者更加快速
的实现一个人工智能应用，目前该工具提供了以下一些Demo API以供方便使用，
并在不断完善中：

Feature List
-------------

- [x] Face Detection
- [x] Face Recognition
- [x] Object Detection
- [x] Person Segmentation
- [x] Speech Recognition
- [ ] ...

## 准备环境

| 依赖  | 安装命令  | 验证可用的版本  |
| :-------: | :-------------------: | :-------------: |
| ADB  | apt-get install android-tools-adb  | Required by Android run, >= 1.0.32  |
| Android NDK  | [NDK installation guide](https://developer.android.com/ndk/guides/setup#install) | Required by Android build, r17c |
| CMake  | apt-get install cmake  | >= 3.11.3  |


How to build
-------------

Build with Android NDK
```sh
tool/cmake-build-android-armeabi-v7a.sh
```

Test on android
```bash
tool/cctest-android.sh
```

For more information(User Guide for Demos, How to contribute, FAQs), please read the [documentation](docs).

## License
[Apache License 2.0](LICENSE).

## Notice
For [third party](third_party) dependencies, please refer to thier licenses.
