add_executable(example example.cc)

target_link_libraries(example PUBLIC gflags)

if(NOT ANDROID AND NOT WIN32)
  target_link_libraries(example PUBLIC pthread)
endif(NOT ANDROID AND NOT WIN32)


hunter_add_package(OpenCV)
find_package(OpenCV REQUIRED)
message("OpenCV_DIR: ${OpenCV_DIR}")
message("OpenCV_CONFIG: ${OpenCV_CONFIG}")
message("OpenCV_LIBS: ${OpenCV_LIBS}")

add_executable(draw_anchor draw_anchor.cc)
target_link_libraries(draw_anchor ${OpenCV_LIBS})

install(TARGETS draw_anchor RUNTIME DESTINATION bin)

