# Dotfiles

This repository is the chezmoi source state for this machine. It manages shell startup, tmux, Git Delta, Serie, VS Code extensions, QMK keymaps, OMP, and Linux desktop configuration. Do not use Stow.

## Bootstrap

Install chezmoi, then initialize this repository:

```sh
chezmoi init --ssh mspiegel31
```

`chezmoi init` writes `~/.config/chezmoi/chezmoi.toml` from `.chezmoi.toml.tmpl`,
recording `sourceDir` so later invocations find this repository. Review the
rendered changes, then apply them:

```sh
chezmoi diff
chezmoi apply
```

### Secrets

No secret is ever rendered into a managed file. OMP resolves them at runtime: a
config value beginning with `!` is a shell command whose stdout becomes the
value, so the OMP templates carry `!cat $HOME/.config/agent-secrets/<name>`
instead of the secret itself.

Provision these files by hand, `chmod 600`, before the first OMP launch that
needs them:

| File in `~/.config/agent-secrets/` | Used by |
| --- | --- |
| `context7-api-key` | `context7` MCP |
| `karakeep-api-key` | `karakeep` MCP |
| `google-workspace-client-id` | `google-workspace` MCP |
| `google-workspace-client-secret` | `google-workspace` MCP |
| `gitops-local-token` | `gitops-local-mcp` |

`~/.config/agent-secrets/` is local-only and never committed. A missing file
degrades only the MCP server that reads it; it does not break `chezmoi apply`.

## Managed targets

Everyday configuration works on macOS and Linux: shell files, tmux, Git Delta, Serie, VS Code extensions, QMK keymaps, and OMP. Awesome, Conky, i3/i3status, IMWheel, KDE, Qtile, desktop autostart, and systemd user files apply only on Linux. `docs/legacy/` preserves the retired Stow installer and Qtile note; chezmoi ignores that directory.

The apply scripts clone Oh My Zsh and QMK firmware only when their destination does not already exist. They refuse to replace a non-Git destination. The VS Code installer skips when `code` is missing. To prepare QMK's toolchain, run:

```sh
qmk setup -H "$HOME/qmk_firmware"
qmk compile -kb planck/rev6 -km mspiegel31
```

`.chezmoidata.yaml` pins `workHostname` to the work laptop's hostname. Work-only
OMP roles and MCPs are gated on `{{ if eq .chezmoi.hostname .workHostname }}`, so
they activate on that host and stay inert everywhere else.

## Daily use

Edit a target through chezmoi, inspect pending changes, and update from the remote:

```sh
chezmoi edit --apply <target>
chezmoi status
chezmoi diff
chezmoi update
```

Run `chezmoi cd` to enter the source repository, then use normal Git commands to publish source changes.
