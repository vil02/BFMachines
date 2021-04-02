if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0.0") OR
   (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0") OR
   (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.28"))
    add_executable(compile_time_arithmetic compile_time_arithmetic.cpp)
    target_link_libraries(compile_time_arithmetic
        PRIVATE
            project_options
            project_warnings)
    target_compile_features(compile_time_arithmetic
        PRIVATE
            cxx_std_20)
    add_test(compile_time_arithmetic_run_test compile_time_arithmetic)
else()
    message(WARNING "skipping: compile_time_arithmetic")
endif()
