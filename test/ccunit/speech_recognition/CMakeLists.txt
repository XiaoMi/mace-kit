file(GLOB SPEECH_RECOGNITION_CC_TEST_SRCS
        *.cc
        )

add_executable(speech_recognition_unitest ${SPEECH_RECOGNITION_CC_TEST_SRCS})
target_link_libraries(speech_recognition_unitest PRIVATE
        kaldi-mace
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(speech_recognition_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS speech_recognition_unitest RUNTIME DESTINATION bin)
