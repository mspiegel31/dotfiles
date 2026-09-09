#!/bin/sh
set -eu

# ~/.agents/skills holds two populations: skills authored here (chezmoi-managed under
# dot_agents/skills) and skills installed from mattpocock/skills by `npx skills add`,
# which chezmoi does not manage. This script restores the second population on a new
# machine. Update the list when adding or removing an upstream skill; the content
# change re-triggers the run.

if ! command -v npx >/dev/null 2>&1; then
  printf '%s\n' 'npx not found; skipping mattpocock/skills install' >&2
  exit 0
fi

skills='ask-matt claude-handoff code-review codebase-design diagnosing-bugs domain-modeling git-guardrails-claude-code grill-me grill-with-docs grilling handoff implement implement-spec improve-codebase-architecture loop-me migrate-to-shoehorn prototype research resolving-merge-conflicts retro scaffold-exercises setup-matt-pocock-skills setup-pre-commit setup-ts-deep-modules tdd teach to-questionnaire to-spec to-tickets triage wait-what wayfinder wizard writing-beats writing-for-agents writing-fragments writing-shape'

missing=''
for skill in $skills; do
  if [ ! -f "$HOME/.agents/skills/$skill/SKILL.md" ]; then
    missing="$missing $skill"
  fi
done

if [ -z "$missing" ]; then
  printf '%s\n' 'All mattpocock/skills present.'
  exit 0
fi

for skill in $missing; do
  npx -y skills add mattpocock/skills --global --yes --skill "$skill" --agent '*'
done
