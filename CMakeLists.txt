cmake_minimum_required (VERSION 3.12)

project (Matrix LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (COMMON_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>)

add_executable (matrix src/driver.cpp)
target_include_directories (matrix PRIVATE include)
target_compile_options (matrix PRIVATE ${COMMON_COMPILE_OPTIONS})
