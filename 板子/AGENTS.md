# 板子生成系统 — 维护指南

> 本文件供 Claude 或其他 AI Agent 在后续维护时参考。

## 项目概述

本目录将仓库中的算法模板（`.cpp`）和笔记（`.md`）自动转换为三份可打印的 Typst PDF 板子，用于算法竞赛。

### 产物

| 板子 | 内容 | 入口文件 | PDF |
|------|------|----------|-----|
| Board 1 算法模板 | `数据结构/`、`图论/`、`字符串/`、`哈希/`、`动态规划/`、`数论/`、`计算几何/`、`博弈论/`、`其他/` 下的 `.cpp` 模板 | `typst/board1-algorithms.typ` | `output/board1-algorithms.pdf` |
| Board 2 数论 | `数论/数论笔记部分/` 与 `数论/Trick/` 下的数论笔记 `.md` | `typst/board2-number-theory.typ` | `output/board2-number-theory.pdf` |
| Board 3 杂项 | `动态规划/dp笔记/`、`图论/Trick/`、`图论/图论笔记/`、`博弈论/`、`其他/Trick/` 下的杂项笔记 `.md` | `typst/board3-misc.typ` | `output/board3-misc.pdf` |

### 一键重建

```bash
# 1. 先修复 .md 格式（幂等，可反复运行）
python 板子/fix_md_math.py

# 2. 生成 Typst 并编译 PDF
python 板子/generate_typst.py
```

---

## Agent 自动操作

以下操作应在每次维护会话开始或结束时由 Agent 自动执行，无需用户指示。

### 1. 自动重新生成板子

**触发时机**：每次维护会话结束前；以及任何影响板子内容、收录范围、转换规则、版式或资源文件的修改完成后。

**执行顺序**：

```bash
# 1. 先修复 .md 格式（幂等，可反复运行）
python 板子/fix_md_math.py

# 2. 生成 Typst 并编译 PDF
python 板子/generate_typst.py
```

**要求**：
- 默认应重新生成三份正式 PDF，不要只改脚本不验证产物
- 若生成失败，应优先修复构建问题，再更新本文件中的「编译失败排查」
- 生成完成后执行垃圾文件清理

### 2. 自动清理垃圾文件

**触发时机**：每次维护会话结束前、或重新生成 PDF 后。

**清理范围**：

| 位置 | 清理规则 |
|------|----------|
| `板子/generated/` | 删除所有 `_` 开头的临时文件（`_test*.typ`、`_test*.pdf`、`_validate.typ`、`_check.py` 等） |
| `板子/` | 删除根目录下的临时脚本（如 `_check.py`、`_debug*.py` 等调试用文件） |
| `板子/output/` | 仅保留三份正式 PDF，删除其他文件 |

**保留文件（不可删除）**：
- `AGENTS.md`、`generate_typst.py`、`fix_md_math.py`、`cpp模板.cpp`
- `generated/board*.typ`、`generated/manifest.json`
- `output/board*.pdf`
- `typst/*.typ`

**执行方式**：
```bash
# 清理 generated/ 下的临时文件
find 板子/generated/ -name '_*' -delete

# 清理板子/根目录下的临时脚本
find 板子/ -maxdepth 1 -name '_*' -delete
```

### 3. 当前仓库目录名映射

仓库当前一级目录已使用中文名，维护脚本里的旧英文目录名需要视为历史遗留别名：

| 旧硬编码 | 当前目录 |
|----------|----------|
| `DataStruct` | `数据结构` |
| `Graph` | `图论` |
| `string` | `字符串` |
| `hash` | `哈希` |
| `Other` | `其他` |

如果脚本仍引用旧英文目录名，会导致模板或笔记漏收录，维护时应优先检查并同步修正。

### 4. 自动更新 AGENTS.md

**触发时机**：当以下任一情况发生时，Agent 应自动更新本文件：

| 变更类型 | 更新内容 |
|----------|----------|
| `fix_md_math.py` 新增/修改规则 | 更新「fix_md_math.py 修复规则」表格 |
| `generate_typst.py` 新增后处理 | 更新「generate_typst.py 后处理规则」表格 |
| 新增/删除笔记分组 | 更新「数据源与收录规则」中的分组表格 |
| 新增/删除源目录 | 更新 Board 1 目录列表 |
| 发现新的 .md 格式要求 | 更新「.md 源文件格式要求」 |
| 新增依赖或工具 | 更新「依赖」表格 |
| 发现新的已知限制 | 更新「已知限制」列表 |
| 修复了编译错误 | 在「编译失败排查」中补充对应条目 |

**原则**：本文件是后续 Agent 的唯一入口文档，必须保持与代码同步。任何影响构建流程的变更都应反映在此。

### 5. 自动扫描文件夹结构

**触发时机**：每次维护会话开始时。

**原因**：用户可能在两次会话之间重命名文件夹、新增/删除目录或移动文件，本文件中记录的目录结构和 `generate_typst.py` 中的硬编码路径可能已过时。

**操作步骤**：

1. **扫描仓库根目录**：`ls` 列出所有一级子目录，与本文件「Board 1 数据源」中的目录列表对比
2. **扫描各目录下的文件**：检查 `.cpp` 和 `.md` 文件是否有新增/删除/重命名
3. **比对脚本中的硬编码**：
   - `generate_typst.py` → `BOARD1_FOLDERS`、`BOARD2_GROUPS`、`BOARD3_GROUPS`
   - `fix_md_math.py` → `dirs` 列表
4. **发现差异时**：
   - 向用户报告变更（如「发现新目录 `xxx/`，是否加入 Board 1？」）
   - 用户确认后更新脚本和本文件
   - **如果仓库已统一改为中文目录名，应同步更新脚本中的旧英文目录硬编码**
   - 不要静默忽略新文件或已删除的文件

**示例差异**：
```
⚠ 目录变更:
  + 新增目录: 线段树/          → 建议加入 Board 1 BOARD1_FOLDERS
  - 目录已删除: hash/          → 需从 BOARD1_FOLDERS 移除
  ~ 重命名: string/ → 字符串/  → 需更新 BOARD1_FOLDERS

⚠ 文件变更:
  + 数论/数论笔记部分/数论笔记(新主题).md   → 建议加入 Board 2 BOARD2_GROUPS
  - 图论/Trick/图相关trick.md 已删除       → 需从 Board 3 BOARD3_GROUPS 移除
```

---

## 目录结构

```
板子/
├── AGENTS.md                 # 本文件（维护指南）
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

1. `数据结构/`
2. `图论/`
3. `字符串/`
4. `哈希/`
5. `动态规划/`
6. `数论/`
7. `计算几何/`
8. `博弈论/`
9. `其他/`

**特殊条目**：
- `板子/cpp模板.cpp` 放在最前面（"通用模板" 分组）

**转换规则**（自写，不依赖 Pandoc）：
- 剔除公共 boilerplate：连续 `#include`、`using namespace std`、注释掉的 `freopen`/`ios`、计时变量、空壳 `main()`
- 保留：核心类/函数/宏、算法说明注释、尾部复杂度注释
- `main()` 处理：空壳删除；含调用示例的提取为"用法示例"代码块
- 目录结构：文件夹名 = 一级标题，文件名（去 `.cpp`）= 二级标题

### Board 2：数论笔记

**数据源**：`数论/` 下的 `.md` 文件，当前实际分布在以下位置，并按主题分组：

| 分组 | 文件 |
|------|------|
| 基础与工具 | `数论/数论笔记部分/数论笔记(筛法).md`、`数论/数论笔记部分/数论笔记(线性逆元).md`、`数论/数论笔记部分/数论笔记(不定方程与同余方程组).md`、`数论/数论笔记部分/数论小结论.md` |
| 卷积与反演 | `数论/数论笔记部分/数论笔记(狄利克雷卷积与莫比乌斯反演 1).md`、`数论/数论笔记部分/数论笔记(狄利克雷卷积与莫比乌斯反演 2).md`、`数论/数论笔记部分/数论笔记(炫酷反演魔术).md`、`数论/数论笔记部分/数论笔记(和式变换).md` |
| 组合与生成函数 | `数论/数论笔记部分/数论笔记(排列组合).md`、`数论/数论笔记部分/数论笔记(排列组合进阶).md`、`数论/数论笔记部分/数论笔记(生成函数).md` |
| 变换与多项式 | `数论/数论笔记部分/FFT笔记.md`、`数论/数论笔记部分/数论笔记(sosdp&fmt&fwt).md`、`数论/数论笔记部分/数论笔记(阶,原根与ntt).md` |
| Trick 与杂项 | `数论/Trick/数学相关trick.md`、`数论/数论笔记部分/数论笔记(杂项).md` |

**特殊条目**：
- `表.jpg` 和 `图.png`（注意：原 `图.jpg` 实际是 PNG 格式，已复制为 `图.png`）放在最前面（"参考图表" 分组）

**转换规则**：使用 Pandoc `markdown → typst`，标题层级 +2。图片路径必须以各自 `.md` 文件所在目录为基准解析，而不是以上层主题目录为基准。

### Board 3：杂项笔记

**数据源**：非数论的笔记 `.md`，当前实际分布在以下位置，并按主题分组：

| 分组 | 文件 |
|------|------|
| 动态规划 | `动态规划/对dp的一些思考.md`、`动态规划/dp笔记/数位dp笔记.md`、`动态规划/dp笔记/普通dp常见状态.md`、`动态规划/dp笔记/状压dp常见状态.md` |
| 图论 | `图论/Trick/图相关trick.md`、`图论/图论笔记/图论笔记(几类特殊图).md` |
| 博弈论 | `博弈论/nim游戏 SG函数.md` |
| 通用 Trick 与杂项 | `其他/Trick/杂项相关trick.md` |

**转换规则**：同 Board 2（Pandoc + 标题层级 +2）。图片路径必须以各自 `.md` 文件所在目录为基准解析。

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
| `(mod \ X)` → `\pmod{X}` | 修复 mod 记号避免 Pandoc 把 `\(` 当成 inline math | `(mod \ m)` → `\pmod{m}` |

**扫描目录**：`数论/`、`数论/Trick/`、`数论/数论笔记部分/`、`动态规划/`、`动态规划/dp笔记/`、`图论/`、`图论/Trick/`、`图论/图论笔记/`、`博弈论/`、`其他/`、`其他/Trick/`

### generate_typst.py 后处理规则（Pandoc 输出修复）

| 规则 | 说明 |
|------|------|
| `\( mod ... \)` → `(mod ...)` | 修复 Pandoc 将 `\pmod{}` 输出为 Typst 中无效的 `\(` 分隔符 |
| `\$\$...\$\$` → 重新转换 | 修复 Pandoc 未能转换的 display math 块 |
| 数学内 `\(` `\)` → `(` `)` | 修复 `$...$` 内部残留的转义括号 |
| `,)` → `)` | 修复 Pandoc 生成的尾随逗号，避免 Typst `unexpected comma` |
| 缺失图片注释 → 红字占位文本 | 避免在 `figure(...)` 参数位置插入注释导致语法错误 |
| 图片相对路径按当前 `.md` 所在目录解析 | 修复 `md.img/...` 这类资源在子目录笔记中的引用失效 |

### .md 源文件格式要求

- **heading 前必须有空行**：Pandoc 要求 `#` 标题前有空行，否则不识别
- **`$$` 块内不要以 `=` 开头**：会被 Pandoc 误识别为 Typst heading。使用 `\begin{aligned}` + `&=` 代替
- **`(mod m)` 写法**：使用 `\pmod{m}` 而非 `(mod \ m)`，后者会导致 Pandoc 产生无效的 `\(` 分隔符

---

## 常见维护场景

> 以下场景中，若 Agent 已进入维护流程，则应按本文件「Agent 自动操作」自动执行重建与清理，无需用户额外指示。用户手动维护时，可按下面步骤执行。

### 新增算法模板

1. 将 `.cpp` 文件放入对应目录（如 `数据结构/新算法.cpp`）
2. 如新增目录，先同步更新 `generate_typst.py` 的 `BOARD1_FOLDERS` 与本文件
3. 运行 `python 板子/generate_typst.py`
4. 新模板会自动出现在 Board 1 对应分组中

### 新增/修改笔记

1. 编辑或新增 `.md` 文件
2. 如果是新文件、新分组或新子目录，先同步更新 `generate_typst.py` 的 `BOARD2_GROUPS` / `BOARD3_GROUPS`、`fix_md_math.py` 的 `dirs` 以及本文件
3. 运行 `python 板子/fix_md_math.py` 修复格式
4. 运行 `python 板子/generate_typst.py` 重新生成

### 新增笔记目录/分组

1. 更新 `generate_typst.py` 中对应的 `BOARD2_GROUPS` 或 `BOARD3_GROUPS`
2. 如果新增目录中含 `.md`，同步更新 `fix_md_math.py` 的 `dirs`
3. 更新本文件中的数据源与扫描目录说明
4. 重新生成并验证 PDF

### 编译失败排查

1. **Pandoc WARNING**：通常是 `.md` 中有 Pandoc 不支持的 LaTeX 命令。在 `fix_md_math.py` 的规则 7 中添加替换。运行 `pandoc -f markdown -t typst 问题文件.md` 可看到具体警告。
2. **Typst 编译错误**：
   - `unknown variable: horizontalrule` → `common.typ` 中已定义，检查 `#import` 是否正确
   - `failed to decode image` → 检查图片实际格式是否与扩展名匹配（如 `.jpg` 实际是 PNG）
   - `failed to load file (access denied)` → Typst root 目录设置问题，`generate_typst.py` 中 `typst.compile(src, root=ROOT)` 的 `ROOT` 应为仓库根目录
   - `unexpected comma` → 先检查是否是图片缺失占位写坏了 `figure(...)` 参数结构，或 Pandoc 在 `table(...)`、`align(...)` 等结构后生成了尾随逗号 `,)`
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
