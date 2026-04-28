# opencode session browser (kasbah/opencode-session-search)
#
# Workaround: the upstream binary hardcodes dirs::data_dir() which on macOS
# resolves to ~/Library/Application Support/opencode/opencode.db, but
# opencode itself uses XDG (~/.local/share/opencode/opencode.db) on macOS.
# The --db flag bypasses the default.
#
# See: https://github.com/kasbah/opencode-session-search
alias oss='opencode-session-search --db $HOME/.local/share/opencode/opencode.db'
