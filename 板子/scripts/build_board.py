#!/usr/bin/env python3
"""
Run the standard board maintenance pipeline.

Usage:
  python 板子/scripts/build_board.py
  python 板子/scripts/build_board.py --check-only
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
BOARD_DIR = ROOT / "板子"
SCRIPTS_DIR = BOARD_DIR / "scripts"
GENERATED_DIR = BOARD_DIR / "generated"
OUTPUT_DIR = BOARD_DIR / "output"

OFFICIAL_PDFS = {
    "board1-algorithms.pdf",
    "board2-number-theory.pdf",
    "board3-misc.pdf",
}


def run(args: list[str]) -> None:
    print(f"$ {' '.join(args)}", flush=True)
    subprocess.run(args, cwd=ROOT, check=True)


def cleanup() -> None:
    for path in GENERATED_DIR.glob("_*"):
        if path.is_file():
            path.unlink()
    for path in BOARD_DIR.glob("_*"):
        if path.is_file():
            path.unlink()
    for path in OUTPUT_DIR.glob("*"):
        if path.is_file() and path.name not in OFFICIAL_PDFS:
            path.unlink()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--check-only",
        action="store_true",
        help="run format/config checks without regenerating PDFs",
    )
    args = parser.parse_args()

    py = sys.executable
    fix_args = [py, str(SCRIPTS_DIR / "fix_md_math.py")]
    if args.check_only:
        fix_args.append("--dry-run")
    run(fix_args)
    run([py, str(SCRIPTS_DIR / "check_note_style.py")])
    run([py, str(SCRIPTS_DIR / "generate_typst.py"), "--check"])
    if not args.check_only:
        run([py, str(SCRIPTS_DIR / "generate_typst.py")])
        cleanup()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
