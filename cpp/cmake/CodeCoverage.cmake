function(enable_code_coverage project_name)
    if(ENABLE_CODE_COVERAGE)
        if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")

            # set Flags
            # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
            target_compile_options(${project_name} INTERFACE -fprofile-instr-generate -fcoverage-mapping)
            # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

        elseif(CMAKE_COMPILER_IS_GNUCXX)
            if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
                message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
            endif()

            target_compile_options(${project_name} INTERFACE --coverage -fprofile-arcs -ftest-coverage)
            target_link_options(${project_name} INTERFACE -lgcov --coverage)
        else()
            message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
        endif()
    endif()
endfunction()