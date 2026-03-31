#!/usr/bin/env python3
"""
generate_typst.py — 从原始 .cpp / .md 生成三份 Typst 板子正文
用法: python generate_typst.py
输出: 板子/generated/board1-algorithms.typ
      板子/generated/board2-number-theory.typ
      板子/generated/board3-misc.typ
      板子/generated/manifest.json
"""

import os, re, json, glob, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# ROOT = Mystudy 根目录
GEN_DIR = os.path.join(ROOT, "板子", "generated")
os.makedirs(GEN_DIR, exist_ok=True)

# Try to import typst for validation
try:
    import typst as _typst
    _TYPST_TMP = os.path.join(GEN_DIR, '_validate.typ')
    def validate_typst_math(expr: str) -> bool:
        """Test if a Typst math expression compiles"""
        content = f'#set page(paper: "a4")\n${expr}$\n'
        with open(_TYPST_TMP, 'w', encoding='utf-8', newline='\n') as f:
            f.write(content)
        try:
            _typst.compile(_TYPST_TMP, root=os.path.dirname(_TYPST_TMP))
            return True
        except:
            return False
except ImportError:
    def validate_typst_math(expr: str) -> bool:
        return True  # skip validation if typst not available

# ── Board 1 目录顺序 ──
BOARD1_FOLDERS = [
    "数据结构", "图论", "字符串", "哈希",
    "动态规划", "数论", "计算几何", "博弈论", "其他",
]

# ── Board 2 数论笔记主题分组 ──
BOARD2_GROUPS = {
    "基础与工具": [
        "数论笔记部分/数论笔记(筛法).md",
        "数论笔记部分/数论笔记(线性逆元).md",
        "数论笔记部分/数论笔记(不定方程与同余方程组).md",
        "数论笔记部分/数论小结论.md",
    ],
    "卷积与反演": [
        "数论笔记部分/数论笔记(狄利克雷卷积与莫比乌斯反演 1).md",
        "数论笔记部分/数论笔记(狄利克雷卷积与莫比乌斯反演 2).md",
        "数论笔记部分/数论笔记(炫酷反演魔术).md",
        "数论笔记部分/数论笔记(和式变换).md",
    ],
    "组合与生成函数": [
        "数论笔记部分/数论笔记(排列组合).md",
        "数论笔记部分/数论笔记(排列组合进阶).md",
        "数论笔记部分/数论笔记(生成函数).md",
    ],
    "变换与多项式": [
        "数论笔记部分/FFT笔记.md",
        "数论笔记部分/数论笔记(sosdp&fmt&fwt).md",
        "数论笔记部分/数论笔记(阶,原根与ntt).md",
    ],
    "Trick 与杂项": [
        "Trick/数学相关trick.md",
        "数论笔记部分/数论笔记(杂项).md",
    ],
}

# ── Board 3 杂项笔记主题分组 ──
BOARD3_GROUPS = {
    "动态规划": [
        ("动态规划", "对dp的一些思考.md"),
        ("动态规划", "dp笔记/数位dp笔记.md"),
        ("动态规划", "dp笔记/普通dp常见状态.md"),
        ("动态规划", "dp笔记/状压dp常见状态.md"),
    ],
    "图论": [
        ("图论", "Trick/图相关trick.md"),
        ("图论", "图论笔记/图论笔记(几类特殊图).md"),
    ],
    "博弈论": [
        ("博弈论", "nim游戏 SG函数.md"),
    ],
    "通用 Trick 与杂项": [
        ("其他", "Trick/杂项相关trick.md"),
    ],
}

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


def escape_typst_text(s: str) -> str:
    """转义普通文本行中的 Typst 特殊字符，但保留已转换的 Typst 语法"""
    result = []
    i = 0
    in_dollar = False
    while i < len(s):
        ch = s[i]
        if ch == '$':
            in_dollar = not in_dollar
            result.append(ch)
        elif in_dollar:
            result.append(ch)
        elif ch == '#' and i + 1 < len(s) and s[i+1].isalpha():
            # Typst function call like #text(...), keep as-is
            result.append(ch)
        elif ch == '<':
            result.append('\\<')
        elif ch == '>':
            result.append('\\>')
        elif ch == '@':
            result.append('\\@')
        else:
            result.append(ch)
        i += 1
    return ''.join(result)


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


# ── Markdown -> Typst 转换 ──
def md_to_typst(md_text: str, heading_offset: int = 2, img_base: str = "") -> str:
    """将 Markdown 文本转换为 Typst 子集"""
    lines = md_text.split('\n')
    out = []
    in_code = False
    code_buf = []
    code_lang = ""
    in_math_block = False
    math_buf = []

    i = 0
    while i < len(lines):
        line = lines[i]

        # fenced code block
        if line.strip().startswith('```') and not in_code:
            lang_match = re.match(r'```(\w*)', line.strip())
            code_lang = lang_match.group(1) if lang_match else ""
            in_code = True
            code_buf = []
            i += 1
            continue
        if line.strip().startswith('```') and in_code:
            in_code = False
            content = escape_raw_content('\n'.join(code_buf))
            if code_lang:
                out.append(f'```{code_lang}')
            else:
                out.append('```')
            out.append(content)
            out.append('```')
            out.append('')
            i += 1
            continue
        if in_code:
            code_buf.append(line)
            i += 1
            continue

        # $$ block math
        # Single-line $$...$$
        single_math = re.match(r'^\s*\$\$(.+?)\$\$\s*$', line)
        if single_math and not in_math_block:
            raw_latex = single_math.group(1).strip()
            math_content = convert_latex_to_typst_math(raw_latex)
            math_lines = [l for l in math_content.split('\n') if l.strip()]
            math_clean = '\n'.join(math_lines)
            out.append('')
            if validate_typst_math(math_clean):
                out.append(f'${math_clean}$')
            else:
                out.append(f'#raw("$$ {raw_latex} $$", block: true)')
            out.append('')
            i += 1
            continue
        # Multi-line $$ ... $$
        if line.strip() == '$$' and not in_math_block:
            in_math_block = True
            math_buf = []
            i += 1
            continue
        if line.strip() == '$$' and in_math_block:
            in_math_block = False
            raw_latex = '\n'.join(math_buf).strip()
            math_content = convert_latex_to_typst_math(raw_latex)
            math_lines = [l for l in math_content.split('\n') if l.strip()]
            math_clean = '\n'.join(math_lines)
            out.append('')
            if validate_typst_math(math_clean):
                out.append(f'${math_clean}$')
            else:
                out.append(f'```\n{raw_latex}\n```')
            out.append('')
            i += 1
            continue
        if in_math_block:
            math_buf.append(line)
            i += 1
            continue

        # \[ ... \] block math (single or multi-line)
        if line.strip().startswith('\\['):
            raw_start = line.strip()[2:]
            if '\\]' in raw_start:
                raw_latex = raw_start[:raw_start.index('\\]')]
                math_content = convert_latex_to_typst_math(raw_latex).strip()
                math_lines = [l for l in math_content.split('\n') if l.strip()]
                math_clean = '\n'.join(math_lines)
                out.append('')
                if validate_typst_math(math_clean):
                    out.append(f'${math_clean}$')
                else:
                    out.append(f'```\n{raw_latex}\n```')
                out.append('')
            else:
                mbuf = [raw_start]
                i += 1
                while i < len(lines):
                    if '\\]' in lines[i]:
                        mbuf.append(lines[i][:lines[i].index('\\]')])
                        break
                    mbuf.append(lines[i])
                    i += 1
                raw_latex = '\n'.join(mbuf).strip()
                math_content = convert_latex_to_typst_math(raw_latex)
                math_lines = [l for l in math_content.split('\n') if l.strip()]
                math_clean = '\n'.join(math_lines)
                out.append('')
                if validate_typst_math(math_clean):
                    out.append(f'${math_clean}$')
                else:
                    out.append(f'```\n{raw_latex}\n```')
                out.append('')
            i += 1
            continue

        # headings
        hm = re.match(r'^(#{1,6})\s+(.*)', line)
        if hm:
            level = len(hm.group(1)) + heading_offset
            title = hm.group(2).strip()
            title = convert_inline_math(title)
            title = convert_inline_formatting(title)
            title = escape_typst_text(title)
            eq_signs = '=' * level
            out.append(f'{eq_signs} {title}')
            out.append('')
            i += 1
            continue

        # horizontal rule
        if re.match(r'^---+\s*$', line):
            out.append('#line(length: 100%, stroke: 0.5pt + luma(180))')
            out.append('')
            i += 1
            continue

        # image
        img_match = re.match(r'!\[([^\]]*)\]\(([^)]+)\)', line.strip())
        if img_match:
            alt = img_match.group(1)
            src = img_match.group(2)
            # 解析相对路径
            if img_base and not src.startswith(('http://', 'https://', '/')):
                full_path = os.path.normpath(os.path.join(img_base, src))
                if os.path.exists(full_path):
                    src_escaped = full_path.replace('\\', '/')
                    out.append(f'#image("{src_escaped}", width: 90%)')
                else:
                    out.append(f'#text(fill: red)[图片缺失: {escape_typst(src)}]')
            else:
                out.append(f'// 图片引用: {src}')
                out.append(f'#text(fill: red)[图片: {escape_typst(src)}]')
            out.append('')
            i += 1
            continue

        # unordered list
        lm = re.match(r'^(\s*)[-*]\s+(.*)', line)
        if lm:
            indent_level = len(lm.group(1)) // 2
            content = convert_inline_math(lm.group(2))
            content = convert_inline_formatting(content)
            content = escape_typst_text(content)
            prefix = '  ' * indent_level
            out.append(f'{prefix}- {content}')
            i += 1
            continue

        # ordered list
        olm = re.match(r'^(\s*)\d+\.\s+(.*)', line)
        if olm:
            indent_level = len(olm.group(1)) // 2
            content = convert_inline_math(olm.group(2))
            content = convert_inline_formatting(content)
            content = escape_typst_text(content)
            prefix = '  ' * indent_level
            out.append(f'{prefix}+ {content}')
            i += 1
            continue

        # blockquote
        bq = re.match(r'^>\s*(.*)', line)
        if bq:
            content = convert_inline_math(bq.group(1))
            content = convert_inline_formatting(content)
            content = escape_typst_text(content)
            out.append(f'#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[{content}]')
            i += 1
            continue

        # normal paragraph line
        converted = convert_inline_math(line)
        converted = convert_inline_formatting(converted)
        converted = escape_typst_text(converted)
        out.append(converted)
        i += 1

    return '\n'.join(out)


def convert_latex_to_typst_math(latex: str) -> str:
    """将 LaTeX 数学转换为 Typst 数学（尽力而为）"""
    s = latex.strip()

    # environments
    s = re.sub(r'\\begin\{cases\}', '', s)
    s = re.sub(r'\\end\{cases\}', '', s)
    s = re.sub(r'\\begin\{aligned\}', '', s)
    s = re.sub(r'\\end\{aligned\}', '', s)
    s = re.sub(r'\\begin\{pmatrix\}', 'mat(', s)
    s = re.sub(r'\\end\{pmatrix\}', ')', s)

    # \frac and \dfrac (handle nested braces simply)
    for _ in range(5):  # multiple passes for nested fracs
        s = re.sub(r'\\d?frac\{([^{}]+)\}\{([^{}]+)\}', r'(\1)/(\2)', s)

    # floor/ceil
    s = s.replace('\\lfloor', 'floor(')
    s = s.replace('\\rfloor', ')')
    s = s.replace('\\lceil', 'ceil(')
    s = s.replace('\\rceil', ')')

    # big operators
    s = re.sub(r'\\sum_\{([^{}]+)\}\^\{([^{}]+)\}', r'sum_(\1)^(\2)', s)
    s = re.sub(r'\\sum_\{([^{}]+)\}', r'sum_(\1)', s)
    s = re.sub(r'\\sum', 'sum', s)
    s = re.sub(r'\\prod_\{([^{}]+)\}\^\{([^{}]+)\}', r'product_(\1)^(\2)', s)
    s = re.sub(r'\\prod_\{([^{}]+)\}', r'product_(\1)', s)
    s = re.sub(r'\\prod', 'product', s)
    s = re.sub(r'\\bigcup', 'union.big', s)
    s = re.sub(r'\\bigcap', 'sect.big', s)

    # \text{...} \mathrm{...} \textbf{...}
    s = re.sub(r'\\text\{([^{}]+)\}', lambda m: f'upright("{m.group(1)}")', s)
    s = re.sub(r'\\mathrm\{([^{}]+)\}', lambda m: f'upright("{m.group(1)}")', s)
    s = re.sub(r'\\textbf\{([^{}]+)\}', lambda m: f'bold("{m.group(1)}")', s)
    s = re.sub(r'\\mathbf\{([^{}]+)\}', lambda m: f'bold("{m.group(1)}")', s)

    # \pmod{p} -> (mod p)
    s = re.sub(r'\\pmod\{([^{}]+)\}', r'(mod \1)', s)

    # Greek letters — explicit mapping for ones that differ
    GREEK_MAP = {
        '\\varphi': 'phi.alt',
        '\\varepsilon': 'epsilon.alt',
        '\\phi': 'phi',
        '\\epsilon': 'epsilon',
        '\\alpha': 'alpha',
        '\\beta': 'beta',
        '\\gamma': 'gamma',
        '\\delta': 'delta',
        '\\Delta': 'Delta',
        '\\lambda': 'lambda',
        '\\Lambda': 'Lambda',
        '\\mu': 'mu',
        '\\nu': 'nu',
        '\\pi': 'pi',
        '\\Pi': 'Pi',
        '\\sigma': 'sigma',
        '\\Sigma': 'Sigma',
        '\\tau': 'tau',
        '\\theta': 'theta',
        '\\omega': 'omega',
        '\\Omega': 'Omega',
        '\\rho': 'rho',
        '\\xi': 'xi',
        '\\zeta': 'zeta',
        '\\eta': 'eta',
        '\\kappa': 'kappa',
        '\\chi': 'chi',
        '\\psi': 'psi',
        '\\Psi': 'Psi',
    }
    for latex_cmd, typst_sym in GREEK_MAP.items():
        s = s.replace(latex_cmd, typst_sym)

    # Symbols
    s = s.replace('\\infty', 'infinity')
    s = s.replace('\\cdots', 'dots.c')
    s = s.replace('\\cdot', 'dot.c')
    s = s.replace('\\ldots', 'dots')
    s = s.replace('\\dots', 'dots')
    s = s.replace('\\times', 'times')
    s = s.replace('\\div', 'div')
    s = s.replace('\\pm', 'plus.minus')
    s = s.replace('\\mp', 'minus.plus')

    # Relations
    s = s.replace('\\equiv', 'equiv')
    s = s.replace('\\neq', 'eq.not')
    s = s.replace('\\leq', 'lt.eq')
    s = s.replace('\\geq', 'gt.eq')
    s = s.replace('\\ll', 'lt.double')
    s = s.replace('\\gg', 'gt.double')
    s = s.replace('\\approx', 'approx')
    s = s.replace('\\sim', 'tilde')
    s = s.replace('\\propto', 'prop')

    # Set/logic
    s = s.replace('\\cup', 'union')
    s = s.replace('\\cap', 'sect')
    s = s.replace('\\subset', 'subset')
    s = s.replace('\\subseteq', 'subset.eq')
    s = s.replace('\\supset', 'supset')
    s = s.replace('\\supseteq', 'supset.eq')
    s = s.replace('\\in', 'in')
    s = s.replace('\\notin', 'in.not')
    s = s.replace('\\emptyset', 'nothing')
    s = s.replace('\\forall', 'forall')
    s = s.replace('\\exists', 'exists')
    s = s.replace('\\neg', 'not')

    # Logic/bitwise
    s = s.replace('\\wedge', 'and.big')
    s = s.replace('\\vee', 'or.big')
    s = s.replace('\\oplus', 'xor')
    s = s.replace('\\land', 'and')
    s = s.replace('\\lor', 'or')

    # Arrows
    s = s.replace('\\Rightarrow', 'arrow.r.double')
    s = s.replace('\\Leftarrow', 'arrow.l.double')
    s = s.replace('\\Leftrightarrow', 'arrow.l.r.double')
    s = s.replace('\\rightarrow', 'arrow.r')
    s = s.replace('\\leftarrow', 'arrow.l')
    s = s.replace('\\leftrightarrow', 'arrow.l.r')
    s = s.replace('\\implies', 'arrow.r.double')
    s = s.replace('\\iff', 'arrow.l.r.double')
    s = s.replace('\\to', 'arrow.r')
    s = s.replace('\\mapsto', 'arrow.r.bar')

    # Misc
    s = s.replace('\\quad', 'quad')
    s = s.replace('\\qquad', 'quad quad')
    s = s.replace('\\star', 'star')
    s = s.replace('\\circ', 'circle.small')
    s = s.replace('\\langle', 'angle.l')
    s = s.replace('\\rangle', 'angle.r')
    s = s.replace('\\mid', 'mid')
    s = s.replace('\\nmid', 'divides.not')
    s = s.replace('\\binom', 'binom')
    s = s.replace('\\sqrt', 'sqrt')
    s = s.replace('\\overline', 'overline')

    # \left \right (remove, Typst auto-sizes)
    s = s.replace('\\left', '')
    s = s.replace('\\right', '')

    # ^{...} -> ^(...)
    s = re.sub(r'\^\{([^{}]+)\}', r'^(\1)', s)
    # _{...} -> _(...)
    s = re.sub(r'_\{([^{}]+)\}', r'_(\1)', s)

    # \\ -> line break in math
    s = s.replace('\\\\', '\\\n')
    # \ (space) -> space
    s = re.sub(r'\\\s', ' ', s)

    # remaining \command -> just strip backslash (best effort)
    s = re.sub(r'\\([a-zA-Z]+)', r'\1', s)

    # Typst math: multi-char lowercase sequences like "mn" need spaces -> "m n"
    # In LaTeX, "mn" means m*n, but in Typst "mn" is a single variable.
    # Insert spaces between adjacent single lowercase letters (not part of known keywords).
    TYPST_KEYWORDS = {
        'sum', 'product', 'infinity', 'alpha', 'beta', 'gamma', 'delta', 'epsilon',
        'zeta', 'eta', 'theta', 'iota', 'kappa', 'lambda', 'mu', 'nu', 'xi',
        'pi', 'rho', 'sigma', 'tau', 'upsilon', 'phi', 'chi', 'psi', 'omega',
        'Delta', 'Gamma', 'Lambda', 'Sigma', 'Pi', 'Omega', 'Psi', 'Phi',
        'equiv', 'quad', 'times', 'div', 'mod', 'gcd', 'lcm', 'max', 'min',
        'log', 'ln', 'exp', 'sin', 'cos', 'tan', 'lim', 'sup', 'inf',
        'det', 'dim', 'ker', 'deg', 'arg', 'floor', 'ceil', 'sqrt', 'not',
        'and', 'or', 'xor', 'in', 'dots', 'tilde', 'hat', 'overline', 'bold',
        'upright', 'forall', 'exists', 'nothing', 'union', 'sect', 'subset',
        'supset', 'approx', 'prop', 'star', 'mid', 'binom', 'cases', 'mat',
        'arrow', 'angle', 'lt', 'gt', 'eq', 'plus', 'minus', 'circle', 'divides',
        'implies', 'iff', 'therefore', 'because', 'where', 'with',
    }

    def split_multichar(m):
        word = m.group(0)
        # Don't split known Typst keywords or dotted names
        if word in TYPST_KEYWORDS or '.' in word:
            return word
        # Only split very short sequences (2-3 chars) that look like variable products
        # Longer words are likely function names or text
        if len(word) <= 3 and word.isalpha() and word.islower() and word not in TYPST_KEYWORDS:
            return ' '.join(word)
        return word

    # Match word-like sequences not preceded by # or .
    s = re.sub(r'(?<![.#\w])[a-z]{2,}(?![.\w])', split_multichar, s)

    return s


def convert_inline_math(line: str) -> str:
    r"""转换行内数学 \(...\) 和 $...$ 中的 LaTeX -> Typst math"""
    # \( ... \) -> $...$
    def repl_inline(m):
        raw = m.group(1)
        content = convert_latex_to_typst_math(raw)
        if validate_typst_math(content):
            return f'${content}$'
        return f'`\\({raw}\\)`'
    line = re.sub(r'\\\((.+?)\\\)', repl_inline, line)

    # $...$ inline math (LaTeX) -> $...$ (Typst math)
    def repl_dollar_math(m):
        raw = m.group(1)
        content = raw
        if '\\' in content or '_{' in content or '^{' in content:
            content = convert_latex_to_typst_math(content)
        if validate_typst_math(content):
            return f'${content}$'
        return f'`${raw}$`'
    # 匹配 $...$ 但不匹配 $$
    line = re.sub(r'(?<!\$)\$(?!\$)(.+?)(?<!\$)\$(?!\$)', repl_dollar_math, line)
    return line


def convert_inline_formatting(line: str) -> str:
    """转换 Markdown 行内格式"""
    # Handle embedded $$...$$ (inline display math) before bold conversion
    def repl_embedded_display(m):
        raw = m.group(1)
        content = convert_latex_to_typst_math(raw)
        if validate_typst_math(content):
            return f'${content}$'
        return f'`$${raw}$$`'
    line = re.sub(r'\$\$(.+?)\$\$', repl_embedded_display, line)
    # **bold** -> *bold* (Typst bold)
    line = re.sub(r'\*\*(.+?)\*\*', r'*\1*', line)
    return line


# ══════════════════════════════════════════════
# Board 1: 算法模板板
# ══════════════════════════════════════════════
def generate_board1() -> tuple[str, list]:
    """生成算法模板板 Typst 正文"""
    manifest = []
    parts = []

    # cpp模板.cpp 放在最前面
    cpp_template = os.path.join(ROOT, "板子", "cpp模板.cpp")
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

    for folder in BOARD1_FOLDERS:
        folder_path = os.path.join(ROOT, folder)
        if not os.path.isdir(folder_path):
            continue
        cpp_files = sorted(
            [f for f in os.listdir(folder_path) if f.endswith('.cpp')],
            key=lambda x: x.lower()
        )
        if not cpp_files:
            continue

        parts.append(f'= {folder}\n')

        for fname in cpp_files:
            fpath = os.path.join(folder_path, fname)
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
def pandoc_md_to_typst(md_text: str, heading_offset: int = 2, img_base: str = '') -> str:
    """用 Pandoc 将 Markdown 转为 Typst，并调整标题层级"""
    result = subprocess.run(
        ['pandoc', '-f', 'markdown', '-t', 'typst', '--wrap=none'],
        input=md_text, capture_output=True, text=True, encoding='utf-8'
    )
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
            ['pandoc', '-f', 'markdown', '-t', 'typst', '--wrap=none'],
            input=f'$$\n{raw_latex}\n$$',
            capture_output=True, text=True, encoding='utf-8'
        )
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
    nt_dir = os.path.join(ROOT, "数论")

    # 参考图表放在最前面
    ref_images = ['表.jpg', '图.png']
    has_refs = [os.path.exists(os.path.join(ROOT, img)) for img in ref_images]
    if any(has_refs):
        parts.append('= 参考图表\n')
        for img, exists in zip(ref_images, has_refs):
            if exists:
                rel_path = os.path.relpath(
                    os.path.join(ROOT, img), GEN_DIR
                ).replace('\\', '/')
                title = os.path.splitext(img)[0]
                parts.append(f'== {title}\n')
                parts.append(f'#image("{rel_path}", width: 100%)\n')

    for group_name, files in BOARD2_GROUPS.items():
        parts.append(f'= {group_name}\n')

        for fname in files:
            fpath = os.path.join(nt_dir, fname)
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
            converted = pandoc_md_to_typst(md_text, heading_offset=2, img_base=img_base)
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

    for group_name, file_list in BOARD3_GROUPS.items():
        parts.append(f'= {group_name}\n')

        for folder, fname in file_list:
            fpath = os.path.join(ROOT, folder, fname)
            title = os.path.splitext(fname)[0]

            entry = {
                'board': 'board3',
                'group': group_name,
                'file': f'{folder}/{fname}',
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
            converted = pandoc_md_to_typst(md_text, heading_offset=2, img_base=img_base)
            parts.append(converted)
            parts.append('')

    return '\n'.join(parts), manifest


# ══════════════════════════════════════════════
# Main
# ══════════════════════════════════════════════
def main():
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

    # 编译 PDF
    try:
        import typst
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
    except ImportError:
        print("typst 未安装，跳过 PDF 编译。请手动运行 typst compile")
    except Exception as e:
        print(f"PDF 编译失败: {e}")


if __name__ == '__main__':
    main()
