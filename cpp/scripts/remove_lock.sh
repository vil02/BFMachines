#!/usr/bin/env bash

set -euo pipefail

rm -rf "$1" ||
{
    printf "Cannot remove lock\n"
    exit 1
}
