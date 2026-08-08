---
name: aar-writer
description: "After-action report (AAR) and post-incident review authoring — blameless root cause analysis, timestamped timelines, quantified impact, and actionable learnings. Use when writing, reviewing, or improving incident reports, post-mortems, retrospectives, or any document that analyzes what happened and what to do differently. Also use when the user mentions AAR, post-mortem, incident review, or retrospective."
---

# AAR Writer

Writing guidance for after-action reports (AARs), post-incident reviews, and post-mortems. These are retrospective documents that analyze what happened, why, and what changes to make — written after an incident, outage, or significant event.

For shared writing principles, load the `writing-foundations` skill.

**How this differs from other writing skills:**
- Forward-looking documents (pitches, PRDs) answer "what should we build?" AARs answer "what happened and what do we change?"
- AARs are **blameless** — they name systemic causes, not individuals.
- The audience is both the immediate team (learnings) and future teams (institutional memory).

---

## Document Anatomy

An AAR has these sections in order:

1. **Executive Summary** — the entire incident in 2-3 paragraphs
2. **Detailed Analysis**
   - Background (system primer for readers unfamiliar with the affected system)
   - Root Cause
   - Detection & Customer Impact
   - Mitigation & Troubleshooting
   - Final Remediations
   - Post-Incident Clean Up
3. **Learnings**
   - What Worked
   - What Needs Improvement

Not every incident needs every subsection. A minor incident might skip Background and Post-Incident Clean Up. But Executive Summary → Root Cause → Impact → Learnings is the core — never skip those.

---

## Section Priority

| Priority | Sections | Why |
|---|---|---|
| **Must nail** | Root Cause | The whole point of the AAR. Must be blameless, specific, and systemic. |
| **Must nail** | Learnings (What Needs Improvement) | Concrete action items that prevent recurrence. The accountability mechanism. |
| **Important** | Executive Summary | The only section many stakeholders will read. Must stand alone. |
| **Important** | Detection & Customer Impact | Quantified: how many users, which use cases, how long. |
| **Valuable** | Mitigation & Troubleshooting | Honest about what was slow and why — prevents the same debugging mistakes. |
| **Valuable** | Background | Educates readers unfamiliar with the affected system. Same modular primer pattern as PRDs. |
| **Procedural** | What Worked | Acknowledges the team's response. Brief is fine. |
| **Procedural** | Post-Incident Clean Up | Records what was done after the incident ended. |

For per-section writing guidance and anti-patterns, read `references/section-guidance.md`.

---

## Quality Checklist

Ordered by priority.

| Check | Pass criteria |
|---|---|
| **Root cause is blameless** | Names the systemic failure, not the individual. "Ambiguous UI" not "[Person] clicked wrong button." |
| **Root cause is specific** | Causal chain with timestamps, not "something went wrong." |
| **Learnings are commitments** | "We will enforce X" not "We should be more careful." Ideas tagged with **(Idea)**. |
| **Impact is quantified** | Number of affected users/services, duration, specific use cases broken. |
| **Timeline is timestamped** | Key events have times: trigger, first alert, first fix, full resolution. |
| **Executive summary stands alone** | A reader who reads only this section gets the full picture. |
| **Troubleshooting is honest** | Names what was slow, what was confusing, what trap was fallen into. |
| **"Why not already?" answered** | For obvious preventable issues, explains why the safeguard wasn't in place. |

---

## Examples

If the project repo has an `examples/` directory with annotated AARs, read them for calibration. Each example includes a PDF export and an annotations file.
