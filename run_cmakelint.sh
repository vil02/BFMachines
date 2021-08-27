#!/usr/bin/env bash

set -euo pipefail

cmakelint --version

shopt -s globstar
cmakelint ./**/*.txt
