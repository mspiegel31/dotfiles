---
name: context-engineering
description: Best practices for crafting AGENTS.md files and skills — covers what belongs where, the density tax, static vs. dynamic context, and how to evolve agent config over time. Also covers the skills → agent promotion pattern for scaling domain expertise.
---

# Context Engineering Best Practices

## The Core Principle

AGENTS.md is a **constitution**, not a manual. Every line is loaded on every session — even ones that have nothing to do with that content. Write accordingly.

---

## AGENTS.md vs Skills — What Goes Where

| Put it in AGENTS.md                                                    | Put it in a skill                                    |
| ---------------------------------------------------------------------- | ---------------------------------------------------- |
| Conventions always relevant (code style, commit format)                | Multi-step workflows with a clear trigger            |
| Error prevention rules (things the agent repeatedly gets wrong)        | Reference material only needed for specific tasks    |
| Pointers to skills so the agent knows they exist                       | Detailed procedures, tables, command sequences       |
| Cross-tool conventions (applies to Cursor, Claude Code, Augment, etc.) | Content too detailed to justify passive context cost |

**Rule of thumb:** If you'd only need this information 10% of sessions, it belongs in a skill with a one-line pointer in AGENTS.md.

---

## The Density Tax

Every line in AGENTS.md is paid on every session. Research (arxiv.org/pdf/2602.11988) confirms:

> _"All context files consistently increase the number of steps required to complete tasks."_

The tax is real. Developer-written, concise files outperform bloated or AI-generated ones. Prefer:

- Decision tables over prose explanations
- Pointers over embedded documentation
- Conventions over tutorials

For detailed guidance on density tax research, static vs. dynamic context, AGENTS.md structuring, and the evolve-from-failures workflow, see [references/density-tax.md](references/density-tax.md).

---

## Skill Design

- **Frontmatter required:** `name` and `description` fields (used for discovery)
- **Description quality matters:** Write it as a trigger condition, not a title.
- **Skills are how-to guides** — procedural, task-oriented, with embedded reference tables.
- **Composability:** Skills can reference other skills. Extract reusable procedures into their own skill.
- **Cite sources for empirical claims:** Include a `## Sources` section when referencing research or eval data.

For detailed guidance on skill anatomy, trigger optimization, scaling skills → agents, and custom agent design, see [references/skill-design.md](references/skill-design.md).

---

## File Locations

| File                                        | Loaded by                     | Scope   |
| ------------------------------------------- | ----------------------------- | ------- |
| `AGENTS.md` (repo root)                     | All agents, every session     | Project |
| `.claude/skills/<name>/SKILL.md`            | OpenCode, on-demand           | Project |
| `.claude/commands/<name>.md`                | Claude Code `/project:<name>` | Project |
| `~/.config/opencode/AGENTS.md`              | OpenCode, every session       | Global  |
| `~/.config/opencode/skills/<name>/SKILL.md` | OpenCode, on-demand           | Global  |

For full sources and citations, see [references/file-locations.md](references/file-locations.md).
