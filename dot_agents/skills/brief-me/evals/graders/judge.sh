#!/bin/bash
# Runs the skill's review rubric through skillgrade-omp-grader (OMP as judge).
# skillgrade hands the task context in SKILLGRADE_INPUT; the adapter expects
# expected.rubric and expected.artifacts. Tasks that set expected.rubric inline keep
# it; the rest get references/rubric.md so its text is not duplicated in eval.yaml.
set -eu

skill="$HOME/.agents/skills/brief-me"
[ -f "$skill/references/rubric.md" ] || skill="$(cd "$(dirname "$0")/../.." && pwd)"

if ! command -v skillgrade-omp-grader >/dev/null 2>&1; then
  echo '{"score": 0, "details": "grader error: skillgrade-omp-grader not on PATH"}'
  exit 0
fi

SKILLGRADE_INPUT=$(python3 - "$skill/references/rubric.md" <<'EOF'
import json, os, sys
data = json.loads(os.environ.get("SKILLGRADE_INPUT") or "{}")
expected = data.get("expected") or {}
expected.setdefault("rubric", open(sys.argv[1], encoding="utf-8").read())
expected.setdefault("artifacts", ["briefing/index.qmd"])
data["expected"] = expected
print(json.dumps(data))
EOF
)
export SKILLGRADE_INPUT
exec skillgrade-omp-grader
