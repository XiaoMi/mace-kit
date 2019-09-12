file(GLOB FACE_DETECTION_CC_TEST_SRCS
        *.cc
        )

add_executable(face_detection_unitest ${FACE_DETECTION_CC_TEST_SRCS})
target_link_libraries(face_detection_unitest PRIVATE
        face_detection
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        ${OpenCV_LIBS}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(face_detection_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS face_detection_unitest RUNTIME DESTINATION bin)