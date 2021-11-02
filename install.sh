sudo apt update && sudo apt-get -y install stow

#==============
# Delete existing dot files and folders
#==============
sudo rm -rf ~/.zshrc > /dev/null 2>&1
stow zsh