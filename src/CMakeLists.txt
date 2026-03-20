add_executable (matrix_det det_driver.cpp)
target_include_directories (matrix_det PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options (matrix_det PRIVATE ${COMMON_COMPILE_OPTIONS} ${SANITIZE_FLAGS})
target_link_options (matrix_det PRIVATE ${SANITIZE_FLAGS})

add_executable (matrix_chain chain_driver.cpp)
target_include_directories (matrix_chain PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options (matrix_chain PRIVATE ${COMMON_COMPILE_OPTIONS} ${SANITIZE_FLAGS})
target_link_options (matrix_chain PRIVATE ${SANITIZE_FLAGS})
