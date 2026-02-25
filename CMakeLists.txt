cmake_minimum_required (VERSION 3.15)

project (Matrix LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (SANITIZE "Enable Address and Undefined sanitizers" OFF)
option (BUILD_TESTS "Build tests" ON)

if(MSVC)
    set (COMMON_COMPILE_OPTIONS /W4 /WX)
else()
    set (COMMON_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

if(SANITIZE AND NOT MSVC)
    set (SANITIZE_FLAGS -fsanitize=address,undefined)
endif()

add_subdirectory (src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory (tests)
endif()
