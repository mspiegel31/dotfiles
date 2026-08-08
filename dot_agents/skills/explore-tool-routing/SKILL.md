---
name: explore-tool-routing
description: Tool selection routing for the explore agent — prefer Augment context engine for local codebase search over grep_app or repomix.
---

## Tool Selection for Codebase Exploration

For searching the **local/current codebase** you are working in:

- **ALWAYS PREFER** `augment-context-engine_codebase-retrieval` for semantic code search. It uses advanced embeddings and understands code structure — far superior to keyword grep for finding relevant code.
- Use `grep` / `glob` tools only for exact string matching, file pattern lookups, or when you need ALL occurrences of a known identifier.

## What NOT to Use

Do **not** use `grep_app` or `repomix` for the current working codebase. Augment's context engine is purpose-built for this and will return better, more contextual results.
