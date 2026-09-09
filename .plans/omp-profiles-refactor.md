# OMP profile refactor

## Problem

The default OMP root, `~/.omp/agent`, currently contains personal runtime configuration and state. Chezmoi manages the corresponding source as `dot_omp/private_agent/`, but it has drifted:

```text
chezmoi status ~/.omp
MM .omp/agent/config.yml
MM .omp/agent/mcp.json
MM .omp/agent/models.yml
```

The configuration also uses hostname conditionals to select work versus personal models and MCP servers. That is the wrong boundary for the desired behavior: the user needs isolated work and personal OMP identities on the same host.

## Facts established

| Category | Current live location | Managed source | Refactor treatment |
| --- | --- | --- | --- |
| Default config | `~/.omp/agent/config.yml` | `dot_omp/private_agent/config.yml.tmpl` | Promote current live config to `personal`; split work settings into a work profile config. |
| Default MCP disablement | `~/.omp/agent/mcp.json` | `dot_omp/private_agent/mcp.json.tmpl` | Profile-specific managed `mcp.json`; adopt the Portainer entry without its plaintext credential. |
| Default MCP servers | `~/.omp/agent/.mcp.json` | `dot_omp/private_agent/dot_mcp.json.tmpl` | Preserve as profile-managed servers; split work-only servers into `work`. |
| Default model providers | `~/.omp/agent/models.yml` | `dot_omp/private_agent/models.yml` | Promote live llama-swap/LiteLLM provider definitions and current model overrides to `personal`. |
| Portable agent config | `~/.agents/` | `dot_agents/` | Keep canonical here; each OMP profile gets a symlink to it. |
| Databases, session history, logs, caches, blobs, OAuth | `~/.omp/**` runtime directories/files | none | Never manage, copy, re-add, or template. Profiles own isolated runtime state. |

Existing profile directories are not valid configuration sources: `work` contains only an empty `rules/` directory; `personal` has only a `setupVersion` config and generated state. They must not be adopted wholesale.

## Target topology

Use native OMP profiles exclusively. Do not set `OMP_PROFILE` in shell startup files.

```text
~/.omp/
├── agent/                                # default-profile compatibility only
│   └── keybindings.*                     # optional shared keyboard mappings
└── profiles/
    ├── personal/
    │   └── agent/
    │       ├── config.yml
    │       ├── mcp.json
    │       ├── .mcp.json
    │       ├── models.yml
    │       └── agents -> ~/.agents/agents
    └── work/
        └── agent/
            ├── config.yml
            ├── mcp.json
            ├── .mcp.json
            ├── models.yml
            └── agents -> ~/.agents/agents
```

Chezmoi source mapping:

```text
dot_omp/
├── private_profiles/
│   ├── private_personal/private_agent/
│   │   ├── config.yml
│   │   ├── mcp.json
│   │   ├── dot_mcp.json
│   │   ├── models.yml
│   │   └── symlink_agents
│   └── private_work/private_agent/
│       ├── config.yml
│       ├── mcp.json
│       ├── dot_mcp.json
│       ├── models.yml
│       └── symlink_agents
└── private_agent/
    └── keybindings.*                     # only if one exists and is intentionally shared
```

Chezmoi strips `dot_` and `private_` prefixes. Therefore those nested source paths render to `~/.omp/profiles/{personal,work}/agent/`; `dot_mcp.json` renders as `.mcp.json`.

## Configuration ownership

### `personal`

Adopt the currently live default `config.yml` and `models.yml` as the personal baseline, including its current role choices, provider disablement, task overrides, LSP option, Codex reset setting, expanded llama-swap overrides, and LiteLLM provider.

Adopt the currently live `mcp.json` server policy and the non-work servers in live `.mcp.json`: Cloudflare, Cloudflare docs, Repomix, Context7, Chrome DevTools, Karakeep, Liftosaur, and Portainer.

Replace every currently materialized credential with the existing secure pattern before writing source:

- Chezmoi Bitwarden lookup for values already handled that way.
- `!cat $HOME/.config/agent-secrets/<name>` for a locally provisioned service credential, including Portainer.
- Never copy values from the live config into Git or plan output.

### `work`

Start from the same non-secret UX/runtime baseline as `personal`, but make it an ordinary YAML file, not a hostname template. Preserve the prior work-model intent from the existing source: SpotOn role mappings (default, smol, slow, plan, advisor, task, tiny) and the corresponding provider policy.

Place work-only MCP servers in `work/.mcp.json`: Slack, AWS Knowledge, Lucid, Rovo, Grafana via `tsh mcp connect`, Backstage, GitOps local MCP, SpotOn MCP, and Google Workspace. Keep their credentials as existing runtime secret commands. Make their disablement list in `work/mcp.json` consistent with which servers are enabled on the work machine.

The live tree does not contain a real work profile config, so it cannot establish current desired work model/MCP policy. The previous work branches of `config.yml.tmpl`, `mcp.json.tmpl`, and `dot_mcp.json.tmpl` are the only available evidence. Before the mutation phase, review those extracted values against current work requirements; do not invent work credentials or move personal servers into the work boundary.

### Shared portable configuration

`dot_agents/` remains the single canonical tree for portable skills, rules, commands, and agent definitions. Both profiles must use `agents -> ../../../../.agents/agents`, the same relative target used today, or the matching source-relative symlink path generated by Chezmoi. Do not copy `dot_agents` into either profile.

OMP profiles do not inherit default-profile skills/rules/commands/MCP or settings. The per-profile `agents` symlink is therefore required for OMP-native task agents, while shared portable capabilities continue at `~/.agents` under their existing discovery mechanism. Confirm exact discovery behavior during smoke testing rather than assuming one path substitutes for the other.

## Implementation sequence

1. **Snapshot and classify live state, read-only.**
   - Run `chezmoi source-path`, `chezmoi status ~/.omp`, and scoped `chezmoi diff` for each managed OMP configuration file.
   - Record only configuration-file paths and metadata. Explicitly exclude all files matching runtime databases, `*-wal`, `*-shm`, sessions, logs, cache, blobs, terminal sessions, native binaries, generated extension data, OAuth/token stores, and locks.
   - Compare the live default config against the target `personal` source content field by field. This is the adoption inventory.

2. **Replace the host split with profile trees.**
   - Create the two profile source trees shown above.
   - Move the live default personal semantics into `private_personal/private_agent/` as non-template config.
   - Extract prior work-only template branches into `private_work/private_agent/` as non-template config.
   - Delete hostname conditional blocks from all OMP configuration. Delete `.chezmoidata.yaml` host values only if no other template consumes them; otherwise retain them for those unrelated consumers.

3. **Preserve credential boundaries.**
   - Replace materialized live credentials with secret references before any file enters source control.
   - Use the `!cat` convention for locally provisioned secret files; retain Bitwarden template lookups where they are already established. Do not introduce a runtime secret into a managed rendered file as plaintext.
   - Inspect staged source for known secret strings and ensure `git diff` contains only references, never values.

4. **Remove default-profile configuration cleanly.**
   - Delete obsolete `dot_omp/private_agent/{config.yml.tmpl,models.yml,mcp.json.tmpl,dot_mcp.json.tmpl,symlink_agents}` after their content has been transferred to the profiles.
   - Do not preserve a default config alias or compatibility configuration. A bare `omp` is intentionally no longer the normal entry point.
   - If shared `keybindings.*` exist, manage only that file at the default root because OMP merges it into named profiles. Otherwise leave the default `agent` config absent.

5. **Add explicit launch commands.**
   - Add shell functions or aliases to the managed `dot_zshrc.tmpl` outside its hostname branches:

     ```zsh
     alias omp-personal='omp --profile personal'
     alias omp-work='omp --profile work'
     ```

   - Prefer these deterministic aliases over `OMP_PROFILE`; do not use OMP's `--alias` feature as the managed mechanism because it writes shell-specific configuration outside this repository. If using OMP's generated alias is preferred after testing, document its output location and arrange for Chezmoi to manage that file rather than allowing unmanaged drift.

6. **Cut over without importing runtime state.**
   - Apply only the new profile source trees and shell aliases.
   - Do not migrate the existing default sessions/auth databases. First launch establishes independent profile-local auth and state. This is the security boundary required by the use case.
   - Retain the existing `~/.omp/agent` runtime tree temporarily as an untracked rollback source. After acceptance, remove only stale managed default configuration files, not session/auth history, unless the user explicitly requests deletion.

7. **Update repository guidance.**
   - Replace the current `AGENTS.md` prohibition on `OMP_PROFILE`/`~/.omp/profiles/*` with: profiles are managed at `dot_omp/private_profiles/`; use explicit `omp-work` / `omp-personal`; do not set ambient `OMP_PROFILE`; profile runtime state is not managed.
   - Retain the rule that `dot_omp/private_agent/config.yml.tmpl` is no longer authoritative; profile `config.yml` files own model roles. Remove stale references to the old single-root layout and hostname-gated roles.

## Verification

Run these after implementation. They test OMP's actual configuration resolution, not merely rendered files.

1. **Chezmoi render and drift**

   ```sh
   chezmoi execute-template < dot_omp/private_profiles/private_personal/private_agent/config.yml
   chezmoi execute-template < dot_omp/private_profiles/private_work/private_agent/config.yml
   chezmoi diff ~/.omp
   chezmoi verify ~/.omp
   ```

   Expected: both configs are valid after rendering; no host conditional controls OMP profile behavior; no unexpected drift remains for managed configuration.

2. **Profile layout and secret safety**

   ```sh
   test -f ~/.omp/profiles/personal/agent/config.yml
   test -f ~/.omp/profiles/work/agent/config.yml
   test -f ~/.omp/profiles/personal/agent/mcp.json
   test -f ~/.omp/profiles/work/agent/mcp.json
   test -f ~/.omp/profiles/personal/agent/models.yml
   test -f ~/.omp/profiles/work/agent/models.yml
   ```

   Expected: all six files exist. Inspect source diffs to confirm no token literals appear.

3. **Launch routing**

   ```sh
   omp-personal -p --no-session 'Report the active default model role.'
   omp-work -p --no-session 'Report the active default model role.'
   ```

   Expected: each run resolves its own profile config; the work run exposes the intended work model role and the personal run exposes its local/personal role. `--no-session` avoids polluting either profile while proving resolution.

4. **MCP isolation**

   Run each profile once and inspect OMP's server listing/connection UI or its diagnostic command. Expected: work-only servers never appear in personal, personal-only servers never appear in work, shared servers appear only where deliberately declared.

5. **State isolation**

   Start one short named session under each alias. Verify new session/runtime artifacts are created below the corresponding `~/.omp/profiles/{name}/` tree and not under the other profile. Verify each profile requires its own authentication state where applicable.

6. **Portable configuration regression**

   Launch a task requiring a known managed skill/agent through each alias. Expected: portable `dot_agents` content still resolves once, without duplicated in-profile copies.

7. **Rollback**

   If config resolution, model routing, or MCP isolation fails, restore the saved default managed configuration source and reapply it; remove only the newly applied profile configuration files. Do not delete either profile's runtime/auth/session data during rollback.

## Acceptance criteria

- `omp-work` and `omp-personal` launch separate OMP profiles without ambient environment selection.
- Model roles, MCP visibility, auth, and session history are isolated by profile.
- The previously live personal configuration is adopted into Chezmoi without serializing secrets or runtime state.
- No OMP hostname conditionals remain; each profile is an explicit source tree.
- Portable agent configuration remains owned solely by `dot_agents/`.
- `chezmoi diff ~/.omp` and the launch/MCP/state smoke tests establish the cutover.
