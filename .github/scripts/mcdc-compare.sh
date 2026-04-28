#!/bin/bash

exec > >(tee -a mcdc_compare.txt) 2>&1

# Function to check if a file exists and return an error message for missing files
check_file_exists() {
  file=$1
  if [ ! -f "$file" ]; then
    echo "Error: File '$file' does not exist."
    missing_files=true
  fi
}

# Function to extract the relevant numbers from a module's "Summary for module" section
extract_module_numbers() {
  file=$1
  module=$2

  total_files_processed=$(sed -n "/^Summary for ${module} module:/,/^$/p" "$file" | head -n 4 | grep -Po 'Total files processed:\s*\K\d*')
  no_condition_data=$(sed -n "/^Summary for ${module} module:/,/^$/p" "$file" | head -n 4 | grep -Po 'Number of files with no condition data:\s*\K\d+')
  condition_outcomes_covered_percent=$(sed -n "/^Summary for ${module} module:/,/^$/p" "$file" | head -n 4 | grep -Po 'Condition outcomes covered:\s*\K[0-9]+(\.[0-9]+)?')
  condition_outcomes_out_of=$(sed -n "/^Summary for ${module} module:/,/^$/p" "$file" | head -n 4 | grep -Po 'Condition outcomes covered:.*of\s*\K\d*')

  echo "$total_files_processed $no_condition_data $condition_outcomes_covered_percent $condition_outcomes_out_of"
}

# Compare results for each module between two files
compare_mcdc_results() {
  main_results_file=$1
  pr_results_file=$2
  modules_file=$3

  # Initialize a flag to track if any files are missing
  missing_files=false

  # Check if the files exist before proceeding
  check_file_exists "$main_results_file"
  check_file_exists "$pr_results_file"
  check_file_exists "$modules_file"

  # If any files are missing, exit early
  if [ "$missing_files" = true ]; then
    echo "Error: One or more input files are missing. Exiting."
    exit 1
  fi

  # Read modules from modules.txt (passed as argument)
  modules=$(cat "$modules_file")

  # Check if modules are empty or not
  if [ -z "$modules" ]; then
    echo "Error: No modules found in $modules_file"
    exit 1
  fi

  # Initialize variables to store the output for modules with and without changes
  modules_with_changes=""
  modules_without_changes=""

  # Loop through all modules to compare each one
  for module in $modules; do

    # Extract numbers for the main results file and PR results file for the current module
    read main_total_files main_no_condition main_condition_covered_percent main_condition_out_of <<< $(extract_module_numbers "$main_results_file" "$module")
    read pr_total_files pr_no_condition pr_condition_covered_percent pr_condition_out_of <<< $(extract_module_numbers "$pr_results_file" "$module")

    # Echo numbers extracted from each file for each module
    echo -e "\nResults for module: $module"
    echo "PR Branch - Total files processed: $pr_total_files, No condition data: $pr_no_condition, Covered condition %: $pr_condition_covered_percent%, Out of value: $pr_condition_out_of"
    echo "Main Branch - Total files processed: $main_total_files, No condition data: $main_no_condition, Covered condition %: $main_condition_covered_percent%, Out of value: $main_condition_out_of"

    # Initialize variables to store differences
    total_files_diff=""
    no_condition_data_diff=""
    condition_outcomes_covered_diff_percent=""
    condition_outcomes_out_of_diff=""

    # Calculate difference between files
    total_files_diff=$((pr_total_files - main_total_files))
    no_condition_data_diff=$((pr_no_condition - main_no_condition))
    condition_outcomes_covered_diff_percent=$(echo "$pr_condition_covered_percent - $main_condition_covered_percent" | bc)
    condition_outcomes_out_of_diff=$((pr_condition_out_of - main_condition_out_of))

    echo "Differences:"
    echo "  Total files processed difference: $total_files_diff"
    echo "  No condition data difference: $no_condition_data_diff"
    echo "  Covered condition % difference: $condition_outcomes_covered_diff_percent"
    echo "  Out of value difference: $condition_outcomes_out_of_diff"
    echo " "

    changes=""

    if [ "$total_files_diff" -gt 0 ]; then
      changes="${changes}    Number of files processed: +$total_files_diff\n"
    elif [ "$total_files_diff" -lt 0 ]; then
      changes="${changes}    Number of files processed: $total_files_diff\n"
    fi

    if [ "$no_condition_data_diff" -gt 0 ]; then
      changes="${changes}    Number of files with no condition data: +$no_condition_data_diff\n"
    elif [ "$no_condition_data_diff" -lt 0 ]; then
      changes="${changes}    Number of files with no condition data: $no_condition_data_diff\n"
    fi

    if [ $(echo "$condition_outcomes_covered_diff_percent > 0" | bc) -eq 1 ]; then
      changes="${changes}    Percentage of covered conditions: +$condition_outcomes_covered_diff_percent%\n"
    elif [ $(echo "$condition_outcomes_covered_diff_percent < 0" | bc) -eq 1 ]; then
      changes="${changes}    Percentage of covered conditions: $condition_outcomes_covered_diff_percent%\n"
    fi

    if [ "$condition_outcomes_out_of_diff" -gt 0 ]; then
      changes="${changes}    Number of conditions: +$condition_outcomes_out_of_diff\n"
    elif [ "$condition_outcomes_out_of_diff" -lt 0 ]; then
      changes="${changes}    Number of conditions: $condition_outcomes_out_of_diff\n"
    fi

    if [ -n "$changes" ]; then
      modules_with_changes="${modules_with_changes}  $module\n$changes\n"
    else
      modules_without_changes="${modules_without_changes}  $module\n"
    fi
  done

  echo " "
  echo "MC/DC results compared to latest dev branch:"
  echo " "
  echo "Modules with changes:"
  echo -e "$modules_with_changes"
  echo "Modules without changes:"
  echo -e "$modules_without_changes"

  # Write results to mcdc_comment.txt / pull request
  if [ -n "$modules_with_changes" ]; then
    echo "MC/DC results compared to latest dev branch:" > mcdc_comment.txt
    echo "" >> mcdc_comment.txt
    echo "Modules with changes:" >> mcdc_comment.txt
    echo -e "$modules_with_changes" >> mcdc_comment.txt
    echo "" >> mcdc_comment.txt
    echo "See file uncovered.json for more details"
  else
    echo "No MC/DC changes were made." > mcdc_comment.txt
  fi

}

# creates single json file that contains info on all uncovered branches
generate_json_report() {
  jq_script=$(find $GITHUB_WORKSPACE -name "uncovered_filter.jq" | tail -n 1)
  if [ -z "$jq_script" ]; then
    echo "Error: Could not find uncovered_filter.jq"
    return 1
  fi

  for zipped_file in *.gcov.json.gz; do
    if [ -f "$zipped_file" ]; then
      base_name="${zipped_file%.gcov.json.gz}"
      gunzip -c "$zipped_file" > "${base_name}.json"
      if [ -f "${base_name}.json" ]; then
        jq -f "$jq_script" "${base_name}.json" > "${base_name}_filtered.json"
      else
        echo "Error: Failed to decompress $zipped_file"
        return 1
      fi
    else
      echo "Warning: No .gcov.json.gz files found"
      return 0
    fi
  done

  if ls *_filtered.json 1> /dev/null 2>&1; then
    jq -s '.' *_filtered.json > uncovered.json
    echo "Successfully created uncovered.json"
  else
    echo "No filtered JSON files found to merge"
    return 1
  fi

  if jq 'flatten' uncovered.json > temp.json; then
    mv temp.json uncovered.json
  else
    rm -f temp.json
    echo "Error processing JSON file"
    exit 1
  fi

  if jq '.' uncovered.json > temp.json; then
    mv temp.json uncovered.json
  else
    rm -f temp.json
    echo "Error processing JSON file"
    exit 1
  fi
}

# Check the script arguments
if [ $# -ne 3 ]; then
  echo "Usage: $0 <main_results_file> <pr_results_file> <modules_file>"
  exit 1
fi

# Run the comparison function with the provided arguments
generate_json_report
compare_mcdc_results "$1" "$2" "$3"