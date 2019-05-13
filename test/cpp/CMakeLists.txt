include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

hunter_add_package(GTest)

find_package(GTest CONFIG REQUIRED) # GTest::main

add_executable(util_test ssd_bbox_test.cc)
target_link_libraries(util_test PUBLIC GTest::main)
target_link_libraries(util_test PUBLIC util)

enable_testing()

if(IOS)
  # Extra cmake modules needed for iOS testing:
  # * https://github.com/ruslo/sugar/tree/master/cmake/core#sugar_add_ios_gtest
  add_test(
      NAME UtilTest COMMAND "${CMAKE_COMMAND}" -E echo "iOS testing ignored"
  )
elseif(ANDROID)
  # Building .apk, testing: https://github.com/forexample/android-cmake
  add_test(
      NAME UtilTest
      COMMAND "${CMAKE_COMMAND}" -E echo "Android testing ignored"
  )
else()
  add_test(NAME UtilTest COMMAND util_test)
endif()
