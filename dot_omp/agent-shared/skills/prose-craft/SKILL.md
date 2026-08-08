---
name: prose-craft
description: "Prose voice, style, and editing craft for any writing or editing task — Zinsser clarity rules, AI-tell detection, rhetorical techniques, and the voice-first AI writing workflow. Use whenever drafting, editing, reviewing, or polishing any written content: blog posts, docs, ADRs, pitches, PRDs, Slack announcements, post-mortems, emails, READMEs, release notes, or any prose. Trigger aggressively — if the task involves words that a human will read, load this skill. Catches AI writing tells (em dashes, 'dive in', 'leverage', throat-clearing, corrective constructions), enforces active voice and concrete specificity, and supplies rhetorical techniques for manual polish passes."
---

# Prose Craft

Prose voice and editing craft. Load this alongside `writing-foundations` and the matched domain skill for every writing task. Writing-foundations covers *what* to say (quantify, modular primers, tradeoff analysis); prose-craft covers *how it sounds* (voice, clarity, rhythm, humanity).

**Attribution.** This skill vendors content from [Isaac Flath](https://isaacflath.com/)'s public agent-starter-wiki. Original sources:
- [writing-with-zinsser.md](https://github.com/Isaac-Flath/agent-starter-wiki/tree/main/writing-with-zinsser.md)
- [elements-of-eloquence.md](https://github.com/Isaac-Flath/agent-starter-wiki/tree/main/elements-of-eloquence.md)
- [cognitive-debt.md](https://github.com/Isaac-Flath/agent-starter-wiki/tree/main/cognitive-debt.md)
- [writing-style SKILL.md](https://github.com/Isaac-Flath/agent-starter-skills/tree/main/.claude/skills/writing-style/SKILL.md)

Vendored so the user can tweak freely. Content has been adapted, not mirrored verbatim.

---

## Core Rules (Zinsser)

Every sentence earns its place. Full details, before/after examples, and critique patterns in [references/zinsser-style.md](references/zinsser-style.md).

- **Strip every sentence to its cleanest components.**
- **No throat-clearing.** Start with the point. Cut "In this post...", "It's worth noting that..."
- **No nounism.** Prefer verbs to noun clusters. "Made a decision to implement" → "Decided to implement."
- **Active voice.** "The bug was fixed" → "The team fixed the bug."
- **Concrete over abstract.** Use specifics from source material. "Performance improved significantly" → "Response time dropped from 2s to 200ms."
- **Short sentences, short paragraphs.** One idea per sentence.
- **Strong verbs.** Replace is/was/has/make/do/get with specific verbs.
- **No dead constructions.** "There are three reasons" → "Three reasons."
- **No corrective constructions.** Don't say "It's not X, it's Y." Just say Y.
- **No em dashes.** They signal AI writing. Use periods, commas, or parentheses.
- **No weasel words.** "Some users reported" → give a number or cut.
- **Parallel structure.** Lists and comparisons match grammatically.

## AI Tells — Never Use

These phrases and patterns leak through AI-generated prose. Cut them on sight:

- "Let's dive in", "dive into", "let's explore", "let's break down"
- "Game-changer", "groundbreaking", "revolutionary", "cutting-edge"
- "Leverage" (use "use"), "utilize", "facilitate", "streamline"
- "In today's fast-paced world", "In the world of X"
- "Whether you're a beginner or expert"
- "Without further ado"
- "It's worth noting that", "Interestingly"
- Starting paragraphs with "So," or "Now,"
- Ending with "Happy coding!" or similar
- Em dashes as connective tissue
- Corrective constructions: "It's not X, it's Y"
- Sentences that wrap up sections too neatly

Full list with subtler smells in [references/zinsser-style.md](references/zinsser-style.md).

## Humanity

Write like you talk to a friend. If you wouldn't say it across a table, don't write it.

- **Small, casual words.** "Use" not "utilize." "Buy" not "purchase." "Help" not "facilitate." The short word is almost always the honest one.
- **Leave yourself in.** Observations that caught you off guard, details that stuck with you, things you found funny — they're what separate your writing from anyone else's.
- **Don't hedge before the content.** "AI writing can be a touchy topic" is apologizing before the reader reads. Cut it.
- **Say what you know.** If you are unsure, be plain about it or leave it out. No hype, no fake urgency.
- **Write tight, but write warm.** Every sentence delivers information — but the reader should hear a person.

## When to Use Each Reference

| If the task involves... | Load |
|---|---|
| Drafting, editing, reviewing any prose | [references/zinsser-style.md](references/zinsser-style.md) — full clarity rules, AI-tells, before/after examples, critique patterns |
| Manual polish pass, making a sentence land | [references/rhetorical-techniques.md](references/rhetorical-techniques.md) — 16 Forsyth techniques + applied examples |
| Starting a writing project from voice notes or raw thoughts | [references/ai-writing-workflow.md](references/ai-writing-workflow.md) — Flath's voice → draft → three-pass edit → polish pipeline |
| Writing about AI-assisted engineering, or explaining why careful writing matters with AI | [references/cognitive-debt.md](references/cognitive-debt.md) — the "shipping faster than you understand" mental model |

## Three-Pass Editing

When editing AI output or your own first draft, pass the text three times:

1. **Clarity pass.** Apply the Zinsser rules above. Strip clutter, kill AI tells, activate passive voice, replace weak verbs.
2. **Structure pass.** Use the critique patterns in [references/zinsser-style.md](references/zinsser-style.md) — check for double-stating, trailing off at interesting parts, list-architecture-without-narrative, repetitive narrative structure.
3. **Voice pass.** Read it aloud. Where a sentence feels flat but the content is right, consult [references/rhetorical-techniques.md](references/rhetorical-techniques.md). One technique per section is plenty.

"You don't use AI to become a better writer. You become a better writer, and then AI has something to work with." — Isaac Flath

## Critique Mode

When asked to review writing rather than produce it, use this checklist:

1. Scan for AI tells from the list above. Flag every instance.
2. Mark throat-clearing openings, dead constructions, nounism, corrective constructions.
3. Check each paragraph for double-stating and trailing off at interesting parts.
4. Check structure: can sections be reordered without loss? If yes, the piece is a catalogue, not a story.
5. Test technical details: does each fact explain a choice, tradeoff, or something surprising? If not, cut it.
6. Check the opening: does it start with the point, or warm up first?

Deliver the critique as specific edits with before/after pairs, not vague advice.
