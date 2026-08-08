---
name: ideation-capture
description: Capture, organize, and review work ideation from LLM conversations using Karakeep. Use this skill whenever the user wants to save an idea, plan, architecture concept, or research finding for later — especially after brainstorming, research, or design discussions. Also use when the user asks to review their ideation backlog, check for duplicate ideas, or survey existing plans. Triggers on phrases like "save this idea", "bookmark this", "add to ideation", "review my ideas", "have I thought about this before", "what ideas do I have about X", or any request to persist a conversation's insights to Karakeep.
---

# Ideation Capture

Capture work ideation from LLM conversations into Karakeep with a consistent format, deduplication, and periodic review.

## Three Modes

This skill operates in three modes. Read the conversation to determine which mode the user needs.

| Mode | When | User says something like |
|---|---|---|
| **Capture** | After brainstorming/research — user wants to save the idea | "save this", "bookmark this idea", "add to ideation" |
| **Survey** | Before or during a new idea — check what already exists | "have I thought about this before", "what ideas do I have about X" |
| **Review** | Periodic backlog grooming | "review my ideas", "what's in my ideation backlog" |

---

## Mode: Capture

Persist a conversation's insights as a structured Karakeep bookmark.

### Step 1: Survey First (Dedup Check)

Before creating anything, check for related existing ideas. This is the core mechanism that prevents re-having the same ideas.

1. Extract 3-5 keywords/phrases from the current conversation that capture the idea's essence
2. Search Karakeep in two passes — narrow then wide:
   ```
   # Pass 1: existing ideation (tagged)
   karakeep_search-bookmarks(query="#work-ideation [keyword]")

   # Pass 2: broader context (catches research briefs, reference links, older notes)
   karakeep_search-bookmarks(query="[keyword]")
   ```
   Pass 2 surfaces related material that wasn't captured through this skill.
   Deduplicate across both passes before presenting.
3. If matches are found, present them to the user (group ideation matches first, then broader context):
   - Show title + executive summary of each match
   - Ask: "I found these related ideas. Is this new, an evolution of one of these, or something you've already captured?"
4. Based on the user's answer:
   - **New idea** → proceed to Step 2
   - **Evolution** → read the existing bookmark's content, incorporate it, and create an updated version (keep the old one but tag it `ideation-superseded`)
   - **Already captured** → stop, optionally update the existing bookmark's status or add a note

### Step 2: Format the Bookmark

Synthesize the conversation into this structure. Every section is required unless noted optional.

```markdown
# Ideation: [Descriptive Title]

## Executive Summary
2-3 sentences answering: What is the idea? Why does it matter? What's the recommended approach?
This section is the primary thing people scan during review — make it count.

## Status
- **Stage**: Exploring
- **Created**: YYYY-MM-DD
- **Related Ideas**: [titles of related ideation bookmarks, or "None"]

## Problem / Opportunity
What prompted this idea. What pain point or gap does it address?
Keep it concrete — reference specific tools, workflows, or friction points.

## Proposed Approach
The plan — architecture, key decisions, tradeoffs.
Include enough detail that someone (including future-you) could evaluate
whether to invest in this without re-doing the research.

Structure this section based on what the conversation produced:
- Architecture diagrams (as ASCII/text)
- Decision tables (when comparing approaches)
- Step-by-step implementation plans
- Recommended vs. rejected alternatives with reasoning

## Open Questions
Things still unresolved. Decisions deferred. Risks identified but not mitigated.
If none, write "None — ready to build."

## Citations
Every URL referenced or discovered during the conversation.
Format: `- [descriptive label](url) — one-line note on what this source contributed`
Order by relevance, most important first.
Include: docs, GitHub repos, blog posts, spec references, API docs.
Do not include: generic search engine URLs, paywalled content without summary.
```

### Step 3: Create the Bookmark

```
karakeep_create-bookmark(
  type="text",
  title="Ideation: [Title]",
  content=[the formatted markdown]
)
```

### Step 4: Tag and Organize

Apply these tags and add to the list:

```
karakeep_attach-tag-to-bookmark(bookmarkId, tagsToAttach=["work-ideation", "ideation-exploring"])
karakeep_add-bookmark-to-list(bookmarkId, listId="n7ji9b271vt8f67dj79d3c0j")
```

The list ID `n7ji9b271vt8f67dj79d3c0j` is the "Work Ideation" list.

Lifecycle tags (exactly one per bookmark):
- `ideation-exploring` — still researching, not actionable yet
- `ideation-ready` — research done, ready to build
- `ideation-parked` — good idea but not now
- `ideation-done` — implemented / shipped
- `ideation-wontdo` — deliberately abandoned or deprecated
- `ideation-superseded` — replaced by a newer version of the idea

### Step 5: Confirm

Tell the user what was saved, show the title and executive summary, and note any related ideas that were found during the dedup check.

---

## Mode: Survey

Check existing ideation before or during a conversation. Use this proactively when the conversation touches a topic that might already have ideation.

1. Ask the user what domain or topic to survey (or infer from conversation context)
2. Search Karakeep in two passes — narrow then wide:
   ```
   # Pass 1: existing ideation (tagged)
   karakeep_search-bookmarks(query="#work-ideation [topic keywords]")

   # Pass 2: broader context (untagged research, reference bookmarks, related notes)
   karakeep_search-bookmarks(query="[topic keywords]")
   ```
   Pass 2 catches related bookmarks that weren't captured through this skill —
   research briefs, reference links, prior brainstorming notes that predate the
   ideation-capture workflow. Deduplicate across both passes before presenting.
3. For each match, fetch the content:
   ```
   karakeep_get-bookmark-content(bookmarkId)
   ```
4. Present a summary table (group ideation matches first, then broader context):

   | # | Title | Stage | Created | Executive Summary |
   |---|---|---|---|---|
   | 1 | Ideation: Project Scaffolder MCP | Exploring | 2026-04-02 | Internal scaffolding server... |

5. Let the user decide how to proceed:
   - "This is related to #1, let me evolve it" → Capture mode with evolution
   - "None of these are relevant" → Continue conversation normally
   - "I want to update #2's status" → Update tags

---

## Mode: Review

Periodic grooming of the ideation backlog.

### Step 1: Fetch Active Ideas

Search for all non-completed ideation:
```
karakeep_search-bookmarks(query="#work-ideation -#ideation-done -#ideation-wontdo -#ideation-superseded")
```

### Step 2: Present the Index

For each result, fetch content and extract the executive summary and status. Present as a table sorted by creation date (newest first):

| # | Title | Stage | Created | Executive Summary (truncated) |
|---|---|---|---|---|
| 1 | ... | Exploring | ... | ... |
| 2 | ... | Ready | ... | ... |

### Step 3: Interactive Grooming

Walk through the list with the user. For each idea, ask:
- **Keep as-is?** → move on
- **Update stage?** → swap lifecycle tag (detach old, attach new)
- **Merge with another?** → create a new combined ideation, mark originals as `ideation-superseded`
- **Park it?** → tag `ideation-parked`
- **Done?** → tag `ideation-done` (implemented/shipped)
- **Won't do?** → tag `ideation-wontdo` (abandoned/deprecated)

### Step 4: Summary

Report what changed: "Updated 3 ideas: moved #2 to Ready, parked #4, merged #1 and #5 into a new ideation."

---

## Tag Reference

| Tag | Purpose | Mutually exclusive with |
|---|---|---|
| `work-ideation` | Master index tag — on every ideation bookmark | — |
| `ideation-exploring` | Actively researching | Other lifecycle tags |
| `ideation-ready` | Research complete, ready to build | Other lifecycle tags |
| `ideation-parked` | Deferred — good idea, not now | Other lifecycle tags |
| `ideation-done` | Implemented / shipped | Other lifecycle tags |
| `ideation-wontdo` | Deliberately abandoned or deprecated | Other lifecycle tags |
| `ideation-superseded` | Replaced by a newer version | Other lifecycle tags |

Karakeep's AI also auto-generates domain tags (e.g., "software architecture", "MCP"). These are useful for cross-cutting searches but are not managed by this skill.

## Karakeep Infrastructure

- **List**: "Work Ideation" (ID: `n7ji9b271vt8f67dj79d3c0j`, icon: bulb)
- **Required MCP tools**: `karakeep_search-bookmarks`, `karakeep_create-bookmark`, `karakeep_get-bookmark-content`, `karakeep_attach-tag-to-bookmark`, `karakeep_detach-tag-from-bookmark`, `karakeep_add-bookmark-to-list`
