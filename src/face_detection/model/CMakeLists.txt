if(ANDROID_ABI)
    set(FACE_DETECTION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libmodel_face_detection.so")
else(ANDROID_ABI)
    set(FACE_DETECTION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux-x86-64/libmodel_face_detection.so")
endif(ANDROID_ABI)
add_library(face_detection_model SHARED IMPORTED GLOBAL)
set_target_properties(face_detection_model PROPERTIES IMPORTED_LOCATION ${FACE_DETECTION_MODEL})

install(FILES ${FACE_DETECTION_MODEL} DESTINATION lib)
