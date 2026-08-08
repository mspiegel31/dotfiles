---
name: work-doc-routing
description: >
  Routes queries about SpotOn-internal knowledge to the right doc backend (infra-docs,
  Confluence, Rovo). Use this skill whenever the user asks how things work at SpotOn —
  infrastructure, deployments, CI/CD pipelines, Kubernetes setup, Helm releases, AWS
  accounts, environment configuration (SIR, kops, prod clusters), rollback procedures,
  runbooks, ADRs, architecture decisions, team processes, observability, alerting, or
  on-call. Trigger on any question using "our", "we", "do we have", or referencing
  company-specific environments and tooling. Also trigger for onboarding-style questions
  ("how do I get set up", "where do I find the doc for X", "my manager mentioned"). If
  the user asks about a general technology (K8s, CI/CD, Helm) but wants to know the
  SpotOn-specific way or is looking for an internal doc/runbook, use this skill. Not for
  open-source library documentation (use librarian-tool-routing instead).
---


## Internal Documentation Routing

Multiple backends serve SpotOn internal documentation. Route to the most
specific match; fall through to Rovo search when uncertain.

### Decision Table

| Query about | Backend | Tool chain |
| --- | --- | --- |
| Infrastructure (K8s, deploys, CI/CD, networking, runbooks) | infra-docs | `infra-docs_search_docs` -> `infra-docs_get_doc_content` |
| Observability (monitoring, alerting, SLOs, dashboards, on-call) | Confluence SnO space | `rovo_searchConfluenceUsingCql` scoped to SnO |
| Anything else internal | Rovo search | `rovo_searchAtlassian` |

When a query could match multiple rows, try the more specific backend first.
Rovo search is always a safe fallback — it searches across all Confluence spaces
and Jira.

### Tool Workflows

**infra-docs** — search, then fetch full content for the best hit(s):

    infra-docs_search_docs(query="how to set up port forwarding")
    infra-docs_get_doc_content(path="/developer/kubernetes-setup/")

**Confluence (specific space)** — CQL-scoped search:

    rovo_searchConfluenceUsingCql(
      cloudId="spotonteam.atlassian.net",
      cql='space = "SnO" AND type = page AND text ~ "alerting runbook"'
    )
    rovo_getConfluencePage(
      cloudId="spotonteam.atlassian.net",
      pageId="<id from search results>"
    )

**Rovo search** — cross-product fallback (Jira + Confluence, all spaces):

    rovo_searchAtlassian(query="deployment process")

### Citations

Always cite the source when presenting information found through these backends.
Include the page title and a direct link so the user can verify and read further.

| Backend | Citation format |
| --- | --- |
| infra-docs | Page title + `docs.corp.spoton.sh` URL (from search result or `get_doc_content`) |
| Confluence | Page title + `spotonteam.atlassian.net/wiki/...` URL (from search result or page metadata) |
| Rovo search | Result title + URL returned in the search response |

If multiple sources informed the answer, cite all of them. If a detail comes
from a specific section of a long page, mention the section name alongside the
link.

Format citations as a bullet list — one source per line, never inline on a
single line:

```
**Sources:**
- [Page title](url)
- [Page title § Section name](url)
```

### Not Covered Here

- **OSS library docs** -> `context7` (see `librarian-tool-routing` skill)
- **Code search across repos** -> `grep_app` / `repomix` (see `librarian-tool-routing` skill)
- **Current codebase** -> `augment-context-engine` (see `explore-tool-routing` skill)

### Adding a New Source

For a new Confluence space, two edits:

1. Add a row to the decision table:

       | Topic keywords for routing | Confluence XX space | `rovo_searchConfluenceUsingCql` scoped to XX |

2. Add a CQL example under **Tool Workflows** matching the Confluence pattern
   above — replace the space key and example query terms.

For a non-Confluence source (new MCP, Google Drive folder, etc.), add both a
table row and a new workflow section with tool call examples.
