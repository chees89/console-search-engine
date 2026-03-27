cmake_minimum_required(VERSION 3.17)
project(search_engine VERSION 1.0)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(googletest json)

add_executable(search_engine
    src/main.cpp
    src/ConverterJSON.cpp
    src/InvertedIndex.cpp
    src/SearchServer.cpp
)

target_include_directories(search_engine PRIVATE include)

target_link_libraries(search_engine
    PRIVATE nlohmann_json::nlohmann_json
)

add_executable(search_engine_tests
    src/test.cpp
    src/ConverterJSON.cpp
    src/InvertedIndex.cpp
    src/SearchServer.cpp
)

target_include_directories(search_engine_tests PRIVATE include)

target_link_libraries(search_engine_tests
    PRIVATE nlohmann_json::nlohmann_json
    gtest_main
)

file(COPY config.json requests.json answers.json DESTINATION ${CMAKE_BINARY_DIR})
file(COPY resurses DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()
include(GoogleTest)
gtest_discover_tests(search_engine_tests)
