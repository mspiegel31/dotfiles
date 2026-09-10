#!/usr/bin/env python3
"""Classify chezmoi destination drift and source-repository changes."""

from __future__ import annotations

import argparse
import json
import os
import shlex
import subprocess
import sys
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import NoReturn, Sequence


@dataclass(frozen=True)
class ManagedChange:
    status: str
    target: str
    source: str
    source_status: str
    kind: str


@dataclass(frozen=True)
class RepoChange:
    status: str
    path: str
    original_path: str | None = None


@dataclass(frozen=True)
class SyncState:
    managed: tuple[ManagedChange, ...]
    repository: tuple[RepoChange, ...]

    @property
    def conflicts(self) -> tuple[RepoChange, ...]:
        return tuple(change for change in self.repository if is_conflict(change.status))


class CommandFailure(RuntimeError):
    def __init__(self, argv: Sequence[str], result: subprocess.CompletedProcess[str]):
        self.argv = tuple(argv)
        self.result = result
        detail = result.stderr.strip() or result.stdout.strip() or "no diagnostic output"
        super().__init__(f"{shlex.join(argv)} failed with exit code {result.returncode}: {detail}")


def fail(message: str, code: int = 1) -> NoReturn:
    print(message, file=sys.stderr)
    raise SystemExit(code)


def run(argv: Sequence[str]) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(argv, text=True, capture_output=True, check=False)
    except FileNotFoundError:
        fail(f"Cannot find {argv[0]!r}. Install chezmoi or pass --chezmoi PATH.", 127)
    if result.returncode != 0:
        raise CommandFailure(argv, result)
    return result


def chezmoi(binary: str, *args: str) -> subprocess.CompletedProcess[str]:
    return run((binary, *args))


def parse_managed_status(output: str) -> list[tuple[str, str]]:
    changes: list[tuple[str, str]] = []
    for line in output.splitlines():
        if not line:
            continue
        if len(line) < 4 or line[2] != " ":
            raise ValueError(f"Unexpected `chezmoi status` line: {line!r}")
        changes.append((line[:2], line[3:]))
    return changes


def parse_repo_status(output: str) -> tuple[RepoChange, ...]:
    records = output.split("\0")
    changes: list[RepoChange] = []
    index = 0
    while index < len(records):
        record = records[index]
        index += 1
        if not record:
            continue
        if len(record) < 4 or record[2] != " ":
            raise ValueError(f"Unexpected `git status --porcelain=v1 -z` record: {record!r}")
        status = record[:2]
        path = record[3:]
        original_path = None
        if status[0] in "RC":
            if index >= len(records) or not records[index]:
                raise ValueError(f"Missing original path for repository status {record!r}")
            original_path = records[index]
            index += 1
        changes.append(RepoChange(status=status, path=path, original_path=original_path))
    return tuple(changes)


def is_conflict(status: str) -> bool:
    return status in {"DD", "AU", "UD", "UA", "DU", "AA", "UU"} or "U" in status


def relative_source(source: str, root: Path) -> str:
    path = Path(source).resolve()
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.as_posix()


def classify(status: str, source: str, source_status: str) -> str:
    actual, target = status
    if actual == " " and target != " ":
        return "apply"
    if actual == " ":
        return "settled"
    if source_status:
        return "concurrent"
    if actual in {"A", "M"} and source.endswith(".tmpl"):
        return "merge"
    if actual in {"A", "M"}:
        return "re-add"
    return "review"


def inspect(binary: str, targets: Sequence[str]) -> SyncState:
    status_args = ["status", "--exclude=dirs", "--path-style=absolute", *targets]
    status_rows = parse_managed_status(chezmoi(binary, *status_args).stdout)

    repository = parse_repo_status(
        chezmoi(binary, "git", "--", "status", "--porcelain=v1", "-z", "--untracked-files=all").stdout
    )
    root = Path(chezmoi(binary, "source-path").stdout.strip()).resolve()
    source_codes: dict[str, str] = {}
    for change in repository:
        source_codes[change.path] = change.status
        if change.original_path:
            source_codes[change.original_path] = change.status

    sources: list[str] = []
    if status_rows:
        source_result = chezmoi(binary, "source-path", *(target for _, target in status_rows))
        sources = source_result.stdout.splitlines()
        if len(sources) != len(status_rows):
            raise ValueError(
                "`chezmoi source-path` returned a different number of paths than `chezmoi status`; "
                "run the native commands separately to inspect the affected entries"
            )

    managed: list[ManagedChange] = []
    for (status, target), source in zip(status_rows, sources):
        source_relative = relative_source(source, root)
        source_status = source_codes.get(source_relative, "")
        managed.append(
            ManagedChange(
                status=status,
                target=target,
                source=source_relative,
                source_status=source_status,
                kind=classify(status, source_relative, source_status),
            )
        )
    return SyncState(managed=tuple(managed), repository=repository)


def grouped(state: SyncState) -> dict[str, list[ManagedChange]]:
    groups: dict[str, list[ManagedChange]] = {}
    for change in state.managed:
        groups.setdefault(change.kind, []).append(change)
    return groups


def quote_command(command: Sequence[str]) -> str:
    return shlex.join(command)


def print_changes(title: str, changes: Sequence[ManagedChange]) -> None:
    if not changes:
        return
    print(f"\n{title}:")
    for change in changes:
        detail = f" -> {change.source}"
        if change.source_status:
            detail += f" (source {change.source_status})"
        print(f"  {change.status} {change.target}{detail}")


def print_state(state: SyncState, binary: str) -> None:
    groups = grouped(state)
    if state.conflicts:
        summary = "merge conflicts"
    elif groups.get("concurrent") or groups.get("review") or groups.get("merge"):
        summary = "attention needed"
    elif state.managed or state.repository:
        summary = "changes found"
    else:
        summary = "clean"
    print(f"Dotfiles sync state: {summary}.")

    if state.conflicts:
        print("\nSource repository conflicts:")
        for change in state.conflicts:
            print(f"  {change.status} {change.path}")

    print_changes("Ready for native re-add", groups.get("re-add", []))
    print_changes("Templates requiring native merge", groups.get("merge", []))
    print_changes("Destination and source both changed", groups.get("concurrent", []))
    print_changes("Pending destination apply", groups.get("apply", []))
    print_changes("Changes requiring review", groups.get("review", []))

    if state.repository:
        print("\nSource repository changes:")
        for change in state.repository:
            rename = f" <- {change.original_path}" if change.original_path else ""
            print(f"  {change.status} {change.path}{rename}")

    commands: list[list[str]] = []
    if not state.conflicts:
        if groups.get("re-add"):
            commands.append([binary, "re-add", *(change.target for change in groups["re-add"])])
        if groups.get("merge"):
            commands.append([binary, "merge", *(change.target for change in groups["merge"])])
        if groups.get("apply"):
            commands.append([binary, "diff", "--skip-secrets"])
    if commands:
        print("\nNext native commands:")
        for command in commands:
            print(f"  {quote_command(command)}")


def state_as_json(state: SyncState) -> str:
    return json.dumps(
        {
            "managed": [asdict(change) for change in state.managed],
            "repository": [asdict(change) for change in state.repository],
            "conflicts": [asdict(change) for change in state.conflicts],
        },
        indent=2,
        sort_keys=True,
    )


def cmd_inspect(args: argparse.Namespace) -> int:
    state = inspect(args.chezmoi, args.targets)
    if args.json:
        print(state_as_json(state))
    else:
        print_state(state, args.chezmoi)
    return 2 if args.check and (state.managed or state.repository) else 0


def cmd_capture(args: argparse.Namespace) -> int:
    state = inspect(args.chezmoi, args.targets)
    if state.conflicts:
        print_state(state, args.chezmoi)
        print("\nResolve source repository conflicts before capturing destination changes.", file=sys.stderr)
        return 2

    safe = grouped(state).get("re-add", [])
    if not safe:
        print("No plain destination changes are safe to re-add automatically.")
        print_state(state, args.chezmoi)
        return 0

    command = [args.chezmoi, "re-add", *(change.target for change in safe)]
    if args.dry_run:
        print(f"Would run: {quote_command(command)}")
        print(f"Would capture {len(safe)} plain destination change(s) with `chezmoi re-add`.")
    else:
        result = run(command)
        if result.stdout:
            print(result.stdout, end="" if result.stdout.endswith("\n") else "\n")
        print(f"Captured {len(safe)} plain destination change(s) with `chezmoi re-add`.")

    remaining = [change for change in state.managed if change.kind != "re-add"]
    if remaining:
        print("Remaining changes need native merge, apply, or review:")
        for change in remaining:
            print(f"  {change.status} {change.target} ({change.kind})")
    return 0


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Classify chezmoi destination drift and source-repository changes."
    )
    result.add_argument(
        "--chezmoi",
        default=os.environ.get("CHEZMOI", "chezmoi"),
        help="chezmoi executable (default: %(default)s)",
    )
    subparsers = result.add_subparsers(dest="command", required=True)

    inspect_parser = subparsers.add_parser("inspect", help="show destination, source, and conflict state")
    inspect_parser.add_argument("targets", nargs="*", help="optional destination paths")
    inspect_parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    inspect_parser.add_argument(
        "--check", action="store_true", help="exit 2 when any managed or repository change exists"
    )
    inspect_parser.set_defaults(function=cmd_inspect)

    capture_parser = subparsers.add_parser(
        "capture", help="re-add safe plain destination changes with native chezmoi"
    )
    capture_parser.add_argument("targets", nargs="*", help="optional destination paths")
    capture_parser.add_argument("--dry-run", action="store_true", help="print the re-add command without running it")
    capture_parser.set_defaults(function=cmd_capture)
    return result


def main() -> None:
    args = parser().parse_args()
    try:
        raise SystemExit(args.function(args))
    except (CommandFailure, ValueError) as error:
        fail(str(error))


if __name__ == "__main__":
    main()
