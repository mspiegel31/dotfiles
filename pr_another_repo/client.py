from github import Github
from ghapi.all import GhApi
from pr_another_repo.settings import settings

gh = Github(settings.action_inputs.github_api_token.get_secret_value())
ghapi = GhApi(
    owner=settings.action_inputs.destination_owner,
    repo=settings.action_inputs.destination_repo,
    token=settings.action_inputs.github_api_token.get_secret_value()
    )
