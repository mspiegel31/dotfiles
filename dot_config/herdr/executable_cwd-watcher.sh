#!/usr/bin/env bash
# Keeps each herdr pane's "$cwd" sidebar token in sync with its actual
# foreground working directory (basename only). Polls the socket API
# snapshot and calls `herdr pane report-metadata` only for panes whose
# cwd basename changed since the last poll.
#
# Intentionally does NOT use `set -e`/`pipefail`: this is a long-running
# poll loop and a single failed herdr call, empty snapshot, or empty
# "changed" set (an ordinary, frequent case) must never take the whole
# daemon down. Every fallible step is guarded explicitly instead.

STATE_FILE="${TMPDIR:-/tmp}/herdr-cwd-watcher.state"
INTERVAL="${CWD_WATCHER_INTERVAL:-2}"
export STATE_FILE

touch "$STATE_FILE" 2>/dev/null

while true; do
  snapshot="$(herdr api snapshot 2>/dev/null)"
  if [[ -n "$snapshot" ]]; then
    changes="$(echo "$snapshot" | python3 -c '
import json, sys, os

STATE_FILE = os.environ["STATE_FILE"]

prev = {}
if os.path.exists(STATE_FILE):
    with open(STATE_FILE) as f:
        for line in f:
            line = line.rstrip("\n")
            if "\t" in line:
                pid, base = line.split("\t", 1)
                prev[pid] = base

try:
    data = json.load(sys.stdin)
except ValueError:
    sys.exit(0)
agents = data.get("result", {}).get("snapshot", {}).get("agents", [])

cur = dict(prev)
changed = []
for a in agents:
    pane_id = a.get("pane_id")
    cwd = a.get("foreground_cwd") or a.get("cwd")
    if not pane_id or not cwd:
        continue
    base = os.path.basename(cwd.rstrip("/")) or cwd
    cur[pane_id] = base
    if prev.get(pane_id) != base:
        changed.append((pane_id, base))

with open(STATE_FILE, "w") as f:
    for pid, base in cur.items():
        f.write(f"{pid}\t{base}\n")

for pid, base in changed:
    print(f"{pid}\t{base}")
' 2>/dev/null)"

    if [[ -n "$changes" ]]; then
      while IFS=$'\t' read -r pane_id base; do
        [[ -z "$pane_id" ]] && continue
        herdr pane report-metadata "$pane_id" --source cwd-watcher --token "cwd=$base" >/dev/null 2>&1
      done <<< "$changes"
    fi
  fi
  sleep "$INTERVAL"
done
