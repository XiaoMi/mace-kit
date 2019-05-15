file(GLOB MACEKIT_CC_TEST_SRCS
        *.cc
        )

add_executable(macekit_cc_unitest ${MACEKIT_CC_TEST_SRCS})
target_link_libraries(macekit_cc_unitest PUBLIC
        core
        util
        gtest
        gtest_main
        )


install(TARGETS macekit_cc_unitest RUNTIME DESTINATION bin)