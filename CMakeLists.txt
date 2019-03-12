cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME "checklist-server")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

# sqlite
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
add_subdirectory(external/sqlitecpp)


set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/rapidjson)
include_directories(external/rapidjson/include)
# sources
add_subdirectory(src)

enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(external/googletest)
include_directories(external/googletest/include)
add_subdirectory(tests)
