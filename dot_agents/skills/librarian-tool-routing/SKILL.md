---
name: librarian-tool-routing
description: Mandatory tool selection rules for external code research — which search tool to use for remote repos, cross-repo patterns, library docs, and current codebase queries. Load when researching code outside the current project.
---

## Tool Selection Rules

You have multiple code search tools. These are **rules, not suggestions** — use
the prescribed tool chain for each task type.

### Decision Table

| Task | Required tool chain | NEVER use |
| --- | --- | --- |
| Understand how a **specific repo** implements X | `repomix_pack_remote_repository` → `repomix_grep_repomix_output` → `repomix_read_repomix_output` | `grep_app` (line snippets lack context) |
| Find **examples of X across many repos** | `grep_app_searchGitHub` with language/repo/path filters | `repomix` (can't search across repos) |
| Look up **library API docs** | `context7_resolve-library-id` → `context7_query-docs` | `grep_app` (finds usage, not docs) |
| Understand the **current working codebase** | `augment-context-engine_codebase-retrieval` | `repomix_pack_codebase` (disabled) |

### Repomix Workflow (pack-once, search-many)

When researching a specific repository:

1. **Pack**: `repomix_pack_remote_repository` with `includePatterns` scoped to
   relevant directories (e.g., `"src/**/*.ts"`, `"lib/**/*.py"`). Scope
   aggressively — large repos without filtering waste tokens.
2. **Search**: `repomix_grep_repomix_output` with regex patterns. The `outputId`
   persists for the session — multiple greps after one pack are cheap.
3. **Read**: `repomix_read_repomix_output` with line ranges for full context
   around matches.

ALWAYS complete all three steps. Do not stop after packing — the pack is
useless without grep and read. Do not grep without reading the surrounding
context.

### grep_app Rules

- ONLY for cross-repo pattern discovery — "how do other projects do X"
- ALWAYS set `language` filter to reduce noise
- Use `useRegexp=true` with `(?s)` prefix for multi-line patterns
- Results are line-level snippets — if you need deeper context from a match,
  follow up with Repomix on that specific repo

### Anti-Patterns

- Using `grep_app` to understand a specific repo → **WRONG**. Pack it with Repomix instead.
- Packing a repo but only reading the file tree → **WRONG**. Grep and read the actual code.
- Using `webfetch` to read a GitHub file when Repomix would give full project context → **WRONG**.
- Calling `context7_query-docs` without first calling `context7_resolve-library-id` → **WILL FAIL**.
- Packing an entire large repo without `includePatterns` → **WASTEFUL**. Scope to relevant dirs.
