---
name: no-absent-or-default-comments
description: "Never comment about steps you deliberately did NOT add, or about runtime/tooling defaults"
condition: ["(#|//|\\*)\\s*No\\s+[a-z-]+/[a-z-]+[^\\n]*:", "(#|//|\\*)\\s*[Nn]o\\s+(setup-node|actions/checkout|checkout|build step|package\\.json)\\b", "(#|//|\\*)\\s*(No|Not)\\s+\\w+[^\\n]*\\s+(step|either)\\b", "(#|//|\\*)[^\\n]*runner-provided"]
scope: ["tool:edit", "tool:write"]
globs:
  - "*.ts"
  - "*.tsx"
  - "*.js"
  - "*.jsx"
  - "*.mjs"
  - "*.cjs"
  - "*.py"
  - "*.go"
  - "*.rs"
  - "*.rb"
  - "*.java"
  - "*.kt"
  - "*.swift"
  - "*.c"
  - "*.h"
  - "*.cpp"
  - "*.hpp"
  - "*.cs"
  - "*.php"
  - "*.lua"
  - "*.sh"
  - "*.bash"
  - "*.zsh"
  - "*.yml"
  - "*.yaml"
  - "*.toml"
  - "*.tf"
  - "*.tfvars"
  - "*.hcl"
  - "*.sql"
  - "*.css"
  - "*.scss"
  - "*.less"
  - "*.proto"
  - "*.graphql"
  - "Dockerfile"
  - "Makefile"
---

## Do not comment on what is not there

You are writing a comment explaining a step you deliberately *omitted*, or restating a runtime/tooling default. Delete it.

A reader scanning a file is asking "what does this do?" — not "which of the infinite things did the author consider and reject?" Nobody hunting a bug greps for the absence of `actions/checkout`.

### Cut these

```yaml
# No actions/checkout: the action only calls the GitHub API...
# No setup-node either -- `runs: using: node24` is runner-provided.
```

```ts
// No package.json here (single root package). No build step.
```

Each one is load-bearing for zero readers:

- **Omitted steps** are invisible. The comment invents a question to answer.
- **Runtime/tooling defaults** are documented by the platform, not your file.
- **Repo-wide conventions** (single root `package.json`, no build step) belong in `AGENTS.md` once, not restated in every file that follows them.

### Where the fact actually belongs

If a mechanism genuinely needs explaining, comment it at the line that *implements* it, not at the gap where something else isn't.

A PnP bootstrap detail belongs next to the `require("../../.pnp.cjs")` that performs it. A dependency-resolution quirk belongs in the file that resolves it. Moving the note there makes it discoverable by the person actually reading that code.

### The test

Before writing any comment, ask: **does this describe a line in this file?**

- Yes, and the *why* is non-obvious → keep it.
- Yes, but it restates the line → delete it.
- No, it describes an absence or a default → delete it.

A comment earns its place by carrying information the file cannot. "I chose not to do X" is not that information.
