# Dotfiles

This repository is the chezmoi source state for this machine. It manages shell startup, tmux, Git Delta, Serie, VS Code extensions, QMK keymaps, OMP, and Linux desktop configuration. Do not use Stow.

## Bootstrap

Install chezmoi, then initialize this repository:

```sh
chezmoi init --ssh mspiegel31
```

Configure Bitwarden Secrets Manager before applying. Create `~/.config/chezmoi/chezmoi.yaml` locally:

```yaml
bitwardenSecrets:
  command: bws
data:
  accessToken: <Bitwarden Secrets Manager service-account access token>
```

Lock the local configuration, review the rendered changes, then apply them:

```sh
chmod 600 ~/.config/chezmoi/chezmoi.yaml
chezmoi diff
chezmoi apply
```

Do not use `chezmoi init --apply`: the local Bitwarden configuration must exist before chezmoi renders OMP's `.mcp.json` files.

The Bitwarden service-account token, rendered Context7 and Karakeep secrets, work-only `!cat` secret files, and `~/.config/chezmoi/chezmoi.yaml` are local-only. Never commit them. Secret UUIDs in the OMP templates are source metadata and remain in Git.

## Managed targets

Everyday configuration works on macOS and Linux: shell files, tmux, Git Delta, Serie, VS Code extensions, QMK keymaps, and OMP. Awesome, Conky, i3/i3status, IMWheel, KDE, Qtile, desktop autostart, and systemd user files apply only on Linux. `docs/legacy/` preserves the retired Stow installer and Qtile note; chezmoi ignores that directory.

The apply scripts clone Oh My Zsh and QMK firmware only when their destination does not already exist. They refuse to replace a non-Git destination. The VS Code installer skips when `code` is missing. To prepare QMK's toolchain, run:

```sh
qmk setup -H "$HOME/qmk_firmware"
qmk compile -kb planck/rev6 -km mspiegel31
```

`.chezmoidata.yaml` intentionally keeps `workHostname: "REPLACE-WITH-WORK-HOSTNAME"`. OMP work roles and MCPs stay disabled until a source commit adds the exact work hostname.

## Daily use

Edit a target through chezmoi, inspect pending changes, and update from the remote:

```sh
chezmoi edit --apply <target>
chezmoi status
chezmoi diff
chezmoi update
```

Run `chezmoi cd` to enter the source repository, then use normal Git commands to publish source changes.
