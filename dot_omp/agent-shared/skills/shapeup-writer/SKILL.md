---
name: shapeup-writer
description: "Shape Up pitch/bet authoring craft — section-by-section writing guidance for long-form technical program documents (problem statements with appetite, shaped solutions, scope cuts, risk assessment, work decomposition). Use when drafting, reviewing, or improving pitch/bet documents, PRDs, or any Shape Up-style technical planning document. Also use when analyzing examples of good/bad technical planning documents to extract writing principles."
---

# Shape Up Writer

Writing guidance for Shape Up pitch/bet documents — long-form technical program documents that evolve from pitch → feedback → bet through collaborative review.

This skill covers the **writing craft**: what makes a good document, section by section. For publishing mechanics (GDocs pipeline, scaffolding), see the `shapeup-publisher` skill in the project repo. For shared writing principles, load the `writing-foundations` skill.

---

## Core Principles

- **Shaped, not specified.** Define boundaries, not implementation paths. Say "climb this hill" not "take this exact route."
- **Appetite is a constraint, not an estimate.** The time budget shapes the solution. A 2-week appetite produces a fundamentally different design than 6 weeks.
- **Sell the problem, design the solution.** The problem section persuades leadership. The solution section shows the team what to build.
- **Concrete over abstract.** Specific stories, real numbers, named systems. "Users can't find their invoices" beats "discoverability challenges in the billing domain."
- **One document, evolving.** A pitch becomes a bet through feedback — it's the same document with increasing confidence, not a handoff between artifacts.

---

## Document Anatomy

A pitch document has these sections in order:

1. **Title** — `Pitch: <Descriptive Name>`
2. **Metadata table** — key-value pairs for tracking (partially dropdown-controlled)
3. **The Problem with Appetite** — why this work matters, scoped by time budget
4. **The Solution** — shaped but not over-specified
5. **No Gos** — explicit scope cuts
6. **Rabbit Holes/Risks/Dependencies** — unknowns and cross-team needs
   - Dependencies sub-table
   - Risks sub-table
   - Slices/Scopes sub-table

---

## Section Priority

Not all sections carry equal weight. When time is limited, invest in this order:

| Priority | Sections | Why |
|---|---|---|
| **Must nail** | Solution + Slices | Tightly coupled — these make or break the bet's evaluability. A great problem with a weak solution is a failed pitch. |
| **Must nail** | Rabbit Holes | Where you make hard calls in advance. Pre-decisions and guardrails that prevent the team from wasting weeks on the wrong path. Chronically underfilled in practice. |
| **Important** | Problem | Necessary but not sufficient. Authors already invest here naturally. |
| **Important** | No Gos | Protects the appetite. Quick to write well when you know the shape. |
| **Valuable when done well** | Finish Line | Quantitative success criteria (30-40% improvement, specific metrics live) are the gold standard. Qualitative ("we'll know it's done when it works") is low-value. |
| **Fill if you have them** | Risks table | Useful in theory, empty in practice across most docs. Real risk thinking happens in the Problem section and Rabbit Holes. Don't let an empty risks table block the pitch. |
| **Fill if you have them** | Dependencies table | Real dependency discussions happen in person before the pitch. The table is a record, not a discovery tool. |

**Key insight:** Rabbit Holes deliver more value than the Risks table. "Rabbit hole" means "place the team could get stuck" — authors immediately have things to say. "Risk" means "bad thing that might happen" — authors draw a blank. The useful risk content naturally lives in Rabbit Holes or the Problem section.

For detailed guidance on each section, see references/section-guidance.md

---

## General Writing Approach

- This is long-form technical writing. Think PRD, not bullet list.
- Be shaped but not over-specified — leave room for the implementing team to determine the "how."
- Every section has a purpose in the Shape Up methodology. Don't blend them.
- Ask clarifying questions when the user's input is too vague for a section, but don't over-interview — write a draft and let them react.

---

## Quality Checklist

Ordered by priority — nail the top items first.

| Check | Pass criteria |
|---|---|
| **Solution is shaped** | Boundaries clear, implementation path left to the team. Multi-component solutions use Purpose/How/Outcome per component. |
| **Slices are demonstrable** | Each can be completed and shown independently. Escalate or roll out logically. |
| **Rabbit holes are pre-decisions** | Each names a specific trap and says "do X, not Y." Never TBD in a final pitch. |
| **Problem is concrete** | Opens with a specific story/situation, not abstract analysis |
| **Appetite is explicit** | Time budget stated and justified as a deliberate constraint |
| **No-gos are surprising** | Each cut is something a reader might assume is included |
| **Finish line is quantitative** | Measurable success criteria, not "we'll know it's done when it works" |
| **Sells the problem** | Problem section persuades; solution section designs |
| **Stands alone** | A reader unfamiliar with prior context can follow the argument |
| **Within appetite** | Solution and slices feel achievable within the stated time budget |

For anti-patterns and examples, see references/examples.md
