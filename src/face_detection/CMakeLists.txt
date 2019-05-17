add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/model)

set(FACE_DETECTION_SRCS
        face_detection_impl.cc)

add_library(face_detection STATIC ${FACE_DETECTION_SRCS})
target_include_directories(face_detection PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/model/include/mace/public)
target_link_libraries(face_detection PRIVATE util mace face_detection_model libprotobuf_lite)

install(TARGETS face_detection ARCHIVE DESTINATION lib)

