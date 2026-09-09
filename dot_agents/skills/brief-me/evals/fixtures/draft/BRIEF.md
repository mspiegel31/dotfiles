# Brief: What SkillGrade counts as passing

## Question
What does SkillGrade 0.3.0 count as a passing trial and a passing task, and which number does `--ci` compare against `--threshold`?

## Decision this feeds
Choose a `--threshold` for gating a skill in CI.

## Audience
self

## Prior knowledge (do not re-explain)
- What an agent skill / SKILL.md is.
- What SkillGrade's eval.yaml, tasks, trials, and graders are.
- YAML and CLI usage.

## Type
tool

## Diátaxis mode
explanation

## Depth
10 min

## Source constraints
Only the three offline copies in `sources/`, already recorded in `sources/LEDGER.md`. Do not fetch anything.

## Worked example
One task, 5 trials, rewards 0.9, 0.6, 0.4, 1.0, 0.45. Show: per-trial pass/fail, successes count, Pass Rate (mean reward), pass@5, pass^5, and whether `--ci --threshold=0.8` passes. Show the arithmetic.

## Interaction
none

## Status
draft

## History
- 2026-09-09: brief written
