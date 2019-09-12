add_subdirectory(model)

set(FACE_RECOGNITION_SRCS
        face_recognition_impl.cc)

add_library(face_recognition SHARED ${FACE_RECOGNITION_SRCS})
target_link_libraries(face_recognition PRIVATE face_recognition_model mace core)
if(NOT APPLE)
    set_target_properties(face_recognition PROPERTIES LINK_FLAGS
            "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version_script.lds")
endif(NOT APPLE)

install(TARGETS face_recognition DESTINATION lib)
