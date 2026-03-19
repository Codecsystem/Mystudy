# 板子生成系统 — 维护指南

> 本文件供 Claude 或其他 AI Agent 在后续维护时参考。

## 项目概述

本目录将仓库中的算法模板（`.cpp`）和笔记（`.md`）自动转换为三份可打印的 Typst PDF 板子，用于算法竞赛。

### 产物

| 板子 | 内容 | 入口文件 | PDF |
|------|------|----------|-----|
| Board 1 算法模板 | 各目录下的 `.cpp` 模板 | `typst/board1-algorithms.typ` | `output/board1-algorithms.pdf` |
| Board 2 数论 | `数论/` 下的笔记 `.md` | `typst/board2-number-theory.typ` | `output/board2-number-theory.pdf` |
| Board 3 杂项 | 其他目录下的笔记 `.md` | `typst/board3-misc.typ` | `output/board3-misc.pdf` |

### 一键重建

```bash
# 1. 先修复 .md 格式（幂等，可反复运行）
python 板子/fix_md_math.py

# 2. 生成 Typst 并编译 PDF
python 板子/generate_typst.py
```

---

## 目录结构

```
板子/
├── AGENT.md                  # 本文件（维护指南）
├── generate_typst.py         # 主生成脚本
├── fix_md_math.py            # .md 格式修复脚本（预处理）
├── cpp模板.cpp               # C++ 通用模板（Board 1 首页）
├── typst/
│   ├── common.typ            # 公共样式（页面、字体、目录、代码块）
│   ├── board1-algorithms.typ # Board 1 入口
│   ├── board2-number-theory.typ
│   └── board3-misc.typ
├── generated/                # 自动生成，勿手改
│   ├── board1-algorithms.typ
│   ├── board2-number-theory.typ
│   ├── board3-misc.typ
│   └── manifest.json         # 收录清单
└── output/                   # 最终 PDF
    ├── board1-algorithms.pdf
    ├── board2-number-theory.pdf
    └── board3-misc.pdf
```

---

## 数据源与收录规则

### Board 1：算法模板

**数据源**：以下目录中的 `.cpp` 文件（按此顺序排列）：

1. `DataStruct/`
2. `Graph/`
3. `string/`
4. `hash/`
5. `动态规划/`
6. `数论/`
7. `计算几何/`
8. `博弈论/`
9. `Other/`

**特殊条目**：
- `板子/cpp模板.cpp` 放在最前面（"通用模板" 分组）

**转换规则**（自写，不依赖 Pandoc）：
- 剔除公共 boilerplate：连续 `#include`、`using namespace std`、注释掉的 `freopen`/`ios`、计时变量、空壳 `main()`
- 保留：核心类/函数/宏、算法说明注释、尾部复杂度注释
- `main()` 处理：空壳删除；含调用示例的提取为"用法示例"代码块
- 目录结构：文件夹名 = 一级标题，文件名（去 `.cpp`）= 二级标题

### Board 2：数论笔记

**数据源**：`数论/` 下的 `.md` 文件，按主题分组：

| 分组 | 文件 |
|------|------|
| 基础与工具 | 筛法、线性逆元、不定方程与同余方程组、数论小结论 |
| 卷积与反演 | 狄利克雷卷积与莫比乌斯反演 1/2、炫酷反演魔术、和式变换 |
| 组合与生成函数 | 排列组合、排列组合进阶、生成函数 |
| 变换与多项式 | FFT笔记、sosdp&fmt&fwt |
| Trick 与杂项 | 数学相关trick、数论笔记(杂项) |

**特殊条目**：
- `表.jpg` 和 `图.png`（注意：原 `图.jpg` 实际是 PNG 格式，已复制为 `图.png`）放在最前面（"参考图表" 分组）

**转换规则**：使用 Pandoc `markdown → typst`，标题层级 +2

### Board 3：杂项笔记

**数据源**：非数论的笔记 `.md`，按主题分组：

| 分组 | 文件 |
|------|------|
| 动态规划 | 对dp的一些思考、数位dp笔记、普通dp常见状态、状压dp常见状态 |
| 图论 | 图相关trick、图论笔记(几类特殊图) |
| 博弈论 | nim游戏 SG函数 |
| 通用 Trick 与杂项 | 杂项相关trick |

**转换规则**：同 Board 2（Pandoc + 标题层级 +2）

---

## fix_md_math.py 修复规则

此脚本在生成前预处理 `.md` 文件，使其对 Pandoc 友好。**幂等**，可反复运行。

| 规则 | 说明 | 示例 |
|------|------|------|
| ````math` → `$$` | 非标准数学代码块转标准格式 | ` ```math\nx^2\n``` ` → `$$\nx^2\n$$` |
| `\[...\]` → `$$...$$` | LaTeX display math 转 Markdown 标准 | `\[ x^2 \]` → `$$\nx^2\n$$` |
| 单行 `$$...$$` → 多行 | Pandoc 要求 `$$` 独占一行 | `$$x^2$$` → `$$\nx^2\n$$` |
| `$$$` → 拆分 | 三连美元符号拆为 `$$` + `$$` | |
| `\(...\)` → `$...$` | LaTeX inline math 转标准 | `\(x\)` → `$x$` |
| `$ content $` → `$content$` | 去除 `$` 与内容间的空格 | `$ m $` → `$m$`（Pandoc 要求紧贴） |
| `\dbinom` → `\binom` | Pandoc 不支持的命令替换 | |
| `\bmod` → `\mod` | 同上 | |

**扫描目录**：`数论/`、`动态规划/`、`Graph/`、`博弈论/`、`Other/`

---

## 常见维护场景

### 新增算法模板

1. 将 `.cpp` 文件放入对应目录（如 `DataStruct/新算法.cpp`）
2. 运行 `python 板子/generate_typst.py`
3. 新模板会自动出现在 Board 1 对应分组中

### 新增/修改笔记

1. 编辑或新增 `.md` 文件
2. 运行 `python 板子/fix_md_math.py` 修复格式
3. 运行 `python 板子/generate_typst.py` 重新生成
4. 如果是新文件，需要在 `generate_typst.py` 的 `BOARD2_GROUPS` 或 `BOARD3_GROUPS` 中添加条目

### 新增笔记目录/分组

在 `generate_typst.py` 中修改对应的 `GROUPS` 字典即可。

### 编译失败排查

1. **Pandoc WARNING**：通常是 `.md` 中有 Pandoc 不支持的 LaTeX 命令。在 `fix_md_math.py` 的规则 7 中添加替换。运行 `pandoc -f markdown -t typst 问题文件.md` 可看到具体警告。
2. **Typst 编译错误**：
   - `unknown variable: horizontalrule` → `common.typ` 中已定义，检查 `#import` 是否正确
   - `failed to decode image` → 检查图片实际格式是否与扩展名匹配（如 `.jpg` 实际是 PNG）
   - `failed to load file (access denied)` → Typst root 目录设置问题，`generate_typst.py` 中 `typst.compile(src, root=ROOT)` 的 `ROOT` 应为仓库根目录
   - `unclosed delimiter` → `.md` 中有未正确格式化的数学公式，运行 `fix_md_math.py`

### 修改版式

编辑 `typst/common.typ`：
- 页边距、字号、字体：`set page` / `set text`
- 代码块样式：`show raw.where(block: true)`
- 标题样式：`show heading.where(level: N)`
- 页码格式：`footer` 中的 `counter(page).display`

---

## 依赖

| 工具 | 用途 | 安装 |
|------|------|------|
| Python 3.10+ | 运行生成脚本 | 已安装 |
| Pandoc 3.x | `.md` → Typst 转换（Board 2/3） | `winget install pandoc` |
| typst (Python) | PDF 编译 | `pip install typst` |

---

## 已知限制

1. Board 2/3 的 Pandoc 转换中，多字母数学变量名（如 `FWT`）会被 Typst 拆成单个字母显示（`F W T`），这是 Typst 数学模式的设计。如需保持连写，在源 `.md` 中用 `\text{FWT}` 或 `\mathrm{FWT}`。
2. `generated/` 目录下的文件是自动生成的，不要手动编辑。
3. 图片路径在生成时会转为相对于 `generated/` 目录的相对路径。新增图片需确保放在对应笔记目录下。
