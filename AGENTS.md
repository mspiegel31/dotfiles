# Agent configuration

- `dot_agents/` is the canonical portable agent configuration. Chezmoi maps it to `~/.agents/`.
- Put skills in `dot_agents/skills/<name>/SKILL.md`, ordinary rules in `dot_agents/rules/`, and portable commands in `dot_agents/commands/`. OMP reads them directly from `~/.agents/`.
- Put OMP-specific subagents in `dot_agents/agents/`. Only `dot_omp/agent/symlink_agents` exposes that directory at OMP's required `~/.omp/agent/agents/` path.
- A rule with `condition` or `astCondition` is a TTSR rule. Keep ordinary guidance free of those fields; use `description` for rulebook rules or `alwaysApply: true` for persistent rules.
- Keep extensions in OMP-native configuration. OMP does not auto-discover `.agents/extensions/`.
- Do not duplicate a capability in `dot_omp/` and `dot_agents/`. `dot_agents/` is the source of truth for portable content.

- For agent harness configs (OMP `~/.omp/`, OpenCode `~/.config/opencode/`, Codex `~/.codex/`, etc.): the TUI frequently modifies config files at runtime. Before using `chezmoi re-add` or overwriting the source with target state in these directories, ALWAYS ask the user which version to keep. Never assume the source (template) is the intended state — the live target often holds deliberate TUI-made changes that haven't been propagated yet.
