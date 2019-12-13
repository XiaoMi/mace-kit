# During developing we copy mace libs here, after that we will manage this
# dependency using external link.

set(KALDI_MACE_SRCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/kaldi-mace")
set(KALDI_MACE_INCLUDE_DIR "${KALDI_MACE_SRCS_DIR}" CACHE PATH "kaldi-mace include directory." FORCE)
set(KALDI_MACE_LIB_DIR "${KALDI_MACE_SRCS_DIR}/lib" CACHE PATH "kaldi-mace lib directory." FORCE)

include_directories(SYSTEM ${KALDI_MACE_INCLUDE_DIR})

if (ANDROID_ABI)
    set(KALDI_MACE_LIBRARIES "${KALDI_MACE_LIB_DIR}/${ANDROID_ABI}/libkaldi-mace.so" CACHE FILEPATH "KALDI_MACE_LIBRARIES" FORCE)
else (ANDROID_ABI)
    set(KALDI_MACE_LIBRARIES "${KALDI_MACE_LIB_DIR}/linux-x86-64/libkaldi-mace.so" CACHE FILEPATH "KALDI_MACE_LIBRARIES" FORCE)
endif (ANDROID_ABI)

add_library(kaldi-mace SHARED IMPORTED GLOBAL)
set_property(TARGET kaldi-mace PROPERTY IMPORTED_LOCATION ${KALDI_MACE_LIBRARIES})

install(FILES ${KALDI_MACE_LIBRARIES} DESTINATION lib)
