#!/usr/bin/env bash

script_lock="gcovr_sonarqube_lock"
readonly script_lock

if ./is_already_running.sh "$script_lock"
then
    printf "Cannot acquire lock (another instance is running?) - exiting.\n"
    exit 1
fi

output_dir=$(./get_cpp_coverage_report_path.sh) ||
{
  ./remove_lock.sh "$script_lock"
  printf "Error while getting the output dir.\n"
  exit 1
}
readonly output_dir

mkdir -p "$output_dir" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Cannot create output dir.\n"
    exit 1
}

output_xml_path="$output_dir"/coverage_report.xml ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while getting the output path.\n"
    exit 1
}
readonly output_xml_path

gcovr -r "$(./get_proj_root.sh)"/.. --sonarqube "$output_xml_path" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while running gcovr.\n"
    exit 1
}

./remove_lock.sh "$script_lock"
