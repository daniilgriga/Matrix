add_executable (matrix_tests
    unit/matrix_tests.cpp
    unit/chain_tests.cpp
)

target_include_directories (matrix_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options (matrix_tests PRIVATE
    ${COMMON_COMPILE_OPTIONS}
    ${SANITIZE_FLAGS}
)

target_link_options (matrix_tests PRIVATE ${SANITIZE_FLAGS})

find_package (GTest REQUIRED)
target_link_libraries (matrix_tests PRIVATE GTest::gtest GTest::gtest_main)

include (GoogleTest)
gtest_discover_tests (matrix_tests)
