// common.typ — 竞赛板子高密度双栏视觉主题（A4 竖版双栏护眼清晰打印版）
// 用法: #import "common.typ": *

// ── 主题调色板 (针对纸质/黑白激光打印强化对比度) ──
#let c-primary     = rgb("#0f172a") // 一级大类深墨蓝底色（黑白打印亦呈实黑）
#let c-accent      = rgb("#1d4ed8") // 强调色（左竖条、编号高亮）
#let c-sec-bg      = rgb("#e2e8f0") // 二级标题底纹（加深对比度，防激光打印发虚）
#let c-code-bg     = rgb("#f8fafc") // 代码块极浅底纹
#let c-code-border = rgb("#94a3b8") // 代码块与表格实线边框（加深确保纸面锐利）
#let c-quote-bg    = rgb("#f1f5f9") // 引用块底纹
#let c-quote-bar   = rgb("#334155") // 引用块左竖线
#let c-text        = rgb("#0f172a") // 正文纯深色
#let c-muted       = rgb("#334155") // 页眉/页脚/次要文字（加深至深炭灰，打印清晰）
#let c-rule        = rgb("#cbd5e1") // 栏间与小节分隔线

// Pandoc 兼容：紧凑水平分隔线
#let horizontalrule = block(above: 0.4em, below: 0.4em)[
  #line(length: 100%, stroke: (paint: c-code-border, thickness: 0.5pt, dash: "densely-dashed"))
]

// 用法示例 / 尾部注释 紧凑徽章标签
#let code-badge(label, color: c-accent) = block(
  above: 0.3em,
  below: 0.15em,
  sticky: true,
)[
  #box(
    fill: color.lighten(86%),
    stroke: 0.45pt + color.lighten(40%),
    inset: (x: 4.5pt, y: 2pt),
    radius: 1.5pt,
  )[
    #text(size: 7.0pt, weight: "bold", fill: color.darken(20%))[#label]
  ]
]

#let board-setup(title: "", subtitle: "ACM/ICPC 竞赛算法与笔记速查手册 · 默认省略通用头文件与空壳 main", body) = {
  set document(title: title)

  // ── 页面与双栏网格设置 ──
  set page(
    paper: "a4",
    flipped: false,
    columns: 2,
    margin: (top: 1.3cm, bottom: 1.0cm, left: 0.85cm, right: 0.85cm),
    // 栏间分隔细线（首页从通栏铭牌下方开始，后续页全高）
    background: context {
      let top-offset = if here().page() == 1 { 2.95cm } else { 1.3cm }
      let bottom-margin = 1.0cm
      place(
        top + center,
        dy: top-offset,
        line(
          start: (0pt, 0pt),
          end: (0pt, 100% - top-offset - bottom-margin),
          stroke: 0.45pt + c-rule,
        )
      )
    },
    // 动态检索页眉：左侧显示【分册名 | 当前大类】，右侧显示【当前页算法/笔记范围】
    header: context {
      let pg = here().page()
      let h1s = query(heading.where(level: 1)).filter(h => h.location().page() <= pg)
      let cur-h1 = if h1s.len() > 0 { h1s.last().body } else { none }

      let h2s-on = query(heading.where(level: 2)).filter(h => h.location().page() == pg)
      let h2s-pre = query(heading.where(level: 2)).filter(h => h.location().page() < pg)
      let cur-h2 = if h2s-on.len() > 1 {
        [#h2s-on.first().body #text(fill: c-muted)[→] #h2s-on.last().body]
      } else if h2s-on.len() == 1 {
        h2s-on.first().body
      } else if h2s-pre.len() > 0 {
        h2s-pre.last().body
      } else {
        none
      }

      block(width: 100%, below: 0pt)[
        #set text(size: 7.4pt, fill: c-muted)
        #grid(
          columns: (1fr, auto),
          align: (left + bottom, right + bottom),
          [
            #text(weight: "bold", fill: c-primary)[#title]
            #if cur-h1 != none [
              #h(4pt) #text(fill: c-code-border)[|] #h(4pt)
              #text(weight: "bold", fill: c-accent.darken(15%))[#cur-h1]
            ]
          ],
          [
            #if cur-h2 != none [
              #text(weight: "medium", fill: c-muted)[#cur-h2]
            ]
          ]
        )
        #v(2.5pt)
        #line(length: 100%, stroke: 0.55pt + c-code-border)
      ]
    },
    header-ascent: 22%,
    // 紧凑高对比度页脚
    footer: context {
      block(width: 100%, above: 0pt)[
        #line(length: 100%, stroke: 0.45pt + c-code-border)
        #v(2.5pt)
        #set text(size: 7.4pt, fill: c-muted)
        #grid(
          columns: (1fr, auto, 1fr),
          align: (left, center, right),
          [#text(weight: "medium")[Mystudy Board System]],
          [
            #box(
              fill: c-sec-bg,
              stroke: 0.4pt + c-code-border,
              inset: (x: 6pt, y: 1.5pt),
              radius: 2pt,
            )[
              #text(weight: "bold", fill: c-primary)[#counter(page).display("1 / 1", both: true)]
            ]
          ],
          [#text(weight: "medium")[#datetime.today().display("[year]-[month]-[day]")]],
        )
      ]
    },
    footer-descent: 25%,
  )

  set columns(gutter: 0.8em)

  // ── 基础字体与段落设置（8.5pt 黄金护眼字号，段落间距 1.15em 留出清晰空行呼吸感） ──
  set text(
    font: ("Noto Sans SC", "Source Han Sans SC", "Microsoft YaHei", "SimHei"),
    size: 8.5pt,
    fill: c-text,
    lang: "zh",
  )
  set par(leading: 0.52em, spacing: 1.15em, justify: true)

  // 仅对 1~2 级标题自动编号，避免笔记内 3~5 级小节出现 1.2.1.1.1 与原文序号重叠
  set heading(numbering: (..nums) => {
    let n = nums.pos()
    if n.len() <= 2 {
      numbering("1.1", ..n)
    }
  })

  // ── 标题视觉层级 ──
  // H1：大类深色胶囊横幅（不强制换页，双栏流式衔接）
  show heading.where(level: 1): it => block(
    width: 100%,
    fill: c-primary,
    inset: (left: 6.5pt, right: 6.5pt, y: 4.5pt),
    radius: 2pt,
    above: 0.85em,
    below: 0.45em,
    sticky: true,
  )[
    #set text(fill: white, size: 10pt, weight: "bold")
    #grid(
      columns: (auto, 1fr),
      column-gutter: 5.5pt,
      align: horizon,
      if it.numbering != none {
        box(
          fill: c-accent,
          inset: (x: 4.5pt, y: 1.5pt),
          radius: 1.5pt,
        )[#text(size: 8.2pt, fill: white, weight: "bold")[#counter(heading).display()]]
      },
      [#it.body],
    )
  ]

  // H2：算法模板 / 笔记篇目 左侧蓝条高对比卡片头
  show heading.where(level: 2): it => block(
    width: 100%,
    fill: c-sec-bg,
    stroke: (left: 2.8pt + c-accent, top: 0.45pt + c-code-border, right: 0.45pt + c-code-border, bottom: 0.45pt + c-code-border),
    inset: (left: 5.5pt, right: 5.5pt, y: 3.5pt),
    radius: (right: 1.5pt),
    above: 0.68em,
    below: 0.32em,
    sticky: true,
  )[
    #set text(fill: c-primary, size: 9.0pt, weight: "bold")
    #if it.numbering != none [
      #text(fill: c-accent.darken(10%), weight: "bold", size: 8.4pt)[#counter(heading).display()]
      #h(4pt)
    ]
    #it.body
  ]

  // H3：笔记内一级小节（带几何菱形引导符）
  show heading.where(level: 3): it => block(
    width: 100%,
    stroke: (bottom: 0.45pt + c-code-border),
    inset: (bottom: 2pt),
    above: 0.55em,
    below: 0.25em,
    sticky: true,
  )[
    #set text(fill: c-primary, size: 8.6pt, weight: "bold")
    #text(fill: c-accent, size: 7.2pt)[◆] #h(3pt) #it.body
  ]

  // H4：笔记内二级小节
  show heading.where(level: 4): it => block(
    above: 0.42em,
    below: 0.18em,
    sticky: true,
  )[
    #set text(fill: c-accent.darken(25%), size: 8.4pt, weight: "bold")
    #text(fill: c-accent)[▸] #h(2.5pt) #it.body
  ]

  // H5 / H6：笔记内细分子项
  show heading.where(level: 5): it => block(
    above: 0.35em,
    below: 0.15em,
    sticky: true,
  )[
    #set text(fill: c-primary, size: 8.2pt, weight: "bold")
    #it.body
  ]
  show heading.where(level: 6): it => block(
    above: 0.3em,
    below: 0.12em,
    sticky: true,
  )[
    #set text(fill: c-muted, size: 8.0pt, weight: "bold")
    #it.body
  ]

  // ── 代码块样式（7.0pt 清晰护眼字号，保留源码原始缩进与换行） ──
  show raw.where(block: true): it => block(
    width: 100%,
    fill: c-code-bg,
    stroke: (left: 1.8pt + rgb("#475569"), top: 0.45pt + c-code-border, right: 0.45pt + c-code-border, bottom: 0.45pt + c-code-border),
    inset: (x: 5pt, y: 4pt),
    radius: (right: 1.5pt),
    above: 0.26em,
    below: 0.32em,
  )[
    #set text(
      font: ("Consolas", "Cascadia Mono", "Noto Sans SC", "Microsoft YaHei"),
      size: 7.0pt,
      fill: rgb("#0f172a"),
    )
    #set par(leading: 0.36em, justify: false)
    #it
  ]

  // 行内代码样式
  show raw.where(block: false): it => box(
    fill: c-quote-bg,
    stroke: 0.35pt + c-code-border,
    inset: (x: 2.5pt, y: 1pt),
    outset: (y: 1pt),
    radius: 1.2pt,
  )[
    #set text(font: ("Consolas", "Cascadia Mono", "Noto Sans SC", "Microsoft YaHei"), size: 7.6pt, fill: c-primary)
    #it
  ]

  // ── 引用块 Callout 样式 ──
  show quote.where(block: true): it => block(
    width: 100%,
    fill: c-quote-bg,
    stroke: (left: 2.2pt + c-quote-bar, rest: 0.4pt + c-code-border),
    inset: (left: 5.5pt, right: 5pt, y: 3.5pt),
    radius: (right: 1.5pt),
    above: 0.32em,
    below: 0.32em,
  )[
    #set text(size: 8.0pt, fill: rgb("#1e293b"))
    #set par(leading: 0.44em)
    #it.body
  ]

  // ── 列表、公式、表格与图片清晰化 ──
  set list(indent: 0.45em, body-indent: 0.4em, spacing: 0.36em, marker: ([#text(fill: c-accent, size: 7pt)[•]], [#text(fill: c-muted, size: 6.5pt)[◦]]))
  set enum(indent: 0.45em, body-indent: 0.4em, spacing: 0.36em)

  show math.equation.where(block: true): set block(above: 0.38em, below: 0.38em)
  show math.equation.where(block: true): set text(size: 8.6pt)

  set table(
    stroke: 0.45pt + c-code-border,
    inset: (x: 4.5pt, y: 3.2pt),
    fill: (x, y) => if y == 0 { c-sec-bg } else { none },
  )
  show table: set text(size: 7.8pt)

  set figure(gap: 0.3em)
  show figure: set block(above: 0.35em, below: 0.35em)
  show figure.caption: set text(size: 7.2pt, fill: c-muted)

  // ── 目录样式定制 ──
  show outline.entry.where(level: 1): it => block(above: 0.5em, below: 0.18em)[
    #set text(size: 8.4pt, weight: "bold", fill: c-primary)
    #it
  ]
  show outline.entry.where(level: 2): it => block(above: 0.2em, below: 0.12em)[
    #set text(size: 7.6pt, fill: rgb("#1e293b"))
    #it
  ]

  // ── 首页通栏紧凑铭牌 (Masthead) ──
  place(
    top + center,
    scope: "parent",
    float: true,
    block(
      width: 100%,
      fill: c-primary,
      stroke: (bottom: 2.2pt + c-accent),
      inset: (x: 10pt, y: 7pt),
      radius: 2.5pt,
      below: 0.45em,
    )[
      #grid(
        columns: (1fr, auto),
        align: (left + horizon, right + horizon),
        [
          #text(size: 14.5pt, weight: "bold", fill: white, tracking: 0.5pt)[#title]
          #v(2.5pt)
          #text(size: 7.4pt, fill: rgb("#e2e8f0"))[#subtitle]
        ],
        [
          #box(
            fill: rgb("#1e293b"),
            stroke: 0.5pt + rgb("#64748b"),
            inset: (x: 6.5pt, y: 4pt),
            radius: 2pt,
          )[
            #set text(size: 7.0pt, fill: white)
            #align(right)[
              编译日期：#datetime.today().display("[year]-[month]-[day]")\
              版式：A4 双栏护眼清晰打印版
            ]
          ]
        ]
      )
    ]
  )

  // ── 双栏紧凑目录（目录结束后直接流式进入正文，消除首页留白） ──
  block(
    width: 100%,
    fill: c-sec-bg,
    stroke: (left: 2.8pt + c-primary, rest: 0.45pt + c-code-border),
    inset: (x: 5.5pt, y: 3.5pt),
    below: 0.35em,
  )[
    #text(size: 8.8pt, weight: "bold", fill: c-primary)[目录索引 (Table of Contents)]
  ]
  outline(title: none, indent: 1em, depth: 2)
  block(above: 0.55em, below: 0.55em)[
    #line(length: 100%, stroke: 0.9pt + c-primary)
  ]

  body
}
