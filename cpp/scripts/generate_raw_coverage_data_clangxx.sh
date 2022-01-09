#!/usr/bin/env bash

set -euo pipefail

output_dir=$(./get_clangxx_coverage_data_path.sh) ||
{
    printf "Cannot get the result folder\n"
    exit 1
}
readonly output_dir

./generate_raw_coverage_data.sh clang++-11 "$output_dir" ||
{
    printf "Cannot generate coverage data\n"
    exit 1
}
