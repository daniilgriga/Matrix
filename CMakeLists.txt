cmake_minimum_required (VERSION 3.15)

project (Matrix LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (SANITIZE "Enable Address and Undefined sanitizers" OFF)
option (BUILD_TESTS "Build tests" ON)

add_executable (matrix src/driver.cpp)
target_include_directories (matrix PRIVATE include)

if(MSVC)
    target_compile_options (matrix PRIVATE /W4 /WX)
else()
    target_compile_options (matrix PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
    )

    target_link_options (matrix PRIVATE
        $<$<BOOL:${SANITIZE}>:-fsanitize=address,undefined>
    )
endif()

if(BUILD_TESTS)
    enable_testing()

    add_executable (matrix_tests tests/gtests.cpp)
    target_include_directories (matrix_tests PRIVATE include)
    target_compile_options (matrix_tests PRIVATE ${COMMON_COMPILE_OPTIONS})

    find_package (GTest REQUIRED)
    target_link_libraries (matrix_tests PRIVATE GTest::gtest GTest::gtest_main)

    include (GoogleTest)
    gtest_discover_tests (matrix_tests)
endif()
