add_subdirectory(model)

set(OBJECT_DETECTION_SRCS
        object_detection_impl.cc)

add_library(object_detection SHARED ${OBJECT_DETECTION_SRCS})
target_link_libraries(object_detection PRIVATE object_detection_model mace util core)
if(NOT APPLE)
    set_target_properties(object_detection PROPERTIES LINK_FLAGS
            "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version_script.lds")
endif(NOT APPLE)


install(TARGETS object_detection DESTINATION lib)
