# Marimo cells in a briefing

Every executable thing in a briefing is a marimo cell run through the `quarto-marimo` extension. Three uses, in increasing weight: a **code example** (code shown, output real, no widgets), a **diagram** (`mo.mermaid`, optionally driven by a control), and an **island** (widgets the reader manipulates). Islands need `BRIEF.md` § Interaction to name what the reader changes and what they observe.

## Document setup

Front matter must declare the engine and the sandbox dependencies:

```yaml
---
title: ...
engine: marimo
pyproject: |
  requires-python = ">=3.11"
  dependencies = [
    "marimo>=0.23.16",
  ]
---
```

Add third-party packages to `dependencies`. Only packages with Pyodide wheels run in the reader's browser; pure-Python packages and the scientific stack (numpy, pandas, polars, altair) are safe. Check with `uv run marimo check --select MW scratch.py` on an extracted cell.

## Cell syntax

````markdown
```python {.marimo}
import marimo as mo
```

```python {.marimo}
k = mo.ui.slider(1, 20, value=5, label="trials (k)")
k
```

```python {.marimo}
p = 0.6
mo.md(f"pass@{k.value} = {1 - (1 - p) ** k.value:.3f}")
```
````

Rules:
- Cells share one namespace; a name is defined in exactly one cell.
- The last expression of a cell is its output. UI elements render where they are the last expression.
- Read `.value` of a UI element only in a *different* cell from the one that defines it.
- Hide code from the reader with `{.marimo hide_code="true"}` when the code is not the point.

## Code examples

A code example is a cell with the code visible and no `mo.ui` element. The last expression, or anything printed, renders under the code at render time; the reader sees real output and can also edit and re-run it in the browser.

````markdown
```python {.marimo}
import numpy as np

rewards = np.array([0.9, 0.6, 0.4, 1.0, 0.45])
successes = int((rewards >= 0.5).sum())
print(f"successes={successes}  pass_rate={rewards.mean():.2f}")
```
````

Rules:
- Sample data is literal and small enough to read; a reader must be able to check the output by eye.
- One computation per cell. A cell that prints three unrelated numbers is three cells.
- Prose after the cell reads the output back in one or two sentences. A code example does not need the three-part interpretation an island needs.
- Imports go in the cell that first uses them unless two or more cells share them; then a first `hide_code` cell holds the imports.

## Diagrams

`mo.mermaid(text)` renders a Mermaid diagram from a string. In a reactive cell the string can depend on a control, so a dropdown rebuilds the diagram and highlights one path:

````markdown
```python {.marimo hide_code="true"}
client = mo.ui.dropdown(["Anthropic SDK", "OpenAI SDK"], value="Anthropic SDK", label="client")
client
```

```python {.marimo hide_code="true"}
hit = "rule2" if client.value == "Anthropic SDK" else "rule1"
mo.mermaid(f"""
flowchart LR
  req[request] --> rule1[rule 1: x-ai-eg-model] --> conv[Converse]
  req --> rule2[rule 2: + anthropic-version] --> inv[InvokeModel]
  style {hit} stroke-width:4px
""")
```
````

Rules:
- Edges are labelled with what flows (a header, a body field, a decision), not just drawn. Boxes alone are the trap named in `type-architecture.md`.
- The control's default value produces the diagram the summary text describes, so the static render matches the prose.
- A diagram driven by a control is an island and needs the interpretation prose below. A diagram with no control is a code example and needs one readback sentence.
- Static alternative when no control is wanted: a Quarto ```` ```{mermaid} ```` block, rendered without Pyodide.

## Required prose after every island

Immediately after the last cell of the island, before any heading:

1. What to change ("move *k* from 1 to 10").
2. What to observe ("the estimate climbs steeply then flattens").
3. What it means for the question in `BRIEF.md` ("a fifth retry buys less than the second; budget accordingly").

## Static fallback

Non-HTML formats and readers without JavaScript see the cell's server-rendered output at its default input values. Choose defaults that make the static view meaningful on its own.

## Constraints

- The rendered site must be served over HTTP for cells to run; `file://` shows the static render only.
- Hidden cells are present in the HTML source. Do not put anything private in a cell.
- One island (widgets) per briefing is the norm. Two needs a reason in `BRIEF.md`. Code examples and diagrams are not counted.
