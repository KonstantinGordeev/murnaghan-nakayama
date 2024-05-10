cmake_minimum_required(VERSION 3.24)
project(cpp)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

find_library(PARI_LIBRARY pari REQUIRED)
find_path(PARI_INCLUDE pari/pari.h REQUIRED)

#find_package(Boost 1.84 REQUIRED)
find_library(PARI_LIBRARY pari)
#include_directories(/usr/local/lib libpari.dylib)
#
#add_executable(cpp main.cpp lib.h partitions.h performance.h pari/test.cpp)
#add_executable(test test.cpp lib.h partitions.h)
add_executable(pari jacobi-trudi/test.cpp)
#
#target_link_libraries(cpp PRIVATE Boost::headers)
#target_link_libraries(test PRIVATE Boost::headers)
target_link_libraries(pari ${PARI_LIBRARY})
target_include_directories(pari PRIVATE ${PARI_INCLUDE})