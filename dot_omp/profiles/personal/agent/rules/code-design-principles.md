---
name: code-design-principles
description: Code design principles for new modules and refactors. Use when structuring or reviewing module boundaries and abstractions.
---

# Code Design Principles

1. **Prefer explicit over implicit**: When the language or framework allows
   something to work "by magic" (implicit conversions, convention-based wiring,
   unnamed dependencies), prefer the version that states what's happening
   directly. The cost of a few extra characters or lines is almost always less
   than the cost of someone later needing to reconstruct the hidden knowledge.
   Several principles below are specific applications of this idea.

2. **Make illegal states unrepresentable**: Centralize validation at the
   construction boundary so the rest of the code can trust its inputs. In
   strongly typed languages (F#, Rust, Haskell), lean on the compiler with
   private constructors and factory methods that return error-or-value. In
   dynamic languages (Python, Ruby, JS), enforce this by convention with
   validated types that are trustworthy once constructed. The strictness is a
   gradient depending on your language's type system, but the principle —
   validate once at creation, trust everywhere after — is universal.

3. **Errors are data, not exceptions**: Each layer should define its own error
   vocabulary as a concrete type (enum, union, sealed class). Higher-level
   errors wrap lower-level ones to preserve full context. Every error type
   should know how to describe itself as text. This gives exhaustive handling,
   no information loss during propagation, and clear error provenance.

4. **Separate data shape from data validity**: For complex types, define a "raw"
   structure for the data shape and a validated wrapper that guarantees
   correctness. Construction goes: raw shape -> validation -> validated wrapper.
   The rest of the system works with the validated form.

5. **Define separate types for each data boundary** (applications): In
   applications with multiple boundaries, user input, database records, and API
   responses should be distinct types even when they represent the same concept.
   A database record has an auto-generated ID; user input doesn't. Making these
   distinct prevents mixing concerns.

6. **Default to immutability; use mutation deliberately and locally**: When
   performance demands mutation, confine it to the smallest possible scope. The
   rest of the system shouldn't know or care.

7. **Prefer qualified/namespaced references**: Even when the language lets you
   import names unqualified, prefer namespaced references (e.g., Module.foo over
   foo). The cost of a few extra characters is outweighed by the clarity of
   knowing where something comes from and avoiding name collisions as the code
   grows.

8. **Ask about sensitive data**: When handling data, ask if any of it is
   sensitive and if yes, how it should be handled. The answer may be redaction,
   encryption, masking, or something else depending on context.

9. **Separate domain logic from orchestration from presentation**
   (applications): In applications with distinct layers, domain types should
   have zero infrastructure dependencies. Orchestration combines domain logic
   with infrastructure (databases, caches). Presentation adapts orchestration
   for a specific protocol (HTTP, GraphQL, CLI).

10. **Design for changeability, not for predicted changes**: Make designs modular
    and replaceable so future needs can be accommodated, but don't add
    abstractions, extension points, or features for changes that haven't
    happened yet. The goal is a design that's easy to modify, not one that
    anticipates specific modifications.

11. **Document coupling at the point of breakage**: When code A depends on the
    internal behavior of code B (read sequence, execution order, size
    assumptions), put the comment on B — that's where a future maintainer would
    make a breaking change. Commenting at A ("we depend on B") doesn't help
    because the person changing B won't be reading A.

12. **Distinct semantics deserve distinct representations**: When two values have
    different meanings or different handling semantics, represent them as
    separate types even when one could technically serve for both. Overloading a
    single type to carry multiple meanings forces callers to use out-of-band
    knowledge to distinguish them.

13. **It is easier to give than take away**: When deciding whether to include
    something in an API (a callback, a parameter, a feature), lean toward
    omitting it. You can always add it later if needed, but removing it is a
    breaking change. Start minimal; expand based on demonstrated need.

**Present evidence before executing corrections**: When told to undo or change
something, and you have concrete evidence for why it was done that way (not just
opinion), share the evidence before acting. The user may not have the same
context you do. This isn't pushback — it's making sure decisions are informed.
Execute the change after sharing, unless the user reconsiders.
