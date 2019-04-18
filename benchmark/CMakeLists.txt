hunter_add_package(benchmark)

find_package(benchmark CONFIG REQUIRED)

add_executable(benchmark example_benchmark.cc)
target_link_libraries(benchmark benchmark::benchmark)
