#!/bin/bash
# Passes when briefing/ survives `briefing.py audit` and every template section has prose.
cli="$HOME/.agents/skills/brief-me/scripts/briefing.py"
[ -f "$cli" ] || cli="$(dirname "$0")/../scripts/briefing.py"
python3 - "$cli" <<'EOF'
import json, re, subprocess, sys
from pathlib import Path

cli = sys.argv[1]
checks = []

def add(name, passed, msg):
    checks.append({"name": name, "passed": passed, "message": msg})

audit = subprocess.run(["python3", cli, "audit", "briefing", "--json"], capture_output=True, text=True)
try:
    report = json.loads(audit.stdout)
    orphans = report["cited_not_in_bib"] + report["cited_not_in_ledger"] + report["bib_not_in_ledger"]
    url_findings = {}
    for name in ("bib_url_missing", "bib_url_escaped", "bib_url_mismatch"):
        keys = report.get(name, [])
        if keys:
            url_findings[name] = keys
    findings = orphans + [f"{name}: {keys}" for name, keys in url_findings.items()]
    add("audit-clean", audit.returncode == 0 and not findings, f"findings: {findings}" if findings else f"{report['citations']} citations, all in bib and ledger")
    add("has-citations", report["citations"] >= 3, f"{report['citations']} distinct citations")
except (json.JSONDecodeError, KeyError):
    add("audit-clean", False, (audit.stderr or audit.stdout).strip()[:200])
    add("has-citations", False, "audit did not run")

qmd = Path("briefing/index.qmd")
text = qmd.read_text(encoding="utf-8", errors="replace") if qmd.exists() else ""
empty = []
for section in ("Summary", "Glossary", "Body", "Worked example", "Limitations", "Next action"):
    m = re.search(rf"^## {re.escape(section)}\s*\n(.*?)(?=^## |\Z)", text, re.S | re.M)
    body = re.sub(r"<!--.*?-->", "", m.group(1), flags=re.S).strip() if m else ""
    if not body:
        empty.append(section)
add("sections-filled", not empty, "empty: " + ", ".join(empty) if empty else "all sections have prose")

add("worked-example-arithmetic", "3.35" in text and "0.67" in text, "mean reward 3.35/5 = 0.67 shown" if "3.35" in text else "worked example arithmetic missing")
add("no-todo", "TODO" not in text, "TODO remains" if "TODO" in text else "no TODO")

passed = sum(c["passed"] for c in checks)
print(json.dumps({"score": passed / len(checks), "details": f"{passed}/{len(checks)} checks passed", "checks": checks}))
EOF
