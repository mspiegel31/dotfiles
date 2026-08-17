# Agent configuration

- `dot_agents/` is the canonical portable agent configuration. Chezmoi maps it to `~/.agents/`.
- Put skills in `dot_agents/skills/<name>/SKILL.md`, ordinary rules in `dot_agents/rules/`, and portable commands in `dot_agents/commands/`. OMP reads them directly from `~/.agents/`.
- Put OMP-specific subagents in `dot_agents/agents/`. Only `dot_omp/agent/symlink_agents` exposes that directory at OMP's required `~/.omp/agent/agents/` path.
- A rule with `condition` or `astCondition` is a TTSR rule. Keep ordinary guidance free of those fields; use `description` for rulebook rules or `alwaysApply: true` for persistent rules.
- Keep extensions in OMP-native configuration. OMP does not auto-discover `.agents/extensions/`.
- Do not duplicate a capability in `dot_omp/` and `dot_agents/`. `dot_agents/` is the source of truth for portable content.

# Obsidian vaults

- Before using direct filesystem access or the `obsidian` CLI against an Obsidian vault, read and follow the `obsidian-rules` skill.

- For agent harness configs (OMP `~/.omp/`, OpenCode `~/.config/opencode/`, Codex `~/.codex/`, etc.): the TUI frequently modifies config files at runtime. Before using `chezmoi re-add` or overwriting the source with target state in these directories, ALWAYS ask the user which version to keep. Never assume the source (template) is the intended state — the live target often holds deliberate TUI-made changes that haven't been propagated yet.

# Secrets
- secrets for personsal config (mcp servers etc) will live in bitwarden via the *bws* cli.  Work items will use the pi `!cat` symbol to read from secret files I will provision on the work machine 

# Chezmoi
- default to instructing the user how to work with chezmoi so they can learn the api/sdk themselves.  explicitly ask for confirmation before performing chezmoi cli operations yourself.

# Machine provisioning

- Host-specific values are gated with `{{ if eq .chezmoi.hostname .workHostname }}`, where `workHostname` lives in `.chezmoidata.yaml`. Leaving it at a placeholder silently disables every work-only branch, so set it before trusting an apply.
- Secrets never enter a managed file. OMP treats a config value starting with `!` as a shell command and uses its stdout, so the templates carry `!cat $HOME/.config/agent-secrets/<name>`. Keep it that way: a build-time secret function bakes plaintext into the rendered target and makes `chezmoi apply` fail whenever the secret backend is unreachable.
- `~/.omp/` is the only OMP config root. Do not reintroduce `OMP_PROFILE` or `~/.omp/profiles/*` — a named profile makes OMP read `~/.omp/profiles/<name>/agent/`, which chezmoi does not manage, so every rendered target is silently ignored.
- `dot_omp/private_agent/config.yml.tmpl` is the only source of `modelRoles`. Do not reintroduce a `PI_CONFIG_FILES` overlay: an overlay outranks the rendered config, so a role set there silently wins and the template's value becomes dead text. Host-gate the roles instead.
- MCP servers shell out to vendor CLIs, never to wrapper code in this repo. `grafana-mcp-server` is `tsh mcp connect grafana-mcp-server` with `TELEPORT_CLUSTER` in `env` — exactly what `tsh mcp config <app>` emits. A local wrapper script used to manage a `tsh proxy mcp` listener plus a stdio bridge; `tsh mcp connect` speaks stdio directly and reissues its own certificate mid-session, so that code was deleted. Before vendoring a launcher, check whether the vendor CLI already has a stdio transport.
