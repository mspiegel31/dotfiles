#!/usr/bin/env python3
"""List vault areas that have journal entries within a date window.

Journal area entries come in two forms (see the vault's AGENTS.md): a heading
whose entire text is a link, or a top-level bullet whose text is only a link.
Both forms must be matched or entries are silently dropped.

Dates come from the journal filename (YYYY-MM-DD) rather than mtime, because
notes are edited after the fact and mtime would misreport recency.

Usage:
  changed-areas.py [--vault PATH] [--days N]
"""
from __future__ import annotations

import argparse
import os
import re
import sys
from datetime import date, datetime, timedelta, timezone
from pathlib import Path

HEADING = re.compile(r"^#{1,6}\s+\[\[([^\]|#]+?)(?:\s*\|[^\]]*)?\]\]\s*$")
BULLET = re.compile(r"^-\s+\[\[([^\]|#]+?)(?:\s*\|[^\]]*)?\]\]\s*$")
STAMP = re.compile(r"^(\d{4})-(\d{2})-(\d{2})")


def note_date(path: Path) -> date | None:
    m = STAMP.match(path.stem)
    if not m:
        return None
    try:
        return date(int(m.group(1)), int(m.group(2)), int(m.group(3)))
    except ValueError:
        return None


def area_links(note: Path, areas_dir: Path) -> tuple[date, list[str]] | None:
    day = note_date(note)
    if day is None:
        return None
    # Only counts as an area if the link resolves to a note under areas/, so
    # task-card bullets ("[[Fire Liftosaur...]]") are not mistaken for areas.
    area_names = {p.stem for p in areas_dir.glob("*.md")} if areas_dir.is_dir() else set()
    seen: set[str] = set()
    areas: list[str] = []
    for line in note.read_text().splitlines():
        rx = HEADING.match(line) or BULLET.match(line)
        if rx is None:
            continue
        name = rx.group(1).split("/")[-1].strip()
        if name in area_names and name not in seen:
            seen.add(name)
            areas.append(name)
    return day, areas


def main() -> int:
    ap = argparse.ArgumentParser(
        description="List vault areas with journal entries in the last N days."
    )
    ap.add_argument("--vault", default=str(Path.home() / "personal-vault" / "personal-vault"))
    ap.add_argument("--days", type=int, default=7)
    args = ap.parse_args()

    journal = Path(args.vault) / "journal"
    areas_dir = Path(args.vault) / "areas"
    if not journal.is_dir():
        print(f"error: journal directory not found at {journal}", file=sys.stderr)
        return 1
    if args.days < 1:
        print("error: --days must be >= 1", file=sys.stderr)
        return 1

    cutoff = datetime.now(timezone.utc).astimezone().date() - timedelta(days=args.days)
    hits: dict[str, list[date]] = {}
    for note in sorted(journal.glob("*.md")):
        found = area_links(note, areas_dir)
        if found is None:
            continue
        day, areas = found
        if day >= cutoff:
            for area in areas:
                hits.setdefault(area, []).append(day)

    if not hits:
        print(f"no area entries in the last {args.days} days")
        return 0

    for area, days in sorted(hits.items(), key=lambda r: max(r[1]), reverse=True):
        print(f"{area} ({len(days)}): {', '.join(d.isoformat() for d in sorted(days, reverse=True))}")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except BrokenPipeError:
        sys.stdout = open(os.devnull, "w")
        sys.exit(0)
