# Security Policy

## Reporting a Vulnerability

For general cFS vulnerabilities, please [open a cFS framework issue](https://github.com/nasa/cfs/issues/new/choose). Please use the "Bug Report" template and provide as much information as possible. Apply appropraite labels for each report.  

For security related vulnerabilities, follow the general cFS vulnerabilites instructions and tag the issue with the "security" label.

## Testing

**Disclaimer: nasa/cFS is not responsible for any liability incurred under the [Apache License 2.0](https://github.com/nasa/cFS/blob/main/LICENSE).**

Testing is an important aspect our team values to improve the cFS bundle. Several tools are used for testing:

### CodeQL

The [cFS CodeQL GitHub Actions workflow](https://github.com/nasa/cFS/actions/workflows/codeql-build.yml) is available to the public. To review the results, fork the cFS repository and run the CodeQL workflow. 

CodeQL is ran for every push and pull-request on all branches of cFS in GitHub Actions. 

For the CodeQL GitHub Actions setup, visit https://github.com/github/codeql-action. 

### LGTM

LGTM [results](https://lgtm.com/projects/g/nasa/cFS?mode=list) are available to the public. Since suppression comments are not used, many warnings and errors may not be accurate. Therefore, the results may contain false positives and other inapplicable warnings and errors. 

LGTM is ran for every pull-request on all branches of cFS.

For more information about LGTM, visit https://lgtm.com/.

### Cppcheck

The [cFS Cppcheck GitHub Actions workflow and results](https://github.com/nasa/cFS/actions/workflows/static-analysis.yml) are available to the public. To view the results, select a workflow and download the artifacts. 

Cppcheck is ran for every push on the main branch and every pull request on all branches of cFS in Github Actions. 

For more information about Cppcheck, visit http://cppcheck.sourceforge.net/.

### CodeSonar

CodeSonar results are not available to the public. 

CodeSonar is typically ran on a “once per release” sort of schedule.  

For more information about CodeSonar, visit https://www.grammatech.com/codesonar-cc. 

### Fuzz Testing

The application used to fuzz test cFS and its results are not available to the public.

The latest version of cFS is fuzzed nightly using a continuous integration pipeline. 

For more information about AFL used by the Fuzzing cFS application, visit https://github.com/google/AFL. 

## Security Reports

To submit security reports or findings, email us at cfs-program@lists.nasa.gov.

## Additional Support

For additional support, submit a GitHub issue. You can also email the cfs community at cfs-community@lists.nasa.gov. 

You can subscribe to the mailing list [here](https://lists.nasa.gov/mailman/listinfo/cfs-community) that includes all the community members/users of the NASA core Flight Software (cFS) product line. The mailing list is used to communicate any information related to the cFS product such as current releases, bug findings and fixes, enhancement requests, community meeting notifications, sending out meeting minutes, etc.

If you wish to report a cybersecurity incident or concern, please contact the NASA Security Operations Center either by phone at 1-877-627-2732 or via email address soc@nasa.gov.
