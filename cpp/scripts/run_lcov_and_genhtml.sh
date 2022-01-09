#!/usr/bin/env bash

script_lock="lcov_html_lock"
readonly script_lock

if ./is_already_running.sh "$script_lock"
then
    printf "Cannot acquire lock (another instance is running?) - exiting.\n"
    exit 1
fi

raw_data_dir=$(./get_gxx_coverage_data_path.sh) ||
{
    ./remove_lock.sh "$script_lock"
    printf "Cannot get the raw data directory\n"
    exit 1
}
readonly raw_data_dir

lcov_res_path="$raw_data_dir"/coverage.info
readonly lcov_res_path

lcov --capture \
  --directory "$raw_data_dir" \
  --output-file "$lcov_res_path" \
  --base-directory "$(./get_proj_root.sh)" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while running lcov\n"
    exit 1
}

html_output_dir="$(./get_cpp_coverage_report_path.sh)"/lcov_html ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while getting the output dir.\n"
    exit 1
}
readonly html_output_dir

./empty_dir.sh "$html_output_dir" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while preparing output dir.\n"
    exit 1
}

genhtml "$lcov_res_path" --output-directory "$html_output_dir" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while running genhtml.\n"
    exit 1
}

./remove_lock.sh "$script_lock"
