#!/bin/bash

# Script to run KLEE Symbolic Execution for Test Generation
# Requires Docker

echo "Running KLEE for Test Generation..."

# KLEE docker image uses an older LLVM version (usually 9 or 11 or 13)
# We will pull klee/klee which has clang and klee installed.

docker run --rm -v "$(pwd)/src:/src" -w /src klee/klee:2.3 /bin/bash -c "\
  /usr/lib/llvm-11/bin/clang -I/usr/include -emit-llvm -c -g -O0 altitude_monitor.c -o altitude_monitor.bc && \
  /usr/lib/llvm-11/bin/clang -I/usr/include -emit-llvm -c -g -O0 klee_harness.c -o klee_harness.bc && \
  /usr/lib/llvm-11/bin/llvm-link klee_harness.bc altitude_monitor.bc -o combined.bc && \
  klee --only-output-states-covering-new --optimize combined.bc && \
  ls -la klee-last"

if [ $? -eq 0 ]; then
    echo "KLEE Test Generation SUCCESSFUL. Tests are in klee-last (inside container, not mapped out in this simple script for brevity)."
else
    echo "KLEE Test Generation FAILED."
fi
