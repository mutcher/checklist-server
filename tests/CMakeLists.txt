set(SOURCE_FILES
        main.cpp
        HttpRequstUnitTests.cpp
)

add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests gtest network)
target_include_directories(tests PRIVATE "../src/")
add_test(NAME tests COMMAND tests)