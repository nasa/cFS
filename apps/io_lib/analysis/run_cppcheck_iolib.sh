#!/bin/bash
# Runs cppcheck on io_lib

io_lib="../fsw"

output_file="./cppcheck_io_lib.txt"
error_file="./cppcheck_errors_io_lib.txt"

command -v cppcheck >/dev/null 2>&1 || { echo >&2 "Error: Requires cppcheck but it's not installed.  Aborting."; exit 1; }

paths_to_check="$io_lib/src/ $io_lib/formats/ $io_lib/services/"

include_dirs="-I $io_lib/public_inc"

flags="-v --report-progress --std=c89"

cppcheck $flags $include_dirs $paths_to_check 2> $error_file > $output_file
