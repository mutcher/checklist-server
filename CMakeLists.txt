cmake_minimum_required (VERSION 2.8.11)
project(server)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -std=c++11 -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)
include_directories(inc)

