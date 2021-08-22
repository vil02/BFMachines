option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)

message( ${CMAKE_SOURCE_DIR} )

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(
      CMAKE_CXX_CPPCHECK
      ${CPPCHECK}
      --suppress=missingIncludeSystem
      --enable=all
      --addon=threadsafety
      --addon=y2038
      --addon=cert
      --inconclusive
      --force
      --error-exitcode=1)
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
  message(${CMAKE_CXX_CPPCHECK})
endif()

if(ENABLE_CLANG_TIDY)
  if(NOT CLANG_TIDY_NAME)
    set(CLANG_TIDY_NAME clang-tidy)
  endif()
  find_program(CLANGTIDY ${CLANG_TIDY_NAME})
  unset(CLANG_TIDY_NAME)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
  if(INCLUDE_WHAT_YOU_USE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
  else()
    message(SEND_ERROR "include-what-you-use requested but executable not found")
  endif()
endif()
