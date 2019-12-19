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
ERRORS=("Aborted" "FAILED" "CANNOT LINK EXECUTABLE" "Segmentation fault")
function check_err() {
  echo "$1"
  for i in "${ERRORS[@]}"; do
  if  grep -q "$i" <<< "$1"; then
    echo "Test Failed!"
    exit 1
  fi
  done
}
for serialno in ${array[@]}; do
    echo "========== Test on device(${serialno}) ==========="
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/lib
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/bin
    adb -s $serialno push cmake-build/android-armeabi-v7a/install/lib/. /data/local/tmp/macekit/lib
    adb -s $serialno push cmake-build/android-armeabi-v7a/install/bin/. /data/local/tmp/macekit/bin
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data/asr-tiny/online/conf
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data/asr-tiny/online/ivector_extractor
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data/asr-tiny/model
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data/asr-tiny/graph_tiny/phones
    adb -s $serialno shell mkdir -p /data/local/tmp/macekit/data/test
    adb -s $serialno push data/asr-tiny/. /data/local/tmp/macekit/data/asr-tiny
    adb -s $serialno push data/test/. /data/local/tmp/macekit/data/test
    adb -s $serialno push ${ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_shared.so /data/local/tmp/macekit/lib/libc++_shared.so

    OUTPUT=$(adb -s $serialno shell "cd /data/local/tmp/macekit; MACE_INTERNAL_STORAGE_PATH=./ LD_LIBRARY_PATH=lib ./bin/face_detection_unitest")
    check_err "$OUTPUT"
    OUTPUT=$(adb -s $serialno shell "cd /data/local/tmp/macekit; MACE_INTERNAL_STORAGE_PATH=./ LD_LIBRARY_PATH=lib ./bin/face_recognition_unitest")
    check_err "$OUTPUT"
    OUTPUT=$(adb -s $serialno shell "cd /data/local/tmp/macekit; MACE_INTERNAL_STORAGE_PATH=./ LD_LIBRARY_PATH=lib ./bin/speech_recognition_unitest")
    check_err "$OUTPUT"
    OUTPUT=$(adb -s $serialno shell "cd /data/local/tmp/macekit; MACE_INTERNAL_STORAGE_PATH=./ LD_LIBRARY_PATH=lib ./bin/object_detection_unitest")
    check_err "$OUTPUT"
    OUTPUT=$(adb -s $serialno shell "cd /data/local/tmp/macekit; MACE_INTERNAL_STORAGE_PATH=./ LD_LIBRARY_PATH=lib ./bin/person_segmentation_unitest")
    check_err "$OUTPUT"
    adb -s $serialno shell "cd /data/local/tmp; rm -rf macekit"
done
