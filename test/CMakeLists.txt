if(MACEKIT_ENABLE_TESTS)
    add_subdirectory(ccunit)
endif()

if(MACEKIT_ENABLE_BENCHMARKS)
    add_subdirectory(ccbenchmark)
endif()
