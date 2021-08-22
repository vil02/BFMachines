add_executable(simple_bf_debugger simple_bf_debugger.cpp)
target_link_libraries(simple_bf_debugger
    PRIVATE
        project_options
        project_warnings)
