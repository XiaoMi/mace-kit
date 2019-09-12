file(GLOB UTIL_CC_TEST_SRCS
        *.cc
        )

add_executable(util_unitest ${UTIL_CC_TEST_SRCS})
target_link_libraries(util_unitest PRIVATE
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        ${OpenCV_LIBS}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(util_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS util_unitest RUNTIME DESTINATION bin)