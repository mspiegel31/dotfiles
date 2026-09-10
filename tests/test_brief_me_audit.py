from __future__ import annotations

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import textwrap
import unittest


ROOT = Path(__file__).parents[1]
CLI = ROOT / "dot_agents/skills/brief-me/scripts/executable_briefing.py"


class BriefMeAuditTests(unittest.TestCase):
    @staticmethod
    def entry(key: str, fields: str) -> str:
        return f"@misc{{{key},\n{textwrap.indent(textwrap.dedent(fields).strip(), '  ')}\n}}\n"

    def run_audit(
        self,
        entries: str,
        rows: dict[str, str],
        *,
        json_output: bool = True,
    ) -> subprocess.CompletedProcess[str]:
        with tempfile.TemporaryDirectory() as directory:
            briefing = Path(directory)
            (briefing / "sources").mkdir()
            keys = [line.split("{", 1)[1].split(",", 1)[0] for line in entries.splitlines() if line.startswith("@")]
            citations = " ".join(f"[@{key}]" for key in keys)
            (briefing / "index.qmd").write_text(f"# Brief\n\n{citations}\n", encoding="utf-8")
            (briefing / "references.bib").write_text(entries, encoding="utf-8")
            ledger = [
                "# Source ledger",
                "",
                "| key | url | fetched | summary |",
                "|-----|-----|---------|---------|",
            ]
            ledger.extend(f"| {key} | {url} | 2026-09-10 | test source |" for key, url in rows.items())
            (briefing / "sources" / "LEDGER.md").write_text("\n".join(ledger) + "\n", encoding="utf-8")
            command = [sys.executable, str(CLI), "audit", str(briefing)]
            if json_output:
                command.append("--json")
            return subprocess.run(command, capture_output=True, text=True, check=False)

    @staticmethod
    def expected(*, citations: int, ledger_rows: int, **arrays: list[str]) -> dict[str, object]:
        return {
            "cited_not_in_bib": [],
            "cited_not_in_ledger": [],
            "bib_not_in_ledger": [],
            "ledger_not_cited": [],
            "bib_url_missing": arrays.get("bib_url_missing", []),
            "bib_url_escaped": arrays.get("bib_url_escaped", []),
            "bib_url_mismatch": arrays.get("bib_url_mismatch", []),
            "citations": citations,
            "ledger_rows": ledger_rows,
        }

    def test_raw_google_and_slack_urls_with_url_punctuation_pass(self) -> None:
        google = "https://docs.google.com/spreadsheets/d/1Tvt0MdHEWDF_ZOdjgGKuMPXGT2oyvrmxh-Xvs32UZn4/edit#gid=0&range=A1_B2"
        slack = "https://app.slack.com/client/T123/C456/thread/1700000000.000000?foo=bar_baz#reply&include=1"
        entries = self.entry("google", f"url = {{{google}}},") + self.entry("slack", f"url = {{{slack}}},")
        process = self.run_audit(entries, {"google": google, "slack": slack})
        self.assertEqual(process.returncode, 0, process.stderr)
        self.assertEqual(json.loads(process.stdout), self.expected(citations=2, ledger_rows=2))

    def test_legacy_howpublished_url_passes(self) -> None:
        url = "https://github.com/example/project#readme"
        entries = self.entry("legacy", rf"howpublished = {{\url{{{url}}}}},")
        process = self.run_audit(entries, {"legacy": url})
        self.assertEqual(process.returncode, 0, process.stderr)
        self.assertEqual(json.loads(process.stdout), self.expected(citations=1, ledger_rows=1))

    def test_tex_escaped_url_fails_as_escaped_and_mismatch(self) -> None:
        raw = "https://docs.google.com/spreadsheets/d/1Tvt0MdHEWDF_ZOdjgGKuMPXGT2oyvrmxh-Xvs32UZn4/edit#gid=0&range=A1_B2"
        escaped = raw.replace("_", r"\_")
        entries = self.entry("escaped", f"url = {{{escaped}}},")
        process = self.run_audit(entries, {"escaped": raw})
        self.assertEqual(process.returncode, 1)
        self.assertEqual(
            json.loads(process.stdout),
            self.expected(citations=1, ledger_rows=1, bib_url_escaped=["escaped"], bib_url_mismatch=["escaped"]),
        )
        human = self.run_audit(entries, {"escaped": raw}, json_output=False)
        self.assertEqual(human.returncode, 1)
        self.assertIn("FAIL bib_url_escaped: @escaped", human.stderr)
        self.assertIn(repr(escaped), human.stderr)
        self.assertIn(raw, human.stderr)

    def test_different_raw_url_fails_as_mismatch(self) -> None:
        bib_url = "https://example.com/source_one?x=1&y=2"
        ledger_url = "https://example.com/source_two?x=1&y=2"
        entries = self.entry("different", f"url = {{{bib_url}}},")
        process = self.run_audit(entries, {"different": ledger_url})
        self.assertEqual(process.returncode, 1)
        self.assertEqual(
            json.loads(process.stdout),
            self.expected(citations=1, ledger_rows=1, bib_url_mismatch=["different"]),
        )
        human = self.run_audit(entries, {"different": ledger_url}, json_output=False)
        self.assertEqual(human.returncode, 1)
        self.assertIn("FAIL bib_url_mismatch: @different", human.stderr)
        self.assertIn(bib_url, human.stderr)
        self.assertIn(ledger_url, human.stderr)

    def test_missing_url_fails(self) -> None:
        entries = self.entry("missing", "title = {No URL},")
        process = self.run_audit(entries, {"missing": "https://example.com/missing"})
        self.assertEqual(process.returncode, 1)
        self.assertEqual(
            json.loads(process.stdout),
            self.expected(citations=1, ledger_rows=1, bib_url_missing=["missing"]),
        )


if __name__ == "__main__":
    unittest.main()
