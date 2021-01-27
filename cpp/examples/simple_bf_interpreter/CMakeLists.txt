add_executable(simple_bf_interpreter.cpp)
target_link_libraries(simple_bf_interpreter
    PRIVATE
        project_options
        project_warnings)

add_test(simple_bf_interpreter_run_test simple_bf_interpreter)

add_test(NAME simple_bf_interpreter_comp_test COMMAND simple_bf_interpreter)
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "Hello world")
