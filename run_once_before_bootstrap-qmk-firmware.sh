#!/bin/sh
set -eu

target="$HOME/qmk_firmware"
remote="https://github.com/qmk/qmk_firmware.git"

if [ -d "$target/.git" ]; then
  exit 0
fi

# chezmoi manages keymaps under this directory, so the destination is often
# already populated by a previous apply and `git clone` would refuse it. Do the
# equivalent in place: the managed keymaps are untracked here and QMK ships
# nothing at keyboards/*/keymaps/mspiegel31, so the checkout cannot clobber them.
mkdir -p "$target"
git init -q "$target"
git -C "$target" remote add origin "$remote"
git -C "$target" fetch -q --depth=1 origin master
git -C "$target" checkout -q -f -b master FETCH_HEAD
