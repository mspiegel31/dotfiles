---
name: diataxis-writer
description: Detailed Diataxis framework reference for documentation writing — per-type quality checklists, structural templates, compass diagnostic, and anti-pattern catalog. Load when authoring or reviewing documentation to verify type classification and quality.
---

# Diataxis Writer — Deep Reference

Use this skill when you need detailed guidance on a specific Diataxis documentation type, want to verify your classification is correct, or need to diagnose quality problems in existing docs.

---

## Compass Diagnostic Procedure

When unsure which type a piece of documentation should be, run through these questions in order:

1. **Is the reader trying to DO something specific right now?**
   - Yes → action-oriented. Go to step 2.
   - No → cognition-oriented. Go to step 3.

2. **Does the reader already know how to use this technology?**
   - Yes → **How-to guide** (practitioner at work, needs to accomplish a goal)
   - No → **Tutorial** (newcomer at study, needs to learn by doing)

3. **Is the reader looking up a specific fact or interface detail?**
   - Yes → **Reference** (practitioner at work, needs precise information)
   - No → **Explanation** (learner at study, needs to understand concepts)

**Edge cases:**
- "Getting started" → Usually a tutorial, NOT a how-to. The reader doesn't have a specific goal yet — they're learning.
- "Quickstart" → Ambiguous. If it teaches, it's a tutorial. If it assumes competence and gets to a result fast, it's a how-to.
- "Best practices" → Usually explanation. It discusses "why" and trade-offs, not "do this now."
- "Troubleshooting" → Usually how-to. "How to fix X when Y happens."
- "Architecture overview" → Explanation. It builds understanding, not task completion.
- "API reference" → Reference. Pure information — parameters, types, return values.
- "Migration guide" → How-to. The reader has a specific goal: migrate from A to B.

---

## Per-Type Quality Checklists

### Tutorial Quality

| Check | Pass criteria |
|---|---|
| **Outcome stated upfront** | First paragraph tells the reader what they will build/achieve |
| **Works when followed exactly** | A reader with only the stated prerequisites can complete it without errors |
| **No unexplained choices** | Every decision point is made for the reader — don't offer alternatives |
| **Minimum viable scope** | Teaches one thing well. Doesn't try to cover everything |
| **Concrete, not abstract** | Uses real values, real file names, real commands — not "yourvalue" placeholders |
| **No mid-flow theory** | Conceptual explanations go in a callout or link to explanation docs |
| **Progress is visible** | Reader can verify each step worked before moving to the next |
| **Encouraging tone** | "You've now..." not "The system has..." — the reader is the protagonist |

**Structural template:**
```
# Tutorial: [What the reader will build/learn]

In this tutorial, you will [concrete outcome]. By the end, you'll have [tangible result].

## Prerequisites
- [Exact version/tool/account needed]
- [Link to installation if needed]

## Step 1: [Verb phrase — what the reader does]
[Instruction]
[Expected result they can verify]

## Step 2: [Verb phrase]
...

## What you've built
[Summary of what they now have]

## Next steps
- [Link to how-to guide for common tasks]
- [Link to reference for the API they just used]
- [Link to explanation for the concepts behind what they built]
```

### How-to Guide Quality

| Check | Pass criteria |
|---|---|
| **Title is "How to [verb]"** | Goal is in the title. Reader knows immediately if this is what they need |
| **Assumes competence** | Doesn't teach fundamentals. Reader already knows the technology |
| **One goal** | Each guide accomplishes exactly one thing. Split multi-goal guides |
| **Steps, not discussion** | Every paragraph either instructs or warns. No essays |
| **Edge cases addressed** | Common variations and gotchas are covered |
| **Skippable prerequisites** | Listed briefly, not explained. Reader knows if they have them |

**Structural template:**
```
# How to [accomplish specific goal]

[One sentence: what this achieves and when you'd want it]

## Prerequisites
- [Brief list — no explanations]

## Steps

1. [Action verb] [what to do]
   ```
   [command or code]
   ```

2. [Action verb] [what to do]
   ...

## Verification
[How to confirm it worked]

## Common variations
- **[Variation A]**: [Modified step]
- **[Variation B]**: [Modified step]
```

### Reference Quality

| Check | Pass criteria |
|---|---|
| **Complete within scope** | Every parameter, method, option, or field is documented. No gaps |
| **Consistent structure** | Every entry follows the same format (name, type, description, default, example) |
| **Scannable** | Tables, definition lists, or consistent heading patterns. Reader can find facts fast |
| **Accurate** | Types, defaults, and behaviors match the actual code. Verify against source |
| **No narrative** | No "you might want to..." — just facts. Link to how-to guides for usage patterns |
| **Examples are minimal** | Show the parameter/method in use, nothing more. Not a tutorial |

**Structural template:**
```
# [API/Module/Component name]

[One line: what this is]

## [Method/Function/Endpoint name]

[One line description]

**Signature:**
```language
function name(param1: Type, param2: Type): ReturnType
```

**Parameters:**

| Name | Type | Required | Default | Description |
|---|---|---|---|---|
| `param1` | `string` | Yes | — | What this parameter does |
| `param2` | `number` | No | `10` | What this parameter does |

**Returns:** `Type` — description

**Errors:**
- `ErrorName` — When this occurs

**Example:**
```language
// Minimal example showing this specific method
```
```

### Explanation Quality

| Check | Pass criteria |
|---|---|
| **Answers "why"** | Reader understands the reasoning, not just the facts |
| **Discusses trade-offs** | Presents alternatives and why this approach was chosen |
| **No step-by-step instructions** | Link to how-to guides instead. Explanation illuminates, doesn't instruct |
| **Connected to broader context** | Relates this concept to adjacent concepts the reader knows |
| **Has a clear scope** | Doesn't try to explain everything. Draws a boundary and stays within it |
| **Conversational but precise** | Reads naturally without sacrificing technical accuracy |

**Structural template:**
```
# [Concept or decision being explained]

[Opening: what this explanation covers and why it matters]

## Background
[Context the reader needs to understand the concept]

## How [concept] works
[The core explanation — conceptual, not procedural]

## Why [this approach/design/decision]
[Trade-offs, alternatives considered, constraints that shaped the decision]

## Relationship to [adjacent concept]
[How this connects to things the reader already knows]

## Summary
[Key takeaways — what the reader should now understand]

## Further reading
- [Link to reference for the implementation details]
- [Link to how-to guide for putting this into practice]
```

---

## Anti-Pattern Catalog

| What you wrote | What's wrong | What type it actually is | Fix |
|---|---|---|---|
| How-to guide that explains concepts for 3+ paragraphs before the steps | Teaching in a task guide | The explanation parts are **explanation** | Extract concept content to an explanation page; link from the how-to |
| Tutorial that says "configure as needed" or "choose your preferred option" | Offering choices in a learning experience | The choice points are **how-to guide** | Make the choice for the reader. Tutorials have one path |
| Reference that says "you should use X when..." | Opinion in a fact sheet | The advisory content is **explanation** or **how-to** | State what X does, not when to use it. Link to explanation or how-to |
| Explanation with numbered steps | Instructions in a concept page | The steps are a **how-to guide** | Extract steps to a how-to; link from the explanation |
| "Getting started" page that lists every feature | Tour, not a tutorial | Mix of **reference** and **tutorial** | Pick one beginner task and build a real tutorial around it |
| Reference that explains implementation rationale | Narrative in a fact sheet | The rationale is **explanation** | Move to an explanation page; keep reference austere |
| Tutorial with a "Reference" section at the bottom | Two types crammed together | Split into **tutorial** + **reference** pages | Separate documents, cross-linked |

---

## Cross-Referencing Between Types

Every documentation page should link to its counterparts in other types:

| From this type | Link to | Phrase pattern |
|---|---|---|
| Tutorial | How-to guides | "Now that you've learned X, see [How to do Y] for common tasks" |
| Tutorial | Reference | "For the full list of options, see the [X reference]" |
| How-to guide | Reference | "For parameter details, see the [X reference]" |
| How-to guide | Explanation | "To understand why this works, see [Explanation of X]" |
| Reference | How-to guides | "For usage examples, see [How to do Y]" |
| Explanation | How-to guides | "To put this into practice, see [How to do Y]" |
| Explanation | Reference | "For the technical specification, see the [X reference]" |
