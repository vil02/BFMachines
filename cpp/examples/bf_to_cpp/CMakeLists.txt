add_executable(bo_to_cpp bo_to_cpp.cpp)
target_link_libraries(bo_to_cpp
    PRIVATE
        project_options
        project_warnings)

add_test(bo_to_cpp_run_test bo_to_cpp)
