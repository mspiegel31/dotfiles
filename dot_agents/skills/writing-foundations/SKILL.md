---
name: writing-foundations
description: "Shared writing principles across all technical writing skills — quantification, concrete specificity, modular background primers, structured analysis, and the writing lifecycle. Loaded automatically by domain-specific writing skills (shapeup-writer, tech-design-writer, slack-writer, adr-writer). Also use standalone when reviewing any technical document for general quality."
---

# Writing Foundations

Shared principles that apply across all technical writing at SpotOn. Domain-specific skills (shapeup-writer, tech-design-writer, etc.) reference this for the common patterns.

---

## The Writing Lifecycle

Each skill maps to a phase. When you're unsure which skill to load, find where you are:

```
Shape → Design → Decide → Build → Document → Announce → Learn
```

| Phase | Skill | Trigger |
|---|---|---|
| **Shape** | `shapeup-writer` | Pitching a bet, writing a pitch doc, shaping work |
| **Design** | `tech-design-writer` | PRD, RFC, technical design doc with alternatives |
| **Decide** | `adr-writer` | Recording one specific architectural decision |
| **Build** | — | (implementation, not writing) |
| **Document** | `diataxis-writer` | Tutorials, how-tos, reference docs, explanations |
| **Announce** | `slack-writer` | Internal marketing, "what we shipped" posts |
| **Learn** | `aar-writer` | Post-incident analysis, retrospectives, after-action reports |
| **Publish** | `shapeup-publisher` | Push local markdown to Google Docs |

---

## Shared Principles

### Quantify everything

Numbers beat adjectives in every document type. This applies universally:

| Instead of | Write |
|---|---|
| "significant improvement" | "30-40% reduction in troubleshooting time" |
| "many applications affected" | "55 of 240 applications had measurable downtime" |
| "small cost overhead" | "$20/month per Hub" |
| "took a long time" | "64 minutes to first restoration, 322 minutes total" |

### Concrete over abstract

Specific stories, real systems, named people. Every document type benefits from grounding:

- **Bet docs:** "When a POS loses Wi-Fi, it creates a finger-pointing scenario" not "connectivity challenges exist"
- **PRDs:** "For the Payments domain, the Hub is hosted in payments-prod-c1" not "each domain gets a Hub"
- **AARs:** "At 10:02 CST, an engineer noticed a service account token out of sync" not "an issue was discovered"
- **Slack posts:** "Deployments are now 3x faster" not "we improved the deployment experience"

### Modular background primers

When your audience has mixed expertise, write background as numbered, skippable subsections. Each primer:
- Opens with a one-sentence definition
- Stands alone (a reader can skip sections they know)
- Includes concrete data (counts, costs, names)
- Signals skippability: "Feel free to skip content you're already familiar with"

This pattern appears in PRDs (technology primers) and AARs (system background).

### Structured tradeoff analysis

When comparing options, use a consistent format:
- **Every con gets a mitigation.** Not just "this is a downside" but "here's how we handle it."
- **Quantify the mitigation.** "$20/month is acceptable because..." not "the cost is manageable."
- **Specific rejection reasons.** "Rejected because it exceeds the 1,000 app limit" not "didn't fit our needs."

This pattern appears in PRDs (Pros/Cons), ADRs (Alternatives Considered), and bet docs (Risks).

### Blameless specificity

For retrospective writing (AARs, post-mortems): be specific about what happened without assigning individual blame. Name the systemic cause, not the person.

- ✅ "Due to confusion with an ambiguous CodeFresh UI, the operation was applied to the entire runtime"
- ❌ "[Person] accidentally deleted everything"

The goal is to fix the system, not to punish the individual.

### Action items over reflections

Learnings and improvements should be commitments, not observations:

- ✅ "We'll automatically enforce the preserveResourcesOnDeletion flag on all production applications"
- ❌ "We should be more careful about production changes"

Tag uncommitted ideas explicitly: "(Idea) Smaller blast radius for deployments" — this distinguishes "we will do this" from "worth considering."
