---
name: Workflows Change
about: Changes to files under .github/workflows/
labels: ci/workflows
---

## Description of Change

<!-- Describe what workflow(s) were changed and why. -->

## Linked Issue

Closes #

## Areas of Expertise Touched

<!-- Check all that apply so the appropriate Experts can be tagged as reviewers. -->
<!-- See the expertise tags in the "cFS Dev Team" Teams channel or ask your team lead to help identify experts. -->
- [ ] ASTRO
- [ ] CI/CD
- [ ] COSMOS
- [ ] Cybersecurity
- [ ] Docker
- [ ] EDS
- [ ] Git
- [ ] PSPs
- [ ] SBN
- [ ] SMP
- [ ] Tables
- [ ] TSN
- [ ] Unit Tests
- [ ] Other

---

## Author Checklist

- [ ] Linked GitHub issue is referenced above
- [ ] Workflow changes were tested on a branch/fork before submitting this PR
- [ ] All workflows ran successfully on this branch

---

## Reviewer Checklist

- [ ] Workflow logic is correct and achieves the stated goal
- [ ] Third-party actions are from trusted sources and pinned to a specific SHA/tag
- [ ] Secrets are handled appropriately (no secrets exposed in logs, least-privilege usage)
- [ ] Workflow permissions follow the principle of least privilege
- [ ] Changes do not break or bypass existing branch protection rules
- [ ] All workflow runs on this PR completed successfully
- [ ] Appropriate Expert areas have been reviewed