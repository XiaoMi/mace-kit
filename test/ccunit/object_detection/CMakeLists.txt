file(GLOB OBJECT_DETECTION_CC_TEST_SRCS
        *.cc
        )

add_executable(object_detection_unitest ${OBJECT_DETECTION_CC_TEST_SRCS})
target_link_libraries(object_detection_unitest PRIVATE
        object_detection
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        ${OpenCV_LIBS}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(object_detection_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS object_detection_unitest RUNTIME DESTINATION bin)
