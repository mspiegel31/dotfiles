"""Helper functions for working with git on local filesystem"""
import subprocess

from pr_another_repo.settings import settings


def init_git_user():
    git_user = settings.action_inputs.user_name
    git_email = settings.action_inputs.user_email
    subprocess.run(["git", "config", "--global", "user.name", git_user], check=True)
    subprocess.run(["git", "config", "--global", "user.email", git_email], check=True)
