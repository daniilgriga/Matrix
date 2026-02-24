cmake_minimum_required (VERSION 3.15)

project (Matrix LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (SANITIZE "Enable Address and Undefined sanitizers" OFF)
option (BUILD_TESTS "Build tests" ON)

add_executable (matrix src/driver.cpp)
target_include_directories (matrix PRIVATE include)

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

target_compile_options (matrix PRIVATE
    ${COMMON_COMPILE_OPTIONS}
    ${SANITIZE_FLAGS}
)

target_link_options (matrix PRIVATE ${SANITIZE_FLAGS})

if(BUILD_TESTS)
    enable_testing()

    add_executable (matrix_tests tests/unit/unit_tests.cpp)
    target_include_directories (matrix_tests PRIVATE include)

    target_compile_options (matrix_tests PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        ${SANITIZE_FLAGS}
    )

    target_link_options (matrix_tests PRIVATE ${SANITIZE_FLAGS})

    find_package (GTest REQUIRED)
    target_link_libraries (matrix_tests PRIVATE GTest::gtest GTest::gtest_main)

    include (GoogleTest)
    gtest_discover_tests (matrix_tests)
endif()
