add_executable(bf_to_cpp_generate.cpp)
target_link_libraries(bf_to_cpp_generate
    PRIVATE
        project_options
        project_warnings)

add_test(bf_to_cpp_generate_run_test bf_to_cpp_generate)
