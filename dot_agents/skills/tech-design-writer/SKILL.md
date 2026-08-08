---
name: tech-design-writer
description: "Technical design document (PRD) authoring — modular background primers, constraint-driven design, alternatives analysis, transition planning, and pedagogical diagram sequences. Use when writing, reviewing, or improving technical design documents, PRDs, RFCs, or any document that proposes an architectural decision with full context and tradeoff analysis. Also use when the user needs to create architecture diagrams for a design doc."
---

# Tech Design Writer

Writing guidance for technical design documents (PRDs, RFCs, design docs). These are detailed proposals that align engineers on *how* to build something — the full landscape, constraints, design with tradeoffs, and implementation path. For shared writing principles, load the `writing-foundations` skill.

**How this differs from other writing skills:**
- **`shapeup-writer`** — persuades leadership to *invest time* (problem + appetite + shaped solution)
- **`tech-design-writer`** — aligns engineers on *how to build it* (background + constraints + design + alternatives)
- **`adr-writer`** — records a *single decision* (context + decision + consequences)

A bet doc often spawns a tech design doc. An ADR often captures one decision from a tech design doc.

---

## Document Anatomy

A tech design doc has these sections in order:

1. **Executive Summary** — the entire proposal in one paragraph
2. **Background** — modular primers that educate readers who lack context
3. **Constraints & Requirements** — what limits the solution space
4. **Proposed Design** — the architecture with analysis
5. **Alternatives Considered** — what else was evaluated and why it was rejected
6. **Transition Plan** — how to get from here to there
7. **FAQ** — edge cases and anticipated questions
8. **Design Approvals** — reviewer sign-off table

Not every doc needs every section. Background can be minimal if the audience is expert. FAQ can be skipped if there are no edge cases. But Constraints → Design → Alternatives is the core — never skip those.

---

## Section Priority

| Priority | Sections | Why |
|---|---|---|
| **Must nail** | Constraints & Requirements | These shape everything. If the constraints are wrong, the design is wrong. |
| **Must nail** | Proposed Design + Alternatives | The core argument: here's what we'll do and why not the other options. |
| **Important** | Executive Summary | The only section some stakeholders will read. Must stand alone. |
| **Important** | Transition Plan | Tells the team "this is achievable" — without it, the design feels theoretical. |
| **Valuable** | Background | Educates newcomers without slowing down experts. Modular and skippable. |
| **Valuable** | FAQ | Preempts reviewer questions. Keeps the main flow clean. |
| **Procedural** | Design Approvals | Tracking mechanism, not a writing challenge. |

→ **Section-by-section writing guidance:** `references/section-guidance.md`

→ **Diagrams (pedagogical sequences, tool selection, quality checklist):** `references/diagrams.md`

---

## Quality Checklist

Ordered by priority.

| Check | Pass criteria |
|---|---|
| **Constraints drive the design** | A reader can trace: constraint → requirement → design decision |
| **Design has a concrete example** | At least one domain/service walked through end-to-end |
| **Alternatives have specific rejections** | Every rejected option has a concrete reason, not "didn't fit" |
| **Cons have mitigations** | Every trade-off paired with how you'll handle it, quantified where possible |
| **Executive summary stands alone** | A reader who reads only this paragraph understands what and why |
| **Background is skippable** | Modular primers with clear sub-headings. Expert readers can jump to Design. |
| **Transition plan is concrete** | Migration unit named, blockers identified, developer impact clarified |
| **Diagrams encode decisions** | Architecture diagram makes the key choice visually obvious |
| **FAQ preempts real questions** | Edge cases and growth scenarios addressed before reviewers ask |
