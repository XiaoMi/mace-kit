file(GLOB PERSON_SEGMENTATION_CC_TEST_SRCS
        *.cc
        )

add_executable(person_segmentation_unitest ${PERSON_SEGMENTATION_CC_TEST_SRCS})
target_link_libraries(person_segmentation_unitest PRIVATE
        person_segmentation
        mace
        core
        util
        gtest
        gtest_main
        ${log-lib}
        ${OpenCV_LIBS}
        )

if(NOT ANDROID AND NOT WIN32)
    target_link_libraries(person_segmentation_unitest PRIVATE pthread)
endif(NOT ANDROID AND NOT WIN32)


install(TARGETS person_segmentation_unitest RUNTIME DESTINATION bin)
