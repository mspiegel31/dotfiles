---
name: reflection-ritual
description: Guides a lightweight weekly journal-combing and monthly strategy review ritual on the personal-vault Obsidian vault. Use when the user wants to run, set up, or reschedule the weekly or monthly reflection review, comb through journal entries, sweep open loops into TickTick, update career and life strategy notes, or check in on the review habit. Don't use for one-off journal reads, single task creation, or vault structural changes.
---

# Reflection Ritual (weekly combing + monthly strategy review)

Operate on `/Users/mike/personal-vault/personal-vault` (alias: `personal-vault`). Follow the obsidian-rules skill: prefer the obsidian CLI for vault-aware work, use `vault=personal-vault` on every call, and never touch `.obsidian/`.

## Decide which run

- If the user asks for a **weekly review / combing** (or it's due), run the Weekly procedure.
- If it's the **first weekend of the month** or the user asks for a **strategy review**, run Weekly first, then the Monthly procedure.
- If the user only asks to **set up** the ritual, run the Setup procedure and stop.

## Weekly procedure (~20 min, guide the user step by step)

Work through the steps in order and keep each short. Ask the user questions one at a time; never batch the whole session into one message.

**Step 1: Rebuild the area logs.**
Run `python3 z_scripts/build-area-logs.py` in the vault root. It prints `updated: ...` or `updated: nothing`. If it errors, read the traceback and stop.

**Step 2: Find what changed since the last review.**
Run the skill's `scripts/changed-areas.py` (default: last 7 days; `--days N` to widen). It targets the personal-vault automatically and prints one line per area with new journal entries, newest first. If it prints `no area entries`, tell the user nothing was logged this week and skip to Step 4.

**Step 3: Comb the top 2–3 areas.**
For each area from Step 2, read the `## Log` section of `areas/<Area>.md` (or the listed journal notes directly via the `vault://personal-vault/` interface). Summarize in 2–3 bullets: patterns, surprises, and anything being avoided or dragged. Draft the summary for the user to confirm or correct.

**Step 4: Pattern check.**
Ask the user the three questions and record the answers in today's journal note under a `- [[Personal Improvement]]` bullet (create today's note from `z_templates/Daily.md` if missing):
- What repeated this week?
- What surprised you?
- What are you avoiding or dragging?

**Step 5: Open-loop sweep.**
Read `## Tasks that need Categorizing` sections across the journal dir (loops can sit in past notes — grep for the heading in `journal/`), plus stray `- [ ]` lines in notes changed this week. For each item, present the three verdicts and wait for the user's call:
- **Do** → create it in TickTick (Routing table in `references/ticktick-routing.md`).
- **Park** → tag `#tickler`; create it in TickTick project `tickler`.
- **Kill** → delete the journal line. State that closing the loop is the win.
Never create TickTick tasks without the user's per-item (or explicit batch) approval.

**Step 6: One next move.**
Ask which item is highest-leverage. WOOP it with the user: Wish → Outcome → inner Obstacle → Plan ("If [specific cue], then I will [small observable action]"). Then create the task in TickTick with the if–then in `--content` and a matching project/tag. Read `references/ticktick-routing.md` for the exact `ticktick task create` syntax and project IDs.

**Step 7: Check in the habit.**
Run `ticktick habit list` to find the `Weekly Review` habit and `ticktick habit checkin <habitId>`. If the habit doesn't exist yet, create it (Setup) and check in.

## Monthly procedure (~60 min, after the weekly steps)

**Step 1: Trajectory check.**
Read `projects/Road to 40.md` and every active project (frontmatter `status: active`). Have the user write 3 sentences: "If I keep this exact trajectory, here's who I am in 12 months." If the answer isn't compelling, propose a concrete edit to the stale part of the plan.

**Step 2: Career strategy update.**
Read the month's `Career` and `Work` area logs. Ask: where did energy actually go, and where did recognition show up? Cross-check `projects/Brag Doc 2026.md`. Propose updates to `projects/Job Hunt 2026-2027.md` or the `Next Job Dream List` (in the `vault` vault at `vault://vault/Next Job Dream List.md`) if the month's evidence contradicts them.

**Step 3: Decision journal (only for live decisions).**
If a real decision is open, write an entry into the relevant project note: options considered, assumptions, confidence %, predicted outcome, review date. Explain that writing the forecast before the outcome is what protects against hindsight bias (Fischhoff 1975) and the planning fallacy (Buehler et al. 1994).

**Step 4: Feed the Brag Doc.**
Propose 1–5 entries from the month's work evidence into `projects/Brag Doc 2026.md`, following its existing `## Month` + bulleted-link format.

**Step 5: 30/60/90 scan.**
For the top 2–3 active projects, ask: when did comparable efforts actually finish? Adjust due dates in TickTick (`ticktick task update <taskId> --due-date ...`) rather than trusting the inside estimate.

**Step 6: Offload batch + prune.**
Create the remaining approved tasks, complete stale ones (`ticktick task complete <projectId> <taskId>`), and prune `#tickler` items the user confirms are dead.

## Setup procedure (one-time)

1. Verify `python3 z_scripts/build-area-logs.py` runs from the vault root.
2. Run `ticktick project list` and diff the IDs against `references/ticktick-routing.md`. If any drifted, update the reference file.
3. If `ticktick habit list` has no `Weekly Review` habit, create it: `ticktick habit create --name "Weekly Review" --goal 1 --repeat "FREQ=WEEKLY;BYDAY=SUN"` (suggest a day; let the user pick). Same for a `Monthly Strategy Review` habit on `FREQ=MONTHLY` if the user wants one.
4. Read `references/resources.md` (the linked research & concepts page) if the user asks why any step exists or wants to go deeper.

## Error handling

- `build-area-logs.py` or `changed-areas.py` fails → show the error, do not hand-edit area notes to work around it; fix or report.
- `ticktick` calls fail with an auth error → run `ticktick auth` and retry once; if still failing, finish the review in the vault and list the pending TickTick ops at the end.
- TickTick project ID not found → re-run `ticktick project list` and re-resolve the name, never guess an ID.
- A journal area has zero entries this month → say so and skip; do not manufacture content.
