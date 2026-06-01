# 板子生成系统维护指南

> 本文件是后续 Agent 维护 `板子/` 的入口文档。做影响生成结果的修改时，代码、配置、文档和产物要一起保持同步。

## 日常命令

优先使用统一入口：

```bash
python 板子/scripts/build_board.py
```

只做检查、不重新生成 PDF：

```bash
python 板子/scripts/build_board.py --check-only
```

排错时再拆开跑：

```bash
python 板子/scripts/fix_md_math.py --dry-run
python 板子/scripts/check_note_style.py
python 板子/scripts/generate_typst.py --check
python 板子/scripts/generate_typst.py
```

## 目录结构

```text
板子/
├── AGENTS.md                  # 本文件
├── cpp模板.cpp                # Board 1 首页通用模板
├── config/
│   └── board_config.json      # 收录范围、分组、笔记规范化状态
├── docs/
│   └── NOTE_STYLE.md          # 正式笔记整理规范
├── scripts/
│   ├── build_board.py         # 标准维护入口
│   ├── generate_typst.py      # 生成 generated/*.typ 并编译 PDF
│   ├── fix_md_math.py         # Markdown 数学格式预处理
│   └── check_note_style.py    # 正式笔记硬性格式检查
├── typst/
│   ├── common.typ             # 页面、字体、目录、代码块等公共样式
│   ├── board1-algorithms.typ
│   ├── board2-number-theory.typ
│   └── board3-misc.typ
├── generated/                 # 自动生成，勿手改
└── output/                    # 三份正式 PDF
```

根目录只保留维护入口和通用模板；脚本、配置、规范文档分别收在 `scripts/`、`config/`、`docs/`。不要再把调试脚本或临时配置放回 `板子/` 根目录。

## 自动维护流程

每次维护会话开始时：

1. 递归扫描源目录中的 `.cpp` 和 `.md`，不要只看一级目录。
2. 对比 `板子/config/board_config.json`：
   - `board1.folders`
   - `board2.groups`
   - `board3.groups`
   - `note_style.normalized_files`
3. 若发现新增、删除、重命名文件，先修配置，再运行检查。

每次维护会话结束前：

1. 运行 `python 板子/scripts/build_board.py`。
2. 确认三份 PDF 成功生成：
   - `板子/output/board1-algorithms.pdf`
   - `板子/output/board2-number-theory.pdf`
   - `板子/output/board3-misc.pdf`
3. 清理临时文件：`build_board.py` 会删除 `generated/_*`、`板子/_*`、以及 `output/` 中非正式 PDF。

如果只做了纯讨论或明确不许修改，可以不生成；一旦改了脚本、配置、笔记、模板、Typst 样式或图片资源，就要完整跑构建。

## 新增笔记处理

发现新增 `.md` 时，不要直接塞进配置然后生成。先派 subagent 审视内容，再整理。

推荐流程：

1. 主 Agent 递归扫描并列出新增 `.md`。
2. 对每个新增笔记分配一个 worker subagent；若内容较长或需要审美判断，使用 `gpt-5.5` + `reasoning_effort: xhigh`。
3. subagent 只处理自己分配的文件，不能修改脚本、配置、生成物，也不能回滚他人的改动。
4. subagent 输出：
   - 应归入 Board 2 还是 Board 3，建议分组名
   - 是否属于黑名单或应暂缓整理
   - 内容是否适合正式板子，是否有明显公式/代码/图片风险
   - 若非黑名单，按 `板子/docs/NOTE_STYLE.md` 直接整理该 `.md`
5. 主 Agent 审核 subagent 修改，再更新：
   - `板子/config/board_config.json` 的 `board2.groups` / `board3.groups`
   - 非黑名单正式笔记还要加入 `note_style.normalized_files`
   - 必要时更新本文件的数据源说明
6. 运行完整构建和样式检查。

黑名单规则：

- 文件名包含 `trick` 的笔记暂不强制整理。
- `note_style.blacklist_names` 中的随笔或暂缓文件不强制整理。
- 黑名单文件可以收录进板子，但默认不加入 `normalized_files`，也不让 `fix_md_math.py` 自动改，除非用户明确要求。

## 数据源

### Board 1：算法模板

配置：`板子/config/board_config.json` → `board1.folders`

当前目录顺序：

1. `数据结构/`
2. `图论/`
3. `字符串/`
4. `动态规划/`
5. `数论/`
6. `计算几何/`
7. `博弈论/`
8. `其他/`

生成规则：

- `板子/cpp模板.cpp` 固定放在最前面。
- 每个配置目录递归收集 `.cpp`。
- 生成器会剔除公共 boilerplate、空壳 `main()`，保留核心代码和有价值的用法示例。

### Board 2：数论笔记

配置：`板子/config/board_config.json` → `board2.reference_images`、`board2.groups`

当前分组：

| 分组 | 文件 |
|------|------|
| 基础与工具 | `数论笔记(筛法)`、`数论笔记(线性逆元)`、`数论笔记(不定方程与同余方程组)`、`数论小结论` |
| 卷积与反演 | `狄利克雷卷积与莫比乌斯反演 1/2`、`炫酷反演魔术`、`和式变换` |
| 组合与生成函数 | `排列组合`、`排列组合进阶`、`生成函数` |
| 变换与多项式 | `FFT笔记`、`sosdp&fmt&fwt`、`阶,原根与ntt` |
| Trick 与杂项 | `数学相关trick`、`数论笔记(杂项)`、`数学笔记(矩阵半环)` |

`表.jpg` 和 `图.png` 作为参考图表放在 Board 2 开头。

### Board 3：杂项笔记

配置：`板子/config/board_config.json` → `board3.groups`

当前分组：

| 分组 | 文件 |
|------|------|
| 动态规划 | `对dp的一些思考`、`数位dp笔记`、`普通dp常见状态`、`状压dp常见状态` |
| 数据结构 | `数据结构trick` |
| 图论 | `图相关trick`、`图论笔记(几类特殊图)` |
| 博弈论 | `nim游戏 SG函数` |
| 通用 Trick 与杂项 | `杂项相关trick` |
| 字符串 | `一些比较神秘的hash手法`、`字符串trick` |

## 笔记规范

正式笔记按 `板子/docs/NOTE_STYLE.md` 整理。核心原则：

- 保留作者原本的信息密度、口吻和竞赛笔记风格。
- 不扩写成教材，不强行套“定义/定理/证明/推论”模板。
- 源 Markdown 优先使用 `####/#####`，照顾本地阅读器字号。
- 生成器会把每个文件中最浅标题映射到板子正文层级。
- 分隔线使用 `<!-- board:hr -->`，不要直接写独占 `---`。
- 代码块写语言标记，公式块使用多行 `$$`。

`check_note_style.py` 默认只检查 `note_style.normalized_files`，避免历史随笔一次性全量变红。需要扫描所有非黑名单笔记时使用：

```bash
python 板子/scripts/check_note_style.py --all
```

## 脚本职责

### `scripts/build_board.py`

标准入口。顺序执行：

1. `fix_md_math.py`
2. `check_note_style.py`
3. `generate_typst.py --check`
4. `generate_typst.py`
5. 清理临时文件

`--check-only` 会把第 1 步改成 `fix_md_math.py --dry-run`，并跳过 PDF 生成与清理。

### `scripts/fix_md_math.py`

幂等预处理，默认只扫描 `board2.groups` 和 `board3.groups` 中的非黑名单 `.md`。

主要规则：

| 规则 | 说明 |
|------|------|
| ````math` → `$$` | 非标准数学代码块转标准公式块 |
| `\[...\]` → `$$...$$` | LaTeX display math 转 Markdown 标准 |
| 单行 `$$...$$` → 多行 | 避免 Pandoc 误解析 |
| `\(...\)` → `$...$` | LaTeX inline math 转标准 |
| `$ content $` → `$content$` | 仅处理不含中文的简单情况；表格行跳过 |
| `\dbinom` → `\binom` | 替换 Pandoc 不支持命令 |
| `\bmod` → `\mod` | 替换 Pandoc 不支持命令 |
| `(mod \ X)` → `\pmod{X}` | 修复同余记号 |

如需手动处理黑名单文件：

```bash
python 板子/scripts/fix_md_math.py --include-blacklisted
```

### `scripts/generate_typst.py`

生成三份 `generated/*.typ`、`generated/manifest.json`，并编译 PDF。

Board 2/3 使用 Pandoc `markdown-yaml_metadata_block → typst`，然后做少量后处理：

| 后处理 | 说明 |
|------|------|
| `<!-- board:hr -->` / 独占 `---` → Typst 横线 | 兼容旧笔记 |
| 标题层级重映射 | 文件内最浅标题映射为正文层级，并封顶到 6 级 |
| `\( mod ... \)` → `(mod ...)` | 修复 `\pmod{}` 转换残留 |
| `\$\$...\$\$` 重转 | 尝试修复 Pandoc 未转换的 display math |
| 数学内 `\(` `\)` → `(` `)` | 修复残留转义括号 |
| `,)` → `)` | 修复部分 Pandoc 尾随逗号 |
| 图片路径按当前 `.md` 所在目录解析 | 缺失图片用红字占位 |

`--check` 只校验配置，不生成。

### `scripts/check_note_style.py`

检查标题空行、代码块语言、公式块、分隔线、图片块、行尾空格等硬性规则。输出格式：

```text
path:line: RULE message
```

## 配置维护

`板子/config/board_config.json` 是收录范围唯一来源。不要在脚本里新增目录或分组硬编码。

配置变更规则：

- 新增/删除算法目录：改 `board1.folders`，同步更新本文件。
- 新增/删除笔记：改 `board2.groups` 或 `board3.groups`。
- 正式整理过的非黑名单笔记：加入 `note_style.normalized_files`。
- 暂缓整理文件：加入 `note_style.blacklist_names`，或确保文件名命中 `blacklist_name_contains`。

旧英文目录名只作为历史别名，不应再写入配置：

| 历史名 | 当前名 |
|------|------|
| `DataStruct` | `数据结构` |
| `Graph` | `图论` |
| `string` | `字符串` |
| `hash` | `哈希` |
| `Other` | `其他` |

## 编译失败排查

- Pandoc 报错：先对问题文件运行同参数转换：`pandoc -f markdown-yaml_metadata_block -t typst --wrap=none 问题文件.md`。
- Typst `unknown variable: horizontalrule`：检查 `typst/common.typ` 是否被正确 import。
- Typst `failed to decode image`：检查图片扩展名和真实格式是否一致。
- Typst `failed to load file (access denied)`：检查 `typst.compile(src, root=ROOT)`，`ROOT` 应为仓库根目录。
- Typst `unexpected comma`：检查 Pandoc 输出里的 `,)` 或图片缺失占位。
- Typst `unclosed delimiter`：优先检查未规范化的数学公式。
- `git diff --check` 提示 `=======`：确认不是生成出的七级 Typst 标题；生成器已将标题封顶到 6 级。

## 依赖

| 工具 | 用途 |
|------|------|
| Python 3.10+ | 运行维护脚本 |
| Pandoc 3.x | Markdown 转 Typst |
| `typst` Python 包 | 编译 PDF |

## 已知限制

1. `generated/` 是自动生成目录，不要手改。
2. 图片路径会在生成时改成相对 `generated/` 的路径；新增图片应放在对应笔记附近。
3. Typst 数学模式会把多字母变量拆开显示；需要连写时在源 Markdown 中用 `\text{FWT}` 或 `\mathrm{FWT}`。
