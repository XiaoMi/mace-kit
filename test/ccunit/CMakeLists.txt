file(GLOB MACEKIT_CC_TEST_SRCS
        util/*.cc
        face_detection/*.cc
        )

if (ANDROID_ABI)
find_library(
        log-lib
        log )
endif(ANDROID_ABI)

add_executable(macekit_cc_unitest ${MACEKIT_CC_TEST_SRCS})
target_link_libraries(macekit_cc_unitest PRIVATE
        core
        util
        face_detection
        gtest
        gtest_main
        ${log-lib}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(macekit_cc_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS macekit_cc_unitest RUNTIME DESTINATION bin)