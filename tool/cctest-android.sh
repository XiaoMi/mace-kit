#!/usr/bin/env sh
array=($(adb devices | grep ".device$"))
i=0
length=${#array[@]}
while [ "$i" -lt "$length" ];do
    if 
        ((i%2!=0))
    then
        unset array[i]  
    fi
    ((i++))
done
for serialno in ${array[@]}; do
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit
    adb -s $serialno push cmake-build/android-armeabi-v7a/install/lib /data/local/tmp/macekit/
    adb -s $serialno push cmake-build/android-armeabi-v7a/install/bin /data/local/tmp/macekit/
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data
    adb -s $serialno push data/asr-tiny /data/local/tmp/macekit/data/
    adb -s $serialno push data/test /data/local/tmp/macekit/data/
    adb -s $serialno push ${ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_shared.so /data/local/tmp/macekit/lib/libc++_shared.so

    adb -s $serialno shell "cd /data/local/tmp/macekit; LD_LIBRARY_PATH=lib ./bin/face_detection_unitest"
    adb -s $serialno shell "cd /data/local/tmp/macekit; LD_LIBRARY_PATH=lib ./bin/face_recognition_unitest"
    adb -s $serialno shell "cd /data/local/tmp/macekit; LD_LIBRARY_PATH=lib ./bin/speech_recognition_unitest"
done
