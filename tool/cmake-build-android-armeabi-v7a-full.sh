#!/usr/bin/env sh

set -e

# build for android armeabi-v7a
BUILD_DIR=cmake-build/android-armeabi-v7a-full
rm -rf ${BUILD_DIR} && mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR}
cmake -DANDROID_ABI="armeabi-v7a" \
      -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
      -DANDROID_NATIVE_API_LEVEL=28       \
      -DCMAKE_BUILD_TYPE=Release          \
      -DCMAKE_ANDROID_STL_TYPE=c++_shared \
      -DCMAKE_INSTALL_PREFIX=install      \
      ../..
make -j6 VERBOSE=1 && make install
cd ../..
