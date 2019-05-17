set(CORE_SRCS
        mat.cc
        ../../include/macekit/type.h ../../include/macekit/status.h ../../include/macekit/export.h)

add_library(core STATIC ${CORE_SRCS})

