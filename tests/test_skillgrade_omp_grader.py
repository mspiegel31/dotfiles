from __future__ import annotations

import json
import os
import stat
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path


ADAPTER_PATH = Path(__file__).parents[1] / "dot_local/bin/executable_skillgrade-omp-grader"


class SkillgradeOmpGraderTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.fake_omp_source = textwrap.dedent(
            """\
            #!/usr/bin/env python3
            import json
            import os
            from pathlib import Path
            import sys
            import time

            record = os.environ.get("FAKE_OMP_RECORD")
            prompt = sys.stdin.read()
            if record:
                Path(record).write_text(
                    json.dumps({"argv": sys.argv[1:], "prompt": prompt}),
                    encoding="utf-8",
                )
            sleep = float(os.environ.get("FAKE_OMP_SLEEP", "0"))
            if sleep:
                time.sleep(sleep)
            output = os.environ.get(
                "FAKE_OMP_OUTPUT",
                '<skillgrade_result>{"score": 1.0, "details": "default"}</skillgrade_result>',
            )
            sys.stdout.write(output)
            sys.exit(int(os.environ.get("FAKE_OMP_EXIT", "0")))
            """
        )

    def run_adapter(
        self,
        workspace: Path,
        expected: dict,
        *,
        profile: str | None = "test-profile",
        model: str | None = None,
        output: str | None = None,
        fake_exit: int = 0,
        fake_sleep: float | None = None,
        timeout: str | None = None,
    ) -> subprocess.CompletedProcess[str]:
        fake = workspace / "fake-omp.py"
        fake.write_text(self.fake_omp_source, encoding="utf-8")
        fake.chmod(fake.stat().st_mode | stat.S_IXUSR)
        record = workspace / "fake-record.json"
        env = os.environ.copy()
        for name in (
            "SKILLGRADE_INPUT",
            "SKILLGRADE_OMP_PROFILE",
            "SKILLGRADE_JUDGE_MODEL",
            "SKILLGRADE_OMP_BIN",
            "SKILLGRADE_JUDGE_TIMEOUT",
            "FAKE_OMP_OUTPUT",
            "FAKE_OMP_EXIT",
            "FAKE_OMP_SLEEP",
        ):
            env.pop(name, None)
        env["SKILLGRADE_INPUT"] = json.dumps(
            {"task": "test task", "trial": 0, "expected": expected}
        )
        if profile is not None:
            env["SKILLGRADE_OMP_PROFILE"] = profile
        env["SKILLGRADE_OMP_BIN"] = str(fake)
        env["FAKE_OMP_RECORD"] = str(record)
        env["FAKE_OMP_EXIT"] = str(fake_exit)
        if output is not None:
            env["FAKE_OMP_OUTPUT"] = output
        if fake_sleep is not None:
            env["FAKE_OMP_SLEEP"] = str(fake_sleep)
        if timeout is not None:
            env["SKILLGRADE_JUDGE_TIMEOUT"] = timeout
        if model is not None:
            env["SKILLGRADE_JUDGE_MODEL"] = model
        return subprocess.run(
            [sys.executable, str(ADAPTER_PATH)],
            cwd=workspace,
            env=env,
            text=True,
            capture_output=True,
            check=False,
        )

    @staticmethod
    def result(process: subprocess.CompletedProcess[str]) -> dict:
        return json.loads(process.stdout)

    def test_happy_path_passes_rubric_and_artifact_to_fake_judge(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.md").write_text("The sky is blue.", encoding="utf-8")
            process = self.run_adapter(
                workspace,
                {"rubric": "Award points for the sky statement.", "artifacts": ["*.md"]},
                output='<skillgrade_result>{"score": 0.95, "details": "meets rubric"}</skillgrade_result>',
            )
            self.assertEqual(process.returncode, 0)
            self.assertEqual(self.result(process), {"score": 0.95, "details": "meets rubric"})
            record = json.loads((workspace / "fake-record.json").read_text(encoding="utf-8"))
            self.assertIn("--profile", record["argv"])
            self.assertIn("--no-tools", record["argv"])
            self.assertIn("Award points for the sky statement.", record["prompt"])
            self.assertIn("The sky is blue.", record["prompt"])

    def test_model_flag_is_present_only_when_model_is_set(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("answer", encoding="utf-8")
            expected = {"rubric": "Read answer.", "artifacts": ["answer.txt"]}
            process = self.run_adapter(workspace, expected, model="judge-model")
            self.assertEqual(process.returncode, 0)
            argv = json.loads((workspace / "fake-record.json").read_text())["argv"]
            self.assertIn(["--model", "judge-model"], [argv[index : index + 2] for index in range(len(argv) - 1)])

            process = self.run_adapter(workspace, expected)
            self.assertEqual(process.returncode, 0)
            argv = json.loads((workspace / "fake-record.json").read_text())["argv"]
            self.assertNotIn("--model", argv)

    def test_missing_profile_is_a_scored_error(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("answer", encoding="utf-8")
            process = self.run_adapter(
                workspace,
                {"rubric": "Read answer.", "artifacts": ["answer.txt"]},
                profile=None,
            )
            self.assertEqual(process.returncode, 0)
            self.assertTrue(self.result(process)["details"].startswith("grader error:"))

    def test_no_artifact_match_is_a_scored_error(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            process = self.run_adapter(
                Path(directory),
                {"rubric": "Read answer.", "artifacts": ["missing.md"]},
            )
            self.assertEqual(process.returncode, 0)
            self.assertTrue(self.result(process)["details"].startswith("grader error:"))

    def test_invalid_model_outputs_are_scored_errors(self) -> None:
        outputs = [
            "plain output",
            '<skillgrade_result>{"score": 0.5, "details": "one"}</skillgrade_result>\n<skillgrade_result>{"score": 0.5, "details": "two"}</skillgrade_result>',
            "<skillgrade_result>not json</skillgrade_result>",
            '<skillgrade_result>{"score": 1.5, "details": "too high"}</skillgrade_result>',
        ]
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("answer", encoding="utf-8")
            expected = {"rubric": "Read answer.", "artifacts": ["answer.txt"]}
            for output in outputs:
                with self.subTest(output=output):
                    process = self.run_adapter(workspace, expected, output=output)
                    self.assertEqual(process.returncode, 0)
                    self.assertTrue(self.result(process)["details"].startswith("grader error:"))

    def test_fake_nonzero_exit_is_a_scored_error(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("answer", encoding="utf-8")
            process = self.run_adapter(
                workspace,
                {"rubric": "Read answer.", "artifacts": ["answer.txt"]},
                fake_exit=7,
            )
            self.assertEqual(process.returncode, 0)
            self.assertTrue(self.result(process)["details"].startswith("grader error:"))

    def test_fake_timeout_is_a_scored_error(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("answer", encoding="utf-8")
            process = self.run_adapter(
                workspace,
                {"rubric": "Read answer.", "artifacts": ["answer.txt"]},
                fake_sleep=2,
                timeout="1",
            )
            self.assertEqual(process.returncode, 0)
            self.assertIn("timeout", self.result(process)["details"])

    def test_single_artifact_sugar_works(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            workspace = Path(directory)
            (workspace / "answer.txt").write_text("sugar artifact", encoding="utf-8")
            process = self.run_adapter(
                workspace,
                {"rubric": "Read answer.", "artifact": "answer.txt"},
            )
            self.assertEqual(process.returncode, 0)
            self.assertEqual(self.result(process)["score"], 1.0)
            record = json.loads((workspace / "fake-record.json").read_text())
            self.assertIn("sugar artifact", record["prompt"])


if __name__ == "__main__":
    unittest.main()
