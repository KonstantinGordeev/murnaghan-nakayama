cmake_minimum_required(VERSION 3.24)
project(cpp)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

find_package(Boost 1.84 REQUIRED)

add_executable(cpp main.cpp lib.h partitions.h performance.h)
add_executable(test test.cpp lib.h partitions.h)

target_link_libraries(cpp PRIVATE Boost::headers)
target_link_libraries(test PRIVATE Boost::headers)