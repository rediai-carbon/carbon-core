#!/usr/bin/env python3
"""Self-test for assert_ctest_results parser (no ctest execution)."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

SAMPLE = """\
100% tests passed, 0 tests failed out of 690

Total Test time (real) =   4.47 sec

The following tests did not run:
\t 27 - CcpThread.CanKillThread (Disabled)
\t128 - CCPMemoryTracker.TextReportContainsAllocatedInformation (Disabled)
\t175 - Callstack.StackSymbolsContainCallerFunctionName (Disabled)
\t176 - Callstack.StackSymbolsAreNotGarbled (Disabled)
\t180 - Callstack.SymbolDumpContainsFunctionName (Disabled)
"""

ROOT = Path(__file__).resolve().parents[2]
ASSERT = ROOT / "scripts/ci/assert_ctest_results.py"


def main() -> int:
    log = ROOT / "scripts/ci/.ctest_parser_selftest.log"
    log.write_text(SAMPLE, encoding="utf-8")
    proc = subprocess.run(
        [sys.executable, str(ASSERT), str(log)],
        capture_output=True,
        text=True,
    )
    log.unlink(missing_ok=True)
    if proc.returncode != 0:
        print(proc.stderr, file=sys.stderr)
        return 1
    print("ctest parser self-test OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
