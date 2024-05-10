cmake_minimum_required(VERSION 3.24)
project(cpp)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

find_library(PARI_LIBRARY pari REQUIRED)
find_path(PARI_INCLUDE pari/pari.h REQUIRED)

find_library(PARI_LIBRARY pari)

add_executable(jacobi-trudi main.cpp)

target_link_libraries(jacobi-trudi ${PARI_LIBRARY})
target_include_directories(jacobi-trudi PRIVATE ${PARI_INCLUDE})