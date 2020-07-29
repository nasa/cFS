#!/bin/bash
# Runs cppcheck on TO

to_app="../fsw"
io_lib="../../io_lib/fsw"

output_file="./cppcheck_to.txt"
error_file="./cppcheck_errors_to.txt"

command -v cppcheck >/dev/null 2>&1 || { echo >&2 "Error: Requires cppcheck but it's not installed.  Aborting."; exit 1; }

paths_to_check="$to_app/src/ $to_app/examples/multi/ $to_app/examples/multi_tf/ $to_app/examples/rs422/ $to_app/examples/udp/"

include_dirs="-I $to_app/platform_inc/ -I $to_app/mission_inc -I $io_lib/public_inc"

flags="-v --report-progress --std=c89"

cppcheck $flags $include_dirs $paths_to_check 2> $error_file > $output_file
