# Source ledger

One row per URL actually fetched. `key` must match the BibTeX key in `references.bib`. A citation with no row here fails `audit`.

| key | url | fetched | summary |
|-----|-----|---------|---------|
| skillgrade-evalrunner | https://github.com/mgechev/skillgrade/blob/8d9e5b8a275fd5f951b1239211a87d9a7133b23b/src/evalRunner.ts | 2026-09-09 | v0.3.0 source: reward = weighted grader mean; success = reward >= 0.5; pass_rate = mean reward; pass@k and pass^k computed with n = k = trials. Offline copy: sources/skillgrade-evalrunner.md |
| skillgrade-run | https://github.com/mgechev/skillgrade/blob/8d9e5b8a275fd5f951b1239211a87d9a7133b23b/src/commands/run.ts | 2026-09-09 | v0.3.0 source: --ci compares report.pass_rate to --threshold (default from eval.yaml, 0.8); results block marks one metric per preset. Offline copy: sources/skillgrade-run.md |
| skillgrade-readme | https://github.com/mgechev/skillgrade#readme | 2026-09-09 | v0.3.0 README: presets --smoke 5 / --reliable 15 / --regression 30 trials; --threshold default 0.8; reward formula. Offline copy: sources/skillgrade-readme.md |
