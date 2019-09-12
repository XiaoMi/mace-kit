file(GLOB FACE_RECOGNITION_CC_TEST_SRCS
        *.cc
        )

add_executable(face_recognition_unitest ${FACE_RECOGNITION_CC_TEST_SRCS})
target_link_libraries(face_recognition_unitest PRIVATE
        face_recognition
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        ${OpenCV_LIBS}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(face_recognition_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS face_recognition_unitest RUNTIME DESTINATION bin)