#!/usr/bin/env python3
"""Parse ctest log lines for pass/fail/disabled floors (M04 baseline)."""

from __future__ import annotations

import argparse
import re
import sys

PASS_RE = re.compile(r"(\d+)\s+tests?\s+passed", re.I)
FAIL_RE = re.compile(r"(\d+)\s+tests?\s+failed", re.I)
DISABLED_RE = re.compile(r"(\d+)\s+tests?\s+disabled", re.I)
PAREN_DISABLED = re.compile(r"\(\s*(\d+)\s+disabled\s*\)", re.I)


def parse_counts(text: str) -> tuple[int | None, int | None, int | None]:
    passed = failed = disabled = None
    for line in text.splitlines():
        if m := PASS_RE.search(line):
            passed = int(m.group(1))
        if m := FAIL_RE.search(line):
            failed = int(m.group(1))
        if m := DISABLED_RE.search(line):
            disabled = int(m.group(1))
        if m := PAREN_DISABLED.search(line):
            disabled = int(m.group(1))
    return passed, failed, disabled


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("logfile", type=argparse.FileType("r", encoding="utf-8", errors="replace"))
    parser.add_argument("--min-passed", type=int, default=690)
    parser.add_argument("--max-failed", type=int, default=0)
    parser.add_argument("--exact-disabled", type=int, default=5)
    args = parser.parse_args()
    text = args.logfile.read()
    passed, failed, disabled = parse_counts(text)
    errors: list[str] = []
    if passed is None:
        errors.append("could not parse passed count from log")
    elif passed < args.min_passed:
        errors.append(f"passed {passed} < minimum {args.min_passed}")
    if failed is None:
        errors.append("could not parse failed count from log")
    elif failed > args.max_failed:
        errors.append(f"failed {failed} > maximum {args.max_failed}")
    if disabled is None:
        errors.append("could not parse disabled count from log")
    elif disabled != args.exact_disabled:
        errors.append(f"disabled {disabled} != expected {args.exact_disabled}")
    if errors:
        for e in errors:
            print(e, file=sys.stderr)
        return 1
    print(
        f"ctest OK: passed={passed} failed={failed} disabled={disabled}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
