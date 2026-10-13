# Security Policy

## Reporting a Vulnerability

### Security Vulnerabilities

**For security-related vulnerabilities, please report them through NASA's official Vulnerability Disclosure Program (VDP):**

Submit your report via the Bugcrowd portal: https://bugcrowd.com/engagements/nasa-vdp

The VDP is the preferred channel for all security vulnerability disclosures. This ensures your report is handled appropriately by NASA's security team.

**When reporting a security vulnerability, please include:**

- **Full steps to reproduce the vulnerability** in the complete cFS bundle environment
- The specific cFS version or commit hash you tested against
- Any configuration files or modifications made to the default setup
- Expected vs. actual behavior
- Potential security impact

**Important:** Vulnerabilities must be reproducible in the **complete cFS bundle** as it is intended to be deployed. Reports based on individual applications tested in isolation with custom harnesses or modified environments that do not reflect actual cFS bundle behavior may not be accepted. The cFS applications are designed to work together as an integrated system, and testing must reflect this architecture.

### General Bugs and Issues

For non-security bugs and general issues, please [open a cFS framework issue](https://github.com/nasa/cfs/issues/new/choose). Please use the "Bug Report" template and provide as much information as possible. Apply appropriate labels for each report.

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

CodeSonar is typically ran on a "once per release" sort of schedule.  

For more information about CodeSonar, visit https://codesecure.com/our-products/codesonar.

### Fuzz Testing

The application used to fuzz test cFS and its results are not available to the public.

The latest version of cFS is fuzzed nightly using a continuous integration pipeline.

For more information about AFL used by the Fuzzing cFS application, visit https://github.com/google/AFL.

## Additional Support

For additional support, submit a GitHub issue.

You can also [join the cfs community mailing list](README.md#join-the-mailing-list) for technical discussions with cFS developers and users.

If you wish to report a cybersecurity incident or concern, please contact the NASA Security Operations Center either by phone at 1-877-627-2732 or via email address soc@nasa.gov.
