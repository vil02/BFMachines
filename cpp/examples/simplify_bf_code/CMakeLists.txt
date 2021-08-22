add_executable(simplify_bf_code simplify_bf_code.cpp)
target_link_libraries(simplify_bf_code
    PRIVATE
        project_options
        project_warnings)

add_test(simplify_bf_code_run_test simplify_bf_code)

add_test(NAME simplify_bf_code_comp_test COMMAND simplify_bf_code)
set_tests_properties(simplify_bf_code_comp_test PROPERTIES PASS_REGULAR_EXPRESSION
    "Input code:\n,>\\-\\+\\+\\-ABC<>,<><\\[\\->\\-\\+\\+<><\\]>\.\nSimplified code:\n,>,<\\[\\->\\+<\\]>\.")
