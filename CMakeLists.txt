cmake_minimum_required(VERSION 3.7)
project(smdedup)

set(CMAKE_VERBOSE_MAKEFILE 1)

#include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_library(stdc++fs UNKNOWN IMPORTED)
#set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a")

include_directories("htrie/src")

#uh oh
#find_package(CryptoPP REQUIRED)

set(SOURCE_FILES main.cpp
    crc32.c
    keycrc.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_compile_options(${PROJECT_NAME} PRIVATE -O0 -ggdb3 -march=native)
target_compile_options(${PROJECT_NAME} PRIVATE -Ofast -ggdb0 -march=native)
target_link_libraries(${PROJECT_NAME} stdc++fs pthread cryptopp)
