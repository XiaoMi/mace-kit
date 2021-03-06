INCLUDE(ExternalProject)

set(MACE_SRCS_DIR "${MACEKIT_THIRD_PARTY_DIR}/mace")
set(MACE_INCLUDE_DIR "${MACE_SRCS_DIR}/src/extern_mace/include" CACHE PATH "mace include directory." FORCE)
set(MACE_LIB_DIR "${MACE_SRCS_DIR}/src/extern_mace/lib" CACHE PATH "mace lib directory." FORCE)

include_directories(SYSTEM ${MACE_INCLUDE_DIR})

if (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/${ANDROID_ABI}/cpu_gpu/libmace.so" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
else (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/linux-x86-64/libmace.so" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
endif (ANDROID_ABI)

set(MACE_URL     "http://cnbj1.fds.api.xiaomi.com/mace/release/libmace-0.11.0-rc0.zip")
set(MACE_HASH    "SHA256=65ee7e57d4b9f6b673a1cee87d07adc67a7c781bdf96b0b0d090b1adec87d4e0")

ExternalProject_Add(
        extern_mace
        URL_HASH         "${MACE_HASH}"
        URL              "${MACE_URL}"
        PREFIX           "${MACE_SRCS_DIR}"
        BINARY_DIR        ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
)

add_library(mace SHARED IMPORTED GLOBAL)
set_property(TARGET mace PROPERTY IMPORTED_LOCATION ${MACE_LIBRARIES})
add_dependencies(mace extern_mace)

install(FILES ${MACE_LIBRARIES} DESTINATION lib)