#!/usr/bin/env bash

set -euo pipefail

declare -i return_value=0
test -d "$1" ||
{
    return_value=1
}
exit "${return_value}"
