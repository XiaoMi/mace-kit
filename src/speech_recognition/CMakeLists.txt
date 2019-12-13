#add_subdirectory (model)
set(ASR_SRCS mace-online-decode.cc)
add_executable(mace_online_decode ${ASR_SRCS})

#link_libraries(${MACE_LIB} ${KALDI_MACE_LIB})

#add link library
TARGET_LINK_LIBRARIES(mace_online_decode PRIVATE kaldi-mace mace)
install (TARGETS mace_online_decode DESTINATION bin)
