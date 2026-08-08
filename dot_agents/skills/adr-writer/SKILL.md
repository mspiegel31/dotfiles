---
name: adr-writer
description: Architecture Decision Record (ADR) authoring — rigid format, section-specific sub-prompts, and quality criteria. Load when writing, reviewing, or updating ADRs.
mcp:
  mermaid:
    command: npx
    args: [-y, "@peng-shawn/mermaid-mcp-server"]
    env:
      CONTENT_IMAGE_SUPPORTED: "false"
  excalidraw:
    command: uvx
    args: [maaker-excalidraw-mcp]
---

# ADR Writer

Use this skill when authoring a new ADR, reviewing an existing one, or updating the status/consequences of a previous decision.

---

## Format Conventions

- **Tooling**: ADRs are scaffolded with [adr-tools](https://github.com/npryce/adr-tools) (`adr new "Title"`, or `adr new -s N "Title"` to supersede)
- **Filename**: `NNNN-kebab-case-title.md` (zero-padded 4-digit number, e.g., `0042-aws-argo-domain-based-hubs.md`)
- **Numbering**: Sequential, never reused
- **Location**: `docs/operations/Architecture-Decision-Records/`
- **Images**: `docs/operations/Architecture-Decision-Records/images/<topic>/NNNN-description.png`
- **Frontmatter**: YAML with `short_title: "NN. Kebab-Or-Short-Title"`
- **Records are never deleted** — mark as superseded or deprecated

---

## Required Structure

Every ADR follows this exact section order:

```markdown
---
short_title: NN. short-title
---
# YYYY-MM-DD: Descriptive Title

## Status

Proposed | Accepted | Deprecated | Superseded by [ADR NNNN](./NNNN-title.md)

## Context

[Forces, constraints, and the situation that necessitates this decision]

## Decision

[Declarative statement of what we will do, followed by implementation details]

## Alternatives Considered

[Each alternative evaluated, with specific reasoning for rejection]

## Consequences

[Balanced assessment — what becomes easier, what becomes harder, risks to mitigate]
```

---

## Section Sub-Prompts

→ See [`references/section-sub-prompts.md`](references/section-sub-prompts.md) for per-section writing techniques and sub-prompt questions (Context, Decision, Alternatives Considered, Consequences).

---

## Decision Chains

→ See [`references/decision-chains.md`](references/decision-chains.md) for how to link ADRs into chains and handle supersession.

---

## Quality Checklist

| Check | Pass criteria |
|---|---|
| **Decision is declarative** | "We will..." not "We should consider..." or "We'd like to..." |
| **Context is forces, not narrative** | Labeled constraints and criteria, not meeting minutes |
| **Constraints are bold-labeled** | Each force/constraint has a **bold label** for scannability |
| **Consequences are balanced** | Downsides listed honestly, each with an explicit mitigation |
| **Consequences are quantified** | Costs, limits, and thresholds use specific numbers where available |
| **Stands alone** | A reader unfamiliar with the discussion can follow the reasoning |
| **One decision per ADR** | Multiple decisions → multiple ADRs linked in a chain |
| **Alternatives have specific rejections** | "Rejected because [specific reason]" not "didn't fit our needs" |
| **Prior decisions are linked** | Related ADRs are referenced with relative markdown links |
| **Status is current** | Superseded ADRs link to their replacement |
| **Diagram included** | Architecture/topology decisions have a visual diagram |

---

## Diagrams in ADRs

→ See [`references/diagrams.md`](references/diagrams.md) for format selection (Mermaid vs Excalidraw vs plaintext trees), MCP protocols, and file conventions.
