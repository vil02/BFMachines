add_executable(all_tests all_tests.cpp)
target_link_libraries(all_tests
    PRIVATE
        project_options
        project_warnings)

add_test(all_tests_run_test all_tests)

