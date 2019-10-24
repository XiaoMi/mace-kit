if(ANDROID_ABI)
    set(FACE_RECOGNITION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libmodel_face_recognition.so")
else(ANDROID_ABI)
    set(FACE_RECOGNITION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/linux-x86-64/libmodel_face_recognition.so")
endif(ANDROID_ABI)

add_library(face_recognition_model SHARED IMPORTED GLOBAL)
set_target_properties(face_recognition_model PROPERTIES IMPORTED_LOCATION ${FACE_RECOGNITION_MODEL})

install(FILES ${FACE_RECOGNITION_MODEL} DESTINATION lib)
