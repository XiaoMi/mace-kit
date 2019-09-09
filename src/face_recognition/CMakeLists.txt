add_subdirectory(model)

set(FACE_RECOGNITION_SRCS
        face_recognition_impl.cc)

add_library(face_recognition SHARED ${FACE_RECOGNITION_SRCS})
target_link_libraries(face_recognition PRIVATE face_recognition_model mace core)

install(TARGETS face_recognition ARCHIVE DESTINATION lib)
