#!/usr/bin/env bash

folders=("apps")
for folder in  "${folders[@]}"
do
    echo ${folder}
    if [[ -d ${folder} ]]; then
	command="cpplint --filter=-whitespace,-readability,-legal --recursive --exclude=apps/libcsp --exclude=apps/sample_app --exclude=apps/ci_lab --exclude=apps/io_lib --exclude=apps/sample_lib --exclude=apps/sch_lab --exclude=apps/to_lab --exclude=apps/*/fsw/unit_test ${folder}"
	echo ${command}
	if ${command} | grep "error"; then
	    echo "::error ::Review code formatting"
	    exit 1
	fi
    fi
done
