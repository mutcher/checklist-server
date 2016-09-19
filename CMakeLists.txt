cmake_minimum_required (VERSION 2.8.7)
project(server)

if (MSVC)

else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -pthread")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -std=c++11 -pthread")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB Inc_Files inc/*.h)
file(GLOB Src_Files src/*.cpp)

source_group("Include Files" FILES ${Inc_Files})
source_group("Source Files" FILES ${Src_Files})

include_directories(${CMAKE_SOURCE_DIR})
add_executable(${CMAKE_PROJECT_NAME} ${Src_Files} ${Inc_Files})

