# Agent configuration

- `dot_agents/` is the canonical portable agent configuration. Chezmoi maps it to `~/.agents/`.
- Put skills in `dot_agents/skills/<name>/SKILL.md`, ordinary rules in `dot_agents/rules/`, and portable commands in `dot_agents/commands/`. OMP reads them directly from `~/.agents/`.
- Put OMP-specific subagents in `dot_agents/agents/`. Only `dot_omp/agent/symlink_agents` exposes that directory at OMP's required `~/.omp/agent/agents/` path.
- A rule with `condition` or `astCondition` is a TTSR rule. Keep ordinary guidance free of those fields; use `description` for rulebook rules or `alwaysApply: true` for persistent rules.
- Keep extensions in OMP-native configuration. OMP does not auto-discover `.agents/extensions/`.
- Do not duplicate a capability in `dot_omp/` and `dot_agents/`. `dot_agents/` is the source of truth for portable content.

- For agent harness configs (OMP `~/.omp/`, OpenCode `~/.config/opencode/`, Codex `~/.codex/`, etc.): the TUI frequently modifies config files at runtime. Before using `chezmoi re-add` or overwriting the source with target state in these directories, ALWAYS ask the user which version to keep. Never assume the source (template) is the intended state — the live target often holds deliberate TUI-made changes that haven't been propagated yet.

# Machine provisioning

- Host-specific values are gated with `{{ if eq .chezmoi.hostname .workHostname }}`, where `workHostname` lives in `.chezmoidata.yaml`. Leaving it at a placeholder silently disables every work-only branch, so set it before trusting an apply.
- Secrets never enter a managed file. OMP treats a config value starting with `!` as a shell command and uses its stdout, so the templates carry `!cat $HOME/.config/agent-secrets/<name>`. Keep it that way: a build-time secret function bakes plaintext into the rendered target and makes `chezmoi apply` fail whenever the secret backend is unreachable.
- `~/.omp/` is the only OMP config root. Do not reintroduce `OMP_PROFILE` or `~/.omp/profiles/*` — a named profile makes OMP read `~/.omp/profiles/<name>/agent/`, which chezmoi does not manage, so every rendered target is silently ignored.
