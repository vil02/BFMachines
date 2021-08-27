add_executable(
    memory_types_performance_comparison
    memory_types_performance_comparison.cpp)
target_link_libraries(memory_types_performance_comparison
    PRIVATE
        project_options
        project_warnings)

add_test(
    memory_types_performance_comparison_run_test
    memory_types_performance_comparison)
