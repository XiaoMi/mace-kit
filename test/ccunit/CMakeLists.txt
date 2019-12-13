if (ANDROID_ABI)
find_library(
        log-lib
        log )
endif(ANDROID_ABI)

hunter_add_package(OpenCV)
find_package(OpenCV REQUIRED)
message("OpenCV_DIR: ${OpenCV_DIR}")
message("OpenCV_CONFIG: ${OpenCV_CONFIG}")
message("OpenCV_LIBS: ${OpenCV_LIBS}")

add_subdirectory(face_detection)
add_subdirectory(face_recognition)
add_subdirectory(speech_recognition)
add_subdirectory(object_detection)
add_subdirectory(util)
