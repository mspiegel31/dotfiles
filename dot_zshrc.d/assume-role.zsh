# Local port of cloudposse/geodesic's `assume-role` shell function, for use
# outside the spoton infra container. Source (verified 2026-07-21):
#   https://github.com/cloudposse/geodesic/blob/master/rootfs/etc/profile.d/aws.sh
#
# Credential-mechanism-agnostic: works the same whether the AWS profile is
# native SSO (`aws configure sso`) or SAML/role-chained — it just scopes
# AWS_PROFILE and lets the AWS SDK resolve credentials.
#
# Usage:
#   ar                              -> fzf-pick a profile, open a subshell scoped to it (exit to leave)
#   ar staging-poweruser            -> open a subshell scoped to that profile
#   ar staging-poweruser chamber list foo   -> run one command scoped to the profile; no export, no leak
#
# Note: this does not refresh AWS SSO tokens. Run `aws sso login --profile <name>`
# once per SSO session (shared across all profiles under the same sso-session)
# before assuming a role whose token has expired.

# Only lists profiles backed by an sso_session (native AWS SSO). Legacy
# source_profile-chained profiles (spoton-gbl-*-identity chains) require the
# geodesic/SAML identity and just fail with InvalidClientTokenId locally.
function aws_choose_role() {
  awk -v RS='' '
    /\[profile / && /sso_session[ \t]*=/ {
      line=$0
      sub(/.*\[profile[ \t]+/, "", line)
      sub(/\].*/, "", line)
      print line
    }
  ' "${AWS_CONFIG_FILE:-$HOME/.aws/config}" |
    fzf --height 30% --reverse --select-1 --prompt='-> ' \
      --tiebreak='begin,index' --header 'Select AWS profile (native SSO only)' \
      --query "${ASSUME_ROLE_INTERACTIVE_QUERY:-}"
}

function assume-role() {
  local role=$1
  [[ -n $role ]] && shift

  [[ -z $role ]] && role=$(aws_choose_role)
  if [[ -z $role ]]; then
    echo "Usage: assume-role <profile> [command...]"
    return 1
  fi

  if [[ $# -eq 0 ]]; then
    echo "# Assuming $role (subshell — exit to return)"
    AWS_PROFILE="$role" ASSUME_ROLE="$role" zsh
  else
    AWS_PROFILE="$role" "$@"
  fi
}
alias ar=assume-role
