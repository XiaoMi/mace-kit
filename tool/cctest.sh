#!/usr/bin/env sh

adb shell mkdir -p /data/local/tmp/macekit
adb push cmake-build/android-armeabi-v7a/install/lib /data/local/tmp/macekit/
adb push cmake-build/android-armeabi-v7a/install/bin /data/local/tmp/macekit/
adb push data /data/local/tmp/macekit/
adb push ${ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_shared.so /data/local/tmp/macekit/lib/

adb shell "cd /data/local/tmp/macekit; LD_LIBRARY_PATH=lib ./bin/face_detection_unitest"
adb shell "cd /data/local/tmp/macekit; LD_LIBRARY_PATH=lib ./bin/face_recognition_unitest"