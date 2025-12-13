cmake_minimum_required(VERSION 3.24)
project(Z7)

set(CMAKE_CXX_STANDARD 17)

add_library(Z7 STATIC library.cpp)

target_compile_options(Z7 PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)

add_subdirectory(benchmarks)
add_subdirectory(tests)
