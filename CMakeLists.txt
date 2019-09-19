cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.111.tar.gz"
    SHA1 "4189bbd869abe4eb69b5303d3f916c03d34dc154"
)

project(MaceKit C CXX)

option(MACEKIT_ENABLE_OPENCL    "whether to enable OpenCL support" OFF)
option(MACEKIT_ENABLE_TESTS     "whether to build unit tests" ON)
option(MACEKIT_ENABLE_BENCHMARK "whether to build micro benchmarks" OFF)

message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

set(MACEKIT_CC_FLAGS "${MACEKIT_CC_FLAGS} -fPIC")
if(MACEKIT_ENABLE_OPT_SIZE)
  if(APPLE)
    set(MACEKIT_LINKER_FLAGS "${MACEKIT_LINKER_FLAGS} -Wl,-dead_strip -Wl,-dead_strip_dylibs")
  else(APPLE)
    set(MACEKIT_LINKER_FLAGS "${MACEKIT_LINKER_FLAGS} -Wl,--strip-all -Wl,--gc-sections")
    set(MACEKIT_CC_FLAGS "${MACEKIT_CC_FLAGS} -ffunction-sections -fdata-sections")
  endif(APPLE)
  set(MACEKIT_CC_FLAGS "${MACEKIT_CC_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
  set(MACEKIT_CODE_CC_FLAGS "${MACEKIT_CODE_CC_FLAGS} -fno-rtti -fno-exceptions -DGOOGLE_PROTOBUF_NO_RTTI -DPROTOBUF_USE_EXCEPTIONS=0")
endif(MACEKIT_ENABLE_OPT_SIZE)

# flags apply only to mace-kit code (third_party excluded)
set(MACEKIT_CODE_CC_FLAGS "${MACEKIT_CODE_CC_FLAGS} -Wall -Werror")
set(MACEKIT_CODE_CC_FLAGS "${MACEKIT_CODE_CC_FLAGS} -std=c++11 -D_GLIBCXX_USE_C99_MATH_TR1")

if(IOS)
  # TODO correct the code
  set(MACEKIT_CODE_CC_FLAGS "${MACEKIT_CODE_CC_FLAGS} -Wno-error=shorten-64-to-32")
endif(IOS)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MACEKIT_CODE_CC_FLAGS} ${MACEKIT_CC_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MACEKIT_LINKER_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MACEKIT_LINKER_FLAGS}")
endif(NOT MSVC)

if(MACEKIT_ENABLE_OPENCL)
  if(IOS)
    message(FATAL_ERROR "OpenCL is not supported for iOS")
  endif(IOS)
  add_definitions(-DMACEKIT_ENABLE_OPENCL)
endif(MACEKIT_ENABLE_OPENCL)

include(${PROJECT_SOURCE_DIR}/third_party/third_party.cmake)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

