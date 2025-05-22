function aws_choose_role ()
{
  NAMESPACE=spoton
  AWS_CONFIG_FILE="$HOME/.aws/config"
  AWS_CREDENTIALS_FILE="$HOME/.aws/credentials"
  cat "${AWS_CREDENTIALS_FILE:-~/.aws/credentials}" "${AWS_CONFIG_FILE:-~/.aws/config}" 2> /dev/null | crudini --get - | sed 's/^ *profile *//' | fzf --height 30% --reverse --select-1 --prompt='-> ' --tiebreak='begin,index' --header 'Select AWS profile' --query "${ASSUME_ROLE_INTERACTIVE_QUERY:-${NAMESPACE:+${NAMESPACE}-}${STAGE:+${STAGE}-}}" 
}

aws_sdk_assume_role ()
{
  local role=$1;
  [[ -z $role ]] && role=$(aws_choose_role);
  if [ -z "${role}" ]; then
    echo "Usage: assume-role <role> [command...]";
    return 1;
  fi;
  export AWS_PROFILE="$role"
}

assume-role ()
{
  aws_sdk_assume_role "$@"
}

alias ar=assume-role
