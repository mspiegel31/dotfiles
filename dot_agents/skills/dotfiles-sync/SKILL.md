---
name: dotfiles-sync
description: Synchronizes a chezmoi-managed dotfiles repository across destination, source, and remote states. Uses native chezmoi commands for inspection, capture, merge, pull, preview, and apply; uses a zero-dependency helper to classify drift and conflicts. Use when propagating GUI or local dotfile edits, pulling dotfiles changes, reconciling source and destination drift, or checking what is out of sync. Don’t use for general Git repositories or initial chezmoi installation.
---

# Dotfiles Sync

Treat chezmoi's destination, source, and target as distinct states. Prefer chezmoi's command for every state transition. Use `scripts/dotfiles_sync.py` only to classify drift, batch safe re-adds, and turn Git porcelain into direct prose.

## Procedures

**Step 1: Inspect once**
1. Run `python3 scripts/dotfiles_sync.py inspect [TARGET...]`.
2. Report its counts and named paths in plain language. Use `chezmoi status` or `chezmoi git -- status` directly only when the helper fails or the user asks for raw output.
3. Treat these helper groups as decisions:
   - `Ready for native re-add`: destination changed, its plain source file is clean.
   - `Templates requiring native merge`: destination changed, source ends in `.tmpl`.
   - `Destination and source both changed`: both sides contain local work; the user must choose or merge.
   - `Pending destination apply`: source target differs from the destination.
   - `Changes requiring review`: deletion or an unsupported state; inspect it before writing.
4. Complete this step when every reported path belongs to one group and all source repository conflicts are named.

**Step 2: Clear source repository conflicts**
1. If the helper reports `Source repository conflicts`, stop all capture, pull, and apply operations.
2. State each conflicted path and its two-character Git status. Use the `resolving-merge-conflicts` skill to resolve it.
3. Re-run `python3 scripts/dotfiles_sync.py inspect` after resolution.
4. Complete this step when the helper reports no source repository conflicts.

**Step 3: Capture destination edits**
1. For `Ready for native re-add`, run `python3 scripts/dotfiles_sync.py capture [TARGET...]`. The helper invokes one native `chezmoi re-add` for the safe batch.
2. For `Templates requiring native merge`, prefer `chezmoi merge TARGET...` when the configured merge tool can run in the current terminal. Preserve template actions and secret lookups in the source result.
3. When an interactive merge tool cannot run, use native commands to expose the states:
   - `chezmoi source-path TARGET` locates the source template.
   - `chezmoi cat TARGET` renders the target state only when the template has no secret lookup.
   - The destination path contains the actual state.
   For a secret-bearing template, compare only the non-sensitive structure needed for the requested change. Edit the source template with the smallest change that reproduces the intended destination behavior.
4. For `Destination and source both changed`, show a short semantic comparison and ask which behavior to keep. Do not infer a winner from timestamps or formatting.
5. For deletions and unsupported states, inspect the target with `chezmoi status TARGET` and `chezmoi source-path TARGET`. State whether the operation would remove source state, destination state, or both before acting.
6. Never use `chezmoi add --force` on a template. It can replace the template with rendered destination content. `chezmoi re-add` is safe but skips templates.
7. Re-run the helper. Complete this step when no intended destination edit remains uncaptured.

**Step 4: Pull remote source changes when requested**
1. Pull without applying: run `chezmoi update --apply=false`. This uses chezmoi's configured update command or its native `git pull --autostash --rebase` path.
2. If the pull reports a conflict, return to Step 2. Quote only the failing paths and the native error's useful line.
3. Run `python3 scripts/dotfiles_sync.py inspect` after a successful pull.
4. Complete this step when remote changes are present in the source tree and every destination effect is classified.

**Step 5: Preview and apply source changes**
1. Run `chezmoi status` for the concise operation list.
2. Run `chezmoi diff --skip-secrets` for content review. For a known secret-bearing target, inspect source structure instead of printing rendered content.
3. Run `chezmoi apply --dry-run --verbose --skip-secrets` when scripts, removals, or permission changes appear. Summarize the operations; do not paste noisy output. Review secret-bearing targets structurally and without verbose rendered output.
4. Run `chezmoi apply [TARGET...]` only when the user requested application or approved the preview. An explicit request to sync or apply the named targets is approval; do not ask twice.
5. Re-run `python3 scripts/dotfiles_sync.py inspect`.
6. Complete this step when the intended destination changes are applied and any remaining source repository changes are named.

**Step 6: Commit or push only when requested**
1. Load the `git-master` skill before any commit, rebase, or push.
2. Use `chezmoi git -- status`, `chezmoi git -- diff`, and `chezmoi git -- <git-args>` when the working directory is outside the source repository. This keeps Git anchored to chezmoi's source tree.
3. Keep unrelated source changes out of the commit. Never enable automatic push as part of a one-off sync.
4. Complete this step when the requested remote operation succeeds and the source repository state is reported.

## Fast paths

- Check drift: `python3 scripts/dotfiles_sync.py inspect`
- Capture safe plain files: `python3 scripts/dotfiles_sync.py capture`
- Pull, review, apply: `chezmoi update --apply=false`, `chezmoi diff --skip-secrets`, `chezmoi apply`
- Edit source first: `chezmoi edit --apply TARGET`

Read `references/chezmoi-commands.md` only when command behavior is unclear, encryption is involved, or a failure changes the safe path.

## Reporting format

Use four short blocks and omit empty ones:

```text
Dotfiles: <clean | changes found | attention needed | merge conflicts>.
Destination: <count and paths, grouped by capture/merge/apply>.
Source: <count and paths, including conflict status>.
Next: <one native command or one user decision>.
```

Describe behavior changes, not serialization noise. Call out a formatting-only diff only when applying it would still rewrite the file.

## Error Handling

- Helper cannot find chezmoi: install or locate chezmoi, then pass `--chezmoi PATH`. Do not reproduce chezmoi behavior in Python.
- Native command fails: preserve its exit code, report the useful diagnostic, and stop that state transition.
- Template cannot render: inspect the source error. `chezmoi merge` can fall back to a two-way merge; verify the template renders before applying.
- Secret lookup fails: stop. Do not replace the lookup with rendered plaintext or print the destination value.
- Helper reports `Destination and source both changed`: ask which behavior wins or perform a three-way merge. Never overwrite either side silently.
- Final inspect still reports drift: state the exact remaining group and path. Do not call the sync complete.
