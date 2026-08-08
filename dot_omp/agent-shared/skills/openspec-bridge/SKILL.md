---
name: openspec-bridge
description: "DISABLED — Bridge between OpenSpec spec-driven workflow and OMO multi-agent execution. Teaches Sisyphus to load OpenSpec artifacts, delegate tasks in parallel, and verify against spec requirements. Do not use: the OMO/Sisyphus runtime it targets is not present here. Remove the DISABLED prefix to enable."
---

# OpenSpec + OMO Integration Bridge

This skill teaches Sisyphus to use OpenSpec change artifacts as the planning input for OMO's parallel multi-agent execution. It **bypasses Prometheus** — OpenSpec's propose phase already did the planning work.

## When This Skill Activates

At session start, check for an `openspec/` directory in the project root. If absent, this skill is irrelevant — ignore it entirely and proceed normally.

## Phase 1: Load OpenSpec Context (MANDATORY before any implementation)

Before writing ANY code, load the active change context:

1. Run `openspec list --json` to get all active changes and their status
2. If the user specified a change name, use that. Otherwise:
   - If ONE active change exists with incomplete tasks → auto-select it
   - If MULTIPLE active changes → ask the user which one to work on
3. Read ALL artifacts for the selected change, in order:
   ```
   openspec/changes/<name>/proposal.md    — the requirements and motivation
   openspec/changes/<name>/design.md      — architecture decisions (if exists)
   openspec/changes/<name>/tasks.md       — the task checklist to implement
   openspec/specs/*/spec.md               — relevant capability specs (scan for specs referenced in proposal/design)
   ```
4. Parse `tasks.md` — identify all unchecked tasks (`- [ ]`), their descriptions, and any subtasks

## Phase 2: Assess Task Independence

Before delegating, analyze the task list:

- **Independent tasks**: No shared files, no output dependencies between them → CAN parallelize
- **Dependent tasks**: Task B needs Task A's output (e.g., "create schema" before "write migration") → MUST sequence
- **Shared-file tasks**: Multiple tasks modify the same file → sequence to avoid merge conflicts

Group tasks into **execution waves** (like Atlas does with Prometheus plans):
- Wave 1: All independent tasks (parallel)
- Wave 2: Tasks that depend on Wave 1 outputs (parallel within wave)
- Continue until all tasks assigned to a wave

## Phase 3: Parallel Execution via task()

For each wave, spawn tasks in parallel:

```typescript
// Independent tasks within a wave — fire simultaneously
task(
  category="<match task domain>",  // visual-engineering for UI, quick for simple, etc.
  load_skills=["openspec-bridge"],
  run_in_background=true,
  description="<short task summary>",
  prompt=`
    TASK: Implement this specific task from an OpenSpec change.

    ## Task
    <paste the specific task description from tasks.md>

    ## Spec Context (read-only reference — do NOT modify these files)
    <paste relevant excerpts from proposal.md, design.md, and spec.md>

    ## Constraints
    - Implement ONLY this specific task — nothing else
    - Do NOT modify any files under openspec/ directory (specs are requirements, not output)
    - Do NOT create .sisyphus/plans/ or invoke Prometheus — planning is already done
    - Follow existing codebase patterns and conventions
    - Run lsp_diagnostics on all changed files before reporting completion

    ## Acceptance Criteria
    <paste from tasks.md if the task has explicit criteria>

    EXPECTED OUTCOME: <concrete deliverable description>
    MUST DO: <specific requirements from the spec>
    MUST NOT DO: Modify openspec/ files, create new planning artifacts, refactor unrelated code
  `
)
```

**Category selection guide for task delegation:**

| Task description contains | Category |
|---|---|
| UI, CSS, styling, layout, components, animation | `visual-engineering` |
| Complex algorithm, architecture decision, data modeling | `ultrabrain` |
| Single file change, config tweak, rename | `quick` |
| Multi-file feature, integration work | `unspecified-high` |
| Tests, documentation | `unspecified-low` |

## Phase 4: Collect Results and Verify

As background tasks complete (via `<system-reminder>` notifications):

1. Call `background_output(task_id="...")` to retrieve results
2. Verify the subagent's work:
   - Did it complete the task as described?
   - Are there lsp_diagnostics errors on changed files?
   - Does the implementation align with the spec requirements?
3. If verification fails → continue the session:
   ```typescript
   task(session_id="<previous_session>", load_skills=[], run_in_background=false,
        prompt="Verification failed: <specific issue>. Fix this.")
   ```

## Phase 5: Update Task Checkboxes

After each task is verified complete, update `openspec/changes/<name>/tasks.md`:
- Change `- [ ]` to `- [x]` for the completed task
- Do NOT modify any other content in the file

## Phase 6: Completion and Archive

When ALL tasks in tasks.md are checked off:

1. Run a final verification pass — read all changed files, confirm they satisfy the spec
2. If the project has build/test commands, run them
3. Report completion to the user with a summary of what was done
4. Suggest: "All tasks complete. Run `/opsx:archive <change-name>` to archive this change and merge delta specs."

## Critical Rules

### MUST DO
- Always read OpenSpec artifacts BEFORE implementing anything
- Always check task independence BEFORE parallelizing
- Always pass spec context to subagents so they understand the requirements
- Always update task checkboxes in tasks.md after verified completion
- Always run lsp_diagnostics on changed files

### MUST NOT
- Do NOT modify files under `openspec/specs/` — these are requirements, not implementation output
- Do NOT modify `openspec/changes/<name>/proposal.md` or `design.md` — these are planning artifacts
- Do NOT create `.sisyphus/plans/` — OpenSpec's change folder IS the plan
- Do NOT invoke Prometheus or `/start-work` — this skill replaces that pipeline
- Do NOT run `openspec archive` automatically — let the user decide when to archive
- Do NOT proceed with implementation if tasks.md has zero unchecked tasks
