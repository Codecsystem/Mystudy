import subprocess

# Test which LaTeX commands Pandoc can handle
cmds_to_test = [
    (r'\dbinom{n}{k}', 'dbinom'),
    (r'\dfrac{a}{b}', 'dfrac'),
    (r'\displaystyle\sum', 'displaystyle'),
    (r'\choose', 'choose'),
    (r'\substack{a \\ b}', 'substack'),
    (r'\mathcal{F}', 'mathcal'),
    (r'\mathbb{Z}', 'mathbb'),
    (r'\boxed{x}', 'boxed'),
    (r'\bmod', 'bmod'),
    (r'\tag{1}', 'tag'),
    (r'\underbrace{x}_{n}', 'underbrace'),
    (r'\lvert x \rvert', 'lvert/rvert'),
    (r'\preceq', 'preceq'),
    (r'\cong', 'cong'),
]

for latex, name in cmds_to_test:
    md = f'${latex}$'
    r = subprocess.run(['pandoc', '-f', 'markdown', '-t', 'typst', '--wrap=none'],
                       input=md, capture_output=True, text=True, encoding='utf-8')
    has_warning = 'WARNING' in r.stderr or r'\\' in r.stdout or r'\$' in r.stdout
    status = 'FAIL' if has_warning else 'OK'
    print(f"  {name:15s}: {status:4s}  ->  {r.stdout.strip()[:60]}")
    if r.stderr.strip():
        print(f"                  WARN: {r.stderr.strip()[:80]}")
