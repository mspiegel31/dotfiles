---
name: no-restating-comments
description: "Don't add comments that restate what code does in English — only docs links, config/behavioral notes essential to understanding"
condition: ["//\\s*(?!TODO|FIXME|NOTE|https?://)[A-Za-z]+\\s+(the|a|an|this|that|to|if|when|for|from|with|it|all|each|every)\\b", "\\*\\s*(?!@|TODO|FIXME|NOTE|https?://)[A-Za-z]+\\s+(the|a|an|this|that|to|if|when|for|from|with|it|all|each|every)\\b", "#\\s+(?!TODO|FIXME|NOTE|https?://)[A-Za-z]+\\s+(the|a|an|this|that|to|if|when|for|from|with|it|all|each|every)\\b"]
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

Cut comments that just restate the code in plain English (e.g. `// checks if the token is valid`, `# Reads all the files under schemas/`). Keep only comments that carry information the code itself can't: a link to docs/spec, a config or env-var detail, the *why* behind a non-obvious choice, or a warning about a gotcha. If the next line already makes the comment's content obvious, delete the comment instead of writing it.

This applies to every language with comments, not just the one you happen to be in. `//`, `#`, and `*` continuation lines are the same violation wearing different syntax.

The trigger is a deliberately narrow tripwire: it catches `<verb> <determiner>` openings and nothing more. Firing means *re-read the comment*, not "this comment is definitely wrong" — a why-comment or gotcha that trips it should stay.
