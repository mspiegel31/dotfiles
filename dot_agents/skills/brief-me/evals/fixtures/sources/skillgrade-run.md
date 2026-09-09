# Offline copy: mgechev/skillgrade src/commands/run.ts and src/utils/cli.ts (main @ 8d9e5b8, v0.3.0)

Fetched 2026-09-09 from
- https://github.com/mgechev/skillgrade/blob/8d9e5b8a275fd5f951b1239211a87d9a7133b23b/src/commands/run.ts
- https://github.com/mgechev/skillgrade/blob/8d9e5b8a275fd5f951b1239211a87d9a7133b23b/src/utils/cli.ts

Relevant excerpts, verbatim.

run.ts — threshold comparison after each task, and CI exit:

```ts
                resultsSummary(report.pass_rate, report.pass_at_k, report.pass_pow_k, trials, opts.preset);

                if (report.pass_rate < (opts.threshold ?? config.defaults.threshold)) {
                    allPassed = false;
                }
```

```ts
    // CI mode: exit with appropriate code
    if (opts.ci) {
        const threshold = opts.threshold ?? config.defaults.threshold;
        if (!allPassed) {
            console.error(`\n  ${fmt.fail('CI FAILED')}  below threshold ${(threshold * 100).toFixed(0)}%\n`);
            throw new Error('CI check failed');
        }
        console.log(`\n  ${fmt.pass('CI PASSED')}  above threshold ${(threshold * 100).toFixed(0)}%\n`);
    }
```

cli.ts — the results block. The preset marks one line with a `◂` marker:

```ts
export function resultsSummary(passRate: number, passAtK: number, passPowK: number, trials: number, preset?: string) {
    const presetLabel = preset === 'smoke' ? ' (smoke test)'
        : preset === 'reliable' ? ' (reliable)'
            : preset === 'regression' ? ' (regression)'
                : '';

    header(`Results${presetLabel}`);

    const fmtPct = (v: number) => `${(v * 100).toFixed(1)}%`.padStart(7);
    const marker = (key: string) => preset === key ? fmt.cyan(' ◂') : '';

    console.log(`    Pass Rate  ${fmt.bold(fmtPct(passRate))}${marker('reliable')}`);
    console.log(`    pass@${trials}     ${fmtPct(passAtK)}${marker('smoke')}`);
    console.log(`    pass^${trials}     ${fmtPct(passPowK)}${marker('regression')}`);
    console.log();
}
```
