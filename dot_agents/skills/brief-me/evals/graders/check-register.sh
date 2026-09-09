#!/bin/bash
# Passes when the planted register and citation faults in briefing/index.qmd are gone and the audit is clean.
cli="$HOME/.agents/skills/brief-me/scripts/briefing.py"
[ -f "$cli" ] || cli="$(dirname "$0")/../scripts/briefing.py"
python3 - "$cli" <<'EOF'
import json, re, subprocess, sys
from pathlib import Path

cli = sys.argv[1]
checks = []

def add(name, passed, msg):
    checks.append({"name": name, "passed": passed, "message": msg})

text = Path("briefing/index.qmd").read_text(encoding="utf-8", errors="replace")
low = text.lower()

audit = subprocess.run(["python3", cli, "audit", "briefing", "--json"], capture_output=True, text=True)
add("audit-clean", audit.returncode == 0, "audit passed" if audit.returncode == 0 else (audit.stderr or audit.stdout).strip()[:200])
add("orphan-key-removed", "@skillgrade-presets" not in text, "@skillgrade-presets gone" if "@skillgrade-presets" not in text else "unfetched @skillgrade-presets still cited")

banned = [p for p in ("three numbers. one gate.", "let's dive in", "seamless", "powerful", "most teams find") if p in low]
add("register-clean", not banned, "banned: " + ", ".join(banned) if banned else "no banned phrases")

docker = re.search(r"docker runs are roughly 30% slower[^\n]*", text, re.I)
add("uncited-claim-removed", docker is None or "[@" in docker.group(0), "uncited Docker speed claim remains" if docker and "[@" not in docker.group(0) else "Docker claim removed or cited")

kept = all(s in text for s in ("3.35", "0.078", "reward is at least 0.5"))
add("content-preserved", kept, "worked example and success rule intact" if kept else "correct content was removed")

passed = sum(c["passed"] for c in checks)
print(json.dumps({"score": passed / len(checks), "details": f"{passed}/{len(checks)} checks passed", "checks": checks}))
EOF
