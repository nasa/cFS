# AI-Assisted Contributions to cFS

The Core Flight System (cFS) requires strict safety review.
This guide covers responsible AI model use for contributions.

## What AI Can Help With

- Unit test scaffolding for new apps
- Documentation for existing interfaces
- Code patterns consistent with cFS conventions

## Dual-Model Review Pattern

For safety-critical changes, use a two-model review:

1. Generator model -- proposes the change
2. Gate model (loaded with cFS architecture docs) -- reviews for
   message bus compatibility, bounded execution, app isolation

The gate model PASS is a pre-condition for human review, not a replacement.

## Key Principle

A model can declare correctness while producing an incorrect artifact.
The gate model with the cFS spec loaded catches the contradiction
before it reaches human review.
