# Review rubric

Apply after drafting, before `render`. Hard-fail conditions block `status: reviewed`. Warnings are reported to the user and fixed when the depth budget allows. The same text is the LLM judge rubric in `eval.yaml`.

## Hard fail

1. **Uncited falsifiable claim.** Any number, version, behaviour, API shape, default, limit, or comparative statement ("faster than", "does not support") without `[@key]` in the sentence or the one before it. Scope: body prose, tables, and glossary definitions. The title, front matter, `.brief-meta` block, section headings, and glossary headwords are exempt; the claim they preview must be cited at its first appearance in prose.
2. **Unfetched source cited.** A `@key` in `index.qmd` with no matching row in `sources/LEDGER.md`. `audit` detects this deterministically.
3. **Slogan or marketing register.** See § Register.
4. **Island without interpretation.** A marimo cell not followed, before the next heading, by prose that names what the reader should observe when they change the input and what that observation means for the question.
5. **Print or keyboard breakage.** Content that disappears or overflows in print preview; any control unreachable by Tab; any information conveyed only by colour or hover.
6. **Formula instead of code.** A computation shown as inline or display math without a preceding executed code cell with sample data. Formulas are allowed only inside a collapsed callout after the code (`voice.md` § Math becomes code).
7. **Dense summary.** A summary that is one paragraph of six or more sentences, or that does not open with a one-sentence answer to the primary `question`. Required shape: answer sentence, then one cited bullet per supporting question (or at most three short paragraphs) (`voice.md` § Summary shape).

## Warn

8. **Concept without example.** In `guide` mode, a `##` section with more than three paragraphs and no code cell, diagram, or table before the next heading.
9. **Definition in the body.** In `guide` mode, a sentence of the form "X is …" defining a glossary-worthy term outside the glossary; move it and link the first use.
10. **Mixed Diátaxis modes.** In the opt-in pure modes only: how-to that pauses for theory longer than one paragraph; reference that argues.
11. **Incomplete worked example.** The example does not show the whole path required by `references/type-<type>.md`, or uses `...` / "and so on" / fragments where a full artefact is required.
12. **Glossary term used before defined.** Any term in the glossary appears in the summary or body before its glossary entry is linked.
13. **Depth overrun.** Rendered reading time exceeds `depth` by more than 25 %.
14. **Cell without readback.** A code example or diagram not followed by at least one sentence saying what the output showed.
15. **Supporting question unanswered.** A question listed in `BRIEF.md` § Supporting questions with no body section that answers it, or a body section that answers no listed question.

## Register

Banned outright:

- Headline fragments as prose: "Four roles. One evidence path." Two-word sentences chained for effect.
- Filler verbs and intensifiers: leverage, empower, unlock, seamless, powerful, robust, cutting-edge, game-changing, supercharge, effortless, elegant (of software), simply, just.
- Throat-clearing openers: "In today's world", "Let's dive in", "It's worth noting", "Imagine a world".
- Empty transitions as sentences or sentence openers: "Two consequences follow", "Reading it:", "For the decision:", "In other words", "Put differently", "With that in mind", "Now that we have", "Note that", "Importantly", "Interestingly", and any sentence whose only content is announcing the next sentence.
- Corrective constructions used for rhythm: "It's not X. It's Y." when X was never claimed.
- Card grids of three benefit statements. Emoji as section markers. Exclamation marks.
- Second-person exhortation: "you'll love", "you need to", "you'll want to". Instructional second person is allowed: "you pass the threshold with `--threshold`".
- Headings that name no concept: "Overview", "Details", "Deep dive", "Putting it together".

Required:

- Declarative sentences with a concrete subject and verb. "Bedrock rejects tool names longer than 64 characters [@bedrock-tooluse]" not "Tool naming can be tricky".
- Numbers with units and a source. Comparisons with a baseline.
- Hedges only where the source hedges; state the source's confidence, not the writer's.

## Scoring for the judge

Score 1.0 when no hard-fail condition is present and at most one warning. Subtract 0.4 for each hard fail (floor 0). Subtract 0.1 for each warning beyond the first. Report the first hard fail found in `details`.
