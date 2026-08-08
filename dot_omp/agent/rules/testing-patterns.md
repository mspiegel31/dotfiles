---
name: testing-patterns
description: Project testing conventions. Use when writing or reviewing tests.
---

# Testing Patterns

### Test Style Preference

**Favor property-based tests (or generative variants) over example-based unit
tests.** Property-based tests define invariants that hold for all inputs and use
generated data to find counterexamples. Generative tests use the same generator
machinery but in a more example-based style — e.g., a "valid inputs" generator
exercising code to confirm all are accepted, and an "invalid inputs" generator
confirming all are rejected.

### Property-Based & Generative Testing Patterns

1. **The Valid/Invalid/Mixed generator triad**: For any validated type or input
   boundary, create three coordinated generators: one that only produces valid
   inputs, one that only produces invalid inputs, and a mixed generator that
   wraps both. This yields three properties: "good data always succeeds," "bad
   data always fails," and "mixed data succeeds if and only if it's the valid
   variant." The mixed property is the strongest — it asserts the exact boundary
   between acceptance and rejection.

2. **Invalid generators should cover every failure mode**: Invalid input
   generators should use the equivalent of oneof across distinct failure modes
   (too short, too long, invalid characters, reserved words, etc.) rather than
   just generating random bad data. This exercises all rejection branches, not
   just the easiest-to-hit path.

3. **Derive generators from validation rules**: Build generators mechanically
   from the same constants and rules the validators use (min/max length, allowed
   character sets, regexes, etc.). Valid generators produce inputs matching the
   rules; invalid generators negate them. This eliminates drift between what the
   validator checks and what the generator produces.

4. **Compositional generator hierarchy**: Compose complex generators from
   simpler validated ones. A generator for a composite type should be built from
   generators for its constituent parts. Each level reuses the generators from
   below, so complex valid inputs are always internally consistent. This is the
   property-based testing equivalent of builder patterns.

5. **Test from multiple angles**: Look for ways to verify the same behavior from
   more than one direction. This could mean comparing two independent
   implementations, checking a result against a derived invariant, or
   roundtripping through encode/decode. Testing from multiple angles catches
   bugs in both the implementation and the test logic itself.

6. **Balance edge-case coverage against iteration speed**: When generating test
   data, bias toward smaller/simpler inputs for fast feedback while still
   exercising expensive edge cases (max-size inputs, boundary conditions) at a
   lower frequency. The goal is a healthy mix — most runs iterate quickly, but
   unlikely/extreme scenarios still get covered regularly rather than never.

7. **Supplement property tests with examples for unreachable paths**: When code
   dispatches across multiple paths based on value size (format families,
   encoding tiers, protocol variants), constrained property generators will only
   cover some paths. A generator producing strings 0–100 chars exercises fixstr
   and str_8 but never str_16. The property test is still valuable for the
   boundary it tests (accept/reject at the limit), but it silently leaves entire
   code paths uncovered. Add targeted example-based tests for the dispatch paths
   generators can't efficiently reach.

### Counterfactual Testing ("Make It Fail, Make It Pass")

**Always do this** after writing new tests unless you truly can't find a way to
break the assertion and are very confident the test is correct. A test you've
never seen fail is a test you don't trust. Temporarily break each assertion to
confirm it actually fires:
1. Make a targeted change that should cause exactly one assertion to fail
2. Run tests, confirm the expected failure message
3. Revert the change

**Key takeaway**: When a counterfactual passes (assertion doesn't fire), that's
the most valuable outcome — it means the assertion is weak/wrong. Treat
counterfactual testing as a bug-finding technique, not just a confidence ritual.

**Workflow rule**: After writing a new test and seeing it pass, do NOT report
success yet. First, do a counterfactual check. Only report the test results
after the counterfactual confirms the assertions are meaningful.

### Debugging Discipline

**"How do you know that you know that?"**: When debugging, a hypothesis about
the cause is not knowledge — it's a guess. Never act on an unverified
hypothesis. Before investing effort in workarounds or fixes, validate
empirically that your suspected cause is actually the cause. Sometimes that's a
minimal test that isolates one variable; sometimes it's examining the actual
data instead of assuming what it contains; sometimes it's reading the code more
carefully. The method varies, but the discipline doesn't: verify first, then
act.

**Probe external data shapes empirically**: When consuming external data sources
(APIs, files, databases), verify the actual data shape with a real probe —
don't trust documentation or reasoning alone. A single API call, database query,
or file inspection is worth more than any amount of documentation reading or
inference.

**CI is the source of truth for build status**: A local build failure does not
mean the build is broken. Local toolchain versions, stale dependency caches, and
environment differences can all cause local failures that don't reproduce in CI.
Never declare a build "broken on main" based on local results — check CI first.
