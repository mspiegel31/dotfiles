# Gate interview

The interview settles every field of `BRIEF.md` before research or drafting. Decisions belong to the user; facts belong to the agent. Ask a round, wait, recompute the frontier, ask the next round. Stop when every field is settled.

## Before round 1

Look up, do not ask:

- Existing briefings: `python3 scripts/briefing.py list`. If one matches the subject, offer refresh instead of a new briefing.
- Type registry: list `references/type-*.md`; read each file's first line for the one-line summary to present as choices.
- `MISSION.md` in cwd (teach workspace): use it to pre-fill question and prior knowledge; still confirm.
- Toolchain: `check` has already run in Step 1.

## Round 1 (no prerequisites)

| # | Field | Question | Recommended default |
|---|-------|----------|---------------------|
| 1 | `question` | What single question must this briefing answer? Phrase it as a question the summary can answer in one paragraph. | The user's own words, sharpened into one sentence. |
| 2 | `decision` | What decision does the answer feed, if any? (Adopt / reject / configure / explain to someone / none.) | Infer from context; offer "none, understanding only" as a choice. |
| 3 | `audience` | Who reads it: `self`, `team`, or `internal` (SpotOn-only sources permitted)? | `self` (structured for team reuse). |
| 4 | `prior_knowledge` | What does the reader already know that the briefing must not re-explain? | List two or three adjacent concepts the agent believes the user knows; let them edit. |
| 5 | `type` | Which subject type? Present every `references/type-*.md` plus "none of these". | The best-matching type. |
| 6 | `location` | Home (`~/briefings/<slug>`) or here (`<cwd>/briefings/<slug>`)? | Home, unless cwd is a repository the subject lives in. |

## Round 2 (depends on `type`)

Read `references/type-<type>.md` first; its defaults populate the recommendations.

| # | Field | Question | Recommended default |
|---|-------|----------|---------------------|
| 7 | `diataxis` | Mode: `explanation`, `how-to`, `reference`, or `tutorial`. | The type's default mode. |
| 8 | `depth` | Reading time budget: `10 min`, `20 min`, `40 min`. | `20 min`. |
| 9 | `source_constraints` | Any sources that must be included or excluded? Minimum publication date? | None beyond the trust policy; date floor = current major version's release. |
| 10 | `worked_example` | Which concrete case does the worked example use? Offer two candidates in the type's required shape. | The candidate closest to the decision in field 2. |
| 11 | `interaction` | Is there a figure the reader should manipulate? If yes, what do they change and what do they observe? | `none` unless the type file names a natural island for this subject. |

## Round 3 (only if "none of these" was chosen for type)

Draft `references/type-<name>.md` from `assets/type.template.md`. Show it. Ask:

| # | Field | Question |
|---|-------|----------|
| 12 | new type | Does this type definition match? Edit the worked-example shape and primary-source rules before approving. |

Then run round 2 against the approved file.

## Prose fallback format

When no structured question tool exists:

```
❓ **Q1** - **<field>**: <question, with choices if any>

➡️ <recommended default>

---

❓ **Q2** - ...
```

## After the last round

1. Write `BRIEF.md` from `assets/BRIEF.md`.
2. Run `python3 scripts/briefing.py check <path>`.
3. Show `BRIEF.md`. Ask: "Go, or change something?" Wait.
