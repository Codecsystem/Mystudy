#!/usr/bin/env python3
"""
generate_typst.py — 从原始 .cpp / .md 生成三份 Typst 板子正文
用法: python generate_typst.py [--check]
输出: 板子/generated/board1-algorithms.typ
      板子/generated/board2-number-theory.typ
      板子/generated/board3-misc.typ
      板子/generated/manifest.json
"""

import glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# ROOT = Mystudy 根目录
GEN_DIR = os.path.join(ROOT, "板子", "generated")
CONFIG_PATH = os.path.join(ROOT, "板子", "board_config.json")
os.makedirs(GEN_DIR, exist_ok=True)


def load_config() -> dict:
    with open(CONFIG_PATH, 'r', encoding='utf-8') as f:
        return json.load(f)


CONFIG = load_config()

# ── Boilerplate 识别 ──
# 匹配 cpp模板.cpp 中的公共头部行
BOILERPLATE_PATTERNS = [
    re.compile(r'^\s*#include\s*<'),
    re.compile(r'^\s*using\s+namespace\s+std'),
    re.compile(r'^\s*using\s+ll\s*=\s*long\s+long'),
    re.compile(r'^\s*//#define\s+int\s+long\s+long'),
    re.compile(r'^\s*//freopen'),
    re.compile(r'^\s*//ios::sync_with_stdio'),
    re.compile(r'^\s*ios::sync_with_stdio'),
    re.compile(r'^\s*int\s+T_start\s*=\s*clock'),
    re.compile(r'^\s*auto\s+T_start\s*=\s*chrono'),
]

def is_boilerplate_line(line: str) -> bool:
    return any(p.match(line) for p in BOILERPLATE_PATTERNS)

def is_empty_main(lines: list[str]) -> bool:
    """判断 main 函数是否为空壳（只含 boilerplate/return 0）"""
    body = []
    for l in lines:
        s = l.strip()
        if not s or s == '{' or s == '}':
            continue
        if s.startswith('signed main') or s.startswith('int main'):
            continue
        if s == 'return 0;':
            continue
        if is_boilerplate_line(l):
            continue
        body.append(s)
    return len(body) == 0

def extract_cpp_content(filepath: str) -> dict:
    """从 .cpp 提取算法主体、尾部注释、可选 usage 示例"""
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        raw_lines = f.readlines()

    # 1. 跳过前导 boilerplate
    start = 0
    for i, line in enumerate(raw_lines):
        if is_boilerplate_line(line) or line.strip() == '':
            start = i + 1
        else:
            break

    # 2. 找 main 函数位置
    main_start = None
    for i in range(start, len(raw_lines)):
        s = raw_lines[i].strip()
        if s.startswith('signed main') or s.startswith('int main'):
            main_start = i
            break

    # 3. 找 main 结束（匹配大括号）
    main_end = None
    if main_start is not None:
        depth = 0
        for i in range(main_start, len(raw_lines)):
            for ch in raw_lines[i]:
                if ch == '{':
                    depth += 1
                elif ch == '}':
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
        if s.startswith('//') or s == '':
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
        main_body = raw_lines[main_start:main_end + 1]
        if not is_empty_main(main_body):
            # 提取 main 中非 boilerplate 行
            in_body = False
            for l in main_body:
                s = l.strip()
                if s.startswith('signed main') or s.startswith('int main'):
                    in_body = True
                    continue
                if not in_body:
                    continue
                if is_boilerplate_line(l):
                    continue
                if s in ('{', '}', 'return 0;', ''):
                    continue
                usage_lines.append(l.rstrip())

    # 清理算法主体尾部空行
    while algo_lines and algo_lines[-1].strip() == '':
        algo_lines.pop()

    return {
        'algo': ''.join(algo_lines).rstrip(),
        'tail_comments': '\n'.join(c for c in tail_comments if c.strip()),
        'usage': '\n'.join(usage_lines) if usage_lines else '',
    }


def escape_typst(s: str) -> str:
    """完全转义 Typst 特殊字符（用于错误消息等纯文本）"""
    s = s.replace('\\', '\\\\')
    s = s.replace('#', '\\#')
    s = s.replace('$', '\\$')
    s = s.replace('@', '\\@')
    s = s.replace('<', '\\<')
    s = s.replace('>', '\\>')
    s = s.replace('_', '\\_')
    s = s.replace('*', '\\*')
    return s


def escape_raw_content(s: str) -> str:
    """确保 raw block 内容不含 ``` 序列"""
    return s.replace('```', '` ` `')


def rel_path(path: str) -> str:
    return path.replace('\\', '/')


def config_md_files(board: str | None = None) -> list[tuple[str, str, str]]:
    """Return (board, group, repo-relative md path) entries from config."""
    entries = []
    boards = ['board2', 'board3'] if board is None else [board]
    for board_name in boards:
        for group in CONFIG[board_name]['groups']:
            for fname in group['files']:
                entries.append((board_name, group['name'], rel_path(fname)))
    return entries


def all_config_md_paths() -> set[str]:
    return {path for _, _, path in config_md_files()}


def validate_config() -> list[str]:
    errors = []

    template = CONFIG.get('board1', {}).get('template')
    if not template or not os.path.isfile(os.path.join(ROOT, template)):
        errors.append(f'board1.template 不存在: {template}')

    folders = CONFIG.get('board1', {}).get('folders', [])
    if not isinstance(folders, list) or not folders:
        errors.append('board1.folders 必须是非空列表')
    for folder in folders:
        if not os.path.isdir(os.path.join(ROOT, folder)):
            errors.append(f'board1.folders 目录不存在: {folder}')

    seen = set()
    for board_name in ('board2', 'board3'):
        groups = CONFIG.get(board_name, {}).get('groups', [])
        if not isinstance(groups, list):
            errors.append(f'{board_name}.groups 必须是列表')
            continue
        for group in groups:
            name = group.get('name')
            files = group.get('files')
            if not name:
                errors.append(f'{board_name}.groups 存在缺失 name 的分组')
            if not isinstance(files, list):
                errors.append(f'{board_name}.{name}.files 必须是列表')
                continue
            for fname in files:
                path = rel_path(fname)
                key = (board_name, path)
                if key in seen:
                    errors.append(f'{board_name} 重复收录: {path}')
                seen.add(key)
                if not path.endswith('.md'):
                    errors.append(f'{board_name} 非 .md 文件: {path}')
                if not os.path.isfile(os.path.join(ROOT, path)):
                    errors.append(f'{board_name} 文件不存在: {path}')

    for img in CONFIG.get('board2', {}).get('reference_images', []):
        if not os.path.isfile(os.path.join(ROOT, img)):
            errors.append(f'board2.reference_images 文件不存在: {img}')

    source_dirs = CONFIG.get('board1', {}).get('folders', [])
    actual_md = {
        rel_path(os.path.relpath(path, ROOT))
        for folder in source_dirs
        for path in glob.glob(os.path.join(ROOT, folder, '**', '*.md'), recursive=True)
    }
    configured_md = all_config_md_paths()
    for path in sorted(actual_md - configured_md):
        errors.append(f'发现未收录 .md: {path}')
    for path in sorted(configured_md - actual_md):
        errors.append(f'配置中的 .md 不在源码目录扫描结果中: {path}')

    return errors


def check_config_or_exit() -> None:
    errors = validate_config()
    if errors:
        print('配置检查失败:')
        for err in errors:
            print(f'  - {err}')
        sys.exit(1)
    print('配置检查通过')


# ══════════════════════════════════════════════
# Board 1: 算法模板板
# ══════════════════════════════════════════════
def generate_board1() -> tuple[str, list]:
    """生成算法模板板 Typst 正文"""
    manifest = []
    parts = []

    # cpp模板.cpp 放在最前面
    cpp_template = os.path.join(ROOT, CONFIG["board1"]["template"])
    if os.path.exists(cpp_template):
        with open(cpp_template, 'r', encoding='utf-8', errors='ignore') as f:
            code = f.read().strip()
        parts.append('= 通用模板\n')
        parts.append('== cpp模板\n')
        parts.append(f'```cpp\n{code}\n```\n')
        manifest.append({
            'board': 'board1', 'folder': '板子',
            'file': 'cpp模板.cpp', 'title': 'cpp模板',
            'has_usage': False, 'has_tail': False,
        })

    for folder in CONFIG["board1"]["folders"]:
        folder_path = os.path.join(ROOT, folder)
        if not os.path.isdir(folder_path):
            continue
        cpp_files = sorted(
            glob.glob(os.path.join(folder_path, '**', '*.cpp'), recursive=True),
            key=lambda x: x.lower()
        )
        if not cpp_files:
            continue

        parts.append(f'= {folder}\n')

        for fpath in cpp_files:
            fname = os.path.basename(fpath)
            title = os.path.splitext(fname)[0]
            result = extract_cpp_content(fpath)

            entry = {
                'board': 'board1',
                'folder': folder,
                'file': fname,
                'title': title,
                'has_usage': bool(result['usage']),
                'has_tail': bool(result['tail_comments']),
            }
            manifest.append(entry)

            parts.append(f'== {title}\n')

            # 尾部注释作为说明放在代码前
            if result['tail_comments']:
                for cline in result['tail_comments'].split('\n'):
                    c = cline.strip().lstrip('/').strip()
                    if c:
                        parts.append(f'// {c}\n')
                parts.append('')

            # 算法主体代码
            if result['algo'].strip():
                algo_escaped = escape_raw_content(result['algo'])
                parts.append('```cpp')
                parts.append(algo_escaped)
                parts.append('```\n')

            # 用法示例
            if result['usage']:
                parts.append('#text(size: 8pt, fill: gray)[用法示例:]\n')
                usage_escaped = escape_raw_content(result['usage'])
                parts.append('```cpp')
                parts.append(usage_escaped)
                parts.append('```\n')

    return '\n'.join(parts), manifest


# ══════════════════════════════════════════════
# Board 2: 数论板
# ══════════════════════════════════════════════
def pandoc_md_to_typst(md_text: str, source_path: str, heading_offset: int = 2, img_base: str = '') -> str:
    """用 Pandoc 将 Markdown 转为 Typst，并调整标题层级"""
    md_text = re.sub(
        r'(?m)^---\s*$',
        '```{=typst}\n#horizontalrule\n```',
        md_text,
    )
    result = subprocess.run(
        ['pandoc', '-f', 'markdown-yaml_metadata_block', '-t', 'typst', '--wrap=none'],
        input=md_text, capture_output=True, text=True, encoding='utf-8'
    )
    if result.returncode != 0:
        raise RuntimeError(f'Pandoc 转换失败: {source_path}\n{result.stderr.strip()}')

    typst_text = result.stdout
    # 调整标题层级：Pandoc 输出的 = 是一级，我们需要降级
    def adjust_heading(m):
        level = len(m.group(1))
        new_level = level + heading_offset
        return '=' * new_level + ' ' + m.group(2)
    typst_text = re.sub(r'^(=+)\s+(.*)', adjust_heading, typst_text, flags=re.MULTILINE)
    # Pandoc 兼容：替换 #horizontalrule
    typst_text = typst_text.replace('#horizontalrule', '#line(length: 100%, stroke: 0.5pt + luma(180))')
    # 修复 Pandoc 生成的尾随逗号（如 table/tuple 参数中的 `,)`），否则 Typst 会报 unexpected comma
    typst_text = re.sub(r',\s*\)', ')', typst_text)

    # ── 修复 Pandoc 输出中的 Typst 数学问题 ──

    # 1. \pmod 修复: Pandoc 将 \pmod{m} 输出为 \( mod med m \)
    #    Typst 中 \( 会被当作未闭合的分隔符，需要替换为普通括号
    #    Pattern: \( mod med ... \) → (mod ...)
    #    Also handles: \( med mod med ... \) from \bmod
    typst_text = re.sub(
        r'\\\\?\(\s*(?:med\s+)?mod\s+med\s+(.+?)\s*\\\\?\)',
        r'(mod \1)',
        typst_text
    )

    # 2. 修复 Pandoc 未转换的 $$...$$ 块（显示为 \$\$...\$\$）
    #    这些通常是 Pandoc 无法解析的 LaTeX（如含 \therefore 等）
    def fix_raw_math_block(m):
        raw_latex = m.group(1).strip()
        # 去掉转义的反斜杠
        raw_latex = raw_latex.replace('\\\\', '\\')
        raw_latex = raw_latex.replace('\\$', '$')
        raw_latex = raw_latex.replace('\\_', '_')
        # 尝试用 Pandoc 单独转换这段数学
        inner_result = subprocess.run(
            ['pandoc', '-f', 'markdown-yaml_metadata_block', '-t', 'typst', '--wrap=none'],
            input=f'$$\n{raw_latex}\n$$',
            capture_output=True, text=True, encoding='utf-8'
        )
        if inner_result.returncode != 0:
            return f'```\n{raw_latex}\n```'
        inner = inner_result.stdout.strip()
        if inner and not inner.startswith('\\$'):
            return inner
        # 回退：原样保留为代码块
        return f'```\n{raw_latex}\n```'
    typst_text = re.sub(
        r'\\$\\$\n?(.*?)\n?\\$\\$',
        fix_raw_math_block,
        typst_text,
        flags=re.DOTALL
    )

    # 3. 修复 Pandoc 输出中 \( 和 \) 在数学上下文中的其他残留
    #    在 $ ... $ 内部，\( 和 \) 应该是普通括号
    def fix_math_parens(m):
        content = m.group(0)
        # 替换数学内部的 \( 和 \) 为普通括号
        content = content.replace('\\(', '(')
        content = content.replace('\\)', ')')
        return content
    # 匹配 $...$ 块（包括多行 display math）
    typst_text = re.sub(r'\$[^$]+\$', fix_math_parens, typst_text)

    # 修复图片路径：将相对路径转为绝对路径
    if img_base:
        def fix_img_path(m):
            prefix = m.group(1)  # 'image(' or '#box(image('
            rel_path = m.group(2)
            abs_path = os.path.normpath(os.path.join(img_base, rel_path))
            if os.path.exists(abs_path):
                # 计算相对于 generated/ 目录的路径
                gen_rel = os.path.relpath(abs_path, GEN_DIR).replace('\\', '/')
                return f'{prefix}"{gen_rel}")'
            else:
                return f'[#text(fill: red)[图片缺失: {escape_typst(rel_path)}]]'
        typst_text = re.sub(r'(#?(?:box\()?image\()"([^"]+)"\)', fix_img_path, typst_text)
    return typst_text


def generate_board2() -> tuple[str, list]:
    """生成数论板 Typst 正文（使用 Pandoc 转换）"""
    manifest = []
    parts = []

    # 参考图表放在最前面
    ref_images = CONFIG["board2"].get("reference_images", [])
    has_refs = [os.path.exists(os.path.join(ROOT, img)) for img in ref_images]
    if any(has_refs):
        parts.append('= 参考图表\n')
        for img, exists in zip(ref_images, has_refs):
            if exists:
                image_rel_path = os.path.relpath(
                    os.path.join(ROOT, img), GEN_DIR
                ).replace('\\', '/')
                title = os.path.splitext(img)[0]
                parts.append(f'== {title}\n')
                parts.append(f'#image("{image_rel_path}", width: 100%)\n')

    for group in CONFIG["board2"]["groups"]:
        group_name = group["name"]
        files = group["files"]
        parts.append(f'= {group_name}\n')

        for fname in files:
            fname = rel_path(fname)
            fpath = os.path.join(ROOT, fname)
            title = os.path.splitext(fname)[0]

            entry = {
                'board': 'board2',
                'group': group_name,
                'file': fname,
                'title': title,
                'warnings': [],
            }

            if not os.path.exists(fpath):
                entry['warnings'].append('文件不存在')
                manifest.append(entry)
                parts.append(f'== {title}\n')
                parts.append(f'#text(fill: red)[文件缺失: {fname}]\n')
                continue

            with open(fpath, 'r', encoding='utf-8', errors='ignore') as f:
                md_text = f.read()

            manifest.append(entry)
            parts.append(f'== {title}\n')
            img_base = os.path.dirname(fpath)
            converted = pandoc_md_to_typst(md_text, fname, heading_offset=2, img_base=img_base)
            parts.append(converted)
            parts.append('')

    return '\n'.join(parts), manifest


# ══════════════════════════════════════════════
# Board 3: 杂项板
# ══════════════════════════════════════════════
def generate_board3() -> tuple[str, list]:
    """生成杂项板 Typst 正文（使用 Pandoc 转换）"""
    manifest = []
    parts = []

    for group in CONFIG["board3"]["groups"]:
        group_name = group["name"]
        file_list = group["files"]
        parts.append(f'= {group_name}\n')

        for fname in file_list:
            fname = rel_path(fname)
            fpath = os.path.join(ROOT, fname)
            title = os.path.splitext(fname)[0]

            entry = {
                'board': 'board3',
                'group': group_name,
                'file': fname,
                'title': title,
                'warnings': [],
            }

            if not os.path.exists(fpath):
                entry['warnings'].append('文件不存在')
                manifest.append(entry)
                parts.append(f'== {title}\n')
                parts.append(f'#text(fill: red)[文件缺失: {fname}]\n')
                continue

            with open(fpath, 'r', encoding='utf-8', errors='ignore') as f:
                md_text = f.read()

            manifest.append(entry)
            parts.append(f'== {title}\n')
            img_base = os.path.dirname(fpath)
            converted = pandoc_md_to_typst(md_text, fname, heading_offset=2, img_base=img_base)
            parts.append(converted)
            parts.append('')

    return '\n'.join(parts), manifest


# ══════════════════════════════════════════════
# Main
# ══════════════════════════════════════════════
def main():
    if '--check' in sys.argv:
        check_config_or_exit()
        return

    check_config_or_exit()
    all_manifest = []

    # Board 1
    content1, m1 = generate_board1()
    with open(os.path.join(GEN_DIR, 'board1-algorithms.typ'), 'w', encoding='utf-8', newline='\n') as f:
        f.write(content1.replace('\r', ''))
    all_manifest.extend(m1)
    print(f"Board 1: {len(m1)} 个算法模板")

    # Board 2 (Pandoc)
    content2, m2 = generate_board2()
    with open(os.path.join(GEN_DIR, 'board2-number-theory.typ'), 'w', encoding='utf-8', newline='\n') as f:
        f.write(content2.replace('\r', ''))
    all_manifest.extend(m2)
    print(f"Board 2: {len(m2)} 个数论笔记")

    # Board 3 (Pandoc)
    content3, m3 = generate_board3()
    with open(os.path.join(GEN_DIR, 'board3-misc.typ'), 'w', encoding='utf-8', newline='\n') as f:
        f.write(content3.replace('\r', ''))
    all_manifest.extend(m3)
    print(f"Board 3: {len(m3)} 个杂项笔记")

    # Manifest
    with open(os.path.join(GEN_DIR, 'manifest.json'), 'w', encoding='utf-8') as f:
        json.dump(all_manifest, f, ensure_ascii=False, indent=2)
    print(f"Manifest: {len(all_manifest)} 条记录")

    try:
        import typst
    except ImportError as exc:
        raise RuntimeError("typst 未安装，无法编译 PDF") from exc

    typst_dir = os.path.join(ROOT, "板子", "typst")
    out_dir = os.path.join(ROOT, "板子", "output")
    os.makedirs(out_dir, exist_ok=True)
    boards = ['board1-algorithms', 'board2-number-theory', 'board3-misc']
    for name in boards:
        src = os.path.join(typst_dir, f'{name}.typ')
        dst = os.path.join(out_dir, f'{name}.pdf')
        pdf = typst.compile(src, root=ROOT)
        with open(dst, 'wb') as f:
            f.write(pdf)
        print(f"  -> {name}.pdf ({len(pdf)//1024}KB)")
    print("PDF 编译完成")


if __name__ == '__main__':
    try:
        main()
    except Exception as exc:
        print(f"生成失败: {exc}", file=sys.stderr)
        sys.exit(1)
