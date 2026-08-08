#!/bin/sh
set -eu

if ! command -v nvim >/dev/null 2>&1; then
  printf '%s\n' 'Neovim not found; skipping PlugInstall.' >&2
  exit 0
fi
nvim --headless '+PlugInstall --sync' +qa
