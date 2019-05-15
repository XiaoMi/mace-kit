set(UTIL_SRCS
        ssd_bbox.cc
        )

add_library(util STATIC ${UTIL_SRCS})
target_link_libraries(util PRIVATE core)
