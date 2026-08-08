#!/bin/sh
set -eu

if ! command -v qmk >/dev/null 2>&1; then
  printf '%s\n' 'QMK CLI not found; skipping QMK defaults.' >&2
  exit 0
fi
qmk config user.keyboard=planck/rev6 user.keymap=mspiegel31
