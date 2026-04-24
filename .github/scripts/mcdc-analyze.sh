#!/bin/bash

# Redirect all echo outputs to mcdc_results.txt and capture gcov output
exec > >(tee -a mcdc_results.txt) 2>&1

# Pass the test modules after running unit tests
# Ex. echo "MODULES=$(grep -oP 'Test #\d+: \K[\w\-\_]+' test_results.txt | tr '\n' ' ' | sed 's/ $//')" >> $GITHUB_ENV
if [ -n "$MODULES" ]; then
    modules="$MODULES"
    echo "Test modules provided: "
    for module in $modules; do
        echo "$module"
    done
else
    echo "No test modules provided."
    exit 1 
fi

# Initialize overall counters
overall_total_functions=0
overall_total_covered_functions=0
overall_file_count=0
overall_no_conditions_count=0
module_count=0
        

# Show coverage for each file in a module and summary coverage for each module
for module in $modules; do
    module_name=$(basename "$module")
    
    # Skip specific files and directories
    if [[ "$module_name" == "core-cpu1" || \
          "$module_name" == "Makefile" || \
          "$module_name" == "CTestTestfile" || \
          "$module_name" == "cmake_install" || \
          "$module_name" == "gmon" || \
          "$module_name" == *"stubs"* ]]; then
        continue
    fi
    
    module_name_no_testrunner=$(echo "$module_name" | sed 's/-testrunner$//')
    
    echo " "
    echo "Processing $module_name_no_testrunner module..."
    
    # Initialize module-level counters
    total_functions=0
    total_covered_functions=0
    file_count=0
    no_conditions_count=0

    module_dirs=""

    if [ -n "$BASE_DIR" ]; then
        # If BASE_DIR is provided, search within the BASE_DIR for the module directories.
        # FIX, module dirs doesn't always show
        module_dirs=$(find "$BASE_DIR" -type d -name "*${module_name}*")
        echo "Base directory specified: $BASE_DIR"
        echo "Searching for .gcda directory..."
    else
        # Otherwise, look for the default module directories.
        module_dirs=$(find "build/native/default_cpu1" -type d -name "*${module_name}*.dir")
        echo "No base directory provided: Searching for .gcda directory..."
    fi

    if [ -n "$module_dirs" ]; then
        for module_dir in $module_dirs; do
            echo "Found module directory: $module_dir"
            
            parent_dir=$(dirname "$module_dir")
            echo "Searching for .gcda files under parent directory: $parent_dir..."
            gcda_files=$(find "$parent_dir" -type d -name "*${module_name_no_testrunner}*.dir" -exec find {} -type f -name "*.gcda" \;)
            
            if [ -n "$gcda_files" ]; then
                for gcda_file in $gcda_files; do
                    c_file=$(echo "$gcda_file" | sed 's/\.gcda$/.c/')
                    
                    echo "Processing corresponding .c file: $c_file"
                    echo "Running gcov on $c_file..."
                    
                    # Capture gcov output and remove header files
                    gcov_output=$(gcov -abcgi "$c_file" | sed "/\.h/,/^$/d")
                    
                    # Output the gcov result of each file and save to mcdc_results.txt
                    echo "$gcov_output" | tee -a mcdc_results.txt

                    # Process gcov results for coverage summary
                    while IFS= read -r line; do
                        if [[ $line == *"Condition outcomes covered:"* ]]; then
                            condition_covered=$(echo "$line" | grep -oP 'Condition outcomes covered:\K[0-9.]+')
                            total_conditions_in_file=$(echo "$line" | grep -oP 'of \K[0-9]+')
                            
                            covered_functions_in_file=$(awk -v pct="$condition_covered" -v total="$total_conditions_in_file" 'BEGIN {printf "%.2f", (pct / 100) * total}')
                            
                            total_functions=$((total_functions + total_conditions_in_file))
                            total_covered_functions=$(awk -v covered="$total_covered_functions" -v new_covered="$covered_functions_in_file" 'BEGIN {printf "%.2f", covered + new_covered}')
                            
                            file_count=$((file_count + 1))
                        elif [[ $line == *"No conditions"* ]]; then
                            no_conditions_count=$((no_conditions_count + 1))
                        fi
                    done <<< "$gcov_output"
                done
            else
                echo "No .gcda files found for $module_name under parent directory $parent_dir."
            fi
        done
    else
        echo "Directory for module $module_name \(e.g., ${module_name}.dir\) not found."
    fi
    
    if [ "$total_functions" -ne 0 ]; then
        average_condition_coverage=$(awk -v covered="$total_covered_functions" -v total="$total_functions" 'BEGIN {printf "%.2f", (covered / total) * 100}')
    else
        average_condition_coverage=0
    fi
    
    overall_total_functions=$((overall_total_functions + total_functions))
    overall_total_covered_functions=$(awk -v covered="$overall_total_covered_functions" -v new_covered="$total_covered_functions" 'BEGIN {printf "%.2f", covered + new_covered}')
    overall_file_count=$((overall_file_count + file_count))
    overall_no_conditions_count=$((overall_no_conditions_count + no_conditions_count))
    
    module_count=$((module_count + 1))
    
    echo "Summary for $module_name_no_testrunner module:"
    echo "  Total files processed: $file_count"
    echo "  Number of files with no condition data: $no_conditions_count"
    echo "  Condition outcomes covered: ${average_condition_coverage}% of $total_functions"
    echo " "
done

if [ "$overall_total_functions" -ne 0 ]; then
    overall_condition_coverage=$(awk -v covered="$overall_total_covered_functions" -v total="$overall_total_functions" 'BEGIN {printf "%.2f", (covered / total) * 100}')
else
    overall_condition_coverage=0
fi

echo " "
echo "Overall summary:"
echo "  Total files processed: $overall_file_count"
echo "  Number of files with no condition data: $overall_no_conditions_count"
echo "  Overall condition outcomes covered: ${overall_condition_coverage}% of $overall_total_functions"