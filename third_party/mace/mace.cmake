INCLUDE(ExternalProject)

set(MACE_SRCS_DIR "${MACE_THIRD_PARTY_DIR}/mace")
set(MACE_INCLUDE_DIR "${MACE_SRCS_DIR}/include" CACHE PATH "mace include directory." FORCE)
set(MACE_LIB_DIR "${MACE_SRCS_DIR}/lib" CACHE PATH "mace lib directory." FORCE)

if (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/${ANDROID_ABI}/cpu_gpu/libmace.a" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
else (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/linux-x86-64/libmace.a" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
endif (ANDROID_ABI)

include_directories(SYSTEM ${MACE_SRCS_DIR})

set(MACE_URL     "https://github.com/XiaoMi/mace/releases/download/v0.11.0-rc0/libmace.zip")
set(MACE_HASH    "SHA256=65ee7e57d4b9f6b673a1cee87d07adc67a7c781bdf96b0b0d090b1adec87d4e0")

ExternalProject_Add(
        mace
        URL_HASH         "${MACE_HASH}"
        URL              "${MACE_URL}"
        PREFIX           "${MACE_SRCS_DIR}"
        BINARY_DIR        ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
)