# GitHub permalink copier — terminal equivalent of VS Code GitLens
# "Copy Remote File URL at Line". SHA-pinned so links survive branch moves.
#
# Why a shell function instead of a plugin:
#   - No upstream oh-my-zsh plugin exists for this
#   - Third-party `gh-permalink` extensions are 2-4 stars (too niche to trust)
#   - This is simple enough to own
#
# Usage:
#   ghp src/app.ts          → file permalink
#   ghp src/app.ts 42       → single line
#   ghp src/app.ts 42-50    → line range
#
# Caveat: commit must be pushed to origin, or the URL will 404.
ghp() {
  local file="$1"
  local lines="$2"
  if [[ -z "$file" ]]; then
    print -u2 "usage: ghp <file> [<line>|<start>-<end>]"
    return 2
  fi
  local sha repo url
  sha=$(git rev-parse HEAD) || return 1
  repo=$(gh repo view --json nameWithOwner -q .nameWithOwner) || return 1
  url="https://github.com/$repo/blob/$sha/$file"
  if [[ -n "$lines" ]]; then
    if [[ "$lines" == *-* ]]; then
      url="$url#L${lines%-*}-L${lines#*-}"
    else
      url="$url#L$lines"
    fi
  fi
  printf '%s' "$url" | pbcopy
  printf 'Copied: %s\n' "$url"
}
