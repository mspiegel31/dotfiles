---
name: skill-creator
description: Authors and structures professional-grade agent skills following the agentskills.io spec. Use when creating new skill directories, drafting procedural instructions, or optimizing metadata for discoverability. Don't use for general documentation, non-agentic library code, or README files.
---

# Skill Authoring Procedure

Follow these steps to generate a skill that adheres to the agentskills.io specification and progressive disclosure principles.

## Step 1: Initialize and Validate Metadata
1.  Define a unique `name`: 1-64 characters, lowercase, numbers, and single hyphens only. **This must exactly match the parent directory name** (e.g., name `angular-testing` must live in `angular-testing/SKILL.md`).
2.  Draft a `description`: Max 1,024 characters, written in the third person. Include positive triggers ("Use when...") and negative triggers ("Don't use for...").
3.  **Execute Validation Script:** Run the validation script to ensure compliance before proceeding:
    `python3 scripts/validate-metadata.py --name "[name]" --description "[description]"`
4.  If the script returns an error, self-correct the metadata based on the `stderr` output and re-run until successful.

## Step 2: Structure the Directory
1.  Create the root directory using the validated `name`.
2.  Initialize the following subdirectories:
    *   `scripts/`: For tiny CLI tools and deterministic logic.
    *   `references/`: For flat (one-level deep) context like schemas or API docs.
    *   `assets/`: For output templates, JSON schemas, or static files.
3.  Ensure no human-centric files (such as `README.md`, `CHANGELOG.md`, `INSTALLATION.md`) are created.

## Step 3: Draft Core Logic (SKILL.md)
1.  Use the template in `assets/SKILL.template.md` as the starting point.
2.  Write all instructions in the **third-person imperative** (e.g., "Extract the text," "Run the build").
3.  **Apply Specific, Procedural Formatting:**
    *   **Use Step-by-Step Numbering:** Define the workflow as a strict chronological sequence. Clearly map out decision trees (e.g., *"Step 2: If source maps are required, run `scripts/build.sh`. Otherwise, skip to Step 3."*).
    *   **Provide Concrete Templates:** If an output has a specific structure, place a template in `assets/` and command the agent to copy its structure instead of describing it in text.
    *   **Use Consistent, Specific Terminology:** Use identical, domain-native terms throughout (e.g., in Angular, use "template" consistently instead of "html" or "view").
4.  **Enforce Progressive Disclosure:**
    *   Keep the main `SKILL.md` under 500 lines.
    *   If a procedure requires a large schema, complex rule set, or bulky context, move it to a flat file in `references/`.
    *   Command the agent to read the specific file only when needed (JiT loading): *"Read references/api-spec.md to identify the correct endpoint."*

## Step 4: Identify and Bundle Scripts
1.  Identify "fragile" tasks (regex, complex parsing, or repetitive boilerplate).
2.  Outline a single-purpose script for the `scripts/` directory designed as a tiny CLI with arguments.
3.  Ensure the script uses standard output (stdout/stderr) to communicate success or failure. **Write descriptive, human-readable error messages** so the agent knows exactly how to self-correct without user intervention.

## Step 5: Skill Composition (Optional)
If a skill is complex, compose it using router/subskill patterns:
1.  Create a high-level router skill.
2.  Define conditional triggers or procedures referencing other specific skill folders (e.g., "To build the client, see [path to client-build skill]. To build the server, see [path to server-build skill].").

## Step 6: Final Logic Validation
1.  Review the `SKILL.md` for "hallucination gaps" (points where the agent is forced to guess).
2.  Verify all file paths are **relative** and use forward slashes (`/`).
3.  Cross-reference the final output against `references/checklist.md`.

## Error Handling
*   **Metadata Failure:** If `scripts/validate-metadata.py` fails, identify the specific error (e.g., "STYLE ERROR") and rewrite the field to remove first/second person pronouns.
*   **Context Bloat:** If the draft exceeds 500 lines, extract the largest procedural block and move it to a file in `references/`.

