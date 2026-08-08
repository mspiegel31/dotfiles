---
name: slack-writer
description: "Slack mrkdwn authoring for internal marketing and announcement posts — formatting reference, post structure patterns, and quality checklist. Use when drafting Slack announcements, team updates, 'here's what we shipped' posts, or any internal advocacy writing destined for Slack. Also use when the user wants to review or improve a draft Slack post."
---

# Slack Writer

Craft internal marketing and announcement posts formatted for Slack's message composer. Output goes to the clipboard — the user copy-pastes into Slack. For shared writing principles, load the `writing-foundations` skill.

**This skill covers:** announcement posts, "what we shipped" updates, team advocacy, internal marketing.
**This skill does NOT cover:** asking for help, incident comms, RFC summaries, or conversational messages.

---

## Output Protocol

1. Draft the post using only Slack-compatible formatting (see reference below)
2. Copy to clipboard using the clipboard MCP tool (`clipboard_copy`)
3. Tell the user: "Copied to clipboard. Paste into Slack."

If the post needs a thread reply (detail below the hook), output the main post first, then the thread reply separately — two clipboard operations.

---

## Post Anatomy

A good announcement post has three parts:

### 1. The Hook (first line)

This is what people see in the channel without expanding. It determines whether anyone reads the rest.

- Lead with the *outcome*, not the process. "Deployments are now 3x faster" not "We finished the CI/CD migration project."
- Use an emoji prefix to signal category — readers scan emoji before text.
- Keep it to one line. If it wraps, it's too long.

### 2. The Body (2-5 short blocks)

Short paragraphs (1-2 sentences each), separated by blank lines. Slack is a feed — walls of text get scrolled past.

- Use `*bold*` for key terms the reader should remember.
- Use emoji as section markers for visual scanning (not decoration).
- Bullet points for lists of 3+ items.
- If something is a link, use `[text](url)` syntax — Slack converts it on send.
- One idea per paragraph. If a paragraph has "and also," split it.

### 3. The Close (what should the reader do?)

Every post needs a reason to exist from the reader's perspective:

- **Awareness:** "No action needed — just wanted to share the win."
- **Feedback:** "Thread any questions or edge cases you're seeing."
- **Action:** "Try it out: [descriptive text](https://link.example.com)"
- **Discussion:** "Curious what you think — thread below."

---

## Threading Strategy

If the post has detail that not everyone needs, use the hook-and-thread pattern:

**Main post:** Hook + 2-3 sentence summary + call to action
**Thread reply:** Technical detail, context, links, caveats

This respects people who just want the headline while giving depth-seekers a path.

---

## Tone Guidance

- **Confident, not boastful.** "This cuts deploy time from 15min to 5min" not "We absolutely crushed it!!!"
- **Specific, not vague.** Numbers, names, links. "The Payments team shipped X" not "Some great work happened."
- **Inclusive.** Not everyone in the channel is an engineer. Avoid unexplained acronyms. If you must use one, expand it once.
- **Brief.** If the post takes more than 10 seconds to read, it's too long for the main channel. Move detail to a thread.

---

## Slack Composer Formatting Reference

Slack's message composer uses mrkdwn — NOT standard markdown. Many GFM features do not render. This reference is empirically tested by pasting into the composer.

### What works ✅

| Element | Syntax | Notes |
|---|---|---|
| Bold | `*text*` | SINGLE asterisk — not `**double**` |
| Italic | `_text_` | Underscores only — not `*single asterisk*` |
| Strikethrough | `~text~` | SINGLE tilde — not `~~double~~` |
| Inline code | `` `code` `` | Backticks, same as markdown |
| Code block | ` ```\ncode\n``` ` | Fenced, NO language tag (see below) |
| Block quote | `> text` | Greater-than prefix |
| Unordered list | `- item` or `• item` | Both work; `•` renders slightly cleaner |
| Ordered list | `1. item` | Number + dot prefix |
| Link | `[display text](url)` | Slack converts to clickable link on send |

### What does NOT work ❌

| Element | Why it fails |
|---|---|
| `**double asterisk bold**` | Renders as literal `**text**` |
| `~~double tilde strike~~` | Renders as literal `~~text~~` |
| `# Headings` | Renders as literal `## text` |
| Tables (`\| col \| col \|`) | Renders as literal pipe characters |
| Code block language tags (` ```bash `) | The tag (e.g. `bash`) appears as literal text on the first line |
| Horizontal rules (`---`) | Not rendered |

### Critical Differences from Standard Markdown

```
Standard markdown    →    Slack mrkdwn
─────────────────         ─────────────
**bold**             →    *bold*
*italic*             →    _italic_
~~strikethrough~~    →    ~strikethrough~
```language           →    ``` (no language tag)
# Heading            →    emoji + *bold* section marker
| table |            →    not available — use lists instead
```

### Section Markers (instead of headings)

Since Slack has no headings, use emoji + bold as section markers:

```
🚀 *What shipped*
Brief description of the change.

📊 *Impact*
Numbers or before/after comparison.

👉 *Try it out*
[Click here](https://link.example.com) to see it in action.
```

### Tables Alternative

Since tables don't render, present tabular data as bold label + value pairs or use a code block:

```
*Average deploy time:* 14m 32s → 4m 48s
*P95 deploy time:* 22m 10s → 7m 15s
*Failed deploys/week:* ~12 → ~3
```

Or for more structured data, use a plain code block (no language tag):

```
Metric               Before      After
Average deploy time   14m 32s     4m 48s
P95 deploy time       22m 10s     7m 15s
Failed deploys/week   ~12         ~3
```

---

## Quality Checklist

| Check | Pass criteria |
|---|---|
| *Hook is an outcome* | First line states what changed, not what you did |
| *Scannable in 5 seconds* | A reader scrolling fast gets the gist from hook + bold terms |
| *No formatting misses* | Uses only Slack-compatible syntax from the ✅ table above |
| *No forbidden syntax* | Zero instances of `**`, `~~`, `##`, table pipes, or language-tagged code blocks |
| *Has a close* | Reader knows whether to act, reply, or just absorb |
| *Under 10 seconds* | Main post body — detail goes in a thread |
| *Links are links* | `[display text](url)` — not raw URLs |
| *Acronyms expanded* | First use of any acronym spells it out |

---

## Examples

If this skill's directory contains an `examples/` subdirectory, read the annotations for calibration. Examples are added over time as the user encounters good posts.

### Adding Examples

Create `examples/<descriptive-name>.md` with:

```markdown
# <Short description>

**Source:** <channel name or context>
**Rating:** strong / adequate / weak
**What works:** <1-2 sentences>
**What doesn't:** <1-2 sentences>

---

<the actual post in Slack mrkdwn>
```

The post content should be the literal mrkdwn that would be pasted into Slack — not a description of it.
