#!/usr/bin/env python3
"""
check_note_style.py — check Markdown source notes used by Board 2/3.

By default this checks only files listed in config/board_config.json
note_style.normalized_files. Use --all to inspect every non-blacklisted
Board 2/3 note. Use --include-blacklisted or --include-deferred to include
deferred notes as well.
"""

from __future__ import annotations

import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CONFIG_PATH = ROOT / "板子" / "config" / "board_config.json"


@dataclass
class Issue:
    path: str
    line: int
    code: str
    message: str

    def format(self) -> str:
        return f"{self.path}:{self.line}: {self.code} {self.message}"


def rel_path(path: Path) -> str:
    return path.relative_to(ROOT).as_posix()


def load_config() -> dict:
    with CONFIG_PATH.open("r", encoding="utf-8") as f:
        return json.load(f)


def is_blacklisted(repo_path: str, config: dict) -> bool:
    name = repo_path.replace("\\", "/").split("/")[-1]
    style = config.get("note_style", {})
    contains = [s.lower() for s in style.get("blacklist_name_contains", [])]
    if any(s in name.lower() for s in contains):
        return True
    return name in set(style.get("blacklist_names", []))


def all_note_files(config: dict, include_blacklisted: bool) -> list[str]:
    files: list[str] = []
    for board_name in ("board2", "board3"):
        for group in config[board_name]["groups"]:
            for repo_path in group["files"]:
                normalized = repo_path.replace("\\", "/")
                if include_blacklisted or not is_blacklisted(normalized, config):
                    files.append(normalized)
    return sorted(set(files))


def iter_note_files(config: dict, include_blacklisted: bool, check_all: bool) -> list[str]:
    if check_all:
        return all_note_files(config, include_blacklisted=include_blacklisted)
    style = config.get("note_style", {})
    files = [path.replace("\\", "/") for path in style.get("normalized_files", [])]
    if not include_blacklisted:
        files = [path for path in files if not is_blacklisted(path, config)]
    return sorted(set(files))


def is_blank(line: str) -> bool:
    return line.strip() == ""


def check_file(repo_path: str) -> list[Issue]:
    path = ROOT / repo_path
    issues: list[Issue] = []
    if not path.is_file():
        return [Issue(repo_path, 1, "MISSING", "configured note file does not exist")]

    text = path.read_text(encoding="utf-8", errors="ignore")
    lines = text.splitlines()
    in_code = False
    in_math = False
    math_start = 0
    code_start = 0
    prev_heading_level = 0

    for idx, line in enumerate(lines, start=1):
        stripped = line.strip()

        if line.rstrip(" \t") != line:
            issues.append(Issue(repo_path, idx, "TRAILING_SPACE", "remove trailing spaces"))

        if stripped.startswith("```"):
            if not in_code:
                code_start = idx
                info = stripped[3:].strip()
                if not info:
                    issues.append(Issue(repo_path, idx, "CODE_LANG", "fenced code blocks need a language tag"))
                if idx > 1 and not is_blank(lines[idx - 2]):
                    issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line before fenced code"))
                in_code = True
            else:
                if idx < len(lines) and not is_blank(lines[idx]):
                    issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line after fenced code"))
                in_code = False
            continue

        if in_code:
            continue

        if stripped == "$$":
            if not in_math:
                math_start = idx
                if idx > 1 and not is_blank(lines[idx - 2]):
                    issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line before display math"))
                in_math = True
            else:
                if idx < len(lines) and not is_blank(lines[idx]):
                    issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line after display math"))
                in_math = False
            continue

        if in_math:
            continue

        heading = re.match(r"^(#{1,6})\s+\S", line)
        if heading:
            level = len(heading.group(1))
            if idx > 1 and not is_blank(lines[idx - 2]):
                issues.append(Issue(repo_path, idx, "HEADING_BLANK", "add a blank line before heading"))
            if idx < len(lines) and not is_blank(lines[idx]):
                issues.append(Issue(repo_path, idx, "HEADING_BLANK", "add a blank line after heading"))
            if prev_heading_level and level > prev_heading_level + 1:
                issues.append(Issue(repo_path, idx, "HEADING_JUMP", "do not skip heading levels"))
            prev_heading_level = level

        if re.match(r"^\s*---\s*$", line):
            issues.append(Issue(repo_path, idx, "RAW_HR", "use <!-- board:hr --> instead of ---"))

        if re.search(r"\\\[|\\\]", line):
            issues.append(Issue(repo_path, idx, "DISPLAY_MATH", "use multiline $$ blocks instead of \\[...\\]"))

        if re.match(r"^\s*\$\$.+\$\$\s*$", line):
            issues.append(Issue(repo_path, idx, "DISPLAY_MATH", "split single-line $$...$$ into multiline display math"))

        if stripped == "<!-- board:hr -->" or re.match(r"!\[[^\]]*\]\([^)]*\)", stripped):
            if idx > 1 and not is_blank(lines[idx - 2]):
                issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line before block"))
            if idx < len(lines) and not is_blank(lines[idx]):
                issues.append(Issue(repo_path, idx, "BLOCK_BLANK", "add a blank line after block"))

    if in_code:
        issues.append(Issue(repo_path, code_start, "CODE_FENCE", "unclosed fenced code block"))
    if in_math:
        issues.append(Issue(repo_path, math_start, "DISPLAY_MATH", "unclosed display math block"))

    return issues


def main() -> int:
    include_blacklisted = (
        "--include-blacklisted" in sys.argv or
        "--include-deferred" in sys.argv
    )
    check_all = "--all" in sys.argv
    config = load_config()
    files = iter_note_files(
        config,
        include_blacklisted=include_blacklisted,
        check_all=check_all,
    )
    issues: list[Issue] = []
    for repo_path in files:
        issues.extend(check_file(repo_path))

    if issues:
        for issue in issues:
            print(issue.format())
        print(f"样式检查失败: {len(issues)} 个问题，扫描 {len(files)} 个文件")
        return 1

    print(f"样式检查通过: {len(files)} 个文件")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
