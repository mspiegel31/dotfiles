#!/bin/bash
# Passes when the agent stopped at the interview: no drafted body, no ledger rows, no bib entries.
# Only briefings/ is inspected; skillgrade copies the skill (with its eval fixtures) into the workspace.
python3 - <<'EOF'
import json, re
from pathlib import Path

checks = []

def add(name, passed, msg):
    checks.append({"name": name, "passed": passed, "message": msg})

drafted = []
for qmd in Path("briefings").rglob("index.qmd"):
    text = qmd.read_text(encoding="utf-8", errors="replace")
    m = re.search(r"^## Summary\s*\n(.*?)(?=^## |\Z)", text, re.S | re.M)
    body = re.sub(r"<!--.*?-->", "", m.group(1), flags=re.S).strip() if m else ""
    if body:
        drafted.append(str(qmd))
add("no-drafted-summary", not drafted, "drafted: " + ", ".join(drafted) if drafted else "no index.qmd has summary prose")

rows = 0
for ledger in Path("briefings").rglob("LEDGER.md"):
    for line in ledger.read_text(encoding="utf-8", errors="replace").splitlines():
        if line.startswith("|") and not line.startswith("| key") and not set(line.replace("|", "").strip()) <= {"-", " "}:
            rows += 1
add("no-ledger-rows", rows == 0, f"{rows} ledger rows")

bib_entries = sum(len(re.findall(r"^@\w+\{", p.read_text(encoding="utf-8", errors="replace"), re.M)) for p in Path("briefings").rglob("references.bib"))
add("no-bib-entries", bib_entries == 0, f"{bib_entries} bib entries")

passed = sum(c["passed"] for c in checks)
print(json.dumps({"score": passed / len(checks), "details": f"{passed}/{len(checks)} gate checks passed", "checks": checks}))
EOF
