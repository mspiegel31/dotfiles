# Terminal notification for an active `ar`/`assume-role` session, ported from
# cloudposse/geodesic (verified 2026-07-21):
#   - iTerm2 badge:  rootfs/etc/profile.d/iterm.sh
#   - Prompt marker: rootfs/etc/profile.d/prompt.sh (geodesic_prompt)
#
# zsh has a native precmd_functions hook, so this skips geodesic's
# PROMPT_COMMAND-array hack (that workaround exists only because bash has no
# equivalent). Uses RPROMPT so it doesn't fight the oh-my-zsh theme's PROMPT.

if [[ "$TERM_PROGRAM" == "iTerm.app" && -n "$ASSUME_ROLE" ]]; then
  printf '\e]1337;SetBadgeFormat=%s\a' "$(printf '%s' "$ASSUME_ROLE" | base64)"

  function _assume_role_iterm_exit() {
    local status=$?
    printf '\e]1337;SetBadgeFormat=\a'
    echo 'Goodbye'
    return $status
  }
  trap _assume_role_iterm_exit EXIT
fi

function _assume_role_prompt_segment() {
  # Recompute the existing kubectx segment (~/dotfiles/zsh/.zshrc sets
  # RPS1='$(kubectx_prompt_info)' — same variable as RPROMPT) instead of
  # clobbering it, so both segments compose.
  local kube_segment=""
  (( $+functions[kubectx_prompt_info] )) && kube_segment="$(kubectx_prompt_info)"
  local kube_display=""
  [[ -n "$kube_segment" ]] && kube_display="☸️  %F{blue}${kube_segment}%f"

  local role_segment=""
  [[ -n "$ASSUME_ROLE" ]] && role_segment="☁️  %F{208}[${ASSUME_ROLE}]%f"

  if [[ -n "$kube_display" && -n "$role_segment" ]]; then
    RPROMPT="${kube_display}  ${role_segment}"
  else
    RPROMPT="${kube_display}${role_segment}"
  fi
}
precmd_functions+=(_assume_role_prompt_segment)
