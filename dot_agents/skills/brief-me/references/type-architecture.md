# architecture — how a specific system is put together and why (the AI Gateway, a deployment pipeline, a service mesh as deployed here)

## Default Diátaxis mode
`explanation`. Use `reference` when `decision` is "operate" and the reader needs lookup tables more than narrative.

## Worked-example shape
One **request trace**: a concrete request or event enters the system → each component it touches, in order, with what that component reads, decides, and emits → where it can be rejected or rerouted → the response. Follow with the same trace under one changed condition (a different model, a failed dependency, a feature flag) to show which components are sensitive to it. Name real components, real config keys, and real files; link each to its source in the ledger.

## What counts as primary
1. The system's own configuration and source (manifests, routes, code) at a named commit.
2. Design documents, ADRs, and runbooks written by the owning team.
3. Vendor documentation for each managed component on the path.
4. Observability data (a real trace, a dashboard export) for latency or volume claims.

Slack threads and meeting notes are community sources: allowed for intent and history, labelled, never for current behaviour.

## Natural islands
Rarely. An island is justified when routing or capacity depends on a parameter the reader controls and the decision logic can be reproduced faithfully in a few lines of Python (e.g. "which backend does model X with N tools hit").

## Known traps
- Diagrams that show boxes without showing what flows between them.
- Describing the intended architecture from the design doc when the deployed config differs. Trace the config.
- Omitting the failure paths; they are usually the reason the architecture looks the way it does.
