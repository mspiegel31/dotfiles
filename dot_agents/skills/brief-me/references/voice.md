# Voice and pacing

The model is the scikit-learn User Guide (https://scikit-learn.org/stable/user_guide.html): professional but informal, low-key, one concept at a time, a runnable example after each concept. Read this file before drafting. When the harness has `prose-craft`, `writing-foundations`, or `diataxis-writer` skills, read them too; this file wins on conflict.

## Default mode: `guide`

A `guide` briefing is explanation prose that stops for a concrete example after each concept. Definitions live in the glossary, not in the body. `how-to` and `reference` remain available as opt-in modes when `BRIEF.md` names them; the pure-mode rules in `rubric.md` then apply.

## Section shape

Each `##` section answers one supporting question from `BRIEF.md`, in the map's order. The heading names the concept the answer turns on, not the question verbatim. Inside a section:

1. One or two short paragraphs (each 2–4 sentences) saying what the concept is and why the reader meets it here.
2. An example: code with sample data, a diagram, or a table. Never a paragraph that restates the prose.
3. One or two sentences reading the example back: what it showed.

A section longer than three paragraphs before its example is two concepts; split it.

## Summary shape

The summary is the answer, not an abstract:

- First line: the answer to the primary `question` in one plain sentence.
- Then one bullet per supporting question in `BRIEF.md`, each a one-sentence answer with its citation. Bullets appear in the order of the body sections that expand them.
- Or, when bullets fight the content, at most three paragraphs of 2–3 sentences each.

Never one paragraph of six or more sentences. For `architecture` briefings the summary also carries the request-path diagram (see `marimo-island.md` § Diagrams).

## Math becomes code

The reader thinks in code, not notation. Any computation is shown as Python with sample data and its printed output. The code is a `{.marimo echo="true"}` cell with no widgets, ending in an expression, so Quarto runs it at render and the output under it is real (see `marimo-island.md` § Code examples). Use numpy or pandas when the shape of the data helps; plain Python when it does not.

````markdown
```python {.marimo echo="true"}
rewards = [0.9, 0.6, 0.4, 1.0, 0.45]
successes = sum(r >= 0.5 for r in rewards)
{"successes": successes, "pass_rate": round(sum(rewards) / len(rewards), 2)}
```
````

The canonical formula, when one exists, goes after the code in a collapsed callout, never inline:

```markdown
::: {.callout-note collapse="true" title="Formula"}
$\text{pass@}k = 1 - \binom{n-c}{k} / \binom{n}{k}$ [@chen2021]
:::
```

## Headings

Noun phrases or short questions that name the concept: "How a rule is chosen", "Pass Rate is mean reward". Not "Overview", "Details", "Deep dive", "Putting it together".

## Sentences

- Declarative, concrete subject and verb. One idea per sentence.
- Instructional second person is fine: "you pass the threshold with `--threshold`". Exhortation is not: "you'll want to", "you need to understand".
- Transitions carry a claim or are deleted. Banned as sentence openers or standalone sentences: "Two consequences follow", "Reading it:", "For the decision:", "In other words", "Put differently", "With that in mind", "Now that we have", "This matters because" (state why instead), "Note that", "Importantly", "Interestingly".
- Cut any sentence that announces the next sentence.

## Language level

Assume the `prior_knowledge` list in `BRIEF.md` and nothing more. Introduce every other term at first use with a glossary link. Prefer the vendor's word for a thing over a synonym.
