# Offline copy: mgechev/skillgrade src/evalRunner.ts (main @ 8d9e5b8, v0.3.0)

Fetched 2026-09-09 from https://github.com/mgechev/skillgrade/blob/8d9e5b8a275fd5f951b1239211a87d9a7133b23b/src/evalRunner.ts

Relevant excerpts, verbatim:

```ts
/**
 * Calculate pass@k: probability of at least 1 success in k trials
 * Using unbiased estimator: 1 - C(n-c, k) / C(n, k)
 */
function calculatePassAtK(n: number, c: number, k: number): number {
    if (n - c < k) return 1.0;
    let result = 1.0;
    for (let i = 0; i < k; i++) {
        result *= (n - c - i) / (n - i);
    }
    return 1.0 - result;
}

/**
 * Calculate pass^k: probability that all k trials succeed
 */
function calculatePassPowK(n: number, c: number, k: number): number {
    const p = c / n;
    return Math.pow(p, k);
}
```

```ts
        const totalReward = trials.reduce((sum, t) => sum + t.reward, 0);
        const successes = trials.filter(t => t.reward >= 0.5).length;

        const report: EvalReport = {
            task: taskName,
            metadata: opts.metadata,
            pass_rate: totalReward / numTrials,
            pass_at_k: calculatePassAtK(numTrials, successes, numTrials),
            pass_pow_k: calculatePassPowK(numTrials, successes, numTrials),
            trials,
            skills_used: skillsPaths.map(p => path.basename(p))
        };
```

```ts
            // Calculate weighted reward
            const totalWeight = graderResults.reduce((sum, r) => sum + r.weight, 0);
            const reward = totalWeight > 0
                ? graderResults.reduce((sum, r) => sum + r.score * r.weight, 0) / totalWeight
                : 0;
```

```ts
            const status = reward >= 0.5 ? fmt.pass('PASS') : fmt.fail('FAIL');
```

A trial that throws (agent timeout, grader timeout, provider error) is recorded with `reward: 0` and `grader_results: []`.
