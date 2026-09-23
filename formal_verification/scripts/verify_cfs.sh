#!/bin/bash

# Script to run Frama-C WP analysis on the NASA cFS isolated code
# Requires Docker

echo "Running Frama-C WP analysis on NASA cFS logic..."

docker run --rm -v "$(pwd)/src:/src" -w /src framac/frama-c:dev \
  frama-c -wp -wp-rte -wp-prover alt-ergo cfs_string_util.c

if [ $? -eq 0 ]; then
    echo "Frama-C Verification SUCCESSFUL."
else
    echo "Frama-C Verification FAILED."
fi
