#!/bin/sh
set -eu

target="$HOME/qmk_firmware"
if [ -d "$target/.git" ]; then
  exit 0
fi
if [ -e "$target" ]; then
  printf '%s\n' "Refusing to replace non-Git $target" >&2
  exit 1
fi
git clone --depth=1 https://github.com/qmk/qmk_firmware.git "$target"
