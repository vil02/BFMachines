#!/usr/bin/env bash

set -euo pipefail

all_cpp_build_data_path="$(./get_all_cpp_build_data_path.sh)"
readonly all_cpp_build_data_path

echo "${all_cpp_build_data_path}"/coverage_gxx
