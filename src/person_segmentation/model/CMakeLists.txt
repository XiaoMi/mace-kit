set(PERSON_SEGMENTATION_MODEL "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libmodel_person_segmentation.so")
add_library(person_segmentation_model SHARED IMPORTED GLOBAL)
set_target_properties(person_segmentation_model PROPERTIES IMPORTED_LOCATION ${PERSON_SEGMENTATION_MODEL})

install(FILES ${PERSON_SEGMENTATION_MODEL} DESTINATION lib)
