cmake_minimum_required (VERSION 3.15)

project (Matrix LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (SANITIZE "Enable Address and Undefined sanitizers" OFF)

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

