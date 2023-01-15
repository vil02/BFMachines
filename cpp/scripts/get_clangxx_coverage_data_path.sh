#!/usr/bin/env bash

set -euo pipefail

build_data_path="$(./get_all_cpp_build_data_path.sh)"
readonly build_data_path

echo "${build_data_path}"/coverage_clangxx
