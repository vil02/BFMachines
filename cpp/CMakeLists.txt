cmake_minimum_required(VERSION 3.16)

project(cpp_bf_machines CXX)
include(cmake/StandardProjectSettings.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(
        ENABLE_BUILD_WITH_TIME_TRACE
        "Enable -ftime-trace to generate time tracing .json files on clang"
        OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

option(ENABLE_TESTING "Enable tests" ON)
if(ENABLE_TESTING)
    enable_testing()
endif()

option(ENABLE_CODE_COVERAGE "Generate coverage data" OFF)
include(cmake/CodeCoverage.cmake)
enable_code_coverage(project_options)

option(ENABLE_BOOST_TESTS "Add boost tests" ON)
if(ENABLE_BOOST_TESTS)
    find_package(Boost 1.66 REQUIRED)
    if(NOT Boost_FOUND)
        message(
            FATAL_ERROR "Fatal error: Boost (version >= 1.66.0) required.\n")
    endif()
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

    add_subdirectory(tests)
endif()
add_subdirectory(examples)
