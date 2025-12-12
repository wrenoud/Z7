cmake_minimum_required(VERSION 3.24)
project(Z7)

set(CMAKE_CXX_STANDARD 17)

add_library(Z7 STATIC library.cpp)

add_subdirectory(tests)
