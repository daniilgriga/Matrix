add_executable (matrix driver.cpp)
target_include_directories (matrix PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options (matrix PRIVATE
    ${COMMON_COMPILE_OPTIONS}
    ${SANITIZE_FLAGS}
)

target_link_options (matrix PRIVATE ${SANITIZE_FLAGS})
