# Core Flight System Contributing Guide

So you'd like to contribute to cFS? Below are some guidelines for contributors to follow. Contributions come in all shapes and sizes. We appreciate your help with documentation, unit tests, framework code, continuous-integration, or simply reporting bugs and improvement ideas. We can't promise that we'll accept every suggestion or fix every bug in a timely manner but we'll respond to you as quickly as possible.

<!-- vscode-markdown-toc -->
* [Code of Conduct](#CodeofConduct)
* [Ways to Contribute](#WaystoContribute)
	* [Discussions and Questions](#DiscussionsandQuestions)
	* [Security Vulnerabilities](#SecurityVulnerabilities)
	* [Bug Reports](#BugReports)
		* [Before Reporting a Bug](#BeforeReportingaBug)
		* [Reporting a Bug](#ReportingaBug)
		* [What Happens to my Bug Report?](#WhatHappenstomyBugReport)
	* [New Feature Requests](#NewFeatureRequests)
		* [Before Requesting a New Feature](#BeforeRequestingaNewFeature)
		* [Requesting a New Feature](#RequestingaNewFeature)
		* [What Happens to my Feature Request?](#WhatHappenstomyFeatureRequest)
	* [Issue Trackers for cFS Components](#IssueTrackersforcFSComponents)
		* [cFS-Framework Components](#cFS-FrameworkComponents)
		* [Lab Apps](#LabApps)
		* [Tools](#Tools)
		* [Other cFS Apps](#OthercFSApps)
	* [Pull Requests](#PullRequests)
		* [Contributor License Agreement (CLA)](#ContributorLicenseAgreementCLA)
		* [Before starting your Pull Request](#BeforestartingyourPullRequest)
		* [Creating a Pull Request](#CreatingaPullRequest)
		* [What Happens to My Pull Request?](#WhatHappenstoMyPullRequest)
* [Writing High-Quality Code](#WritingHigh-QualityCode)
	* [Using GitHub Actions Workflows](#UsingGitHubActionsWorkflows)
		* [View Failed Workflows](#ViewFailedWorkflows)
		* [View Workflow Results](#ViewWorkflowResults)
		* [Configure Workflows](#ConfigureWorkflows)
		* [cFS Workflows](#cFSWorkflows)
	* [Git Commit Guidelines](#GitCommitGuidelines)
		* [Working with git commits](#Workingwithgitcommits)

<!-- vscode-markdown-toc-config
	numbering=false
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## <a name='CodeofConduct'></a>Code of Conduct

This project and everyone participating in it is governed by the [cFS Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the [product team](README.md#contact-the-cfs-product-team).

## <a name='WaystoContribute'></a>Ways to Contribute

### <a name='DiscussionsandQuestions'></a>Discussions and Questions

For discussions, questions, or ideas, [start a new discussion](https://github.com/nasa/cFS/discussions/new) in the cFS repository under the Discussions tab. If you prefer email, you can also [join the cfs community mailing list](README.md#join-the-mailing-list).

### <a name='SecurityVulnerabilities'></a>Security Vulnerabilities

Please view our [Security Policy](https://github.com/nasa/cFS/security/policy) for more information.

### <a name='BugReports'></a>Bug Reports

#### <a name='BeforeReportingaBug'></a>Before Reporting a Bug
1. Perform a cursory search to see if the bug has already been reported. For issues in each submodule, check the issue trackers in the relevant [cFS component/submodule](#IssueTrackersforcFSComponents). If a bug has been reported and the issue is still open, add a comment to the existing issue instead of opening a new one.
2. Determine which repository the bug should be reported in. If you are not sure, place the [issue in NASA/cFS](https://github.com/nasa/cFS/issues/new?assignees=&labels=&template=bug_report.md&title=).

#### <a name='ReportingaBug'></a>Reporting a Bug

If you find a bug in our code don't hesitate to report it:

1. Open an issue using the bug report template.
2. Describe the issue.
3. Describe the expected behavior if the bug did not occur.
4. Provide the reproduction steps that someone else can follow to recreate the bug or error on their own.
5. If applicable, add code snippets or references to the software.
6. Provide the system the bug was observed on including the hardware, operating system, and versions.
7. Provide any additional context if applicable.
8. Provide your full name or GitHub username and your company organization if applicable.

#### <a name='WhatHappenstomyBugReport'></a>What Happens to my Bug Report?

1. The cFS team will label the issue.
2. A team member will try to reproduce the issue with your provided steps. If the team is able to reproduce the issue, the issue will be left to be implemented by someone.

### <a name='NewFeatureRequests'></a>New Feature Requests

cFS has a multitude of users from different fields and backgrounds. We appreciate your ideas for enhancements! 

#### <a name='BeforeRequestingaNewFeature'></a>Before Requesting a New Feature

1. Review the [cFS README](README.md) to see if your feature is in the major future work.
2. Perform a cursory search to see if the feature has already been requested. For issues in each submodule, check the issue trackers in the relevant [cFS component/submodule](#IssueTrackersforcFSComponents). If a feature request has been reported and the issue is still open, add a comment to the existing issue instead of opening a new one.
3. Determine which repository the feature should be reported in. If you are not sure, [place the issue in NASA/cFS](https://github.com/nasa/cFS/issues/new?assignees=&labels=&template=feature_request.md&title=).

#### <a name='RequestingaNewFeature'></a>Requesting a New Feature

1. Open an issue using the feature request template.
2. Describe the feature.
3. Describe the solution you would like.
4. Describe alternatives you've considered.
5. Provide any additional context if applicable.
6. Provide your full name or GitHub username and your company organization if applicable.

#### <a name='WhatHappenstomyFeatureRequest'></a>What Happens to my Feature Request?

1. The project team will label the issue.
2. The project team will evaluate the feature request, possibly asking you more questions to understand its purpose and any relevant requirements. If the issue is closed, the team will convey their reasoning and suggest an alternative path forward.
3. If the feature request is accepted, it will be marked for implementation.

### <a name='IssueTrackersforcFSComponents'></a>Issue Trackers for cFS Components

Before you [report bugs](#BugReports) or submit [feature requests](#FeatureRequests), search through the open issues in each submodule to ensure that your ticket is relevant, not redundant, nor in conflict with other open or pull requests issues:

If your bug or feature hasn't been reported or requested before, create a new issue in the appropriate repository. If you find a similar issue, please add a comment on it with your experience or input.

Please ensure that your name is associated with your github profile before contributing.

For issues related to continuous testing workflows or multi-component "epics" start at the "framework bundle"
- [cFS Bundle](https://github.com/nasa/cfs/issues)


#### <a name='cFS-FrameworkComponents'></a>cFS-Framework Components

- [cFE](https://github.com/nasa/cfe/issues)
- [OSAL](https://github.com/nasa/osal/issues)
- [PSP](https://github.com/nasa/psp/issues)

#### <a name='LabApps'></a>Lab Apps 

- [ci\_lab](https://github.com/nasa/ci_lab/issues)
- [sample\_app](https://github.com/nasa/sample_app/issues)
- [sample\_lib](https://github.com/nasa/sample_lib/issues)
- [sch\_lab](https://github.com/nasa/sch_lab/issues)
- [to\_lab](https://github.com/nasa/to_lab/issues)

#### <a name='Tools'></a>Tools

- [cFS-GroundSystem](https://github.com/nasa/cFS-GroundSystem/issues)
- [elf2cfetbl](https://github.com/nasa/elf2cfetbl/issues)
- [tblCRCTool](https://github.com/nasa/tblCRCTool/issues)

#### <a name='OthercFSApps'></a>Other cFS Apps

- [BP](https://github.com/nasa/BP/issues)
- [CF](https://github.com/nasa/CF/issues)
- [CS](https://github.com/nasa/CS/issues)
- [DS](https://github.com/nasa/DS/issues)
- [FM](https://github.com/nasa/FM/issues)
- [HK](https://github.com/nasa/HK/issues)
- [HS](https://github.com/nasa/HS/issues)
- [LC](https://github.com/nasa/LC/issues)
- [MD](https://github.com/nasa/MD/issues)
- [MM](https://github.com/nasa/MM/issues)
- [SC](https://github.com/nasa/SC/issues)
- [SCH](https://github.com/nasa/SCH/issues)


### <a name='PullRequests'></a>Pull Requests

#### <a name='ContributorLicenseAgreementCLA'></a>Contributor License Agreement (CLA)

Before your code can be merged into our repositories, we require a contributor license agreement (CLA) for individual and corporate contributors. You can submit this by downloading, signing, and emailing the appropriate form to [GSFC-SoftwareRelease@mail.nasa.gov](mailto:GSFC-SoftwareRelease@mail.nasa.gov) and copy [cfs-program@lists.nasa.gov](mailto:cfs-program@lists.nasa.gov). 

Currently we have separate forms for the Framework repositories and the Apps. See the sections below to know which one to submit. We plan on combining these in the future. 

##### <a name='cFS-FrameworkCLA'></a>cFS-Framework CLA

For contributions to the cFS-Framework repositories: [cFE](https://github.com/nasa/cFE), [osal](https://github.com/nasa/osal), [psp](https://github.com/nasa/psp), or submodules in the [apps](apps), [libs](libs), and [tools](tools) directories. 

- [cFS-Framework Corporate CLA](cFS_Framework_Corporate_CLA.pdf)

- [cFS-Framework Individual CLA](cFS_Framework_Individual_CLA.pdf)

##### <a name='cFS-AppsCLA'></a>cFS-Apps CLA

For contributions to the cFS Apps repositories [BP](https://github.com/nasa/bp), [CF](https://github.com/nasa/cf), [CS](https://github.com/nasa/cs), [DS](https://github.com/nasa/ds), [FM](https://github.com/nasa/fm), [HK](https://github.com/nasa/HK/), [HS](https://github.com/nasa/HS/), [LC](https://github.com/nasa/LC/), [MD](https://github.com/nasa/MD/), [MM](https://github.com/nasa/MM/), [SC](https://github.com/nasa/SC), [SCH](https://github.com/nasa/SCH)

- [cFS-Apps Corporate CLA](cFS_Apps_Corporate_CLA.pdf)

- [cFS-Apps Individual CLA](cFS_Apps_Individual_CLA.pdf)

#### <a name='BeforestartingyourPullRequest'></a>Before starting your Pull Request

Ready to Add Your Code? Follow GitHub's fork-branch-pull request pattern.

1. Fork the relevant cFS component (eg. cfe, osal, psp).

2. Find the related issue number or create an associated issue that explains the intent of your new code. 

3. Create a new branch in your fork to work on your fix. We recommend naming your branch `fix-ISSUE_NUMBER-<FIX_SUMMARY>`.

3. Add commits to your branch. For information on commit messages, review [How to Write a Git Commit Message](https://chris.beams.io/posts/git-commit/) and our [git commit guidelines](#GitCommitGuidelines).

#### <a name='CreatingaPullRequest'></a>Creating a Pull Request

We recommend creating your pull-request as a "draft" and to commit early and often so the community can give you feedback at the beginning of the process as opposed to asking you to change hours of hard work at the end.

1. For the title, use the title convention `Fix #XYZ, SHORT_DESCRIPTION`.
2. Describe the contribution. First document which issue number was fixed using the template "Fix #XYZ". Then describe the contribution.
3. Provide what testing was used to confirm the pull request resolves the link issue. If writing new code, also provide the associated coverage unit tests. 
4. Provide the expected behavior changes of the pull request.
5. Provide the system the bug was observed on including the hardware, operating system, and versions.
6. Provide any additional context if applicable.
7. Provide your full name or GitHub username and your company or organization if applicable.
8. Verify that the PR passes all workflow checks. If you expect some of these checks to fail. Please note it in the Pull Request text or comments. 

#### <a name='WhatHappenstoMyPullRequest'></a>What Happens to My Pull Request?

1. The cFS team will label and evaluate the pull request in the [weekly configuration control board meeting](https://github.com/nasa/cFS/wiki/The-cFS-CCB-Process).
2. If the pull request is accepted, it will be merged into cFS.

## <a name='WritingHigh-QualityCode'></a>Writing High-Quality Code

1. Follow cFS code conventions (formatting, symbol naming, file naming, etc). Do *not* change or reformat existing code, except to address your changes.
    - The Format Check Workflow helps users follow [our coding style](https://github.com/nasa/cFS/blob/main/.clang-format); see [Using GitHub Actions Workflows](#using-github-actions-workflows) to learn more about these.  
    - Check our [Naming Convention Guide](https://github.com/nasa/cFE/blob/main/docs/cFS_IdentifierNamingConvention.md) provides a simple naming convention guide for cFE/cFS for commands and telemetry that simplifies the EDS to ground telemetry and commands database conversion.
    - The [cFE Application Developers Guide](https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md) contains code conventions such as naming conventions for header files.

2. For new APIs, add unit tests to cover nominal and off-nominal conditions.

3. Add or expand stub functions for the unit test codebase when adding or modifying functions.

4. If changing existing APIs, update the unit tests to cover these changes and remove tests that are no longer relevant.

5. Review the static code analyses results from the Static Analysis and CodeQL Analysis workflows. For more information on how to use these workflows, view [Using GitHub Actions Workflows](#using-github-actions-workflows).  
    - Push code changes to the appropriate forked repository.
    - Go to the Actions tab and enable GitHub Actions Workflow. The CodeQL Analysis and Static Analysis will be triggered for all pushed code.
    - Review these workflows for any warnings or errors.
    - Once successful, create a pull request.

### <a name='UsingGitHubActionsWorkflows'></a>Using GitHub Actions Workflows

Several of our GitHub Actions Workflows are used to ensure pushed code and pull requests do not break cFS, create vulnerabilities, and follow our code conventions. Other workflows are used for documentation purposes.

Most of the workflows in the NASA/cFS repository will run for all branches when code is pushed and a pull request is created, except for the changelog workflow that runs manually.

All of our workflows will be available for forked repositories once enabled. To enable workflows, navigate to the Actions tab and click "I understand my workflows, go ahead and enable them".

#### <a name='ViewFailedWorkflows'></a>View Failed Workflows

1. Navigate to Actions in the selected repository. For newly forked repositories, enable workflows after clicking on Actions.
2. In the left sidebar, click the workflow you want to view.
3. From the list of workflow runs, click the name of the run you want to see.
4. Under Jobs or in the visualization graph, click the failed job.
5. Any failed steps are automatically expanded to display the results.

#### <a name='ViewWorkflowResults'></a>View Workflow Results

1. Navigate to Actions in the selected repository. For newly forked repositories, enable workflows after clicking on Actions.
2. In the left sidebar, click the workflow you want to view.
3. From the list of workflow runs, click the name of the run you want to see.
4. Scroll to the bottom of the page and download the artifacts. For CodeQL results, navigate to the Security Tab and click Code scanning alerts. CodeQL results will only be available on your forked repository.

or

1. In pull requests, click the Checks tab.
2. From the list of workflow runs, click the name of the run you want to see.
4. Scroll to the bottom of the page and download the artifacts. For CodeQL results, expand Code scanning alerts at the bottom of the list of checks and select CodeQL.

#### <a name='ConfigureWorkflows'></a>Configure Workflows

1. Workflows are under [.github/workflows](.github/workflows).
2. Configure the files as needed. For more information on how to configure GitHub Actions, visit [Workflow syntax for GitHub Actions](https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions).

#### <a name='cFSWorkflows'></a>cFS Workflows

The cFS testing workflows are found in [.github/workflows](.github/workflows) you can read a summary in the [that directory's README](.github/workflows/README.md).

### <a name='GitCommitGuidelines'></a>Git Commit Guidelines

1. Please follow commit message convention. This format links github issues to pull requests with code and helps us to deliver high-quality documentation. The format-check workflow will also alert you when your commits or pull request do not match the format below.
```
Fix #XYZ, SHORT_DESCRIPTION

LONG_DESCRIPTION (optional)
```

2. Verify there is a single commit message per topic. For a good example, see this multi-topic [pull request](https://github.com/nasa/cFE/pull/1203/commits).

3. [Squash](#how-to-squash-commits) or [amend](#how-to-amend-commits) the commit messages as needed.


#### <a name='Workingwithgitcommits'></a>Working with git commits

The following sections are short "How-Tos" on techniques that can help you edit your commits to better match our format guidelines.

##### <a name='HowtoSquashCommits'></a>How to Squash Commits

###### Interactive Rebase

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

###### Soft Reset

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

###### Replace Branch

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

##### <a name='HowtoAmendCommits'></a>How to Amend Commits

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
git add <files-to-amend>
```
2. Amend the commit:

```sh
git commit --amend
```
3. Now the last commit is replaced by your new and improved commit. Push the commit:

```sh
git push --force
```

