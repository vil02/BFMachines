#!/usr/bin/env bash

script_lock="gcovr_sonarqube_lock"
readonly script_lock

if ./is_already_running.sh "$script_lock"
then
    printf "Cannot acquire lock (another instance is running?) - exiting.\n"
    exit 1
fi

output_dir=$(./get_cpp_coverage_report_path.sh)/gcovr_html ||
{
  ./remove_lock.sh "$script_lock"
  printf "Error while getting the output dir.\n"
  exit 1
}
readonly output_dir

./empty_dir.sh "$output_dir" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Cannot create output dir.\n"
    exit 1
}


gcovr -r "$(./get_proj_root.sh)" --html-details "$output_dir"/index.html ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while running gcovr.\n"
    exit 1
}

./remove_lock.sh "$script_lock"
