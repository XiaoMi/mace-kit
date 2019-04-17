MACE-Kit: a Machine Learning Toolkit Powered by MACE
====================================================

How to build
-------------
Build with local
```sh
mkdir build
cd build
cmake ../ && make
```

Build with Android NDK
```sh
mkdir build
cd build
cmake -DANDROID_ABI=armeabi-v7a \
      -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
      -DANDROID_NATIVE_API_LEVEL=16 \
      ..
make
```
