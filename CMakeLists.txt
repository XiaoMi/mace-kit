cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.111.tar.gz"
    SHA1 "4189bbd869abe4eb69b5303d3f916c03d34dc154"
)

project(MaceKit)

option(MACEKIT_UNIT_TESTS "whether to build unit tests" ON)

# TODO add cross toolchain with Polly: https://github.com/ruslo/polly

#add_subdirectory(src)
if (MACEKIT_UNIT_TESTS)
  add_subdirectory(test/cc)
endif()

