// common.typ — 公共样式与页面设置
// 用法: #import "common.typ": *

// Pandoc 兼容：水平线
#let horizontalrule = line(length: 100%, stroke: 0.5pt + luma(180))

#let board-setup(title: "", body) = {
  set document(title: title)
  set page(
    paper: "a4",
    margin: (top: 1.8cm, bottom: 1.5cm, left: 1.5cm, right: 1.5cm),
    header: align(right, text(size: 8pt, fill: gray)[#title]),
    footer: context align(center, text(size: 8pt)[
      #counter(page).display("1 / 1", both: true)
    ]),
  )
  set text(font: ("Source Han Sans SC", "Noto Sans SC", "Microsoft YaHei", "SimHei"), size: 9pt, lang: "zh")
  set par(leading: 0.55em, justify: true)
  set heading(numbering: "1.1")

  // 标题样式
  show heading.where(level: 1): it => {
    pagebreak(weak: true)
    block(above: 1.2em, below: 0.6em)[
      #text(size: 16pt, weight: "bold")[#it]
    ]
  }
  show heading.where(level: 2): it => {
    block(above: 0.8em, below: 0.4em)[
      #text(size: 11pt, weight: "bold")[#it]
    ]
  }
  show heading.where(level: 3): it => {
    block(above: 0.6em, below: 0.3em)[
      #text(size: 10pt, weight: "bold")[#it]
    ]
  }

  // 代码块样式：紧凑小字号
  show raw.where(block: true): it => {
    block(
      width: 100%,
      fill: luma(248),
      stroke: 0.5pt + luma(200),
      inset: 6pt,
      radius: 2pt,
      above: 0.4em,
      below: 0.4em,
    )[
      #set text(font: ("Consolas", "Source Code Pro", "Courier New"), size: 7.5pt)
      #set par(leading: 0.4em)
      #it
    ]
  }

  // 封面
  align(center)[
    #v(3cm)
    #text(size: 28pt, weight: "bold")[#title]
    #v(0.5cm)
    #text(size: 10pt, fill: gray)[生成时间: #datetime.today().display()]
    #v(0.3cm)
    #text(size: 9pt, fill: gray)[默认省略通用 C++ 头文件与空壳 main]
    #v(1cm)
  ]

  // 目录
  outline(title: "目录", indent: 1.5em, depth: 2)
  pagebreak()

  body
}
