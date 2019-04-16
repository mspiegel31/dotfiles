# Dotfiles
portable configuratio at its finest

# Prerequisites
1. install the `stow` package from your package manager. [Stow website](https://www.gnu.org/software/stow/stow.html)

# Installation 
1. `cd dotfiles`
1. `stow <desired package>`

## Installation snippets
1. vscode [command reference](https://github.com/Microsoft/vscode/issues/42994#issuecomment-453184324) - `cat vscode/extensions.list | grep -v '^#' | xargs -L1 code --install-extension`