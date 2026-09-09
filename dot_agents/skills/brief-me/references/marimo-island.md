# Marimo islands in a briefing

An island is one or more reactive marimo cells embedded in `index.qmd` through the `quarto-marimo` extension. Use one only when `BRIEF.md` § Interaction names what the reader changes and what they observe.

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

## Required prose after every island

Immediately after the last cell of the island, before any heading:

1. What to change ("move *k* from 1 to 10").
2. What to observe ("the estimate climbs steeply then flattens").
3. What it means for the question in `BRIEF.md` ("a fifth retry buys less than the second; budget accordingly").

## Static fallback

Non-HTML formats and readers without JavaScript see the cell's server-rendered output at its default input values. Choose defaults that make the static view meaningful on its own.

## Constraints

- The rendered site must be served over HTTP for islands to run; `file://` does not work.
- Speaker notes and hidden cells are present in the HTML source. Do not put anything private in a cell.
- One island per briefing is the norm. Two needs a reason in `BRIEF.md`.
