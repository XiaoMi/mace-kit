add_subdirectory(model)

set(FACE_DETECTION_SRCS
        face_detection_impl.cc)

add_library(face_detection SHARED ${FACE_DETECTION_SRCS})
target_link_libraries(face_detection PRIVATE face_detection_model mace util core)
if(NOT APPLE)
    set_target_properties(face_detection PROPERTIES LINK_FLAGS
            "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version_script.lds")
endif(NOT APPLE)


install(TARGETS face_detection DESTINATION lib)
