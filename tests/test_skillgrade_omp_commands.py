from __future__ import annotations

import json
import os
from pathlib import Path
import stat
import subprocess
import tempfile
import textwrap
import unittest


ROOT = Path(__file__).parents[1]
AGENT_PATH = ROOT / "dot_local/bin/executable_skillgrade-omp-agent"
RUNNER_PATH = ROOT / "dot_local/bin/executable_skillgrade-omp"


class SkillgradeOmpCommandTests(unittest.TestCase):
    @staticmethod
    def write_executable(path: Path, source: str) -> None:
        path.write_text(textwrap.dedent(source), encoding="utf-8")
        path.chmod(path.stat().st_mode | stat.S_IXUSR)

    def test_agent_forwards_prompt_profile_model_and_extra_flags(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            record = root / "record.json"
            fake_omp = root / "omp"
            self.write_executable(
                fake_omp,
                """\
                #!/usr/bin/env python3
                import json
                import os
                from pathlib import Path
                import sys

                Path(os.environ["RECORD"]).write_text(json.dumps({
                    "argv": sys.argv[1:],
                    "stdin": sys.stdin.read(),
                }))
                """,
            )
            env = os.environ.copy()
            env.update({
                "RECORD": str(record),
                "SKILLGRADE_OMP_BIN": str(fake_omp),
                "SKILLGRADE_OMP_PROFILE": "work",
                "SKILLGRADE_AGENT_MODEL": "spoton/gpt-5-6-sol",
            })
            process = subprocess.run(
                ["/bin/sh", str(AGENT_PATH), "--no-skills"],
                input="evaluate this skill",
                text=True,
                capture_output=True,
                env=env,
                check=False,
            )
            self.assertEqual(process.returncode, 0, process.stderr)
            invocation = json.loads(record.read_text(encoding="utf-8"))
            self.assertEqual(invocation["stdin"], "evaluate this skill")
            self.assertIn(["--profile", "work"], [invocation["argv"][i : i + 2] for i in range(len(invocation["argv"]) - 1)])
            self.assertIn(["--model", "spoton/gpt-5-6-sol"], [invocation["argv"][i : i + 2] for i in range(len(invocation["argv"]) - 1)])
            self.assertIn("--no-skills", invocation["argv"])
            self.assertIn("--auto-approve", invocation["argv"])

    def test_agent_requires_profile(self) -> None:
        env = os.environ.copy()
        env.pop("SKILLGRADE_OMP_PROFILE", None)
        process = subprocess.run(
            ["/bin/sh", str(AGENT_PATH)],
            input="prompt",
            text=True,
            capture_output=True,
            env=env,
            check=False,
        )
        self.assertEqual(process.returncode, 2)
        self.assertIn("SKILLGRADE_OMP_PROFILE is required", process.stderr)

    def test_runner_applies_standard_skillgrade_overrides(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            record = root / "record.json"
            fake_skillgrade = root / "skillgrade"
            fake_agent = root / "skillgrade-omp-agent"
            self.write_executable(
                fake_skillgrade,
                """\
                #!/usr/bin/env python3
                import json
                import os
                from pathlib import Path
                import sys

                Path(os.environ["RECORD"]).write_text(json.dumps(sys.argv[1:]))
                """,
            )
            self.write_executable(fake_agent, "#!/bin/sh\nexit 0\n")
            env = os.environ.copy()
            env.update({
                "PATH": f"{root}:{env['PATH']}",
                "RECORD": str(record),
            })
            process = subprocess.run(
                ["/bin/sh", str(RUNNER_PATH), "--trials=3", "--preview"],
                text=True,
                capture_output=True,
                env=env,
                check=False,
            )
            self.assertEqual(process.returncode, 0, process.stderr)
            argv = json.loads(record.read_text(encoding="utf-8"))
            self.assertEqual(
                argv,
                [
                    "--agent=command",
                    "--command=skillgrade-omp-agent",
                    "--provider=local",
                    "--trials=3",
                    "--preview",
                ],
            )


if __name__ == "__main__":
    unittest.main()
