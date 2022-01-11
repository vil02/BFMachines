#!/usr/bin/env bash

script_lock="llvm-cov_lock"
readonly script_lock

if ./is_already_running.sh "$script_lock"
then
    printf "Cannot acquire lock (another instance is running?) - exiting.\n"
    exit 1
fi

raw_data_dir=$(./get_clangxx_coverage_data_path.sh) ||
{
    ./remove_lock.sh "$script_lock"
    printf "Cannot get the raw data directory\n"
    exit 1
}
readonly raw_data_dir

res_dir="$(./get_cpp_coverage_report_path.sh)"/llvm-cov_resports ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while getting the output dir.\n"
    exit 1
}
readonly res_dir

./empty_dir.sh "$res_dir" ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error while preparing output dir.\n"
    exit 1
}
function proc_single()
{
    local in_path="$1"
    readonly in_path
    local result_val=0
    local rawprof_path="${in_path%/*}"/default.profraw
    readonly rawprof_path
    if test -f "$rawprof_path"; then
        local profdata_path="${in_path%/*}"/tmp_res.profdata
        readonly profdata_path
        local core_name
        core_name=$(basename "$in_path" suffix)
        readonly core_name
        {
            llvm-profdata merge "$rawprof_path" --output "$profdata_path" &&
            llvm-cov show \
                -format=html \
                --instr-profile "$profdata_path" \
                "$in_path" > "$res_dir"/"$core_name".html &&
            llvm-cov report \
                --instr-profile "$profdata_path" \
                "$in_path" > "$res_dir"/"$core_name".txt
        } || {
            result_val=1
        }
    fi
    return $result_val
}

find "$raw_data_dir" -executable -type f ! -name "*.*" | while read -r cur_file; do
    echo "$cur_file"
    proc_single "$cur_file" ||
    {
        ./remove_lock.sh "$script_lock"
        printf "Error while processing %s.\n" "$cur_file"
        exit 1
    }
done ||
{
    ./remove_lock.sh "$script_lock"
    printf "Error in processing.\n"
    exit 1
}

./remove_lock.sh "$script_lock"
