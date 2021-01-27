add_executable(compile_time_arithmetic compile_time_arithmetic.cpp)
target_link_libraries(compile_time_arithmetic
    PRIVATE
        project_options
        project_warnings)
target_compile_features(compile_time_arithmetic
    PRIVATE
        cxx_std_20)
add_test(compile_time_arithmetic_run_test compile_time_arithmetic)

