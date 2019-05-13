cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.111.tar.gz"
    SHA1 "4189bbd869abe4eb69b5303d3f916c03d34dc154"
)

project(MaceKit)

option(MACEKIT_BUILD_TESTS     "whether to build unit tests" ON)
option(MACEKIT_BUILD_BENCHMARK "whether to build micro benchmarks" ON)

# TODO add cross toolchain with Polly: https://github.com/ruslo/polly

add_definitions(-std=c++11)

# Download dependencies
hunter_add_package(gflags)
find_package(gflags CONFIG REQUIRED)

include_directories(include)
add_subdirectory(src)
if (MACEKIT_BUILD_TESTS)
  add_subdirectory(test/cpp)
endif()
if (MACEKIT_BUILD_BENCHMARK)
  add_subdirectory(test/benchmark)
endif()

