# During developing we copy mace libs here, after that we will manage this
# dependency using external link.

set(MACE_SRCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mace")
set(MACE_INCLUDE_DIR "${MACE_SRCS_DIR}/include" CACHE PATH "mace include directory." FORCE)
set(MACE_LIB_DIR "${MACE_SRCS_DIR}/lib" CACHE PATH "mace lib directory." FORCE)

include_directories(SYSTEM ${MACE_INCLUDE_DIR})

if (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/${ANDROID_ABI}/libmace.so" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
else (ANDROID_ABI)
    set(MACE_LIBRARIES "${MACE_LIB_DIR}/linux-x86-64/libmace.so" CACHE FILEPATH "MACE_LIBRARIES" FORCE)
endif (ANDROID_ABI)

add_library(mace SHARED IMPORTED GLOBAL)
set_property(TARGET mace PROPERTY IMPORTED_LOCATION ${MACE_LIBRARIES})

install(FILES ${MACE_LIBRARIES} DESTINATION lib)