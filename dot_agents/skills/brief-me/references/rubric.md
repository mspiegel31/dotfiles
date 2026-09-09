# Review rubric

Apply after drafting, before `render`. Hard-fail conditions block `status: reviewed`. Warnings are reported to the user and fixed when the depth budget allows. The same text is the LLM judge rubric in `eval.yaml`.

## Hard fail

1. **Uncited falsifiable claim.** Any number, version, behaviour, API shape, default, limit, or comparative statement ("faster than", "does not support") without `[@key]` in the sentence or the one before it. Scope: body prose, tables, and glossary definitions. The title, front matter, `.brief-meta` block, section headings, and glossary headwords are exempt; the claim they preview must be cited at its first appearance in prose.
2. **Unfetched source cited.** A `@key` in `index.qmd` with no matching row in `sources/LEDGER.md`. `audit` detects this deterministically.
3. **Slogan or marketing register.** See § Register.
4. **Island without interpretation.** A marimo cell not followed, before the next heading, by prose that names what the reader should observe when they change the input and what that observation means for the question.
5. **Print or keyboard breakage.** Content that disappears or overflows in print preview; any control unreachable by Tab; any information conveyed only by colour or hover.

## Warn

6. **Mixed Diátaxis modes.** Explanation that gives numbered steps; how-to that pauses for theory longer than one paragraph; reference that argues.
7. **Incomplete worked example.** The example does not show the whole path required by `references/type-<type>.md`, or uses `...` / "and so on" / fragments where a full artefact is required.
8. **Summary does not answer the question.** The first section must answer `question` from `BRIEF.md` in one paragraph without a forward reference.
9. **Glossary term used before defined.** Any term in the glossary appears in the summary or body before its glossary entry is linked.
10. **Depth overrun.** Rendered reading time exceeds `depth` by more than 25 %.

## Register

Banned outright:

- Headline fragments as prose: "Four roles. One evidence path." Two-word sentences chained for effect.
- Filler verbs and intensifiers: leverage, empower, unlock, seamless, powerful, robust, cutting-edge, game-changing, supercharge, effortless, elegant (of software), simply, just.
- Throat-clearing openers: "In today's world", "Let's dive in", "It's worth noting", "Imagine a world".
- Corrective constructions used for rhythm: "It's not X. It's Y." when X was never claimed.
- Card grids of three benefit statements. Emoji as section markers. Exclamation marks.
- Second-person exhortation: "you'll love", "you need to".

Required:

- Declarative sentences with a concrete subject and verb. "Bedrock rejects tool names longer than 64 characters [@bedrock-tooluse]" not "Tool naming can be tricky".
- Numbers with units and a source. Comparisons with a baseline.
- Hedges only where the source hedges; state the source's confidence, not the writer's.

## Scoring for the judge

Score 1.0 when no hard-fail condition is present and at most one warning. Subtract 0.4 for each hard fail (floor 0). Subtract 0.1 for each warning beyond the first. Report the first hard fail found in `details`.
