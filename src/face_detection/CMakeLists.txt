set(FACE_DETECTION_SRCS
        face_detection.cc
        )

add_library(face_detection STATIC ${FACE_DETECTION_SRCS})
target_link_libraries(face_detection util)

install(TARGETS face_detection ARCHIVE DESTINATION lib)

