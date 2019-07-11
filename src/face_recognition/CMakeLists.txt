add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/model)

set(FACE_RECOGNITION_SRCS
        face_recognition_impl.cc)

add_library(face_recognition STATIC ${FACE_RECOGNITION_SRCS})
target_include_directories(face_recognition PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/model/include/mace/public)
target_link_libraries(face_recognition PRIVATE util mace face_recognition_model libprotobuf_lite)

install(TARGETS face_recognition ARCHIVE DESTINATION lib)

