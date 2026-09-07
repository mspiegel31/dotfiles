---
name: prefer-off-the-shelf-utils
description: "Search npm/PyPI/Maven/etc. for an established utility library before hand-rolling search, parse, sort, or traversal logic in any language"
condition: ["hand-rolled", "roll(?:ing)? (?:my|our) own", "write a (?:custom|simple) (?:parser|walker|sort|search)", "implement(?:ing)? (?:a|our own) (?:recursive )?(?:walk|parser|sort|search)"]
scope: ["thinking", "text"]
---

Before writing custom logic for searching, parsing, sorting, tree-walking, diffing, or similar generic data-manipulation tasks, check whether the language's standard library or an already-available/well-known package (npm, PyPI, Maven, crates.io, etc.) already solves it. Prefer that over a hand-rolled implementation regardless of language. Only write custom logic when no suitable stdlib or off-the-shelf library exists for the case, and state that explicitly before proceeding.