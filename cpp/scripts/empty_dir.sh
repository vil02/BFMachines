#!/usr/bin/env bash

set -euo pipefail

mkdir -p "$1" ||
{
    printf "Cannot create the folder\n"
    exit 1
}

rm -rf "${1:?}"/* ||
{
    printf "Cannot clean the folder\n"
    exit 1
}
