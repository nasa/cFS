---
name: FSW Code Change
about: Flight Software code changes
labels: fsw
---

## Description of Change

<!-- Describe what was changed, why, and how. Include any design notes that will help reviewers. -->

## Linked Issue

Closes #

## Requirements Impact

<!-- List any requirements impacted by this change. Indicate whether each was updated, or confirm the change continues to satisfy existing requirements. -->
- Requirement ID(s):
- [ ] Requirements updated as necessary
- [ ] Existing requirements are still satisfied by this change

## Testing Evidence

<!-- Paste relevant test output, link to CI runs, or attach screenshots. -->

### Unit Tests (UT Assert)
<!-- Link to CI run or paste summary -->

### COSMOS Test Suite
<!-- Link to CI run, paste summary, or note "N/A — no relevant changes" -->

## Areas of Expertise Touched

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
- [ ] Code has been formatted with `.clang-format`
- [ ] Static analysis workflows ran and passed
- [ ] Unit tests (UT Assert) updated/added to cover code changes
- [ ] Unit test workflows ran and passed
- [ ] COSMOS test suite was run; tests updated/added if relevant changes were made
- [ ] Requirements have been reviewed; updated or confirmed still satisfied (see above)
- [ ] Testing evidence is included above
- [ ] Self-review of the diff completed

---

## Reviewer Checklist

- [ ] Code logic is correct and matches the stated intent
- [ ] Code is readable, maintainable, and follows project conventions
- [ ] `.clang-format` has been applied
- [ ] Static analysis results reviewed and acceptable
- [ ] Unit tests are meaningful and adequately cover the changes
- [ ] **The change has been exercised by the unit tests** (not just that tests pass — the new/changed code paths are actually covered)
- [ ] **COSMOS test suite was executed against this change** and results reviewed (or confirmed N/A with justification)
- [ ] **Reviewer has independently verified the change behaves as described** (e.g., by running the tests locally, reviewing CI output in detail, or performing additional ad-hoc testing as warranted)
- [ ] **Memory safety** reviewed (allocation, bounds, lifetime, stack usage)
- [ ] Requirements impact reviewed and appropriate
- [ ] Error handling is appropriate
- [ ] Appropriate Expert areas have been reviewed

### Reviewer Testing Notes

<!--
Describe how you independently verified this change. For example:
  - "Pulled the branch locally, ran `make test`, all UT Assert tests passed including the new ones for X."
  - "Ran the COSMOS test suite against the branch; observed expected telemetry behavior for Y."
  - "Reviewed CI run #1234; verified the new code paths are covered by inspecting the coverage report."
  - "Performed ad-hoc test by [describe scenario]; observed [result]."
-->