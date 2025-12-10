cmake_minimum_required(VERSION 3.24)
project(Z7)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(Z7 STATIC library.cpp)


enable_testing()

add_executable( tests
    tests.cpp
)

target_link_libraries( tests
    Z7
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)