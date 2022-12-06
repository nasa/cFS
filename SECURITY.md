# Security Policy

## Reporting a Vulnerability

For general cFS vulnerabilities, please [open a cFS framework issue](https://github.com/nasa/cfs/issues/new/choose). Please use the "Bug Report" template and provide as much information as possible. Apply appropriate labels for each report.  

For security related vulnerabilities, follow the general cFS vulnerabilities instructions and tag the issue with the "security" label.

## Security Reports

To submit sensitive security reports or findings, [contact the cFS Product Team](README.md#contact-the-cfs-product-team).

## Testing

**Disclaimer: nasa/cFS is not responsible for any liability incurred as stated under the [Apache License 2.0](https://github.com/nasa/cFS/blob/main/LICENSE).**

Testing is an important aspect our team values to improve the cFS bundle. Several tools are used for testing:

### CodeQL

The [cFS CodeQL GitHub Actions workflow](https://github.com/nasa/cFS/actions/workflows/codeql-analysis.yml) is available to the public. To review the results, fork the cFS repository and run the CodeQL workflow.

CodeQL is ran for every push and pull-request on all branches of cFS in GitHub Actions.

For the CodeQL GitHub Actions setup, visit https://github.com/github/codeql-action.

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

## Additional Support

For additional support, submit a GitHub issue.

You can also [join the cfs community mailing list](README.md#join-the-mailing-list) for technical discussions with cFS developers and users.

If you wish to report a cybersecurity incident or concern, please contact the NASA Security Operations Center either by phone at 1-877-627-2732 or via email address soc@nasa.gov.
