---
name: brief-me
description: Produces an evidence-backed technical briefing (Quarto HTML, optional marimo island) on a tool, technology, agentic technique, system architecture, or any other subject the user needs to understand before a decision. Runs a hard-gated interview that writes BRIEF.md before any research or drafting, cites every falsifiable claim to a fetched primary source, and self-reviews against a rubric. Use when the user asks for a briefing on a subject, asks to be briefed before deciding, or asks to refresh or publish an existing briefing. Don't use for runbooks, ADRs, PRDs, slide decks, or multi-session tutoring (use runbook-writer, adr-writer, tech-design-writer, or teach).
---

# Brief Me

A briefing answers one question for one reader, with every claim traceable to a source the agent actually read. The reader is the user first; the document is structured so it can be shared to colleagues unchanged.

Terminology used throughout: **briefing** (one directory: `index.qmd`, `BRIEF.md`, `references.bib`, `sources/LEDGER.md`), **home** (the Quarto website project that holds briefings, default `~/briefings`), **type** (a subject kind in `references/type-*.md`), **cell** (an executed marimo code block: a code example, a diagram, or an island with controls), **ledger** (the record of every URL fetched).

## Procedures

**Step 1: Check prerequisites**
1. Run `python3 scripts/briefing.py check`. It verifies Quarto >= 1.9.20 and uv, reports the PATH marimo version (cells render in a uv sandbox pinned to marimo >= 0.23.16 by the document front matter, so the PATH version is informational), and reports whether the home exists and has the quarto-marimo extension.
2. If it exits non-zero for a missing tool, stop. Report the missing prerequisite and the install hint it printed. Do not fall back to Markdown or hand-built HTML.
3. If it reports a missing home or extension, continue; `init` creates both.

**Step 2: Locate the home**
1. Default home is `~/briefings` (override with `--home PATH` or `BRIEFINGS_HOME`). A briefing may instead live at `<cwd>/briefings/<slug>/` via `init --here`.
2. Always confirm the location with the user in the first interview round. Never assume.
3. If the current directory contains `MISSION.md` (a `teach` workspace), read it before the interview and use it to ground the question and prior-knowledge fields.

**Step 3: Run the gate interview**
1. Read `references/interview.md` for the question set, recommended defaults, and frontier order. Draft the question map (primary question plus 3–6 supporting questions) from the user's request before round 1 and present it as the editable default; the map, not the single question, is what the briefing must answer.
2. Use the harness's structured question tool when one exists (OMP `ask`, Claude Code `AskUserQuestion`). Give every question a recommended default. When no structured tool exists, ask numbered prose rounds in the format from `references/interview.md`.
3. Ask only questions whose prerequisites are settled. Round 1 has no prerequisites; round 2 depends on the type chosen in round 1.
4. When the user picks "none of these" for type, draft a new `references/type-<name>.md` from `assets/type.template.md`, show it, and get approval before continuing.
5. Look up facts yourself (toolchain, existing briefings via `list`, repository state). Only decisions go to the user.

**Step 4: Write BRIEF.md and wait**
1. Run `python3 scripts/briefing.py init <slug> [--here] --type <type> --title "<title>"`. It creates the home if absent, installs the quarto-marimo extension, and scaffolds the briefing from `assets/`.
2. Fill `BRIEF.md` from the interview answers. Mirror `title`, `type`, `diataxis`, `audience`, `question`, `decision` into the `index.qmd` front matter.
3. Run `python3 scripts/briefing.py check <path>`. It fails on any empty or `TODO` field.
4. Show `BRIEF.md` to the user. Do not research or draft until the user says go. Fields the user marked "you choose" may be defaulted; all others are the user's decisions.

**Step 5: Research**
1. Read `references/type-<type>.md` for what counts as a primary source for this type.
2. Fetch primary and vendor sources first. Community sources are allowed only for opinion or experience and must be labelled as such in prose.
3. Internal sources (Confluence, infra-docs, internal repositories) are allowed only when `BRIEF.md` has `audience: internal`; the rendered document then carries a visible "Internal" badge from the template.
4. Record every URL actually fetched in `sources/LEDGER.md` as one row: key, URL, fetch date, one-line summary. A URL that was not fetched may not be cited.
5. Write a BibTeX entry in `references.bib` for each ledger row, using the same key. Cite in prose with `[@key]`.
6. Never rely on parametric knowledge for a falsifiable claim (a number, a behaviour, an API shape, a version constraint). Find the source or drop the claim.

**Step 6: Draft index.qmd**
1. Read `references/voice.md` first. If the harness exposes `prose-craft`, `writing-foundations`, or `diataxis-writer` skills, read them too; `voice.md` wins on conflict. Read `references/rubric.md` § Register.
2. Follow the section order in `assets/index.qmd`: summary, glossary, one `##` per supporting question in `BRIEF.md`, worked example, limitations, next action, references. The summary is a one-sentence answer to the primary question followed by one cited bullet per supporting question, never one dense paragraph.
3. Default mode is `guide` (`voice.md` § Default mode): each concept section is one or two short paragraphs, then an example (code cell, diagram, or table), then a one-sentence readback. Definitions go in the glossary, not the body. In the opt-in pure modes, stay in the mode: how-to does not stop for theory; reference does not persuade.
4. Show every computation as an executed code cell with literal sample data (`references/marimo-island.md` § Code examples); a formula appears only in a collapsed callout after the code. Keep `engine: marimo` in the front matter whenever the document has any cell.
5. For `architecture` briefings, the summary carries a request-path diagram driven by one control (`marimo-island.md` § Diagrams) unless `BRIEF.md` § Interaction says static.
6. Add an island with controls only for a figure the reader benefits from manipulating. Every island is immediately followed by prose stating what to change, what to observe, and what it means.
7. Build the worked example in the shape required by `references/type-<type>.md`. Show the whole path, not fragments.
8. Every falsifiable claim carries `[@key]`.

**Step 7: Review and render**
1. Run `python3 scripts/briefing.py audit <path>`. It cross-checks `@key` citations, `references.bib`, and the ledger, and exits non-zero on any orphan. Fix every finding.
2. Read `references/rubric.md` and review the draft against every hard-fail condition. Fix, do not annotate.
3. Run `python3 scripts/briefing.py render <path> --open`. Inspect the rendered page: print layout (`Cmd+P` preview) and keyboard reachability of every control.
4. Set `status: reviewed` in front matter and `BRIEF.md`.
5. If a `teach` workspace is present, offer to file a learning record capturing the decision the briefing informed.

**Step 8: Refresh an existing briefing (on request)**
1. Run `python3 scripts/briefing.py refresh <path>`. It prints every ledger row with its fetch date and the `index.qmd` lines citing it.
2. Re-fetch each URL. For each cited claim, confirm it still holds; propose edits for any that changed.
3. Skip the interview unless the user says the question changed. Run `python3 scripts/briefing.py audit <path>`, then `refresh <path> --touch "<what changed>"` to bump `updated:` and append the note to `BRIEF.md` § History.

**Step 9: Publish (on request)**
1. Run `python3 scripts/briefing.py publish <path> --to <quarto-site-repo>`. It copies the briefing into `<repo>/briefings/<slug>/` and warns if the target lacks the quarto-marimo extension or a `briefings/` listing.
2. Do not open a pull request from this skill; hand the working tree to the user or `git-master`.

## Extending the type registry

Each type is one file `references/type-<name>.md` with four sections: default Diátaxis mode, worked-example shape, what counts as primary, and known traps. Copy `assets/type.template.md`. The interview reads the registry at runtime, so adding a file adds a choice.

## Error Handling
* `check` reports a missing tool: stop and report; the skill does not degrade.
* `check` reports quarto-marimo missing in the home: run `quarto add marimo-team/quarto-marimo --no-prompt` inside the home, then re-run.
* `audit` reports a cited key with no ledger row: the source was not fetched. Fetch it and add the row, or remove the citation and the claim.
* `render` fails inside a marimo cell: extract the cell body to a scratch `.py`, run it with `uv run --with 'marimo>=0.23.16' python scratch.py`, fix, paste back. When the cell imports third-party packages, check browser availability with `uv run --with 'marimo>=0.23.16' marimo check --select MW scratch.py`.
* The user answers the interview with facts the agent should have looked up: look them up now, restate the question with the fact settled.
* Draft exceeds the reader's stated depth: cut body sections before cutting the worked example or limitations.
