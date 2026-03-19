#!/usr/bin/env python3
"""
fix_md_math.py — 批量修复 .md 文件中的数学公式格式
使其对 Pandoc md→typst 转换友好

修复内容：
1. 单行 $$...$$ → 多行格式（前后各一行 $$）
2. \[...\] → $$...$$
3. 连续 $$$ → 拆分
4. 修复不规范的行内/行间混用
"""

import re, os, sys

CJK_RE = re.compile(r'[\u4e00-\u9fff\u3000-\u303f\uff00-\uffef]')


def fix_spaced_inline_math(line: str) -> str:
    r"""修复 $ content $ → $content$（Pandoc 要求 $ 紧贴内容）
    只处理 content 不含中文的情况（避免误匹配跨 inline math 的中文文本）"""
    def repl(m):
        inner = m.group(1)
        if CJK_RE.search(inner):
            return m.group(0)  # 含中文，不处理
        if '$' in inner:
            return m.group(0)  # 含嵌套 $，不处理
        return f'${inner.strip()}$'
    return re.sub(r'(?<!\$)\$(\s+\S.*?\s+)\$(?!\$)', repl, line)


def fix_md_math(text: str) -> str:
    lines = text.split('\n')
    out = []
    i = 0
    in_code = False

    while i < len(lines):
        line = lines[i]

        # 跳过代码块（但转换 ```math 为 $$）
        if line.strip().startswith('```'):
            if line.strip() == '```math':
                # ```math → $$
                out.append('$$')
                i += 1
                while i < len(lines):
                    if lines[i].strip() == '```':
                        out.append('$$')
                        i += 1
                        break
                    out.append(lines[i])
                    i += 1
                continue
            in_code = not in_code
            out.append(line)
            i += 1
            continue
        if in_code:
            out.append(line)
            i += 1
            continue

        # 1. \[...\] 单行 → $$...$$
        m = re.match(r'^(\s*)\\\[(.+)\\\]\s*$', line)
        if m:
            indent = m.group(1)
            content = m.group(2).strip()
            out.append(f'{indent}$$')
            out.append(f'{indent}{content}')
            out.append(f'{indent}$$')
            i += 1
            continue

        # 2. \[ 开始多行 → $$...$$
        if re.match(r'^\s*\\\[\s*$', line):
            out.append('$$')
            i += 1
            while i < len(lines):
                if re.match(r'^\s*\\\]\s*$', lines[i]):
                    out.append('$$')
                    i += 1
                    break
                out.append(lines[i])
                i += 1
            continue

        # 3. 单行 $$...$$ → 多行
        m = re.match(r'^(\s*)\$\$(.+?)\$\$\s*$', line)
        if m:
            indent = m.group(1)
            content = m.group(2).strip()
            out.append(f'{indent}$$')
            out.append(f'{indent}{content}')
            out.append(f'{indent}$$')
            i += 1
            continue

        # 4. 修复 $$$ (三个连续美元符号)
        # 情况: ...$0$$$\text{Sum}...$$ → ...$0$
        #                                 $$
        #                                 \text{Sum}...
        #                                 $$
        line = re.sub(r'\$\$\$', r'$$\n$$', line)

        # 5. 修复 \( \) → $ $
        line = re.sub(r'\\\((.+?)\\\)', r'$\1$', line)

        # 6. 修复 $ content $ → $content$
        line = fix_spaced_inline_math(line)

        # 7. 替换 Pandoc 不支持的 LaTeX 命令
        line = line.replace(r'\dbinom', r'\binom')
        line = line.replace(r'\bmod', r'\mod')

        # 8. 修复 (mod \ m) 格式 → \pmod{m}
        #    匹配 (mod \ X) 或 (mod\ X) 在数学环境中
        #    这样 Pandoc 不会把 \( 当成 inline math 开始
        line = re.sub(r'\(mod\s*\\\s*(\w+(?:\([^)]*\))?)\s*\)', r'\\pmod{\1}', line)

        # 9. 修复 \therefore 等 Pandoc 不支持的命令
        #    (保留原样，由 generate_typst.py 后处理)

        out.append(line)
        i += 1

    return '\n'.join(out)


def process_file(fpath: str, dry_run: bool = False) -> int:
    """处理单个文件，返回是否有变更"""
    with open(fpath, 'r', encoding='utf-8', errors='ignore') as f:
        original = f.read()

    fixed = fix_md_math(original)

    if fixed == original:
        return 0

    name = os.path.basename(fpath)
    if dry_run:
        print(f"  [DRY] {name}")
    else:
        with open(fpath, 'w', encoding='utf-8', newline='\n') as f:
            f.write(fixed)
        print(f"  [FIX] {name}")
    return 1


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    dry_run = '--dry-run' in sys.argv

    dirs = ['数论', '动态规划', 'Graph', '博弈论', 'Other']
    md_files = []
    for d in dirs:
        dpath = os.path.join(root, d)
        if os.path.isdir(dpath):
            for f in os.listdir(dpath):
                if f.endswith('.md'):
                    md_files.append(os.path.join(dpath, f))

    print(f"扫描到 {len(md_files)} 个 .md 文件")
    changed = 0
    for fpath in sorted(md_files):
        c = process_file(fpath, dry_run=dry_run)
        if c > 0:
            changed += 1

    if changed == 0:
        print("所有文件格式正常")
    else:
        print(f"共修改 {changed} 个文件")


if __name__ == '__main__':
    main()
