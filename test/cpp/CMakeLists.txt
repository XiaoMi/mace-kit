hunter_add_package(GTest)

find_package(GTest CONFIG REQUIRED) # GTest::main

add_executable(example_test example_test.cc)
target_link_libraries(example_test PUBLIC GTest::main)

enable_testing()

if(IOS)
  # Extra cmake modules needed for iOS testing:
  # * https://github.com/ruslo/sugar/tree/master/cmake/core#sugar_add_ios_gtest
  add_test(
      NAME ExampleTest COMMAND "${CMAKE_COMMAND}" -E echo "iOS testing ignored"
  )
elseif(ANDROID)
  # Building .apk, testing: https://github.com/forexample/android-cmake
  add_test(
      NAME ExampleTest
      COMMAND "${CMAKE_COMMAND}" -E echo "Android testing ignored"
  )
else()
  add_test(NAME ExampleTest COMMAND example_test)
endif()
