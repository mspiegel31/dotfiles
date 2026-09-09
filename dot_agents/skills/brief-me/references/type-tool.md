# tool — a developer tool, CLI, library, or service the reader might adopt or operate

## Default Diátaxis mode
`explanation`. Switch to `how-to` only when `decision` is "configure" or "operate" and the reader has already adopted the tool.

## Worked-example shape
One **end-to-end run**: real input → exact command or API call → complete output → what the output means for the question. Show the full artefacts (config file, command line, output) with nothing elided. If the tool has stages (define → run → grade → report), show every stage on the same example. A second, contrasting run (a failure or an edge case) is required when the tool's value depends on distinguishing outcomes.

## What counts as primary
1. The tool's own documentation at the version named in `BRIEF.md`.
2. Its source repository (README, CHANGELOG, tests) for behaviour the docs omit.
3. The vendor's release notes or blog for design intent.
4. Standards or papers the tool implements (for metric definitions, protocols).

Community posts, comparison articles, and vendor marketing pages are not primary. Benchmarks are primary only for the numbers they themselves measured.

## Natural islands
Metrics that depend on inputs the reader controls (retry counts, thresholds, sample sizes). Cost or capacity calculators. Nothing that is a static table in disguise.

## Known traps
- Describing the feature list instead of answering the question.
- Version drift: the docs read are for a newer or older version than the one the reader will run. Record the version in the ledger summary.
- Reproducing the vendor's framing ("the easiest way to...") as if it were a finding.
