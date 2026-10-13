cmake_minimum_required(VERSION 3.28)
project(LogFlow VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(logflow
    src/main.cpp
    src/log_parser.cpp
    src/analyzer.cpp
    src/report.cpp
)

if(MSVC)
    target_compile_options(logflow PRIVATE /W4)
else()
    target_compile_options(logflow PRIVATE -Wall -Wextra)
endif()

add_executable(early_exit_tests
    tests/early_exit_tests.cpp
    src/legacy_reader.cpp
    src/file_tracker.cpp
    src/log_parser.cpp
)
target_include_directories(early_exit_tests PRIVATE src)

if(MSVC)
    target_compile_options(early_exit_tests PRIVATE /W4)
    target_compile_definitions(early_exit_tests PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(early_exit_tests PRIVATE -Wall -Wextra)
endif()

add_executable(reader_tests
    tests/reader_tests.cpp
    src/batch_reader.cpp
    src/log_parser.cpp
)
target_include_directories(reader_tests PRIVATE src)

if(MSVC)
    target_compile_options(reader_tests PRIVATE /W4)
    target_compile_definitions(reader_tests PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(reader_tests PRIVATE -Wall -Wextra)
endif()
