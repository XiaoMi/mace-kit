set(FACE_DETECTION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libmodel_face_detection.so")
add_library(face_detection_model SHARED IMPORTED GLOBAL)
set_target_properties(face_detection_model PROPERTIES IMPORTED_LOCATION ${FACE_DETECTION_MODEL})

install(FILES ${FACE_DETECTION_MODEL} DESTINATION lib)
