#!/usr/bin/env python3
"""
generate_typst.py — 从原始 .cpp / .md 生成三份 Typst 板子正文并编译 PDF
用法: python 板子/scripts/generate_typst.py [--check]
输出: 板子/generated/board1-algorithms.typ
      板子/generated/board2-number-theory.typ
      板子/generated/board3-misc.typ
      板子/generated/manifest.json
"""

import glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
# ROOT = Mystudy 根目录
GEN_DIR = os.path.join(ROOT, "板子", "generated")
CONFIG_PATH = os.path.join(ROOT, "板子", "config", "board_config.json")
os.makedirs(GEN_DIR, exist_ok=True)


def load_config() -> dict:
    with open(CONFIG_PATH, "r", encoding="utf-8") as f:
        return json.load(f)


CONFIG = load_config()

# ── Boilerplate 识别 ──
# 匹配 cpp模板.cpp 中的公共头部行
BOILERPLATE_PATTERNS = [
    re.compile(r"^\s*#include\s*<"),
    re.compile(r"^\s*using\s+namespace\s+std"),
    re.compile(r"^\s*using\s+namespace\s+__gnu_pbds"),
    re.compile(r"^\s*using\s+ll\s*=\s*long\s+long"),
    re.compile(r"^\s*//#define\s+int\s+long\s+long"),
    re.compile(r"^\s*//freopen"),
    re.compile(r"^\s*//ios::sync_with_stdio"),
    re.compile(r"^\s*ios::sync_with_stdio"),
    re.compile(r"^\s*int\s+T_start\s*=\s*clock"),
    re.compile(r"^\s*auto\s+T_start\s*=\s*chrono"),
]


def is_boilerplate_line(line: str) -> bool:
    return any(p.match(line) for p in BOILERPLATE_PATTERNS)


def is_empty_main(lines: list[str]) -> bool:
    """判断 main 函数是否为空壳（只含 boilerplate/return 0）"""
    body = []
    for l in lines:
        s = l.strip()
        if not s or s == "{" or s == "}":
            continue
        if s.startswith("signed main") or s.startswith("int main"):
            continue
        if s == "return 0;":
            continue
        if is_boilerplate_line(l):
            continue
        body.append(s)
    return len(body) == 0


def extract_cpp_content(filepath: str) -> dict:
    """从 .cpp 提取算法主体、尾部注释、可选 usage 示例（保持源码原始缩进与换行）"""
    with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
        raw_lines = f.readlines()

    # 1. 跳过前导 boilerplate
    start = 0
    for i, line in enumerate(raw_lines):
        if is_boilerplate_line(line) or line.strip() == "":
            start = i + 1
        else:
            break

    # 2. 找 main 函数位置
    main_start = None
    for i in range(start, len(raw_lines)):
        s = raw_lines[i].strip()
        if s.startswith("signed main") or s.startswith("int main"):
            main_start = i
            break

    # 3. 找 main 结束（匹配大括号）
    main_end = None
    if main_start is not None:
        depth = 0
        for i in range(main_start, len(raw_lines)):
            for ch in raw_lines[i]:
                if ch == "{":
                    depth += 1
                elif ch == "}":
                    depth -= 1
                    if depth == 0:
                        main_end = i
                        break
            if main_end is not None:
                break

    # 4. 提取尾部注释（main 之后的行）
    tail_comments = []
    tail_start = (main_end + 1) if main_end is not None else len(raw_lines)
    for i in range(tail_start, len(raw_lines)):
        s = raw_lines[i].strip()
        if s.startswith("//") or s == "":
            tail_comments.append(raw_lines[i].rstrip())
        elif s:
            tail_comments.append(raw_lines[i].rstrip())

    # 5. 算法主体 = start .. main_start (不含 main)
    if main_start is not None:
        algo_lines = raw_lines[start:main_start]
    else:
        algo_lines = raw_lines[start:]

    # 6. 判断 main 是否有 usage 价值
    usage_lines = []
    if main_start is not None and main_end is not None:
        main_body = raw_lines[main_start : main_end + 1]
        if not is_empty_main(main_body):
            in_body = False
            stripped_outer_open = False
            last_line = len(main_body) - 1
            for idx, l in enumerate(main_body):
                s = l.strip()
                if s.startswith("signed main") or s.startswith("int main"):
                    in_body = True
                    stripped_outer_open = "{" in s
                    continue
                if not in_body:
                    continue
                if is_boilerplate_line(l):
                    continue
                if idx == last_line and s == "}":
                    continue
                if not stripped_outer_open and s == "{":
                    stripped_outer_open = True
                    continue
                if s in ("return 0;", ""):
                    continue
                usage_lines.append(l.rstrip())

    # 清理算法主体尾部空行
    while algo_lines and algo_lines[-1].strip() == "":
        algo_lines.pop()

    return {
        "algo": "".join(algo_lines).rstrip(),
        "tail_comments": "\n".join(c for c in tail_comments if c.strip()),
        "usage": "\n".join(usage_lines) if usage_lines else "",
    }


def escape_typst(s: str) -> str:
    """完全转义 Typst 特殊字符（用于错误消息等纯文本）"""
    s = s.replace("\\", "\\\\")
    s = s.replace("#", "\\#")
    s = s.replace("$", "\\$")
    s = s.replace("@", "\\@")
    s = s.replace("<", "\\<")
    s = s.replace(">", "\\>")
    s = s.replace("_", "\\_")
    s = s.replace("*", "\\*")
    return s


def escape_raw_content(s: str) -> str:
    """确保 raw block 内容不含 ``` 序列"""
    return s.expandtabs(4).replace("```", "` ` `")


def clean_generated_content(s: str) -> str:
    """Normalize generated Typst text before writing it to disk."""
    return "\n".join(line.rstrip() for line in s.replace("\r", "").split("\n"))


def rel_path(path: str) -> str:
    return path.replace("\\", "/")


def clean_note_title(fname: str) -> str:
    """剥离冗长的目录前缀，返回干净的笔记标题"""
    return os.path.splitext(os.path.basename(fname))[0]


def is_unnormalized_note(source_path: str) -> bool:
    norm_set = {rel_path(p) for p in CONFIG.get("note_style", {}).get("normalized_files", [])}
    return rel_path(source_path) not in norm_set


def config_md_files(board: str | None = None) -> list[tuple[str, str, str]]:
    """Return (board, group, repo-relative md path) entries from config."""
    entries = []
    boards = ["board2", "board3"] if board is None else [board]
    for board_name in boards:
        for group in CONFIG[board_name]["groups"]:
            for fname in group["files"]:
                entries.append((board_name, group["name"], rel_path(fname)))
    return entries


def all_config_md_paths() -> set[str]:
    return {path for _, _, path in config_md_files()}


def validate_config() -> list[str]:
    errors = []

    template = CONFIG.get("board1", {}).get("template")
    if not template or not os.path.isfile(os.path.join(ROOT, template)):
        errors.append(f"board1.template 不存在: {template}")

    folders = CONFIG.get("board1", {}).get("folders", [])
    if not isinstance(folders, list) or not folders:
        errors.append("board1.folders 必须是非空列表")
    for folder in folders:
        if not os.path.isdir(os.path.join(ROOT, folder)):
            errors.append(f"board1.folders 目录不存在: {folder}")

    seen = set()
    for board_name in ("board2", "board3"):
        groups = CONFIG.get(board_name, {}).get("groups", [])
        if not isinstance(groups, list):
            errors.append(f"{board_name}.groups 必须是列表")
            continue
        for group in groups:
            name = group.get("name")
            files = group.get("files")
            if not name:
                errors.append(f"{board_name}.groups 存在缺失 name 的分组")
            if not isinstance(files, list):
                errors.append(f"{board_name}.{name}.files 必须是列表")
                continue
            for fname in files:
                path = rel_path(fname)
                key = (board_name, path)
                if key in seen:
                    errors.append(f"{board_name} 重复收录: {path}")
                seen.add(key)
                if not path.endswith(".md"):
                    errors.append(f"{board_name} 非 .md 文件: {path}")
                if not os.path.isfile(os.path.join(ROOT, path)):
                    errors.append(f"{board_name} 文件不存在: {path}")

    for img in CONFIG.get("board2", {}).get("reference_images", []):
        if not os.path.isfile(os.path.join(ROOT, img)):
            errors.append(f"board2.reference_images 文件不存在: {img}")

    source_dirs = CONFIG.get("board1", {}).get("folders", [])
    actual_md = {
        rel_path(os.path.relpath(path, ROOT))
        for folder in source_dirs
        for path in glob.glob(os.path.join(ROOT, folder, "**", "*.md"), recursive=True)
    }
    configured_md = all_config_md_paths()
    for path in sorted(actual_md - configured_md):
        errors.append(
            f"发现未收录 .md: {path}；"
            "新增笔记需先由 subagent 审视内容并按 NOTE_STYLE 整理，再写入 board_config.json"
        )
    for path in sorted(configured_md - actual_md):
        errors.append(f"配置中的 .md 不在源码目录扫描结果中: {path}")

    return errors


def check_config_or_exit() -> None:
    errors = validate_config()
    if errors:
        print("配置检查失败:")
        for err in errors:
            print(f"  - {err}")
        sys.exit(1)
    print("配置检查通过")


# ══════════════════════════════════════════════
# Board 1: 算法模板板
# ══════════════════════════════════════════════
def generate_board1() -> tuple[str, list]:
    """生成算法模板板 Typst 正文"""
    manifest = []
    parts = ['#import "../typst/common.typ": *\n']

    # cpp模板.cpp 放在最前面
    cpp_template = os.path.join(ROOT, CONFIG["board1"]["template"])
    if os.path.exists(cpp_template):
        with open(cpp_template, "r", encoding="utf-8", errors="ignore") as f:
            code = f.read().strip()
        parts.append("= 通用模板\n")
        parts.append("== cpp模板\n")
        parts.append(f"```cpp\n{code}\n```\n")
        manifest.append(
            {
                "board": "board1",
                "folder": "板子",
                "file": "cpp模板.cpp",
                "title": "cpp模板",
                "has_usage": False,
                "has_tail": False,
            }
        )

    for folder in CONFIG["board1"]["folders"]:
        folder_path = os.path.join(ROOT, folder)
        if not os.path.isdir(folder_path):
            continue
        cpp_files = sorted(
            glob.glob(os.path.join(folder_path, "**", "*.cpp"), recursive=True),
            key=lambda x: x.lower(),
        )
        if not cpp_files:
            continue

        parts.append(f"= {folder}\n")

        for fpath in cpp_files:
            fname = os.path.basename(fpath)
            title = os.path.splitext(fname)[0]
            result = extract_cpp_content(fpath)

            entry = {
                "board": "board1",
                "folder": folder,
                "file": fname,
                "title": title,
                "has_usage": bool(result["usage"]),
                "has_tail": bool(result["tail_comments"]),
            }
            manifest.append(entry)

            parts.append(f"== {title}\n")

            # 尾部注释放在独立代码块中，避免以 Typst `//` 注释输出后不可见。
            if result["tail_comments"]:
                tail_escaped = escape_raw_content(result["tail_comments"])
                parts.append("```cpp")
                parts.append(tail_escaped)
                parts.append("```\n")

            # 算法主体代码
            if result["algo"].strip():
                algo_escaped = escape_raw_content(result["algo"])
                parts.append("```cpp")
                parts.append(algo_escaped)
                parts.append("```\n")

            # 用法示例
            if result["usage"]:
                parts.append('#code-badge("用法示例 (main)")\n')
                usage_escaped = escape_raw_content(result["usage"])
                parts.append("```cpp")
                parts.append(usage_escaped)
                parts.append("```\n")

    return "\n".join(parts), manifest


# ── Pandoc 数学公式中常见的被拆散多字母标识符修复表 ──
SPACED_MATH_WORDS = [
    ("s p l i t m i x 64", '"splitmix64"'),
    ("s p l i t m i x", '"splitmix"'),
    ("c h k l i s", '"chklis"'),
    ("l a b e l", '"label"'),
    ("m x l a b", '"mxlab"'),
    ("H a s h", '"Hash"'),
    ("h a s h", '"hash"'),
    ("h a c k", '"hack"'),
    ("g c d", "gcd"),
    ("l c m", "lcm"),
    ("m a x", "max"),
    ("m i n", "min"),
    ("s u m", '"sum"'),
    ("c n t", '"cnt"'),
    ("v i s", '"vis"'),
    ("p e o", '"peo"'),
    ("s o n", '"son"'),
    ("i n v", '"inv"'),
    ("a n s", '"ans"'),
    ("d e g", "deg"),
    ("r k", '"rk"'),
]


# ══════════════════════════════════════════════
# Board 2 & 3: Pandoc 转换与排版后处理
# ══════════════════════════════════════════════
def pandoc_md_to_typst(
    md_text: str,
    source_path: str,
    file_title: str = "",
    heading_offset: int = 2,
    img_base: str = "",
) -> str:
    """用 Pandoc 将 Markdown 转为 Typst，并做标题清洗、公式修复、随笔呼吸感与图片紧凑化"""
    # 预处理：修复单侧带空格的行内公式（如 `$ (L + R)$` 或 `$x^{\underline{n}} ... $`）
    fixed_lines = []
    in_fence = False
    for line in md_text.split("\n"):
        if line.strip().startswith("```") or line.strip() == "$$":
            in_fence = not in_fence
            fixed_lines.append(line)
            continue
        if not in_fence and "|" not in line and "$$" not in line:
            line = re.sub(
                r"(?<!\$)\$(?!\$)(\s*[^\$\n]+?\s*)(?<!\$)\$(?!\$)",
                lambda m: f"${m.group(1).strip()}$"
                if not re.search(r"[\u4e00-\u9fff]", m.group(1))
                else m.group(0),
                line,
            )
        fixed_lines.append(line)
    md_text = "\n".join(fixed_lines)

    md_text = re.sub(
        r"(?m)^<!--\s*board:hr\s*-->\s*$",
        "```{=typst}\n#horizontalrule\n```",
        md_text,
    )
    md_text = re.sub(
        r"(?m)^---\s*$",
        "```{=typst}\n#horizontalrule\n```",
        md_text,
    )

    # 对未规范化的随笔/trick 笔记实现三级间距层次：
    # 1. 自动折行 (leading: 0.50em)
    # 2. 作者单行回车 \n (spacing: 0.78em，约半行空隙，区分折行与换行)
    # 3. 作者空行分段 \n\n (额外插入 #v(0.5em)，合计约 1.3em 完整空行，清晰分隔不同 trick)
    unnormalized = is_unnormalized_note(source_path)
    pandoc_from = (
        "markdown+hard_line_breaks-yaml_metadata_block"
        if unnormalized
        else "markdown-yaml_metadata_block"
    )
    result = subprocess.run(
        ["pandoc", "-f", pandoc_from, "-t", "typst", "--wrap=none"],
        input=md_text,
        capture_output=True,
        text=True,
        encoding="utf-8",
    )
    if result.returncode != 0:
        raise RuntimeError(f"Pandoc 转换失败: {source_path}\n{result.stderr.strip()}")

    typst_text = result.stdout
    if unnormalized:
        # 先把真正的段落空行 (\n\n) 标记为显式大空行（跳过列表项 `+`/`-` 及缩进续行，以免打断有序列表编号）
        if not re.search(r"(?m)^\+\s+", typst_text):
            typst_text = re.sub(
                r"\n{2,}(?![ \t]*(?:[\+\-]|\#))", "\n\n#v(0.5em)\n\n", typst_text
            )
        # 再把单行回车 (\) 转为小段间距分段
        typst_text = re.sub(r"(?m)\\\s*$", "\n\n", typst_text)
        typst_text = f"#[\n#set par(leading: 0.50em, spacing: 0.78em)\n#set enum(spacing: 0.85em)\n{typst_text}\n]"

    # 调整标题层级：以当前文件内最浅标题作为正文三级标题 (===)
    heading_re = re.compile(r"^(=+)\s+(?![&=])(.*)$", flags=re.MULTILINE)
    heading_levels = [len(m.group(1)) for m in heading_re.finditer(typst_text)]
    min_heading_level = min(heading_levels) if heading_levels else 1

    def adjust_heading(m):
        level = len(m.group(1))
        new_level = min(level - min_heading_level + 1 + heading_offset, 6)
        return "=" * new_level + " " + m.group(2)

    typst_text = heading_re.sub(adjust_heading, typst_text)

    # 若笔记开头的第一个小节标题与文件标题完全同义（如 `== FFT笔记` 下紧跟 `=== FFT`），自动消重
    if file_title:
        norm_ft = re.sub(
            r"[\s\(\)（）_\-]|数论|图论|数学|笔记|部分|^记", "", file_title
        ).lower()
        first_h_match = re.match(r"^\s*(=+\s+([^\n]+))\n(?:<[^>]+>\n)?", typst_text)
        if first_h_match:
            h_text = first_h_match.group(2).strip()
            norm_ht = re.sub(
                r"[\s\(\)（）_\-]|数论|图论|数学|笔记|部分|^记", "", h_text
            ).lower()
            if norm_ht and norm_ht == norm_ft:
                typst_text = typst_text[first_h_match.end() :].lstrip("\n")

    # 修复 Pandoc 生成的尾随逗号（如 table/tuple 参数中的 `,)`）
    typst_text = re.sub(r",\s*\)", ")", typst_text)

    # ── 修复 Pandoc 输出中的 Typst 数学问题 ──

    # 1. \pmod 修复: Pandoc 将 \pmod{m} 输出为 \( mod med m \)
    typst_text = re.sub(
        r"\\\\?\(\s*(?:med\s+)?mod\s+med\s+(.+?)\s*\\\\?\)",
        r"(mod \1)",
        typst_text,
    )

    # 2. 修复 Pandoc 未转换的 $$...$$ 块（显示为 \$\$...\$\$）
    def fix_raw_math_block(m):
        raw_latex = m.group(1).strip()
        raw_latex = raw_latex.replace("\\\\", "\\")
        raw_latex = raw_latex.replace("\\$", "$")
        raw_latex = raw_latex.replace("\\_", "_")
        inner_result = subprocess.run(
            ["pandoc", "-f", "markdown-yaml_metadata_block", "-t", "typst", "--wrap=none"],
            input=f"$$\n{raw_latex}\n$$",
            capture_output=True,
            text=True,
            encoding="utf-8",
        )
        if inner_result.returncode != 0:
            return f"```\n{raw_latex}\n```"
        inner = inner_result.stdout.strip()
        if inner and not inner.startswith("\\$"):
            return inner
        return f"```\n{raw_latex}\n```"

    typst_text = re.sub(
        r"\\\$\n?\\?\$(.*?)\n?\\\$\\?\$",
        fix_raw_math_block,
        typst_text,
        flags=re.DOTALL,
    )

    # 3. 修复数学公式内部残留的 \( \) 与被拆散的多字母变量（如 g c d -> gcd, H a s h -> "Hash"）
    def fix_math_internals(m):
        content = m.group(0)
        content = content.replace("\\(", "(").replace("\\)", ")")
        for spaced, replacement in SPACED_MATH_WORDS:
            content = re.sub(
                rf'(?<![a-zA-Z"]){re.escape(spaced)}(?![a-zA-Z"])',
                replacement,
                content,
            )
        return content

    typst_text = re.sub(r"\$[^$]+\$", fix_math_internals, typst_text)

    # 4. 清理无意义的默认图片图注（如 `caption: [本地图片]` / `caption: [alt text]`）
    #    对宽幅高密度截图（卡特兰数.png、FFT1~7.png）采用跨双栏通栏放大显示，确保纸质打印字迹清晰
    def replace_figure_block(m):
        img_call = m.group(1)
        if any(
            k in img_call
            for k in (
                "卡特兰数.png",
                "FFT1.png",
                "FFT2.png",
                "FFT3.png",
                "FFT4.png",
                "FFT5.png",
                "FFT6.png",
                "FFT7.png",
            )
        ):
            return (
                '#place(auto, scope: "parent", float: true)['
                "#block(width: 100%, fill: white, inset: (y: 2pt))["
                "#align(center)[#block(width: 84%, fill: white, stroke: 0.45pt + c-code-border, radius: 2pt, clip: true)["
                f"#{img_call}"
                "]]]]"
            )
        return (
            "#block(width: 100%, fill: white, above: 0.3em, below: 0.3em, stroke: 0.45pt + c-code-border, radius: 1.5pt, clip: true)["
            f"#align(center)[#{img_call}]"
            "]"
        )

    typst_text = re.sub(
        r"#figure\(\s*(image\([^\)]+\))\s*,\s*caption:\s*\[\s*(?:本地图片|alt text|image|图片)\s*\]\s*\)",
        replace_figure_block,
        typst_text,
    )

    # 5. 修复图片路径：转为相对于 generated/ 目录的路径，并对竖向图限制高度
    if img_base:
        def fix_img_path(m):
            prefix = m.group(1)
            rpath = m.group(2)
            abs_path = os.path.normpath(os.path.join(img_base, rpath))
            if os.path.exists(abs_path):
                gen_rel = os.path.relpath(abs_path, GEN_DIR).replace("\\", "/")
                if os.path.basename(abs_path) == "image.png":
                    return f'{prefix}"{gen_rel}", height: 4.0cm, fit: "contain")'
                return f'{prefix}"{gen_rel}", width: 100%)'
            else:
                return f'[#text(fill: red)[图片缺失: {escape_typst(rpath)}]]'

        typst_text = re.sub(r'(#?(?:box\()?image\()"([^"]+)"\)', fix_img_path, typst_text)

    return typst_text


def generate_board2() -> tuple[str, list]:
    """生成数论板 Typst 正文（使用 Pandoc 转换）"""
    manifest = []
    parts = ['#import "../typst/common.typ": *\n']

    # 参考图表放在最前面
    ref_images = CONFIG["board2"].get("reference_images", [])
    has_refs = [os.path.exists(os.path.join(ROOT, img)) for img in ref_images]
    if any(has_refs):
        parts.append("= 常用速查图表\n")
        for img, exists in zip(ref_images, has_refs):
            if exists:
                image_rel_path = os.path.relpath(
                    os.path.join(ROOT, img), GEN_DIR
                ).replace("\\", "/")
                title = os.path.splitext(img)[0]
                parts.append(f"== {title}\n")
                if os.path.basename(img) == "图.png":
                    # 横版三栏速查大图采用跨双栏通栏排布，保证打印字迹清晰可读
                    parts.append(
                        "#text(size: 7.8pt, fill: c-muted)[（见本页下方通栏大图：U 群常见问题速查）]\n"
                    )
                    parts.append(
                        f'#place(bottom + center, scope: "parent", float: true)['
                        f"#block(width: 100%, fill: white, stroke: 0.45pt + c-code-border, radius: 2pt, clip: true)["
                        f'#image("{image_rel_path}", width: 100%)]]\n'
                    )
                else:
                    parts.append(
                        f'#block(fill: white, stroke: 0.45pt + c-code-border, radius: 1.5pt, clip: true)[#image("{image_rel_path}", width: 100%)]\n'
                    )

    for group in CONFIG["board2"]["groups"]:
        group_name = group["name"]
        files = group["files"]
        parts.append(f"= {group_name}\n")

        for fname in files:
            fname = rel_path(fname)
            fpath = os.path.join(ROOT, fname)
            clean_title = clean_note_title(fname)

            entry = {
                "board": "board2",
                "group": group_name,
                "file": fname,
                "title": clean_title,
                "warnings": [],
            }

            if not os.path.exists(fpath):
                entry["warnings"].append("文件不存在")
                manifest.append(entry)
                parts.append(f"== {clean_title}\n")
                parts.append(f"#text(fill: red)[文件缺失: {fname}]\n")
                continue

            with open(fpath, "r", encoding="utf-8", errors="ignore") as f:
                md_text = f.read()

            manifest.append(entry)
            parts.append(f"== {clean_title}\n")
            img_base = os.path.dirname(fpath)
            converted = pandoc_md_to_typst(
                md_text,
                fname,
                file_title=clean_title,
                heading_offset=2,
                img_base=img_base,
            )
            parts.append(converted)
            parts.append("")

    return "\n".join(parts), manifest


def generate_board3() -> tuple[str, list]:
    """生成杂项板 Typst 正文（使用 Pandoc 转换）"""
    manifest = []
    parts = ['#import "../typst/common.typ": *\n']

    for group in CONFIG["board3"]["groups"]:
        group_name = group["name"]
        file_list = group["files"]
        parts.append(f"= {group_name}\n")

        for fname in file_list:
            fname = rel_path(fname)
            fpath = os.path.join(ROOT, fname)
            clean_title = clean_note_title(fname)

            entry = {
                "board": "board3",
                "group": group_name,
                "file": fname,
                "title": clean_title,
                "warnings": [],
            }

            if not os.path.exists(fpath):
                entry["warnings"].append("文件不存在")
                manifest.append(entry)
                parts.append(f"== {clean_title}\n")
                parts.append(f"#text(fill: red)[文件缺失: {fname}]\n")
                continue

            with open(fpath, "r", encoding="utf-8", errors="ignore") as f:
                md_text = f.read()

            manifest.append(entry)
            parts.append(f"== {clean_title}\n")
            img_base = os.path.dirname(fpath)
            converted = pandoc_md_to_typst(
                md_text,
                fname,
                file_title=clean_title,
                heading_offset=2,
                img_base=img_base,
            )
            parts.append(converted)
            parts.append("")

    return "\n".join(parts), manifest


# ══════════════════════════════════════════════
# Main
# ══════════════════════════════════════════════
def main():
    if "--check" in sys.argv:
        check_config_or_exit()
        return

    check_config_or_exit()
    all_manifest = []

    # Board 1
    content1, m1 = generate_board1()
    with open(
        os.path.join(GEN_DIR, "board1-algorithms.typ"),
        "w",
        encoding="utf-8",
        newline="\n",
    ) as f:
        f.write(clean_generated_content(content1))
    all_manifest.extend(m1)
    print(f"Board 1: {len(m1)} 个算法模板")

    # Board 2 (Pandoc)
    content2, m2 = generate_board2()
    with open(
        os.path.join(GEN_DIR, "board2-number-theory.typ"),
        "w",
        encoding="utf-8",
        newline="\n",
    ) as f:
        f.write(clean_generated_content(content2))
    all_manifest.extend(m2)
    print(f"Board 2: {len(m2)} 个数论笔记")

    # Board 3 (Pandoc)
    content3, m3 = generate_board3()
    with open(
        os.path.join(GEN_DIR, "board3-misc.typ"),
        "w",
        encoding="utf-8",
        newline="\n",
    ) as f:
        f.write(clean_generated_content(content3))
    all_manifest.extend(m3)
    print(f"Board 3: {len(m3)} 个杂项笔记")

    # Manifest
    with open(os.path.join(GEN_DIR, "manifest.json"), "w", encoding="utf-8") as f:
        json.dump(all_manifest, f, ensure_ascii=False, indent=2)
    print(f"Manifest: {len(all_manifest)} 条记录")

    try:
        import typst
    except ImportError as exc:
        raise RuntimeError("typst 未安装，无法编译 PDF") from exc

    typst_dir = os.path.join(ROOT, "板子", "typst")
    out_dir = os.path.join(ROOT, "板子", "output")
    os.makedirs(out_dir, exist_ok=True)
    boards = ["board1-algorithms", "board2-number-theory", "board3-misc"]
    for name in boards:
        src = os.path.join(typst_dir, f"{name}.typ")
        dst = os.path.join(out_dir, f"{name}.pdf")
        pdf = typst.compile(src, root=ROOT)
        with open(dst, "wb") as f:
            f.write(pdf)
        pages = len(re.findall(rb"/Type\s*/Page\b", pdf))
        print(f"  -> {name}.pdf ({pages} 页, {len(pdf)//1024}KB)")
    print("PDF 编译完成")


if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        print(f"生成失败: {exc}", file=sys.stderr)
        sys.exit(1)
