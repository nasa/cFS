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

[Quick Links to Submodules](#quick-links-to-submodules)

## Code of Conduct

This project and everyone participating in it is governed by the [cFS Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the [product team](README.md#contact-the-cfs-product-team).

## Ways to Contribute

### Discussions and Questions

For discussions, questions, or ideas, [start a new discussion](https://github.com/nasa/cFS/discussions/new) in the cFS repository under the Discussions tab. 

[Join the cfs community mailing list](README.md#join-the-mailing-list).


### Report Bugs

#### Before Submitting a Bug
1. Perform a cursory search to see if the bug has already been reported. For issues in each submodule, visit the section [Quick Links to Submodules](#quick-links-to-submodules). If a bug has been reported and the issue is still open, add a comment to the existing issue instead of opening a new one.
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
2. Perform a cursory search to see if the feature has already been requested. For issues in each submodule, visit the section [Quick Links to Submodules](#quick-links-to-submodules). If a feature request has been reported and the issue is still open, add a comment to the existing issue instead of opening a new one.
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
4. Add commits to your branch. For information on commit messages, review [How to Write a Git Commit Message](https://chris.beams.io/posts/git-commit/). Please follow commit message convention:
```
Fix #XYZ, SHORT_DESCRIPTION

LONG_DESCRIPTION (optional)
```
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
1. Verify the commit message and PR title uses the template `Fix #XYZ, SHORT_DESCRIPTION`. For information on commit messages, review [How to Write a Git Commit Message](https://chris.beams.io/posts/git-commit/). The commit message may use the template:
```
Fix #XYZ, SHORT_DESCRIPTION

LONG_DESCRIPTION (optional)
```
2. Verify there is one commit message per topic. For example, review the [provided pull request](https://github.com/nasa/cFE/pull/1203/commits).
3. Squash or amend the commit messages if necessary. For more information, review the sections [How to Squash Commits](#how-to-squash-commits) and [How to Amend Commits](#how-to-amend-commits) .  
3. Verify that the PR passes all checks.
4. The project team will label the issue and evaluate the pull request in the weekly configuration control board (CCB) meeting. For more information, visit [The cFS CCB Process](https://github.com/nasa/cFS/wiki/The-cFS-CCB-Process).
5. If the pull request is accepted, it will be merged into cFS.

#### How to Squash Commits

##### Interactive Rebase

1. Switch to the main branch and ensure you are up to date:
```sh
git checkout main && git pull
```
2. Checkout your feature branch:
```sh
git merge feature_branch
```
3. Use rebase to open the vi or other editor that lists the commits:
```sh
git rebase main -i
```
4. A text editor will open with a file that lists all the commits in your branch, and in front of each commit is the word "pick". It looks something like this:
```sh
pick 1fc6c95 do something
pick 6b2481b do something else
pick dd1475d changed some things
pick c619268 fixing typos
```
5. For every line except the first, you want to replace the word "pick" with the word "squash" or with "f". "squash" merges the commit into previous commit. "f" is like "squash", but discard this commit's log message. So, if you wish to skip the step where you have to update the commit message then use "f". To edit the first commit message, replace "pick" with "r". For example, it should look like this:
```sh
pick 1fc6c95 do something
squash 6b2481b do something else
squash dd1475d changed some things
squash c619268 fixing typos
```
or
```sh
r 1fc6c95 do something
f 6b2481b do something else
f dd1475d changed some things
f c619268 fixing typos
```
6. Save and close the file. If you used "pick" and "squash", a new file should pop up in your editor, combining all the commit messages of all the commits. Reword this commit message as you want, and then save and close that file as well.

7. Push the commit:
```sh
git push --force
```

##### Soft Reset
Use the "soft" method with caution. Ensure that you reset back to the original baseline. If you have switched branches and pulled from the remote since starting the branch originally, you may inadvertently overwrite other changes.

1. To tell Git to reset HEAD to another commit, so index and the working directory will not be altered in any way use a soft reset. All of the files changed between the original HEAD and the commit will be staged. To use a soft reset:
```sh
git reset --soft main
```
2. Add all changes:
```sh
git add -A
```
3. Add a commit message:
```sh
git commit -m "Fix #XYZ, SHORT_DESCRIPTION

LONG_DESCRIPTION (optional)"
```
4. Push the commit:
```sh
git push --force
```

##### Replace Branch
This method had no chances of inadvertently overwriting other stuff.

1. Make a new branch with a new name at the current main:
```sh
git checkout -b "fix-ISSUE_NUMBER-<FIX_SUMMARY>".
```
2. Merge:
```sh
git merge --squash ${old_branch}
```
3. Test the result, then commit to write a new commit message summarizing the full change:
```sh
git commit
```
4. Rename your new branch over your old branch to replace it:
```sh
git branch -m -f ${new_branch} ${old_branch}
```
5. Push to GitHub:
```sh
git push --force
```

#### How to Amend Commits
1. To modify your last commit message:
```sh
git commit --amend
```
2. The previous commit message will load an editor session, where you can make changes to the message, save those changes and exit. When you save and close the editor, the editor writes a new commit containing that updated commit message and makes it your new last commit. Push the new changes:
```sh
git push --force
```

1. To change the actual content of your last commit, stage those changes:
```sh
git add <file>
```
2. Amend the commit:
```sh
git commit --amend
```
3. Now the last commit is replaced by your new and improved commit. Push the commit:
```sh
git push --force
```

## Writing High-Quality Code

1. Follow cFS code conventions (formatting, symbol naming, file naming, etc). Do *not* change/reformat existing code, except to address your changes.
    - The cFS submodules use the Format Check workflow to ensure users follow the [clang-format-10](https://github.com/nasa/cFS/blob/main/.clang-format) style. For more information on how to use the Format Check workflow, view [Using GitHub Actions Workflows](#using-github-actions-workflows).  
    - The [cFS_IdentifierNamingConvention document](https://github.com/nasa/cFE/blob/main/docs/cFS_IdentifierNamingConvention.md) provides a simple naming convention guide for cFE/cFS for commands and telemetry that simplifies the EDS to ground telemetry and commands database conversion.
    - The [cFE Application Developers Guide](https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md) contains code conventions such as naming conventions for header files.

2. For any new API's, add unit tests to cover nominal and off-nominal conditions.

3. Add/edit stubs to the unit test codebase for any new/modified functions.

4. For any changes to existing API's, alter the unit tests to cover the changes (and remove tests made irrelevant due to your changes).

5. Review the static code analyses results from the Static Analysis and CodeQL Analysis workflows. For more information on how to use these workflows, view [Using GitHub Actions Workflows](#using-github-actions-workflows).  
    - Push code changes to the appropriate forked repository.
    - Go to the Actions tab and enable GitHub Actions Workflow. The CodeQL Analysis and Static Analysis will be triggered for all pushed code.
    - Review these workflows for any warnings or errors.
    - Once successful, create a pull request.

### Using GitHub Actions Workflows

Several of our GitHub Actions Workflows are used to ensure pushed code and pull requests do not break cFS, create vulnerabilities, and follow our code conventions. Other workflows are used for documentation purposes.

Most of the workflows in the NASA/cFS repository will run for all branches when code is pushed and a pull request is created, except for the changelog workflow that runs manually.

All of our workflows will be available for forked repositories once enabled. To enable workflows, navigate to the Actions tab and click "I understand my workflows, go ahead and enable them".

#### View Failed Workflows

1. Navigate to Actions in the selected repository. For newly forked repositories, enable workflows after clicking on Actions.
2. In the left sidebar, click the workflow you want to view.
3. From the list of workflow runs, click the name of the run you want to see.
4. Under Jobs or in the visualization graph, click the failed job.
5. Any failed steps are automatically expanded to display the results.

#### View Workflow Results
1. Navigate to Actions in the selected repository. For newly forked repositories, enable workflows after clicking on Actions.
2. In the left sidebar, click the workflow you want to view.
3. From the list of workflow runs, click the name of the run you want to see.
4. Scroll to the bottom of the page and download the artifacts. For CodeQL results, navigate to the Security Tab and click Code scanning alerts. CodeQL results will only be available on your forked repository.

or

1. In pull requests, click the Checks tab.
2. From the list of workflow runs, click the name of the run you want to see.
4. Scroll to the bottom of the page and download the artifacts. For CodeQL results, expand Code scanning alerts at the bottom of the list of checks and select CodeQL.

#### Configure Workflows

1. Workflows are under [.github/workflows](https://github.com/nasa/cFS/tree/main/).
2. Configure the files as needed. For more information on how to configure GitHub Actions, visit [Workflow syntax for GitHub Actions](https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions).

#### Our Workflows

Information on our GitHub Action Workflows can be found in the [.github/workflows README.md](https://github.com/nasa/cFS/tree/main/.github/workflows/README.md) document.

## Quick Links to Submodules
Before you [report bugs](#report-bugs) or submit [feature requests](#feature-requests), search through the open issues in each submodule to ensure that your ticket is relevant, not redundant, nor in conflict with other tickets:
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

If your bug or feature hasn't been reported or requested before, create a new issue in the appropriate repository. If you find a similar issue, please add a comment on it with your experience or input.

Please ensure that your name is associated with your github profile before contributing.
