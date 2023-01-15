#!/usr/bin/env bash

set -euo pipefail

proj_root="$(./get_proj_root.sh)"
readonly proj_root

echo "${proj_root}"/cpp
