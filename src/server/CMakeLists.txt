set(SOURCE_FILES
        RestApiService.cpp
        main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "..")
target_link_libraries(${PROJECT_NAME} data network SQLiteCpp)