---
name: ensemble-method
description: Multi-agent ensemble for design reviews, architectural planning, complex code reviews, and research — when completeness matters and a single pass will miss things. Spawns 3-5 parallel agents with different attention focuses, then synthesizes their reviewed outputs.
---

# Ensemble Method

Produce higher-confidence outputs through decorrelated reasoning paths. Multiple
agents work the same problem with slightly different attention focuses, then a
synthesizer integrates their reviewed outputs. Small differences in focus cascade
through the reasoning chain, producing meaningfully different outputs that cover
more of the solution space than any single attempt.

---

## When to Use

- Design reviews where completeness matters
- Architectural planning with multiple competing concerns
- Complex code reviews (large PRs, unfamiliar domains)
- Research tasks where a single pass will inevitably miss things

## When NOT to Use

- Straightforward bug fixes
- Single-file changes with clear success criteria
- Tasks where the answer is obvious after reading the code
- Anything where one good pass is sufficient

---

## Process

### 1. Select Attention Focuses

The user provides them, or select contextually appropriate ones. Focuses should
be **small perturbations, not fundamentally different approaches**. The diversity
comes from how small differences cascade through the reasoning chain.

Good focuses for a **design review**:
- Type safety and correctness
- API ergonomics for new users
- Consistency with existing patterns
- Error handling completeness
- Test coverage of boundary conditions

Good focuses for a **code review**:
- Correctness and edge cases
- Security implications
- Performance and resource usage
- Test coverage gaps
- Maintainability and readability

Good focuses for **research/analysis**:
- Architecture and data flow
- Failure modes and recovery paths
- Concurrency and state management
- External integration points
- Testable properties and invariants

3-5 focuses is the sweet spot. More than that pays for repetition without
meaningful diversity.

### 2. Spawn Parallel Agents

Fire one `task()` per attention focus, all in the same turn:

```
task(
  category="ultrabrain",
  load_skills=[],
  run_in_background=true,
  description="Ensemble: [focus name]",
  prompt="[see Agent Prompt Template below]"
)
```

Every agent starts with **fresh context** — no shared conversation history. That
fresh start is part of why small differences in attention focus cascade into
meaningfully different outputs.

### 3. Collect and Synthesize

After all agents complete, pass their outputs to a synthesis step. Follow the
Synthesis Methodology below — either perform it yourself or delegate to a
separate `task(category="ultrabrain")`.

### 4. Present Result

Present the synthesized output with the synthesis rationale, remaining
uncertainties, and agent contribution summary.

---

## Agent Prompt Template

Each agent's prompt must include:

```
You are an ensemble agent analyzing [task description].

INSTRUCTIONS:
- Read and follow all AGENTS.md principles (Code Design Principles, Code Change
  Discipline, Testing, etc.)
- Cover the full scope of the task, but go deeper on your attention focus

ATTENTION FOCUS: [short directive — a spotlight, not blinders]
Pay particular attention to [focus area]. This shifts where you go deeper, but
you still cover everything.

TASK:
[Full task description with file paths, context, constraints]

OUTPUT FORMAT (required):
## Approach
[Your actual output — analysis, findings, plan, or review]

## Key Decisions
For each significant choice:
- What was decided
- Alternatives considered
- Confidence level (high/medium/low)
- Reasoning

## Uncertainties
[Things you weren't sure about, flagged explicitly]

## Assumptions
[Things taken as given that could be wrong]
```

The structured output format is critical — it makes synthesis procedural rather
than guesswork. One agent's uncertainty may be another's confident conclusion.

---

## Synthesis Methodology

### DO

**Find the high-confidence foundation.** Identify where agents agree — these are
the strongest building blocks.

**Resolve divergences.** Where agents disagree, determine why. One may have gone
deeper on that aspect due to its attention focus. Evaluate which approach is
stronger on the merits, not on which agent produced it.

**Mine the intersections.** Look at alternatives that one agent rejected but
another successfully used. These intersections are where emergent solutions
live — combinations no individual agent would have reached alone.

**Cross-reference uncertainties.** Check each agent's uncertainties against other
agents' confident conclusions. Where one agent's confidence resolves another's
uncertainty, incorporate that resolution. Where ALL agents share the same
uncertainty, flag it — that's a genuinely hard subproblem to escalate.

**Recognize dominance.** When one agent's output is clearly superior and the
others are noise, say so and use it. Blending for the sake of inclusion is a
failure mode.

### DO NOT

- Pick the "best" output and discard the rest
- Average or blend outputs for the sake of inclusion
- Add your own independent analysis as if you were another agent
- Dilute a dominant output by mixing in weaker elements

### Synthesis Output Format

```
## Integrated Result
[The synthesized output]

## Synthesis Rationale
For each significant integration decision:
- What was chosen and from which agent(s)
- Why preferred over alternatives
- What emerged from the combination that no individual agent had

## Remaining Uncertainties
[Shared across all agents — these need the user's input]

## Agent Contribution Summary
Brief note on what each agent's attention focus caused it to catch that others
missed. This calibrates which focuses produce useful diversity for future
ensemble invocations.
```

---

## OmO Configuration Notes

- **Category**: `ultrabrain` routes to Opus — use this for ensemble agents
- **Parallelism**: All `task(run_in_background=true)` calls in the same turn
  execute concurrently
- **Concurrency**: If you hit rate limits with 3-5 simultaneous Opus calls, add
  to `oh-my-opencode.json`:
  ```json
  "background_task": {
    "modelConcurrency": {
      "amazon-bedrock/us.anthropic.claude-opus-4-6-v1": 5
    }
  }
  ```
- **No custom categories needed**: The skill constructs attention-focus prompts
  dynamically — hardcoding focuses in config reduces flexibility

---

<!-- When updating this skill, maintain citations below. Link to primary sources, not summaries. -->

## Sources

### Primary

- Allen, S.T. [An Ensemble of Claudes](https://www.seantallen.com/posts/an-ensemble-of-claudes/). Mar 2026. — Origin of the ensemble method for LLM agents: decorrelated attention focuses, structured output format, synthesis agent, and the insight that small prompt perturbations cascade into meaningfully different outputs. Includes the actual ensemble and synthesizer skill definitions.
- Allen, S.T. [Teaching Claude to Write Pony](https://www.seantallen.com/posts/teaching-claude-to-write-pony/). Feb 2026. — Prerequisite context: the reviewer loop, principle-driven development, and the mentoring metaphor that the ensemble builds on.

### Background (Ensemble Methods in ML)

- [Random Forests](https://en.wikipedia.org/wiki/Random_forest) — The ML technique Sean draws the analogy from: many weak models with random feature subsets outperform one strong model. The LLM ensemble uses attention focuses instead of feature subsets.
- [Boosting](https://en.wikipedia.org/wiki/Boosting_(machine_learning)) — Sequential ensemble variant (not used here, but part of the conceptual family).
