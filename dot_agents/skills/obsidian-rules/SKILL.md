---
name: obsidian-rules
description: Rules for safely interacting with Obsidian vaults. Use before any task that reads, searches, creates, edits, moves, renames, or deletes vault notes or attachments, or invokes the `obsidian` CLI. Covers both direct Markdown access and Obsidian CLI workflows.
---

# Obsidian rules

## Scope

Apply these rules to every vault operation, whether using direct filesystem access or the `obsidian` CLI. Vault content is user-authored material. Preserve its structure and meaning unless the user requests a change.

## Choose the interface

- Use the Obsidian CLI for vault-aware work: searching notes, resolving note names, properties, tasks, tags, backlinks, templates, daily notes, and plugin commands.
- Use direct Markdown edits for exact, explicitly scoped note changes and deliberate bulk transformations.
- When the CLI supports the needed semantic operation, prefer it over reproducing Obsidian behavior from raw text.

## Work safely

1. Identify the intended vault before reading or changing content. Use `vault="<name>"` with CLI commands. Never rely on the most recently focused vault.
2. Inspect the relevant note, frontmatter, links, and surrounding conventions before editing.
3. Preserve YAML frontmatter, wikilinks, embeds, aliases, callouts, and attachments unless the task explicitly changes them.
4. Before moving or renaming a note, inspect inbound links and migrate every affected reference.
5. After structural changes, use the CLI to check the affected properties, backlinks, tasks, or unresolved links.

## Guardrails

- Do not modify `.obsidian/`, install or enable plugins, or run `obsidian eval` without explicit approval.
- Require explicit approval for deletion, overwrite, broad rewrites, mass frontmatter changes, or folder-wide moves.
- Do not treat this skill as access control. If a hard boundary is needed, use a constrained wrapper or MCP server rather than direct filesystem access.
