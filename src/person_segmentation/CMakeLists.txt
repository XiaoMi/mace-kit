add_subdirectory(model)

set(PERSON_SEGMENTATION_SRCS
        person_segmentation_impl.cc)

add_library(person_segmentation SHARED ${PERSON_SEGMENTATION_SRCS})
target_link_libraries(person_segmentation PRIVATE person_segmentation_model mace util core)
if(NOT APPLE)
    set_target_properties(person_segmentation PROPERTIES LINK_FLAGS
            "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version_script.lds")
endif(NOT APPLE)


install(TARGETS person_segmentation DESTINATION lib)
