#### 1.1 欧拉定理
**欧拉定理**：设 $a\in\mathbb{Z},\,m\in\mathbb{N}^*$，若 $\gcd(a,m)=1$，则 $a^{\varphi(m)}\equiv 1\pmod m$。
$\varphi(m)$ 是欧拉函数，表示小于 $m$ 且与 $m$ 互质的正整数个数。
>证明：考虑模 $m$ 的简化剩余系，即
 $$
 R=\{r_1,r_2,\cdots,r_{\varphi(m)}\}
 $$
这个集合包含了所有在 $1$ 到 $m$ 之间且与 $m$ 互质的整数。显然，这个集合里有 $\varphi(m)$ 个元素。
现在，我们把集合 $R$ 里的每一个元素都乘上 $a$，注意$\gcd(a, m) = 1$，得到一个新的集合：$$S = \{ar_1, ar_2, \dots, ar_{\varphi(m)}\}$$
注意到 1.$S$ 中的元素都与 $m$ 互质; 2.$S$ 中的元素在模 $m$ 下两两不同(可以用反证法证明)。
既然 $S$ 中有 $\varphi(m)$ 个与 $m$ 互质且模 $m$ 两两不同的数，那么它们在模 $m$ 意义下一定与 $R$ 里的数一一对应。
那么：
$$
\prod_{i=1}^{\varphi(m)} (ar_i) \equiv \prod_{i=1}^{\varphi(m)} r_i \pmod m
$$
$$
a^{\varphi(m)} \prod_{i=1}^{\varphi(m)} r_i \equiv \prod_{i=1}^{\varphi(m)} r_i \pmod m
$$
$$
a^{\varphi(m)} \equiv 1 \pmod m
$$

这边用到的一个厉害的性质是，模 $m$ 的简化剩余系，他在模 $m$ 意义下对乘法（和 $m$ 互质的数 $a$）是封闭的。

#### 1.2.1 阶，原根

**阶**：若满足同余式 $a^n \equiv 1\pmod m$ 的最小正整数解 $n$ 存在，则称 $n$ 为 $a$ 模 $m$ 的阶，记作 $\delta_m(a)$。
**原根**：设 $a\in\mathbb{Z}$，$m\in\mathbb{N}^*$，若 $\delta_m(a)=\varphi(m)$，则称 $a$ 为模 $m$ 的一个原根。
>不难注意， $\delta_m(a)$ 是一个最小循环节，而$\delta_m(a)$ 一定是 $\varphi(m)$ 的一个约数。

#### 1.2.2 原根数量
如何求解 $m$ 的原根数量？
因为 $a$ 是 $m$ 的一个原根，所以 $a^1,a^2,a^3,\cdots,a^{\varphi(m)}$ 在取模 $m$ 意义下均不相同（否则就与 $\delta_m(a)$ 的最短循环节定义相矛盾了）。
设$t$是某个与$\varphi(m)$互质的正整数。
容易得到$t,2t,\cdots,\varphi(m)\cdot t$在模$\varphi(m)$意义下均不相同。
>实际上这个结论是，完全剩余系在模 $m$ 意义下对乘法（乘一个与$m$互质的数）是封闭的。（证明和上面是一致的）

根据欧拉定理，那么 $a^t,a^{2t},\cdots,a^{\varphi(m)\cdot t}$ 在取模 $m$ 的意义下也就两两不同。
所以 $a^t$ 也是模 $m$ 的一个原根，因为 $t$ 是一个跟 $\varphi(m)$ 互质的数，也就是说满足这样条件的数有 $\varphi(\varphi(m))$ 个。
>由于 $a^t$ 的不断乘方（即 $(a^t)^1, (a^t)^2, \dots$）也能遍历所有互质同余类，这完全符合原根的定义。所以，$a^t$ 就是 $m$ 的一个新的原根。

#### 1.2.3 原根存在定理
**原根存在定理**：
一个数 $m$ 存在原根，当且仅当$$m=2,4,p^\alpha,2p^\alpha$$
其中 $p$ 为奇素数，$\alpha\in\mathbb{N}^*$。
>太难了，不想证。
>然后我们就能O(m^(1/4) *logm^2)求出m的原根了。

#### 2. NTT
##### 2.1 性质
考虑单位根和原根所构造的单位根的相似性，有如下四点性质:
1. 周期性
FFT（复数域）： $\omega_n^n = e^{2\pi i} = 1$。转了一圈回到 $1$。
NTT（模 $p$ 域）： 考虑NTT的模数是$p = c \cdot 2^k + 1$的形式，考虑$g$是模$p$的原根，根据欧拉定理和上述定义，我们知道 $g^{p-1} \equiv 1 \pmod p$。
考虑$n = 2^t $,令$g_n \equiv g^{\frac{p-1}{n}} \pmod p$（单位根），那么$(g_n)^n \equiv \left(g^{\frac{p-1}{n}}\right)^n \equiv g^{p-1} \equiv 1 \pmod p$
>对比FFT，可以发现此处的$2^t$正好是块长。
2. 互异性
FFT（复数域）： $\omega_n^0, \omega_n^1, \dots, \omega_n^{n-1}$ 在复平面上均匀分布，互不相等。这样代入多项式才能得到 $n$ 个独立的点值。
NTT（模 $p$ 域）： $g_n^0, g_n^1, \dots, g_n^{n-1}$ 模 $p$ 下互不相同，此处由原根的性质保证。
3. 折半引理
FFT（复数域）： $\omega_{2n}^{2} = \omega_n$。这是 FFT 能把偶数项和奇数项拆开递归的核心。
NTT（模 $p$ 域）：注意：
$$
g_{2n}^2 \equiv \left(g^{\frac{p-1}{2n}}\right)^2 \equiv g^{\frac{p-1}{n}} \equiv g_n \pmod p
$$
4. 求和引理
FFT（复数域）： 对于任意不为 $0$ 的整数 $k$（且 $k$ 不是 $n$ 的倍数），$\sum_{j=0}^{n-1} (\omega_n^k)^j = 0$。这是把点值转回系数的关键。
NTT（模 $p$ 域）： 这是一个等比数列求和：$$\sum_{j=0}^{n-1} (g_n^k)^j \equiv \frac{1 - (g_n^k)^n}{1 - g_n^k} \pmod p$$
注意分子为$0$，分母不为$0$，这个式子依旧$=0$。
##### 2.2 递归过程
考虑重新手推一下NTT的递归过程。
假设我们有一个长度为 $n$（$n$ 是 $2$ 的幂次）的多项式：$$A(x) = a_0 + a_1x + a_2x^2 + \dots + a_{n-1}x^{n-1}$$我们要把 $n$ 个点 $x = g_n^0, g_n^1, \dots, g_n^{n-1}$ 代入进去求值。
设：
偶数项：$A_0(x) = a_0 + a_2x + a_4x^2 + \dots + a_{n-2}x^{\frac{n}{2}-1}$
奇数项：$A_1(x) = a_1 + a_3x + a_5x^2 + \dots + a_{n-1}x^{\frac{n}{2}-1}$
于是：
$$
A(x) = A_0(x^2) + x \cdot A_1(x^2)
$$
带入$x = g_n^k$ :
$$
A(g_n^k) = A_0((g_n^k)^2) + g_n^k \cdot A_1((g_n^k)^2)
$$
注意:
$$
(g_n^k)^2 \equiv \left(g^{k \cdot \frac{p-1}{n}}\right)^2 \equiv g^{k \cdot \frac{p-1}{n/2}} \equiv g_{n/2}^k \pmod p
$$
于是:
$$
A(g_n^k) \equiv A_0(g_{n/2}^k) + g_n^k \cdot A_1(g_{n/2}^k) \pmod p
$$
考虑后半段$g_n^{k + n/2}$（此时 $k < n/2$），代入：
$$
A(g_n^{k + n/2}) = A_0((g_n^{k + n/2})^2) + g_n^{k + n/2} \cdot A_1((g_n^{k + n/2})^2)
$$

注意：
$$
(g_n^{k + n/2})^2 \equiv g_n^{2k + n} \equiv g_n^{2k} \cdot g_n^n \equiv g_n^{2k} \cdot 1 \equiv (g_n^k)^2 \equiv g_{n/2}^k \pmod p
$$

再次注意：
$$
g_n^{k + n/2} \equiv g_n^k \cdot g_n^{n/2} \pmod p
$$
其中 $g_n^{n/2}$：
$$
g_n^{n/2} \equiv \left(g^{\frac{p-1}{n}}\right)^{\frac{n}{2}} \equiv g^{\frac{p-1}{2}} \pmod p
$$
于是：
$$
g_n^{k + n/2} \equiv -g_n^k \pmod p
$$

最终：
前半段：$$A(g_n^k) \equiv A_0(g_{n/2}^k) + g_n^k \cdot A_1(g_{n/2}^k) \pmod p$$
后半段：$$A(g_n^{k + n/2}) \equiv A_0(g_{n/2}^k) - g_n^k \cdot A_1(g_{n/2}^k) \pmod p$$
>把递归写成递推跟FFT是一致的，不再赘述
##### 2.3 INTT
假设我们原本的多项式系数是 $a_0, a_1, \dots, a_{n-1}$。
经过正向 NTT 后，我们得到了 $n$ 个点值 $y_0, y_1, \dots, y_{n-1}$。
正向 NTT 的方程是这样的：$$y_k = \sum_{j=0}^{n-1} a_j (g_n^k)^j \pmod p$$
构造：
$$
c_i = \sum_{k=0}^{n-1} y_k (g_n^{-1})^{ki} \pmod p
$$
代入：
$$
c_i = \sum_{k=0}^{n-1} \left( \sum_{j=0}^{n-1} a_j g_n^{kj} \right) g_n^{-ki} \pmod p
$$
交换求和顺序：
$$
c_i = \sum_{j=0}^{n-1} a_j \left( \sum_{k=0}^{n-1} (g_n^{j-i})^k \right) \pmod p
$$
对括号里面的东西用求和引理，当仅当$j = i$等于 $n$，否则等于 $0$。
于是：
$$
c_i \equiv a_i \cdot n \pmod p
$$
那么：
$$
a_i \equiv c_i \cdot n^{-1} \pmod p
$$
于是对做完点乘的点值做一次反向 NTT ，然后乘上 $n$ 的逆元即可。