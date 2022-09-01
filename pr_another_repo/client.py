from github import Github

from pr_another_repo.settings import settings

gh = Github(settings.action_inputs.github_api_token.get_secret_value())
