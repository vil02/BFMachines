#!/usr/bin/env bash

set -euo pipefail

compiler_name="${1}"
readonly compiler_name

output_dir="${2}"
readonly output_dir

script_lock=$(basename "$output_dir")"_lock"
readonly script_lock

if ./is_already_running.sh "$script_lock"
then
    printf "Cannot acquire lock (another instance is running?) - exiting.\n"
    exit 1
fi

./create_lock.sh "$script_lock"

./empty_dir.sh "$output_dir" ||
{
      ./remove_lock.sh "$script_lock"
      printf "Cannot create the result folder\n"
      exit 1
}

cpp_source=$(./get_cpp_source_path.sh) ||
{
    ./remove_lock.sh "$script_lock"
    printf "Cannot get cpp source folder\n"
    exit 1
}
readonly cpp_source

{
    cmake -S "$cpp_source" -B "$output_dir" \
        -D ENABLE_CODE_COVERAGE=ON \
        -D ENABLE_BOOST_TESTS=ON \
        -D CMAKE_BUILD_TYPE=Debug \
        -D CMAKE_CXX_COMPILER="$compiler_name" &&
    cmake --build "$output_dir" &&
    cd "$output_dir" &&
    ctest &&
    cd -
} || {
    ./remove_lock.sh "$script_lock"
    printf "Cannot build and test\n"
    exit 1
}

./remove_lock.sh "$script_lock"
