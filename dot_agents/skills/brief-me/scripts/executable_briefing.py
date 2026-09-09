#!/usr/bin/env python3
"""brief-me CLI: scaffold, check, audit, render, list, and publish briefings.

Standard library only. Exit 0 on success, 1 on a finding the agent must fix,
2 on a missing prerequisite the agent must report to the user.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import webbrowser
from pathlib import Path
from typing import NoReturn

ASSETS = Path(__file__).resolve().parent.parent / "assets"
DEFAULT_HOME = Path(os.environ.get("BRIEFINGS_HOME", "~/briefings")).expanduser()

MIN_QUARTO = (1, 9, 20)
MIN_MARIMO = (0, 23, 16)

BRIEF_FIELDS = (
    "Question",
    "Supporting questions",
    "Decision this feeds",
    "Audience",
    "Prior knowledge (do not re-explain)",
    "Type",
    "Diátaxis mode",
    "Depth",
    "Source constraints",
    "Worked example",
    "Interaction",
    "Status",
)
FRONT_FIELDS = ("title", "type", "diataxis", "audience", "question", "decision", "status")

_VERSION_RE = re.compile(r"(\d+)\.(\d+)(?:\.(\d+))?")
_BIB_KEY_RE = re.compile(r"^@\w+\s*\{\s*([^,\s]+)\s*,", re.MULTILINE)
_CITE_RE = re.compile(r"(?<![\w@/.])@([A-Za-z][\w:.-]*[\w])")
_FENCE_RE = re.compile(r"^```.*?^```\s*$", re.MULTILINE | re.DOTALL)
_INLINE_CODE_RE = re.compile(r"`[^`\n]*`")


def fail(msg: str, code: int = 1) -> NoReturn:
    print(msg, file=sys.stderr)
    sys.exit(code)


def ok(msg: str) -> None:
    print(msg)


def parse_version(text: str) -> tuple[int, ...] | None:
    m = _VERSION_RE.search(text)
    if not m:
        return None
    parts = [int(g) for g in m.groups() if g is not None]
    return tuple(parts + [0] * (3 - len(parts)))


def run(cmd: list[str], cwd: Path | None = None, timeout: int = 120) -> subprocess.CompletedProcess[str]:
    return subprocess.run(cmd, cwd=cwd, text=True, capture_output=True, timeout=timeout, check=False)


def tool_version(cmds: list[list[str]]) -> tuple[str | None, tuple[int, ...] | None]:
    for cmd in cmds:
        if shutil.which(cmd[0]) is None:
            continue
        try:
            cp = run(cmd)
        except subprocess.TimeoutExpired:
            continue
        v = parse_version(cp.stdout + cp.stderr)
        if v:
            return " ".join(cmd), v
    return None, None


def fmt(v: tuple[int, ...]) -> str:
    return ".".join(str(x) for x in v)


# ---------------------------------------------------------------- front matter


def read_front_matter(qmd: Path) -> dict[str, str]:
    text = qmd.read_text(encoding="utf-8")
    if not text.startswith("---"):
        return {}
    end = text.find("\n---", 3)
    if end == -1:
        return {}
    out: dict[str, str] = {}
    for line in text[3:end].splitlines():
        if not line or line.startswith(" ") or line.startswith("#") or ":" not in line:
            continue
        key, _, value = line.partition(":")
        value = value.strip()
        if len(value) >= 2 and value[0] == value[-1] and value[0] in "\"'":
            value = value[1:-1]
        out[key.strip()] = value
    return out


def brief_sections(brief: Path) -> dict[str, str]:
    out: dict[str, str] = {}
    current: str | None = None
    buf: list[str] = []
    for line in brief.read_text(encoding="utf-8").splitlines():
        if line.startswith("## "):
            if current is not None:
                out[current] = "\n".join(buf).strip()
            current = line[3:].strip()
            buf = []
        elif current is not None:
            if line.strip().startswith("<!--"):
                continue
            buf.append(line)
    if current is not None:
        out[current] = "\n".join(buf).strip()
    return out


# ---------------------------------------------------------------- home / init


def find_home(start: Path) -> Path | None:
    for p in [start, *start.parents]:
        if (p / "_quarto.yml").exists():
            return p
    return None


def ensure_home(home: Path) -> list[str]:
    notes: list[str] = []
    home.mkdir(parents=True, exist_ok=True)
    for src, dest in (("_quarto.yml", "_quarto.yml"), ("home-index.qmd", "index.qmd"), ("brief.scss", "brief.scss"), ("ieee.csl", "ieee.csl")):
        target = home / dest
        if not target.exists():
            shutil.copy(ASSETS / src, target)
            notes.append(f"created {target}")
    if not (home / "_extensions" / "marimo-team" / "marimo").exists():
        if shutil.which("quarto") is None:
            fail("quarto not found; cannot install quarto-marimo extension", 2)
        cp = run(["quarto", "add", "marimo-team/quarto-marimo", "--no-prompt"], cwd=home, timeout=300)
        if cp.returncode != 0:
            fail(f"quarto add marimo-team/quarto-marimo failed:\n{cp.stderr.strip()}", 2)
        notes.append(f"installed quarto-marimo into {home / '_extensions' / 'marimo-team' / 'marimo'}")
    return notes


def cmd_init(a: argparse.Namespace) -> None:
    if not re.fullmatch(r"[a-z0-9][a-z0-9-]*", a.slug):
        fail("slug must be lowercase letters, digits, and hyphens")
    types = sorted(p.stem.removeprefix("type-") for p in (ASSETS.parent / "references").glob("type-*.md"))
    if a.type not in types:
        fail(f"unknown type {a.type!r}; registry has: {', '.join(types)}. Add references/type-{a.type}.md first.")
    problems = check_toolchain()
    if problems:
        fail("\n".join(problems), 2)
    home = Path.cwd() / "briefings" if a.here else Path(a.home).expanduser()
    for note in ensure_home(home):
        ok(note)
    target = home / a.slug
    if target.exists():
        fail(f"{target} already exists; choose another slug or use refresh")
    target.mkdir()
    (target / "sources").mkdir()
    today = dt.date.today().isoformat()
    subs = {
        "{{title}}": a.title or a.slug.replace("-", " ").title(),
        "{{type}}": a.type,
        "{{audience}}": a.audience,
        "{{created}}": today,
        "{{audience_badge}}": "[Internal]{.audience-internal} " if a.audience == "internal" else "",
    }

    def render_asset(name: str) -> str:
        text = (ASSETS / name).read_text(encoding="utf-8")
        for k, v in subs.items():
            text = text.replace(k, v)
        return text

    (target / "index.qmd").write_text(render_asset("index.qmd"), encoding="utf-8")
    (target / "BRIEF.md").write_text(render_asset("BRIEF.md"), encoding="utf-8")
    (target / "sources" / "LEDGER.md").write_text(render_asset("LEDGER.md"), encoding="utf-8")
    (target / "references.bib").write_text("", encoding="utf-8")
    ok(f"created briefing at {target}")
    ok("next: fill BRIEF.md from the interview, then run `check` on it")


# ---------------------------------------------------------------- check


def check_toolchain() -> list[str]:
    problems: list[str] = []
    _, qv = tool_version([["quarto", "--version"]])
    if qv is None:
        problems.append("quarto not found. Install: https://quarto.org/docs/get-started/")
    elif qv < MIN_QUARTO:
        problems.append(f"quarto {fmt(qv)} < {fmt(MIN_QUARTO)} required by quarto-marimo")
    else:
        ok(f"quarto {fmt(qv)}")
    if shutil.which("uv") is None:
        problems.append("uv not found. Install: https://docs.astral.sh/uv/getting-started/installation/")
    else:
        ok("uv present")
    # quarto-marimo runs cells in a uv sandbox pinned by the document's `pyproject`
    # front matter, so only uv's ability to resolve marimo matters for rendering.
    # The PATH marimo is reported because `marimo check --select MW` uses it.
    _, mv = tool_version([["marimo", "--version"]])
    if mv is None:
        ok("marimo not on PATH; sandbox will resolve it. Optional: uv tool install marimo")
    elif mv < MIN_MARIMO:
        ok(f"marimo {fmt(mv)} on PATH is older than the sandbox minimum {fmt(MIN_MARIMO)}; "
           f"use `uv run --with 'marimo>={fmt(MIN_MARIMO)}' marimo ...` for CLI checks")
    else:
        ok(f"marimo {fmt(mv)}")
    return problems


def check_briefing(path: Path) -> list[str]:
    problems: list[str] = []
    brief = path / "BRIEF.md"
    qmd = path / "index.qmd"
    if not brief.exists() or not qmd.exists():
        return [f"{path} is not a briefing (needs BRIEF.md and index.qmd)"]
    sections = brief_sections(brief)
    for field in BRIEF_FIELDS:
        value = sections.get(field, "")
        if not value or "TODO" in value:
            problems.append(f"BRIEF.md § {field}: empty or TODO")
    fm = read_front_matter(qmd)
    for field in FRONT_FIELDS:
        value = fm.get(field, "")
        if not value or "TODO" in value:
            problems.append(f"index.qmd front matter `{field}`: empty or TODO")
    if fm.get("type") and sections.get("Type") and fm["type"] != sections["Type"].strip():
        problems.append("type differs between BRIEF.md and front matter")
    if fm.get("audience") and sections.get("Audience") and fm["audience"] != sections["Audience"].strip():
        problems.append("audience differs between BRIEF.md and front matter")
    return problems


def cmd_check(a: argparse.Namespace) -> None:
    problems = check_toolchain()
    if problems:
        fail("\n".join(problems), 2)
    if a.path:
        path = Path(a.path).expanduser().resolve()
        home = find_home(path)
        if home is None:
            problems.append(f"no _quarto.yml above {path}; run init to create a home")
        elif not (home / "_extensions" / "marimo-team" / "marimo").exists():
            problems.append(f"quarto-marimo missing in {home}; run: cd {home} && quarto add marimo-team/quarto-marimo --no-prompt")
        problems.extend(check_briefing(path))
    else:
        home = DEFAULT_HOME
        if not (home / "_quarto.yml").exists():
            ok(f"home {home} does not exist yet; init will create it")
        elif not (home / "_extensions" / "marimo-team" / "marimo").exists():
            ok(f"home {home} lacks quarto-marimo; init will install it")
        else:
            ok(f"home {home} ready")
    if problems:
        fail("\n".join(problems))
    ok("check passed")


# ---------------------------------------------------------------- audit


def prose_only(text: str) -> str:
    if text.startswith("---"):
        end = text.find("\n---", 3)
        if end != -1:
            text = text[end + 4 :]
    text = _FENCE_RE.sub("", text)
    return _INLINE_CODE_RE.sub("", text)


def ledger_rows(ledger: Path) -> dict[str, dict[str, str]]:
    rows: dict[str, dict[str, str]] = {}
    for line in ledger.read_text(encoding="utf-8").splitlines():
        if not line.startswith("|"):
            continue
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) < 4 or cells[0] in ("key", "") or set(cells[0]) <= {"-"}:
            continue
        rows[cells[0]] = {"url": cells[1], "fetched": cells[2], "summary": cells[3]}
    return rows


def collect_citations(path: Path) -> tuple[dict[str, list[int]], set[str], dict[str, dict[str, str]]]:
    qmd, bib, ledger = path / "index.qmd", path / "references.bib", path / "sources" / "LEDGER.md"
    for p in (qmd, bib, ledger):
        if not p.exists():
            fail(f"missing {p}")
    cited: dict[str, list[int]] = {}
    for n, line in enumerate(prose_only(qmd.read_text(encoding="utf-8")).splitlines(), 1):
        for m in _CITE_RE.finditer(line):
            cited.setdefault(m.group(1), []).append(n)
    return cited, set(_BIB_KEY_RE.findall(bib.read_text(encoding="utf-8"))), ledger_rows(ledger)


def cmd_audit(a: argparse.Namespace) -> None:
    path = Path(a.path).expanduser().resolve()
    cited, bib_keys, rows = collect_citations(path)
    report = {
        "cited_not_in_bib": sorted(set(cited) - bib_keys),
        "cited_not_in_ledger": sorted(set(cited) - set(rows)),
        "bib_not_in_ledger": sorted(bib_keys - set(rows)),
        "ledger_not_cited": sorted(set(rows) - set(cited)),
        "citations": len(cited),
        "ledger_rows": len(rows),
    }
    if a.json:
        print(json.dumps(report, indent=2))
        sys.exit(1 if report["cited_not_in_bib"] or report["cited_not_in_ledger"] or report["bib_not_in_ledger"] or report["citations"] == 0 else 0)
    for name in ("cited_not_in_bib", "cited_not_in_ledger", "bib_not_in_ledger"):
        for key in report[name]:
            print(f"FAIL {name}: @{key}", file=sys.stderr)
    for key in report["ledger_not_cited"]:
        print(f"warn ledger_not_cited: {key}")
    ok(f"{report['citations']} distinct citations, {report['ledger_rows']} ledger rows")
    hard = report["cited_not_in_bib"] or report["cited_not_in_ledger"] or report["bib_not_in_ledger"]
    if hard:
        sys.exit(1)
    if report["citations"] == 0:
        fail("no citations found in index.qmd prose")
    ok("audit passed")


def cmd_refresh(a: argparse.Namespace) -> None:
    path = Path(a.path).expanduser().resolve()
    cited, _, rows = collect_citations(path)
    if a.touch:
        today = dt.date.today().isoformat()
        qmd = path / "index.qmd"
        text = re.sub(r"^updated: .*$", f"updated: {today}", qmd.read_text(encoding="utf-8"), count=1, flags=re.M)
        qmd.write_text(text, encoding="utf-8")
        brief = path / "BRIEF.md"
        note = a.touch if a.touch is not True else "refreshed: ledger re-fetched, claims re-checked"
        with brief.open("a", encoding="utf-8") as f:
            f.write(f"- {today}: {note}\n")
        ok(f"updated: {today}; history noted in BRIEF.md")
        return
    if a.json:
        print(json.dumps([{"key": k, **row, "cited_at": cited.get(k, [])} for k, row in rows.items()], indent=2))
        return
    for key, row in rows.items():
        ok(f"{key}\t{row['fetched']}\t{row['url']}")
        ok(f"    {row['summary']}")
        ok(f"    cited at index.qmd lines: {cited.get(key, [])}")
    ok(f"\n{len(rows)} sources to re-fetch; confirm each cited claim, then run `refresh {a.path} --touch \"<what changed>\"`")


# ---------------------------------------------------------------- render / list / publish


def cmd_render(a: argparse.Namespace) -> None:
    path = Path(a.path).expanduser().resolve()
    home = find_home(path)
    if home is None:
        fail(f"no _quarto.yml above {path}")
    cp = subprocess.run(["quarto", "render", str(path.relative_to(home))], cwd=home, check=False)
    if cp.returncode != 0:
        fail("quarto render failed")
    out = home / "_site" / path.relative_to(home) / "index.html"
    ok(f"rendered {out}")
    if a.open:
        if a.serve:
            ok("serving via quarto preview (islands need HTTP); Ctrl-C to stop")
            subprocess.run(["quarto", "preview", str(path.relative_to(home)), "--no-watch-inputs"], cwd=home, check=False)
        else:
            webbrowser.open(out.as_uri())
            ok("opened over file://; marimo islands will not run. Use --serve for HTTP.")


def cmd_list(a: argparse.Namespace) -> None:
    home = Path(a.home).expanduser()
    if not home.exists():
        fail(f"{home} does not exist")
    rows = []
    for qmd in sorted(home.glob("*/index.qmd")):
        fm = read_front_matter(qmd)
        rows.append((qmd.parent.name, fm.get("type", "?"), fm.get("diataxis", "?"), fm.get("status", "?"), fm.get("updated", "?"), fm.get("title", "")))
    if a.json:
        print(json.dumps([dict(zip(("slug", "type", "diataxis", "status", "updated", "title"), r)) for r in rows], indent=2))
        return
    if not rows:
        ok(f"no briefings in {home}")
        return
    for r in rows:
        ok("\t".join(r))


def cmd_publish(a: argparse.Namespace) -> None:
    src = Path(a.path).expanduser().resolve()
    repo = Path(a.to).expanduser().resolve()
    if not (src / "index.qmd").exists():
        fail(f"{src} is not a briefing")
    if not (repo / "_quarto.yml").exists():
        fail(f"{repo} has no _quarto.yml; publish targets a Quarto site")
    fm = read_front_matter(src / "index.qmd")
    if fm.get("status") != "reviewed":
        fail(f"status is {fm.get('status')!r}; only reviewed briefings publish")
    warnings = []
    if fm.get("engine") == "marimo" and not (repo / "_extensions" / "marimo-team" / "marimo").exists():
        warnings.append("target lacks _extensions/marimo-team/marimo; islands will not render there")
    if not (repo / "briefings" / "index.qmd").exists():
        warnings.append("target has no briefings/index.qmd listing; copy assets/home-index.qmd")
    dest = repo / "briefings" / src.name
    if dest.exists():
        fail(f"{dest} exists; remove it first if replacing")
    shutil.copytree(src, dest, ignore=shutil.ignore_patterns("_site", ".quarto", "*_files"))
    ok(f"copied to {dest}")
    for w in warnings:
        print(f"warn {w}")
    ok("publish complete; commit and open the PR from the target repository")


# ---------------------------------------------------------------- main


def main() -> None:
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)
    p = argparse.ArgumentParser(prog="briefing.py", description=__doc__)
    sub = p.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("check", help="verify toolchain, home, and (optionally) a briefing's BRIEF.md")
    s.add_argument("path", nargs="?")
    s.set_defaults(fn=cmd_check)

    s = sub.add_parser("init", help="scaffold a briefing (creates the home if needed)")
    s.add_argument("slug")
    s.add_argument("--type", required=True)
    s.add_argument("--title")
    s.add_argument("--audience", default="self", choices=("self", "team", "internal"))
    s.add_argument("--here", action="store_true", help="use <cwd>/briefings as the home")
    s.add_argument("--home", default=str(DEFAULT_HOME))
    s.set_defaults(fn=cmd_init)

    s = sub.add_parser("audit", help="cross-check @citations, references.bib, and sources/LEDGER.md")
    s.add_argument("path")
    s.add_argument("--json", action="store_true")
    s.set_defaults(fn=cmd_audit)

    s = sub.add_parser("refresh", help="list ledger sources with the lines citing them; --touch records the refresh")
    s.add_argument("path")
    s.add_argument("--touch", nargs="?", const=True, metavar="NOTE", help="bump `updated:` and append a History line")
    s.add_argument("--json", action="store_true")
    s.set_defaults(fn=cmd_refresh)

    s = sub.add_parser("render", help="quarto render a briefing")
    s.add_argument("path")
    s.add_argument("--open", action="store_true")
    s.add_argument("--serve", action="store_true", help="with --open: serve over HTTP so islands run")
    s.set_defaults(fn=cmd_render)

    s = sub.add_parser("list", help="list briefings in the home")
    s.add_argument("--home", default=str(DEFAULT_HOME))
    s.add_argument("--json", action="store_true")
    s.set_defaults(fn=cmd_list)

    s = sub.add_parser("publish", help="copy a reviewed briefing into another Quarto site's briefings/")
    s.add_argument("path")
    s.add_argument("--to", required=True)
    s.set_defaults(fn=cmd_publish)

    a = p.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
