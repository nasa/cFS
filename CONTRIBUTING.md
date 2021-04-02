# Core Flight System Contributing Guide

So you'd like to contribute to cFS? Below are some guidelines for contributors to follow. Contributions come in all shapes and sizes. We appreciate your help with documentation, unit tests, framework code, continuous-integration, or simply reporting bugs and improvement ideas. We can't promise that we'll accept every suggestion or fix every bug in a timely manner but at the very least we'll respond to you.

## Table of Contents
[Code of Conduct](#code-of-conduct)

[Ways to Contribute](#ways-to-contribute)
* [Report Bugs](#report-bugs)
* [Feature Requests](#feature-requests)
* [Security Vulnerabilities](#security-vulnerabilities)
* [Pull Requests](#pull-requests)
* [Discussions and Questions](#discussions-and-questions)

[Writing High-Quality Code](#writing-high-quality-code)

[Getting Started](#getting-started)

## Code of Conduct 
This project and everyone participating in it is governed by the [cFS Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the project team at cfs-program@lists.nasa.gov.

## Ways to Contribute

### Discussions and Questions
For questions or help, submit a GitHub isssue or email us at cfs-program@lists.nasa.gov. 

[Subscribe to our mailing list](https://lists.nasa.gov/mailman/listinfo/cfs-community) that includes all the community members/users of the NASA core Flight Software (cFS) product line. The mailing list is used to communicate any information related to the cFS product such as current releases, bug findings and fixes, enhancement requests, community meeting notifications, sending out meeting minutes, etc.

### Report Bugs
#### Before Submitting a Bug
1. Perform a cursory search to see if the bug has already been reported. If it has and the issue is still open, add a comment to the existing issue instead of opening a new one.
2. Determine which repository the bug should be reported in. If you are not sure, place the [issue in NASA/cFS](https://github.com/nasa/cFS/issues/new?assignees=&labels=&template=bug_report.md&title=).

#### Reporting a Bug
If you run into a bug with the project:
1. Open an issue using the bug report template. 
2. Describe the issue. 
3. Describe the expected behavior if the bug did not occur. 
4. Provide the reproduction steps that someone else can follow to recreate the bug or error on their own.
5. If applicable, add code snippets or references to the software.
6. Provide the system the bug was observed on including the hardware, operating system, and versions.
7. Provide any additional context if applicable.
8. Provide your full name or GitHub username and your company organization if applicable.

#### Once a bug is submitted:
1. The project team will label the issue.
2. A team member will try to reproduce the issue with your provided steps. If the team is able to reproduce the issue, the issue will be left to be implemented by someone.

### Feature Requests
#### Before Submitting a Feature Request
1. Review the cFS README.md file to see if your feature is in the major future work. 
2. Perform a cursory search to see if the feature has already been requested. If it has and the issue is still open, add a comment to the existing issue instead of opening a new one.
3. Determine which repository the feature should be reported in. If you are not sure, [place the issue in NASA/cFS](https://github.com/nasa/cFS/issues/new?assignees=&labels=&template=feature_request.md&title=).

#### Submitting a Feature Request
1. Open an issue using the feature request template. 
2. Describe the feature.
3. Describe the solution you would like.
4. Describe alternatives you've considered.
5. Provide any additional context if applicable.
6. Provide your full name or GitHub username and your company organization if applicable.

#### Once a feature request is submitted:
1. The project team will label the issue.
2. The project team will evaluate the feature request, possibly asking you more questions to understand its purpose and any relevant requirements. If the issue is closed, the team will convey their reasoning and suggest an alternative path forward.
3. If the feature request is accepted, it will be marked for implementation.

### Security Vulnerabilities
Please view our [Security Policy](https://github.com/nasa/cFS/security/policy) for more information.

### Pull Requests
#### Ready to Add Your Code?

Follow GitHub's fork-branch-pull request pattern.
1. Fork the relevant cFS component (eg. cfe, osal, psp).
2. Create a new branch in your fork to work on your fix. We recommend naming your branch `fix-ISSUE_NUMBER-<FIX_SUMMARY>`.
3. Submit a pull request to the nasa `main` branch. We recommend creating your pull-request as a "draft" and to commit early and often so the community can give you feedback at the beginning of the process as opposed to asking you to change hours of hard work at the end.
4. Add commits to your branch. Please follow commit message convention: `Fix ISSUE_NUMBER, 50-ish-character-long summary of commit content`.
5. Sign and email the appropriate Contributor License agreement down below to GSFC-SoftwareRelease@mail.nasa.gov and copy cfs-program@lists.nasa.gov.
    - Corporate Contributor License agreement : https://github.com/nasa/cFE/blob/main/docs/GSC_18128_Corp_CLA_form_1219.pdf
    - Individual Contributor License agreement : https://github.com/nasa/cFE/blob/main/docs/GSC_18128_Ind_CLA_form_1219.pdf

#### Create a Pull Request
1. For the title, use the title convention `Fix #XYZ, SHORT_DESCRIPTION`.
2. Describe the contribution. First document which issue number was fixed using the template "Fix #XYZ". Then describe the contribution.
3. Provide what testing was used to confirm the pull request resolves the link issue.
4. Provide the expected behavior changes of the pull request.
5. Provide the system the bug was observed on including the hardware, operating system, and versions.
6. Provide any additional context if applicable.
7. Provide your full name or GitHub username and your company or organization if applicable.

#### Once a Pull Request Ready for Review
1. Verify the commit message and PR title use the template `Fix #XYZ, SHORT_DESCRIPTION`.
2. Verify there is only one commit message. Squash or amend the commit messages if necessary.
3. Verify that the PR passes all checks.
4. The project team will label the issue and evaluate the pull request in the weekly configuration control board (CCB) meeting. For more information, visit [The cFS CCB Process.](https://github.com/nasa/cFS/wiki/The-cFS-CCB-Process)
5. If the pull request is accepted, it will be merged into cFS.

## Writing High-Quality Code

1. Follow cFS code conventions (formatting, symbol naming, file naming, etc). Do *not* change/reformat existing code, except to address your changes.
    - The cFS submodules uses the Format Check workflow to ensure users follow the [clang-format-10](https://releases.llvm.org/10.0.0/tools/clang/docs/ClangFormat.html) style. 
    - The [cFS_IdentifierNamingConvention document](https://github.com/nasa/cFE/blob/main/docs/cFS_IdentifierNamingConvention.md) provides a simple naming convention guide for cFE/cFS for commands and telemetry that simplifies the EDS to ground telemetry and commands database conversion.
    - The [cFE Application Developers Guide](https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md) contains code conventions such as naming conventions for header files. 

2. For any new API's, add unit tests to cover nominal and off-nominal conditions.

3. Add/edit stubs to the unit test codebase for any new/modified functions.

4. For any changes to existing API's, alter the unit tests to cover the changes (and remove tests made irrelevant due to your changes).

5. Review the static code analyses results. 
    - Push code changes to the appropriate forked repository. 
    - Go to the Actions tab and enable GitHub Actions Workflow. The CodeQL Analyis and Static Analysis will be triggered for all pushed code. 
    - Review these workflows for any warnings or errors. 
    - Once successful, create a pull request. 

For more information on our GitHub Action Workflows, view our [Security Policy](https://github.com/nasa/cFS/security/policy).

## Getting Started
Before you begin, search through the open issues in each submodule to ensure that your ticket is relevant, not redundant, nor in conflict with other tickets:
* [cFS Bundle Issues](https://github.com/nasa/cfs/issues)
* [cFE Issues](https://github.com/nasa/cfe/issues)
* [OSAL Issues](https://github.com/nasa/osal/issues)
* [PSP Issues](https://github.com/nasa/psp/issues)

### Apps
* [ci_lab Issues](https://github.com/nasa/ci_lab/tree/296d12cde4f90d112d1578cb584ddae200a2d282)
* [sample_app Issues](https://github.com/nasa/sample_app/issues)
* [sample_lib Issues](https://github.com/nasa/sample_lib/issues)
* [sch_lab Issues](https://github.com/nasa/sch_lab/tree/882846bb778432c8780555b9d4bef45535584174)
* [to_lab Issues](https://github.com/nasa/to_lab/tree/031de3dde3f5265e98d7cd2bc154d93bee8520b0)

### Tools
* [cFS-GroundSystem Issues](https://github.com/nasa/cFS-GroundSystem/issues)
* [elf2cfetbl Issues](https://github.com/nasa/elf2cfetbl/tree/6762b1c3b455665dae57e35f14a50fe327830391)
* [tblCRCTool Issues](https://github.com/nasa/tblCRCTool/tree/b02864ba56b12e00ab152225e3e8f9d6c039d48c)

If your bug or feature hasn't been reported or requested before, create a new issue in the appropriate repository. If it you find a similar issue, please add a comment on it with your experience or input.

Please ensure that your name is associated with your github profile before contributing.
