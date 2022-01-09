#!/usr/bin/env bash

set -euo pipefail

mkdir "$1" ||
{
    printf "Cannot create lock\n"
    exit 1
}
