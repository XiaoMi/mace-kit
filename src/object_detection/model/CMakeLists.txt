set(OBJECT_DETECTION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libmodel_object_detection.so")
add_library(object_detection_model SHARED IMPORTED GLOBAL)
set_target_properties(object_detection_model PROPERTIES IMPORTED_LOCATION ${OBJECT_DETECTION_MODEL})

install(FILES ${OBJECT_DETECTION_MODEL} DESTINATION lib)
