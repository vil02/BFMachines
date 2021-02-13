if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_subdirectory(execute_optimized)
else()
    message(WARNING "skipping: execute_optimized")
endif()

if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0.0") OR
   (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0") OR
   (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.28"))
    add_subdirectory(compile_time_arithmetic)
else()
    message(WARNING "skipping: compile_time_arithmetic")
endif()

add_subdirectory(memory_types_performance_comparison)
add_subdirectory(simple_bf_debugger)
add_subdirectory(simple_bf_interpreter)
add_subdirectory(simplify_bf_code)
