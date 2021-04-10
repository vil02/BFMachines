add_executable(bf_to_cpp_generate bf_to_cpp_generate.cpp)

target_link_libraries(bf_to_cpp_generate
    PRIVATE
        project_options
        project_warnings)

add_executable(bf_to_cpp_run bf_to_cpp_run.cpp)
target_link_libraries(bf_to_cpp_run
    PRIVATE
        project_options
        project_warnings)


add_custom_target(bf_to_cpp_generate_execute
    COMMAND bf_to_cpp_generate
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "execute bf_to_cpp_generate in ${CMAKE_CURRENT_SOURCE_DIR}"
    SOURCES bf_to_cpp_generate.cpp)

add_dependencies(bf_to_cpp_run bf_to_cpp_generate_execute)

add_test(bf_to_cpp_generate_run_test bf_to_cpp_generate)
add_test(bf_to_cpp_run_run_test bf_to_cpp_run)

