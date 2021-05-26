add_executable(execute_optimized execute_optimized.cpp)
target_link_libraries(execute_optimized
    PRIVATE
        project_options
        project_warnings)

add_test(execute_optimized_run_test execute_optimized)

add_test(NAME execute_optimized_comp_test COMMAND execute_optimized)
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "0[ \t]*1")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "1[ \t]*1")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "2[ \t]*2")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "3[ \t]*6")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "4[ \t]*24")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "5[ \t]*120")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "6[ \t]*720")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "10[ \t]*3628800")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "11[ \t]*39916800")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "12[ \t]*479001600")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "13[ \t]*6227020800")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "14[ \t]*87178291200")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "15[ \t]*1307674368000")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "16[ \t]*20922789888000")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "17[ \t]*355687428096000")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "18[ \t]*6402373705728000")
set_tests_properties(execute_optimized_comp_test
    PROPERTIES PASS_REGULAR_EXPRESSION "19[ \t]*121645100408832000")
