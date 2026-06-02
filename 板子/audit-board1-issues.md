# Board 1 算法模板事实核查问题清单

本清单来自多 agent 只读审计，重点是 `板子/output/board1-algorithms.pdf` 对应的算法模板本身：算法是否正确、能否编译、复制后是否可用、边界条件是否会炸。

验证方式包括：

- `python 板子/scripts/build_board.py --check-only`
- `g++ 14.2.0 -std=c++20 -fsyntax-only` / `-std=gnu++23 -fsyntax-only`
- worker 在临时目录内构造的小样例、暴力/反例验证
- `板子/generated/board1-algorithms.typ` 与源 `.cpp` 的复制可用性对照

## Critical

### 数据结构/普通莫队.cpp:27-41, 121-129

问题：BIT 直接以 `a[i]` 作下标，未离散化；`a[i]=0` 时 `lowbit(0)=0` 会死循环，`a[i]>n` 时更新被忽略或查询越界读。

证据：临时样例 `n=1, a[1]=100, query 1 1 1` 运行输出随机大数 `1936942451`。

建议：启用离散化；BIT 下标从 1 开始；查询使用压缩 rank，并改为 `pre(r)-pre(l-1)`。
> 特定解法 没问题 主要记录莫队思想

### 图论/差分约束.cpp:52, 62-66

问题：负环检测会误判可行系统。`cnt[s]=1` 且 `cnt[v]>=n+1`，会把从超级源到普通点的合法 `n` 边路径当作负环。

反例：

```text
2 1
2 1 -1
```

该系统无负环，程序输出 `NO`。

建议：源点计数从 0 开始，按包含超级源后的点数判断；或改成标准入队松弛次数/边数阈值。
>fixed
### 图论/johnson最短路.cpp:61, 116, 122-123

问题：`1e9` / `1e8` 作为 INF 裁剪会破坏合法距离与不可达距离，合法距离 `100000000` 被改成 `1e9`。

反例：

```text
2 1
1 2 100000000
```

第 1 行应为 `200000000`，程序输出 `2000000000`。

建议：距离使用 `long long INF = 4e18`；先判断 `dis == INF` 再做势能还原，不要用 `>= 1e8` 裁剪。
>fixed

### 数论/线性基(gauss).cpp:34

问题：`vector<int>` 却按 `bit=64` 做 `num[j] >> i`，`i>=32` 时是 C++ 未定义行为；`main` 第 110 行直接 `basic b(a,64)`。

建议：改为 `long long` / `uint64_t`，或 `int` 版本只用 31/32 位；同步修正 `1ull<<cnt`、`rk/count` 的溢出边界。
>fixed

### 数论/乘法逆元.cpp:22

问题：快速幂用 `int` 做 `ans*a`、`a*a`，模数约 `1e9` 时溢出。

反例：`invMod1(1000000006,1000000007)` 输出 `483451163`，正确是 `1000000006`。

建议：乘法提升到 `long long` 或 `__int128`。

>仅思想 无需fixed

### 数论/FFT(快速傅里叶变换).cpp:66

问题：结果用 `x + 0.5` 四舍五入，负系数错误。

反例：两个常数多项式 `-1` 和 `1`，输出 `0`，应为 `-1`。

建议：使用 `llround(real/len)`，或明确模板只支持非负卷积结果。

>fixed

### 图论/欧拉回路(路径,Hierholzer法).cpp:53-56, 89-94

问题：不可编译。lambda 使用 C++23 显式对象参数 `this auto`，但函数体直接访问类成员 `del/mp/vis`，G++17/23 都报 `invalid use of non-static data member`。

建议：改成捕获 `this` 或普通递归 lambda：`auto dfs = [&](auto&& self, int u)->void { ... self(self, v); };`。

>fixed

### 图论/最近公共祖先（LCA）(欧拉序&dfs序(O(1))).cpp:26, 69

问题：同一文件重复定义 `class StLca`，不可编译。

建议：两个实现拆文件，或分别命名为 `EulerStLca` / `DfnStLca`。

>只是把两个合在一起写了 不fixed

### 计算几何/三角剖分.cpp:30-35

问题：文件不可编译。`pit CO` 在 `pit` 还是 incomplete type 时定义，`vec(pit a)` 也按值使用 incomplete type。

证据：`g++ -std=c++20 -fsyntax-only` 报 `aggregate 'pit CO' has incomplete type`；同时 `getS` 中还会报 `db was not declared`。

建议：先完整定义 `pit`，再定义全局 `CO`；或移除 `vec(pit)` 构造器，改成在 `pit` 定义后实现转换。

>fixed

### 数论/安全取模类.cpp:26

问题：单文件无法编译，`int64_t` 未包含 `<cstdint>`。

证据：`g++ -std=c++20 -fsyntax-only` 报 `error: 'int64_t' does not name a type`。

建议：补 `<cstdint>` 或改用已有的 `long long` typedef。
>fixed
### 数论/组合数预处理.cpp:26

问题：单文件无法编译，`int64_t` 未包含 `<cstdint>`。

证据：`g++ -std=c++20 -fsyntax-only` 报 `error: 'int64_t' does not name a type`。

建议：补 `<cstdint>` 或改用已有的 `long long` typedef。
>fixed

## High

### 图论/ShortestPath（dijkstra）.cpp:47, 62-64

问题：距离使用 `int`，权值和溢出。

证据：输入两条 `1500000000` 边，期望到 3 的距离为 `3000000000`，程序输出负数。

建议：距离、权值和、优先队列 key 全部改 `long long`，INF 取 `4e18`。
>仅思想 无需fixed

### 图论/最小生成树（prim）.cpp:47, 87-95

问题：MST 答案使用 `int`，大边权和溢出。

证据：两条 `2000000000` 边的 MST，期望 `4000000000`，程序输出 `-294967296`。
>仅思想 无需fixed

建议：边权、累计答案、优先队列 key 全部使用 `long long`。

### 图论/最小生成树（kruskal）.cpp:71-82

问题：MST 答案使用 `int`，大边权和溢出。

证据：两条 `2000000000` 边的 MST，期望 `4000000000`，程序输出 `-294967296`。

建议：边权和答案使用 `long long`。
>仅思想 无需fixed

### 图论/最小生成树（kruskal）.cpp:82, 98

问题：用 `-1` 同时表示“不连通”和合法 MST 权值，负权 MST 会被误报 `orz`。

反例：

```text
2 1
1 2 -1
```

合法答案 `-1`，程序输出 `orz`。

建议：返回 `{ok, ans}`，或单独记录 `cnt == n-1`。

>仅思想 无需fixed

### 图论/分层图.cpp:47, 59-70

问题：距离使用 `int`，多边路径会溢出。

证据：与 Dijkstra 同类，两条 `1500000000` 边即可让合法距离超过 `int`。

建议：距离、边权和、优先队列 key 全部改 `long long`。
>仅思想 无需fixed

### 图论/二分图染色.cpp:92, 113-120

问题：硬编码 `freopen("in.txt","r",stdin)`，正常 stdin 无法工作；二分还会访问 `Data[m]`。

证据：三角形三条边权均为 3 时，在临时 `in.txt` 运行输出 `0`，来自越界/零初始化边界。

建议：移除 `freopen`；二分答案值域改为 `[0,maxW]`；明确使用 `>` 还是 `>=`，避免 `Data[m]`。

>仅思想 无需fixed

### 图论/最大流(dinic).cpp:81-82

问题：`#define int long long` 后仍用 `INT_MAX` 作为单次增广上限，超过 `2^31` 的容量会靠多轮 BFS 慢慢推，容量大时复杂度爆炸。

证据：单边容量 `5000000000` 虽能算对，但会重复 BFS。

建议：使用 `const long long INF = 4e18`，并在同一 BFS 内循环 `while ((f = dfs(s, INF))) res += f`。
>fixed

### 数论/(ex)CRT((扩展)中国剩余定理)(旧版).cpp:126

问题：`main` 调的是互质版 `CRT()`，不是 `_exCRT()`。

反例：`x ≡ 2 (mod 4), x ≡ 4 (mod 6)` 输出 `20`，且不满足任一同余。

建议：示例入口改调 `_exCRT()`，或强约束输入模数两两互质。
>仅思想

### 数论/(ex)CRT((扩展)中国剩余定理)(旧版).cpp:88

问题：`accumulate(..., 1LL, ...)` 让乘积中间类型仍是 `long long`，即使文件 `#define int __int128` 也会溢出。

证据：大模数样例输出负数。

建议：初值使用 `(__int128)1`。
>fixed
### 数论/(ex)CRT((扩展)中国剩余定理)(新版).cpp:60

问题：`merge` 中 `ll nM = m1 / g * m2` 先溢出，再尝试第 120 行裁剪已经来不及。

反例：`m1=3037000500, m2=3037000501` 得到负模数。

建议：用 `i128` 计算 lcm，越界时返回显式 overflow 状态。
>注释部分已修复

### 数论/组合数预处理.cpp:153

问题：`preD()` 对 `d[1]`、`d[2]` 无条件写入，`Pre(0, m)` 或 `Pre(1, m)` 越界。

建议：初始化 `d[0]=1`，按 `n>=1` / `n>=2` 分支赋值。
>fixed

### 数论/组合数预处理.cpp:199

问题：错排函数 `D(n,m)` 返回 `d[n]*C(n,m)`，按注释“选 m 个做错排”应为 `C(n,m)*d[m]`。

证据：`D(3,2)` 当前为 `6`，正确为 `3`。

建议：改公式；`m=n` 的特例会自然成立。

>fixed

### 数论/NTT(快速数论变换).cpp:140

问题：`998244353` 只支持长度到 `2^23`，代码没有断言；超过后不存在对应单位根，会静默错误。

建议：`assert(len <= 1<<23)`，或切换多模/FFT 方案。

>fixed

### 数据结构/可删改堆.cpp:48-65, 78-90

问题：删除/弹出后清理的是 `ph[size]`，但应清理被删元素的插入编号 `ph[hp[size]]`；会留下已删除元素的有效位置，后续 `modify/remove` 可复活已删元素。

证据：临时样例 `push 30,20,10; pop(); modify(3,100)` 后 `top()` 变成 `100`，被弹出的第 3 个插入元素仍可修改。

建议：swap 后先记录 `removedId=hp[size]`，清 `ph[removedId]`；所有按插入编号操作检查是否仍有效。

>文件已删除 不fixed

### 数据结构/珂朵莉树(ODT).cpp:38-51

问题：`split(r+1)` 在 `r` 为最后位置时会插入非法区间 `[n,n-1]`；`pos` 越界/小于首区间也有 UB。

证据：临时样例 `a={1,2,3}; assign(0,2,7)` 后集合含 `[3,2]=3`。

建议：加入哨兵 `[n,n,?]` / `[n+1,n+1]`，或在 `pos>last_r` 时直接返回 `end()`；校验 `l,r`。

>fixed

### 数据结构/李超线段树.cpp:83-90

问题：当前 `cmp` 是 min 模式，但竖直线段 `x1==x2` 固定取 `max(y1,y2)`，会把最小值查询写成最大端点。

证据：插入 `(5,1)-(5,10)` 后 `ask(5)` 返回 `10`，min 应为 `1`。

建议：min 模式取 `min`，max 模式取 `max`，或按比较器选更优端点。

>fixed

### 数据结构/线段树二分.cpp:62-90, 159-166

问题：模板版 `upd/qry` 缺少无交/非法区间保护；外部传入越界或空区间可递归到叶子后继续访问子节点，甚至无限递归/错误 pushup。

证据：代码无 `if (x > r || y < l || x > y) return`，同目录 `jiangly线段树(改).cpp:62-86` 有保护。

建议：补齐无交 guard；wrapper 校验 `[l,r]` 在 `[1,n]` 且 `l<=r`。

>fixed

### 计算几何/三分.cpp:103

问题：`freopen("in.txt","r",stdin);` 未注释，评测/样例 stdin 会被忽略；若无 `in.txt`，`cin >> t` 失败后 `t` 未初始化，行为未定义。

建议：提交模板默认注释 `freopen`，或判断返回值并初始化输入变量。

>fixed

### 计算几何/向量.cpp:82-85

问题：`scross` 对共线但不相交的线段返回 true。

反例：`ab=(0,0)-(1,0)`，`cd=(2,0)-(3,0)`，四个 `cross` 都是 0，函数返回有交点。

建议：共线分支用投影区间/`onSeg` 判断；普通相交再用跨立实验。

>fixed

### 其他/整体二分.cpp:91-114

问题：答案域从 `[1,m]` 开始，漏掉 0 条边已经满足的查询。

反例：`n=4, m=1, edge 1-2, query 1 2 2`，0 条边时两个单点并集大小已为 2，但程序输出 1。

建议：整体二分范围改为 `[0,m]`，并处理 `mid=0` 时不加边。

>仅思想 不fixed

### 动态规划/dp笔记/数位dp(例题1,数位和).cpp:26

问题：使用 `int64_t` 但未包含 `<cstdint>`，当前 MinGW g++ 14.2 下不可编译。

证据：`error: 'int64_t' does not name a type`；加 `-include cstdint` 后可过语法检查。

建议：显式 `#include <cstdint>`。

>fixed

## Medium

### 图论/ShortestPath（Floyed）.cpp:16-31, 22

问题：不是可用 Floyd 模板：固定 `T=10`、随机造图、无输入；同时声明两个 `10000*10000 int` 矩阵，内存约 800MB。

建议：拆成真正模板和随机测试器，矩阵按 `n` 动态分配。

>仅思想 不fixed

### 图论/DSU on tree(树上启发式合并).cpp:33, 63, 70, 79

问题：颜色数组 `s(n+5)` 直接用 `c[i]` 下标，隐含 `1<=color<=n`；颜色值大于 n 会越界。

建议：写明前提，或先离散化颜色。

>仅思想 不fixed

### 图论/点分治.cpp:74-79, 105-107

问题：每个距离都遍历全部 `m` 个询问，实际复杂度约 `O(n log n * m)`，注释的 `O(nlogn)` 不成立。

建议：若多询问大，改用排序/哈希批处理，或标注适用规模。

>仅思想 不fixed

### 图论/虚树(带边权).cpp:121-133

问题：Needs verification。若关键点包含根 `1`，`dp(1)` 直接返回 `len[1][0]=1e18`，可能输出 INF；内部关键点直接返回父边权也强依赖“切掉关键点上方即可覆盖其子树”的题意。

建议：补充题意前提；若作为通用虚树 DP，关键点也应继续处理子节点，根特判为 0/不可切。

>仅思想 不fixed

### 数论/乘法逆元.cpp:42

问题：`invMod2` 不检查 `gcd(a,m)==1`，如 `2 mod 4` 会返回伪“逆元”。

建议：返回 `{ok, inv}`，或无逆元时抛出/返回哨兵。

>fixed

### 数论/(最小)原根.cpp:83

问题：`getr/getar` 直接访问 `ex[m]`、`phi[m]`，`main` 固定预处理到 `1e6+5`，查询更大 `n` 会越界。

建议：按最大查询构造 `Pre`，或入口断言 `m <= this->n`。

>fixed

### 数论/组合数预处理.cpp:136

问题：逆元递推隐含 `mod` 为质数且 `n < mod`；`n>=mod` 时阶乘逆元和 `inv[mod]` 都不成立。

建议：写明前提，或改 Lucas/分块阶乘等方案。

>fixed

### 数论/数论预处理.cpp:75

问题：逆元递推隐含 `mod` 为质数且 `n < mod`；`n>=mod` 时 `inv[mod]` 不成立。

建议：写明前提，或跳过不可逆项。

>fixed

### 数论/矩阵快速幂(新版).cpp:42

问题：Needs verification。Min/Max-plus 半环的 `mul` 直接 `a+b`，矩阵乘只跳过左侧 zero，右侧为 `INF` 时仍可能溢出。

建议：在 `Op::mul` 内处理 zero，或乘法循环同时跳过 `r[k][j]==Op::zero()`。

>fixed

### 数论/整除分块.cpp:34

问题：`b1/b2` 若 `s==0` 或 `e>n/m`，会出现 `n/l==0` 后再除以 0。

建议：强制 `s>=1`，并把终点 clamp 到 `min(e,n,m)`。

>fixed

### 数论/线性基(贪心法).cpp:31

问题：构造参数 `bit` 未被 `ins` 使用，固定循环 `63..0`；`bit<64` 会越界，且 signed `long long` 不适合完整 64 位异或空间。

建议：循环从 `bit-1` 到 `0`；值类型改 `uint64_t`。

>fixed

### 数论/FFT(快速傅里叶变换).cpp:54

问题：Needs verification。普通 `double FFT` 未做拆系数/CRT，较大系数或长度下整数卷积会精度丢失。

建议：标注安全范围，或使用 NTT/三模/拆系数。

>fixed

### 数据结构/线段树二分.cpp:244-251

问题：mex 示例无条件 `mdf(a[r]+1)`；若数组允许负数，负值会被写到 mex=0 的叶子，答案错误。

证据：输入 `n=2, a=[-1,1], query [1,2]` 输出 `2`，正确 mex 是 `0`。

建议：只处理 `0<=a[r]<=n` 的值；否则忽略。

>问题不存在 mex 输入默认非负 不fixed

### 数据结构/FHQTreap(无旋平衡树).cpp:144-151

问题：`range_add` 修改子树根的 `val/tag`，但未同步 `sum += add*sz`；同时任意负增量会破坏 BST 分割前提。

建议：同步更新 `sum`；文档写明 `range_add(lim, add)` 仅在不会跨过左半区最大值时合法，或重建/重新插入。

>fixed

### 数据结构/01tire.cpp:33-50, 59-76

问题：对有符号 `int` 做 `x>>31` 和 `1<<31`，涉及实现定义/未定义行为；负数或高位数据不安全。

建议：使用 `uint32_t` / `uint64_t` 和 `1u<<i`，明确值域。

>fixed

### 其他/动态bitset.cpp:41-49, 70-87, 101-118

问题：大小语义像 1-based `[1,n]`，但接口允许 `set(0)`；`operator&` 不 `sant()`，比较/计数依赖未屏蔽高位；`operator<` 是低块优先字典序，未必是数值序。

建议：明确 0-based 还是 1-based；所有构造/按位运算后统一 mask；越界 assert。

>fixed

### 数据结构/普通莫队.cpp:34-41

问题：BIT 的 `query(l,r)` 不是通用区间和，`l>1` 时会把 Fenwick 块左侧多算进去。

建议：实现 `pre()`，`query(l,r)=pre(r)-pre(l-1)`。

>仅思想 不fixed

### 数据结构/笛卡尔树(新版).cpp:51-60

问题：递归深度可达 `O(n)`，链状数据可能爆栈。

建议：大数据场景改迭代 DFS，或确认 OJ 栈足够。

>OJ 栈空间足够 不fixed

### 数据结构/主席树(例2).cpp:97-105

问题：递归深度可达 `O(n)`，链状数据可能爆栈。

建议：大数据场景改迭代 DFS，或确认 OJ 栈足够。

>OJ 栈空间足够 不fixed

### 数据结构/回撤并查集&可持久化并查集(离线).cpp:93-102

问题：递归深度可达 `O(n)`，链状数据可能爆栈。

建议：大数据场景改迭代 DFS，或确认 OJ 栈足够。

>OJ 栈空间足够 不fixed

### 计算几何/旋转卡壳.cpp:136-162

问题：退化点集未处理。`Andrew` 对单点会返回空 hull；`rot` 对空 hull 输出初始 `1e14`。

证据：输入 1 个点输出面积 `100000000000000.00000`。

建议：显式处理 `n==0/1/2`、全共线、重复点；若题目保证非退化，也要写清前提。

>已加前提注释 不fixed

### 计算几何/凸包.cpp:54-75

问题：退化点集未处理。`Andrew` 对单点会返回空 hull。

建议：显式处理 `n==0/1/2`、全共线、重复点；若题目保证非退化，也要写清前提。

>已加前提注释 不fixed

### 计算几何/三角剖分.cpp:103-124

问题：Needs verification after compile fix。`getDP2` 在垂足不在线段上时把 `d` 改成端点距离，但仍用原垂足 `e` 计算圆交点，可能得到错误交点。

反例方向：圆心原点、`R=1`，线段 `(0.5,0)-(2,0)` 实际交点是 `(1,0)`，代码会按 `h=sqrt(1-0.5^2)` 从垂足构造。

建议：距离判定和直线交点计算分离；交点先按无限直线算，再用 `onSeg` 过滤。

>fixed

### 字符串/广义后缀自动机(GSAM).cpp:54-65, 97

问题：Needs verification。若复用 `sz` 表示出现次数，会漏计复用已有转移的 Trie 节点路径。

证据：Python 等价对拍，词集 `["aab","ab"]` 中子串 `"a"` 实际出现 3 次，聚合 `sz` 得到 2。

建议：为每个 Trie 节点记录对应 SAM 状态并逐点累加访问次数，再沿 parent tree 汇总；若只用 `dist_sub()`，小范围对拍未发现问题。

>fixed

### 字符串/exKMP.cpp:21, 65

问题：`#define int long long` 使 `z/p` 两个大数组按 64 位存，`exkmp(2e7+5)` 固定分配约 320MB，仅数组就可能 MLE。

建议：数组下标/LCP 长度用 32 位 `int`，只让最终 xor/乘法用 `long long`；按实际输入长度分配。

>仅思想 不fixed

### 动态规划/dp笔记/数位dp(例题1,数位和).cpp:120, 132

问题：`dp` 第二维写死 `18*9+5`，但 `#define int long long` 后输入可能到 19 位；另 `this auto&&` 是 C++23 写法。

建议：按 `max_digits*9` 分配；若要 C++17/20 通用，用普通递归函数或 `std::function`。

>仅思想 不fixed

### 字符串/随机底数固定模数单hash.cpp:41-45

问题：`calc/get` 隐含字符串 1-based；普通 0-based `std::string` 传入会漏首字符并读 `s[n]`。

建议：统一接口为 0-based，或强制注释/断言要求 `s = " " + s`。

>已有 1base 注释 不fixed

### 字符串/随机底数固定模数双hash.cpp:43-55

问题：`calc/get` 隐含字符串 1-based；普通 0-based `std::string` 传入会漏首字符并读 `s[n]`。

建议：统一接口为 0-based，或强制注释/断言要求 `s = " " + s`。

>已有 1base 注释 不fixed

## Low

### 数论/(ex)CRT((扩展)中国剩余定理)(新版).cpp:79

问题：文件名是 exCRT 模板，但 `main` 是一段题解树查询逻辑，还用了 C++23 风格 `this auto&& dfs`。

建议：把 `_EX_CRT` 单独成模板文件，题解入口移走；目标编译器较旧时需验证。

>仅思想 不fixed

### 数论/FMT&FWT(快速莫比乌斯&沃尔什变换).cpp:83

问题：`conv` 接收非常量引用，会 resize 并变换后再还原，调用者输入长度会被永久扩到 2 的幂。

建议：改为按值传参，或明确副作用。

>已加副作用注释 不fixed

### 数论/筛法求积性函数.cpp:23

问题：当前只是积性函数筛法骨架，实际只返回 primes，`f(p)` / `f(m)` 都是注释占位。

建议：文件名或注释标明“模板骨架”，避免误当完整实现。

>已加骨架注释 不fixed

### 数论/(最小)原根.cpp:68

问题：`assert(n>=4)` 让小范围预处理不可用，`getr` 理论上靠数学保证返回但代码没有兜底 return。

建议：支持小 `n`，或补明确断言/返回。

>fixed

### 数据结构/st表.cpp:22-37

问题：文件名未标 1-based，但实现要求 `a[1..n]`；0-based 调用会越界。

建议：改名/注释明确，或提供 0-based 构造。

>fixed

### 数据结构/主席树.cpp:75-77

问题：`k` 未校验；`k<1` 或 `k>r-l+1` 会返回无意义值。

建议：加 assert 或返回错误哨兵。

>仅思想 不fixed

### 数据结构/st表*.cpp、数据结构/jiangly线段树(info).cpp

问题：使用 GCC 扩展 `__lg/std::__lg`，生成器裁剪或 MSVC 环境不可移植。

建议：用 `bit_width` / 预处理 log 表，或注明 GCC/Clang 依赖。

>GCC 竞赛环境 不fixed

### 其他/动态bitset.cpp:26

问题：使用 `uint64_t` 但未直接 include `<cstdint>`，当前靠传递 include 编译。

建议：补显式依赖，避免生成器裁剪后丢依赖。

>fixed 已拆为 0base/1base 且补 cstdint

### 数据结构/手写堆.cpp:67-70

问题：示例输出循环 `while(h.size>1)` 漏掉最后一个元素；`top/pop` 对空堆无保护。

建议：示例改为 `while(h.size)`，库接口加断言。

>文件已删除 不fixed

### 字符串/kmp.cpp:16-24

问题：`prefix_init_f("")` 会写 `dp[0]` 越界；`kmp()` 自己避开了空 pattern，但前缀函数单独复用不安全。

建议：空串直接返回空 vector。

>pattern 不会为空 不fixed

### 字符串/AC自动机(可拓展版).cpp:55-61

问题：`query` 会把 `cnt[j]` 置 `-1`，语义是“出现过的模式数”，不是总出现次数，且不可重复查询。

证据：模式 `a, aa`，文本 `aaa` 返回 2，而总出现次数应为 5。

建议：注释明确 destructive/distinct 语义；总次数版用 fail 树统计或 DP 版。

>已加语义注释 不fixed

## Board 1 生成/复制可用性问题

这些问题不一定说明源算法逻辑错，但会影响最终板子里复制代码是否能直接用。

### 板子/scripts/generate_typst.py:30, 68

问题：`#include <...>` 全部当 boilerplate 剥掉。

证据：`BOILERPLATE_PATTERNS` 匹配所有 include，前导扫描持续跳过。

建议：不要无差别剥 include；至少保留非首页模板已有头，或生成每块“依赖头”提示。

>通过 cpp模板 头文件 LCM 覆盖 不改生成器

### 板子/scripts/generate_typst.py:129

问题：usage 提取时删除单独 `{` / `}`，导致许多“用法示例”括号明显失衡，复制不可用。

建议：usage 保留块括号；只删 main 外壳，不删内部控制流/作用域括号。

>fixed

### 生成后缺头：数据结构/FHQTreap(无旋平衡树).cpp:26

问题：使用 `chrono::steady_clock` 初始化 `mt19937`，但 `<chrono>` 被剥。

建议：保留 `<chrono>` 或把 `<chrono>` 加入首页模板。

>fixed cpp模板 已含 chrono/random

### 生成后缺头：字符串/防hack的umap,gp_hash_table.cpp:23

问题：PBDS 头和 `<chrono>` 被剥，生成块拼通用模板后 `__gnu_pbds` / `chrono` 不可用。

建议：该块必须自带 `#include <ext/pb_ds/assoc_container.hpp>` 和 `<chrono>`。

>fixed cpp模板 已含 PBDS/chrono

### 生成后缺头：字符串/随机底数固定模数单hash.cpp:37

问题：`mt19937_64 + chrono` 依赖 `<chrono>`，生成后缺头。

建议：保留 `<chrono>`。

>fixed cpp模板 已含 chrono/random

### 生成后缺头：字符串/随机底数固定模数双hash.cpp:35

问题：`mt19937 + chrono` 依赖 `<chrono>`，生成后缺头。

建议：保留 `<chrono>`。

>fixed cpp模板 已含 chrono/random

### 生成后缺头：数论/(最小)原根.cpp:68

问题：`assert(n>=4)` 依赖 `<cassert>` / `<assert.h>`，生成后缺头。

建议：保留 `<cassert>` / `<assert.h>`。

>fixed cpp模板 已含 cassert/assert.h

### 生成后缺头：数论/FFT(快速傅里叶变换).cpp:24

问题：`complex<double>` 依赖 `<complex>`，生成后缺头。

建议：保留 `<complex>`。

>fixed cpp模板 已含 complex

### 生成后缺头：数论/矩阵快速幂(新版).cpp:102

问题：`assert` 缺 `<cassert>`；`numeric_limits` 依赖 `<limits>`，当前靠传递头不稳。

建议：保留 `<cassert>`，并显式加 `<limits>`。

>fixed cpp模板 已含 cassert/limits

### 生成后缺头：其他/动态bitset.cpp:26

问题：使用 `uint64_t`，首页模板没有 `<cstdint>`。

建议：显式保留 `<cstdint>`。

>fixed cpp模板 已含 cstdint

### 生成后缺头：数论/NTT(快速数论变换).cpp:82

问题：使用 `uint64_t/uint32_t`，首页模板没有 `<cstdint>`。

建议：显式保留 `<cstdint>`。

>fixed cpp模板 已含 cstdint

### 生成后缺头：图论/二分图最大匹配.cpp:84

问题：使用 `INT_MAX`，首页模板无 `<climits>`。

建议：显式加 `<climits>`。

>fixed cpp模板 已含 climits

### 生成后缺头：图论/最大流(dinic).cpp:82

问题：使用 `INT_MAX`，首页模板无 `<climits>`。

建议：显式加 `<climits>`。

>fixed cpp模板 已含 climits

### 生成后仅剩 usage：图论/DSU on tree(树上启发式合并).cpp

问题：对应标题后无算法 code block，只有“用法示例”。

建议：作为完整例题/整程序保留，或抽出真正模板函数。

>完整例题/思想展示 不fixed

### 生成后仅剩 usage：图论/找环(topsort).cpp

问题：对应标题后无算法 code block，只有“用法示例”。

建议：作为完整例题/整程序保留，或抽出真正模板函数。

>完整例题/思想展示 不fixed

### 生成后仅剩 usage：图论/点分治.cpp

问题：对应标题后无算法 code block，只有“用法示例”。

建议：作为完整例题/整程序保留，或抽出真正模板函数。

>完整例题/思想展示 不fixed

### 生成后仅剩 usage：其他/离散化.cpp

问题：对应标题后无算法 code block，只有“用法示例”。

建议：作为完整例题/整程序保留，或抽出真正模板函数。

>完整例题/思想展示 不fixed

### C++23 依赖：算法块本体

问题：`图论/欧拉回路(路径,Hierholzer法).cpp` 算法块本体使用 `this auto&&` lambda，复制后依赖 C++23，C++17/20 不可用。

建议：改成普通递归 lambda，或明确标注“需 C++23”。

>已在 cpp模板 注明 C++23 this auto&& 依赖 不fixed

### C++23 依赖：usage 示例

问题：以下用法示例含 `this auto&&` 或 `this auto&`，在 C++17/20 不可用：

- `数据结构/主席树(例2).cpp`
- `数据结构/回撤并查集&可持久化并查集(离线).cpp`
- `图论/DSU on tree(树上启发式合并).cpp`
- `动态规划/dp笔记/数位dp(例题1,数位和).cpp`
- `数论/(ex)CRT((扩展)中国剩余定理)(新版).cpp`
- `其他/整体二分.cpp`

建议：改成普通递归 lambda，或明确标注“需 C++23”。

>已在 cpp模板 注明 C++23 this auto&& 依赖 不fixed

### 类名冲突

问题：复制多个模板进同一首页模板会重定义同名类/结构。已发现冲突名包括：

- `AC`：两份 AC 自动机
- `BIT`：两份 BIT + 莫队
- `DSU`：并查集 + 两份 Boruvka
- `FHQTreap`：应用版 + 通用版
- `HJTree`：两份主席树
- `Heap`：两份堆
- `Info/Tag`：jiangly 线段树 / 线段树二分
- `Mcmf`：三份费用流
- `Pre`：原根 / 数论预处理 / 组合数
- `REDSU`：回撤并查集 / 线段树分治 / 整体二分
- `SAM`：高低注释
- `SHASH`：单/双 hash
- `SMC`：多个模数类
- `SegTree`：多个线段树
- `Trie`：tiretree / GSAM
- `basic`：两份线性基
- `maxflow`：二分图匹配 / 最大流
- `st`：两份 ST 表
- `pit/vec`：多份几何
- `vtree`：两份虚树

建议：每块包 `namespace`，或按用途重命名；至少在板子中标注“不可与同名模板同贴”。

>竞赛按需复制模板，同名冲突无所谓 不fixed

## 附录：笔记事实问题

这些不是 Board 1 算法模板本体，但会影响 Board 2/3 笔记可信度。

### High

- `数论/数论笔记部分/数论笔记(不定方程与同余方程组).md:88`：线性同余方程写成“若 `gcd(a,m)!=1` 则无解”，错误；正确条件是 `gcd(a,m) | b`。

>fixed

- `数论/数论笔记部分/数论笔记(不定方程与同余方程组).md:108`：`ax+mk=b` 的通解写成 `x=x1+mt, k=k1-at`，只在 `gcd(a,m)=1` 时完整；一般应除以 `d=gcd(a,m)`。

>fixed

- `数论/数论笔记部分/数论笔记(sosdp&fmt&fwt).md:324`：AND/OR 卷积的 FWT 标签反了。

>fixed

- `数论/数论笔记部分/数学笔记(矩阵半环).md:70`：max-plus 矩阵乘法中“无边设为 0”错误，应为 `-∞`。

>fixed

- `数论/数论笔记部分/数论笔记(线性逆元).md:5`：线性递推求逆元缺少 `n < p` 条件。

>fixed

- `数据结构/数据结构笔记/数据结构trick.md:7`：二阶差分等差加最后一项符号错误，应为 `+= k+(r-l)*d`。

>fixed

- `其他/Trick/杂项相关trick.md:4,8-9`：中位数判定第二个条件比较对象写错，应和 `>m` 比较。

>fixed

- `其他/Trick/杂项相关trick.md:53`：曼哈顿/切比雪夫变换少了尺度说明。

>fixed

- `图论/图论笔记/图论笔记(几类特殊图).md:21,23,41`：可比图和 Hasse 图混淆。

>fixed

- `图论/图论笔记/图论笔记(几类特殊图).md:83,91-93`、`其他/Trick/杂项相关trick.md:59`：DAG 最小路径覆盖和偏序最小链划分混用。

>fixed

- `字符串/笔记/一些比较神秘的hash手法.md:19,25,45,51`：前缀 hash 只能直接判连续区间，不能计任意子序列。

>fixed

- `其他/Trick/杂项相关trick.md:63`：Hall 定理表述缺条件；该条件保证覆盖一侧，不一定是完美匹配。

>fixed

- `图论/Trick/图相关trick.md:25-31`：欧拉图连通条件过强，应忽略零度点。

>fixed


### Medium / Low

- `数论/数论笔记部分/数论笔记(不定方程与同余方程组).md:192`：CRT 逆元定义式写错，应为 `M_i * M_i^{-1} ≡ 1 (mod m_i)`。

>fixed

- `数论/数论笔记部分/数论笔记(不定方程与同余方程组).md:306`：exCRT 推导中 `k_2` 的符号错误。

>fixed

- `数论/数论笔记部分/数论笔记(阶,原根与ntt).md:79`：NTT 模数条件不完整，需质数且 `n | p-1`。

>fixed

- `数论/数论笔记部分/数论笔记(阶,原根与ntt).md:99`：NTT 求和引理缺少 `n ∤ k` 条件。

>fixed

- `数论/数论笔记部分/数论笔记(狄利克雷卷积与莫比乌斯反演 2).md:79`：`μ * id = φ` 证明链中误写 `ε * μ = φ`。

>fixed

- `数论/数论笔记部分/数论笔记(炫酷反演魔术).md:389`：二项式反演形式二的子集反演证明方向写反。

>fixed

- `数论/数论笔记部分/数论笔记(炫酷反演魔术).md:557`：Min-Max 容斥期望公式把空集也纳入求和。

>fixed

- `图论/图论笔记/图论笔记(几类特殊图).md:97`：区间图定义写成“包含关系”，标准区间图是相交连边。

>fixed

- `博弈论/nim游戏 SG函数.md:41-44`：终态说明中“非零堆推出异或非零”对多堆 Nim 不成立。

>fixed

- `其他/Trick/杂项相关trick.md:78`：置换环定义错误，把封闭置换块当成一个 cycle。

>fixed

- `数论/数论笔记部分/数论笔记(筛法).md:29`：欧拉函数“与次数无关”表述误导。

>fixed

- `数论/数论笔记部分/数论笔记(生成函数).md:283`：`3` 个 A、`1` 个 B 的排列示例第一项应为 `AAAB`。

>fixed

- `数论/数论笔记部分/数论小结论.md:7`：`gcd(x,y)=x xor y -> gcd(x,y)=y-x` 缺少顺序条件，应为 `|x-y|` 或注明 `x<=y`。

>fixed
