add_executable(bf_to_cpp bf_to_cpp.cpp)
target_link_libraries(bf_to_cpp
    PRIVATE
        project_options
        project_warnings)

add_test(bf_to_cpp_run_test bf_to_cpp)
