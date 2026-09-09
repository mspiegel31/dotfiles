# technology — an infrastructure technology, protocol, or platform capability (Kubernetes, gRPC, WebAssembly, a cloud service)

## Default Diátaxis mode
`guide`.

## Worked-example shape
One **failure scenario**: a realistic configuration → the event that stresses it → what the technology does, step by step, with the mechanism named at each step → the observable outcome → what changing one parameter would have done. Failure is used because it exposes the mechanism; a happy path hides it. Show real manifests, packets, or API payloads, not prose descriptions of them.

## What counts as primary
1. The specification or the project's official documentation for the version in scope.
2. The reference implementation's source or design docs (KEPs, RFCs, ADRs).
3. The cloud vendor's documentation for the managed variant, if the reader runs one.
4. Peer-reviewed or vendor-published measurements for performance claims.

Blog posts explaining the technology are not primary even when correct; cite the spec they paraphrase.

## Natural islands
Capacity and scheduling arithmetic (replicas × requests vs node capacity), timing (backoff, TTL, lease expiry) where the reader benefits from moving one value and watching the outcome flip.

## Known traps
- Explaining the abstraction without the mechanism underneath it.
- Conflating the open-source project with a vendor's managed offering.
- Stale defaults: many defaults changed across major versions. Cite the version.
