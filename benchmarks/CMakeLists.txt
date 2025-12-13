include(FetchContent)
set(BENCHMARK_ENABLE_TESTING off)    # to suppress benchmark internal tests
FetchContent_Declare(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.6.0)              # or "master" for latest
FetchContent_MakeAvailable(googlebenchmark)

add_executable( benchmarks
    benchmarks.cpp
)

target_link_libraries( benchmarks
    Z7
    benchmark::benchmark
)
