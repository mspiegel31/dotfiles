---
name: software-forensics
description: >
  Active codebase investigation using software forensics tools — run onefetch, scc, git-fame,
  lizard, repomix, depcruise and related tools directly, interpret their output, and synthesize
  findings into a structured report. Load this skill for any of these requests: orienting to an
  unfamiliar repo ("give me the lay of the land", "brief me on this repo", "what does this
  codebase do"), finding risky files before a refactor ("investigate before we change X",
  "surface edge cases", "what should I know before touching the build system"), hotspot analysis
  ("why does this service keep breaking every sprint", "which files cause the most pain",
  "find the hotspots", "I want data not vibes", "what's causing our sprint pain"),
  contributor/ownership questions ("who owns this", "who are the top contributors", "what's our
  bus factor"), architecture mapping ("what's the module graph", "are there circular deps",
  "map the execution flow"), and LLM context packing ("pack this module for Claude", "what
  should I include when briefing an LLM on this codebase"). Also load when the user says
  "forensics", "churn analysis", "hotspot", "code-maat", "lizard complexity", or asks you to
  investigate a repo before making changes to it. Run the tools — do not just describe them.
---

# Software Forensics Skill

You are the analyst. When asked to understand a codebase, **run the tools yourself** using bash,
read their output, and synthesize what you find into a structured report. Don't describe what
the user could run — run it, interpret it, and tell them what it means.

The methodology is from Adam Tornhill's *Your Code as a Crime Scene*: git history is evidence,
complexity is risk, and the intersection of high churn and high complexity is where bugs live.

## Core principle: stop when you have the answer

Work through tiers in order. Each tier costs more time. Stop as soon as you can answer the
question — don't run Tier 3 forensics when Tier 1 orientation is all that's needed.

---

## How to run an investigation

### Step 1: Orient (always start here)

Run these from the repo root. Read every line of output — it's evidence.

```bash
onefetch                          # languages, top authors, LOC, commit cadence, pending changes
scc --by-file | head -40          # LOC + cyclomatic complexity per file, largest first
git-fame --sort=loc | head -20    # who owns surviving code (commit count lies; LOC doesn't)
git log --oneline -20             # recent history — active, dormant, or abandoned?
```

**Interpret and report:**
- What's the primary language and rough size?
- Which files are largest and most complex? (candidates for deeper investigation)
- Who are the top 2-3 authors by surviving LOC? Are they still on the team?
- Is the repo actively maintained or has it gone quiet?

### Step 2: Map dependencies (if architecture matters)

Run the appropriate tool for the language:

```bash
# JS/TS — check for circular dependencies (silent killers in older repos)
depcruise --output-type err-long --no-config src

# JS/TS — generate a dependency graph
madge --circular src/

# Python
pydeps <package_name> --max-bacon 3
```

**Interpret and report:**
- Are there circular dependencies? How many, and between which modules?
- What are the central hub modules that everything depends on?

### Step 3: Find hotspots (if refactoring or debugging)

A hotspot = high churn × high complexity. Complex files nobody touches are fine. Simple files
that change constantly are fine. The dangerous files are both — they accumulate bugs and slow
the team down.

```bash
# Find highest-churn files
git log --pretty=format: --name-only | grep -v '^$' | sort | uniq -c | sort -rn | head -20

# Check complexity of the top churners
lizard src/ -s cyclomatic_complexity | head -20
lizard <specific-file>             # per-function breakdown

# Find logical coupling — files that change together but aren't in the same module
git log --all --numstat --date=short --pretty=format:'--%h--%ad--%aN' \
  --no-renames > /tmp/git_log.txt
# (code-maat requires Java: alias code-maat='java -jar ~/tools/code-maat.jar')
code-maat -l /tmp/git_log.txt -c git2 -a coupling | head -20
```

**Interpret and report:**
- Which files are in the top-churn AND top-complexity lists? Those are the hotspots.
- Are any hotspots logically coupled — changing together despite living in different modules?
- Verdict for each hotspot: refactor now / watch / leave alone (see decision logic below)

**Hotspot decision logic:**
- High churn + high complexity + recent bugs → **refactor now**
- High churn + high complexity, no recent bugs → **refactor next quarter**
- High churn + low complexity → **monitor** (active but not dangerous)
- Low churn + any complexity → **leave alone** (risk without payoff)

### Step 4: Check ownership concentration (if bus factor matters)

```bash
hercules --burndown --burndown-people . | labours --burndown --burndown-people
```

A high Gini coefficient means knowledge is concentrated in one person. If that person is gone
or unavailable, the team is flying blind in that part of the codebase.

---

## What to produce

After running the investigation, synthesize findings into a structured report. Every report
must include a **Tools Used** section and a **Glossary** footer — these are non-negotiable
regardless of how short the report is.

```
## Codebase Investigation: [repo name]

### Overview
- Language(s), size (LOC), age, activity status
- Top contributors by surviving LOC; who's still reachable

### Architecture (if investigated)
- Module structure summary with file/line references (e.g. `src/auth/index.ts:1`)
- Any circular dependencies found
- Central hub modules
- Mermaid diagram of key execution flows or module dependencies when the question
  involves understanding how code paths work:

  ```mermaid
  flowchart TD
    A[entry.ts] --> B[validator.ts:validateMany]
    B --> C[catalog-model: EntityPolicies]
    B --> D[policies.ts:SpotOnPolicies]
  ```

### Hotspots (if investigated)
| File | Churn (commits) | Max CCN | Verdict |
|------|----------------|---------|---------|
| ...  | ...            | ...     | ...     |

### Key findings
- [Most important thing you found]
- [Second most important]
- [Third]

### Recommended next steps
- [Specific, actionable]

---

### Tools used
| Tool | Command run | What it found |
|------|-------------|---------------|
| onefetch | `onefetch` | [one-line summary] |
| scc | `scc --by-file \| head -40` | [one-line summary] |
| ... | ... | ... |

This section documents the methodology so findings can be reproduced or extended.

---

### Glossary
- **Churn**: Number of git commits that touched a file. High churn = frequently modified.
  A file touched in 50 commits has been a change target 50 times — each touch is a bug
  opportunity.
- **Cyclomatic complexity (CCN)**: Number of independent execution paths through a function.
  CCN 1 = one path (trivial). CCN 10 = ten paths (needs tests for each). CCN > 15 = lizard
  warning zone. CCN > 20 = hazard; bugs hide in untested branches.
- **Hotspot**: A file with both high churn AND high complexity. The intersection is where
  defects concentrate and where refactoring has the highest ROI.
- **Surviving LOC**: Lines of code still present in HEAD, attributed to the author who wrote
  them. More honest than commit count, which inflates for formatting/typo fixes.
- **Logical coupling**: Two files that frequently change in the same commit despite having
  no import relationship. Hidden dependency — a change to one silently requires a change
  to the other.
- **Bus factor**: Number of people who must be hit by a bus before a module becomes
  unmaintainable. Bus factor 1 = one person holds all the knowledge.
```

Tailor depth to the question. A "what is this?" question needs Overview only. A "where should
we refactor?" question needs Hotspots. Always include Tools Used and Glossary.

---

## Decision tree

```
"What is this repo?"                      → Step 1 only (onefetch + scc + git log)
"Who should I ask about X?"               → Step 1 (git-fame)
"What's the module graph?"                → Step 2 (depcruise / madge / pydeps)
"Any circular dependencies?"              → Step 2 (depcruise --output-type err-long)
"Where should we refactor?"               → Step 3 (churn + lizard)
"Why does this module keep breaking?"     → Step 3 (coupling analysis)
"What's our bus factor?"                  → Step 4 (hercules)
"Investigate before I make changes"       → Steps 1 + 3 (orient + hotspots)
```

---

## Custom analysis with PyDriller

When the pre-built tools don't ask exactly the right question, write a script:

```python
from pydriller import Repository

# Example: find commits touching two modules together (hidden coupling)
for commit in Repository('.').traverse_commits():
    files = [f.filename for f in commit.modified_files]
    if any('agent/' in f for f in files) and any('backend/' in f for f in files):
        print(commit.hash[:8], commit.msg[:60])
```

---

## Tool reference

| Tool | What it answers |
|------|-----------------|
| `onefetch` | Repo dashboard: languages, authors, LOC, churn |
| `scc --by-file` | LOC + complexity per file, COCOMO cost estimate |
| `git-fame --sort=loc` | Author attribution by surviving LOC (not commit count) |
| `depcruise` | JS/TS module graph + circular dependency detection |
| `madge --circular` | Lightweight JS/TS circular dep check |
| `pydeps` | Python module dependency graph |
| `lizard` | Cyclomatic complexity per function, any language |
| `hercules` | Burndown charts, Gini ownership coefficient |
| `pydriller` | Custom git analysis as Python scripts |
| `code-maat` | Logical coupling, knowledge loss (Java jar — `java -jar ~/tools/code-maat.jar`) |
| `git-of-theseus` | Code age cohort analysis ("was that refactor a rewrite?") |
| `git-dive` | Enhanced git blame with syntax highlighting |
| `repomix` | Pack repo into LLM context (`repomix --include "src/auth/**"`) |
| `code2prompt` | Token-counted LLM prompt builder |

---

## References

- Adam Tornhill, *Your Code as a Crime Scene, Second Edition* (Pragmatic Bookshelf, Feb 2024)
- Full tool status and install commands: `docs/software-forensics-toolkit.md` in this repo
