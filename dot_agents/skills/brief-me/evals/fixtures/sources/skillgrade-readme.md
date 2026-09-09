# Offline copy: mgechev/skillgrade README.md (main @ 8d9e5b8, v0.3.0), excerpts

Fetched 2026-09-09 from https://github.com/mgechev/skillgrade#readme

## Presets

| Flag | Trials | Use Case |
|------|--------|----------|
| `--smoke` | 5 | Quick capability check |
| `--reliable` | 15 | Reliable pass rate estimate |
| `--regression` | 30 | High-confidence regression detection |

## Options (excerpt)

| Flag | Description |
|------|-------------|
| `--trials=N` | Override trial count |
| `--ci` | CI mode: exit non-zero if below threshold |
| `--threshold=0.8` | Pass rate threshold for CI mode |

## eval.yaml defaults (excerpt)

```yaml
defaults:
  trials: 5
  timeout: 300           # seconds
  threshold: 0.8         # for --ci mode
```

## Combining Graders

Final reward = `Σ (grader_score × weight) / Σ weight`

## CI Integration

Exits with code 1 if pass rate falls below `--threshold` (default: 0.8).

`skillgrade --help` output for presets (v0.3.0):

```
  Presets:
    --smoke            Quick smoke test (5 trials, reports pass@k)
    --reliable         Reliable pass rate (15 trials, reports mean reward)
    --regression       High-confidence regression (30 trials, reports pass^k)
```
