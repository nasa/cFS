#!/bin/bash

# Script to run Frama-C WP analysis on the altitude monitor
# Requires Docker

echo "Running Frama-C WP analysis (Formal Verification)..."

docker run --rm -v "$(pwd)/src:/src" -w /src framac/frama-c:dev \
  frama-c -wp -wp-rte -wp-prover alt-ergo altitude_monitor.c

if [ $? -eq 0 ]; then
    echo "Frama-C Verification SUCCESSFUL."
else
    echo "Frama-C Verification FAILED. See output above for counterexamples."
fi
