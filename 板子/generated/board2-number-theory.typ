= 参考图表

== 表

#image("../../表.jpg", width: 100%)

== 图

#image("../../图.png", width: 100%)

= 基础与工具

== 数论笔记(筛法)

===== 欧拉函数的定义
<欧拉函数的定义>
$1 tilde.op n$ 中与 $n$ 互质的数的个数称为欧拉函数，记为 $phi ( n )$ \
#strong[例];：$phi ( 1 ) = 1 \, med phi ( 2 ) = 1 \, med phi ( 3 ) = 2 \, med phi ( 4 ) = 2 \, med phi ( 5 ) = 4$

===== 欧拉函数的性质
<欧拉函数的性质>
+ 若 $p$ 是质数，则 $phi ( p ) = p - 1$ \
+ 若 $p$ 是质数，则 $phi ( p^k ) = ( p - 1 ) p^(k - 1)$ \
+ #strong[积性函数];：若 $gcd ( m \, n ) = 1$，则 $phi ( m n ) = phi ( m ) phi ( n )$

===== 欧拉函数的计算公式
<欧拉函数的计算公式>
由唯一分解定理 $n = product_(i = 1)^s p_i^(alpha_i) = p_1^(alpha_1) p_2^(alpha_2) dots.h.c p_s^(alpha_s)$， \
$ phi ( n ) & = product_(i = 1)^s phi ( p_i^(alpha_i) )\
 & = product_(i = 1)^s p_i^(alpha_i - 1) ( p_i - 1 )\
 & = product_(i = 1)^s p_i^(alpha_i) (1 - 1 / p_i)\
 & = (product_(i = 1)^s p_i^(alpha_i)) times (product_(i = 1)^s (1 - 1 / p_i))\
 & = n times product_(i = 1)^s frac(p_i - 1, p_i)\
 & = n times frac(p_1 - 1, p_1) times frac(p_2 - 1, p_2) times dots.h.c times frac(p_s - 1, p_s) $ \
\> 欧拉函数仅由 $n$ 和质因子决定，与次数无关。 \
#strong[例];：$phi ( 12 ) = 12 times frac(2 - 1, 2) times frac(3 - 1, 3) = 4$

==== 筛法求欧拉函数
<筛法求欧拉函数>
#quote(block: true)[
若 $i$ 是质数，$phi \[ i \] = i - 1$。
]

#strong[在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的];。 \
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = p_j times i$ 筛掉。

===== 分两种情况计算：
<分两种情况计算>
+ #strong[若 $i$ 能被 $p_j$ 整除];（即 $i equiv 0 med (mod p_j)$），则 $i$ 包含了 $m$ 的所有质因子： \
  $ phi ( m ) & = m times product_(k = 1)^s frac(p_k - 1, p_k)\
   & = p_j times i times product_(k = 1)^s frac(p_k - 1, p_k)\
   & = p_j times phi ( i ) $ \
  #strong[例];：$phi ( 12 ) = phi ( 2 times 6 ) = 2 times phi ( 6 )$

+ #strong[若 $i$ 不能被 $p_j$ 整除];（即 $gcd ( i \, p_j ) = 1$），则 $i$ 和 $p_j$ 互质： \
  $ phi ( m ) & = phi ( p_j times i )\
   & = phi ( p_j ) times phi ( i )\
   & = ( p_j - 1 ) times phi ( i ) $ \
  #strong[例];：$phi ( 75 ) = phi ( 3 times 25 ) = ( 3 - 1 ) times phi ( 25 )$

```cpp
vector<int> euler()
{
    vector<int> phi(n+1);
    phi[1]=1;
    vector<int> primes;
    vector<bool>v(n+1,0);
    for(int i=2;i<=n;i++)
    {
        if(!v[i])primes.push_back(i),phi[i]=i-1;
        for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
        {
            int m=primes[j]*i;
            v[m]=1;
            if(i%primes[j]==0)
            {
                phi[m]=phi[i]*primes[j];
                break;
            }
            else phi[m]=phi[i]*(primes[j]-1);
        }
    }
}
```

=== 筛法求约数个数
<筛法求约数个数>
==== 问题
<问题>
给定整数 $n$ ($n lt.eq 10^6$)，输出 $1 tilde.op n$ 中每个数的约数个数。

==== 约数个数定理
<约数个数定理>
若正整数 $n$ 有质因数分解 $n = product_(i = 1)^s p_i^(alpha_i)$，则约数个数为： $ d ( n ) = product_(i = 1)^s ( alpha_i + 1 ) $

===== 证明
<证明>
- 对每个质因子 $p_i^(alpha_i)$，其约数可取 $p_i^0 \, p_i^1 \, dots.h.c \, p_i^(alpha_i)$ 共 $( alpha_i + 1 )$ 种选择
- 根据乘法原理，总约数个数为各质因子选择数的乘积： $ d ( n ) = ( alpha_1 + 1 ) times ( alpha_2 + 1 ) times dots.h.c times ( alpha_s + 1 ) $

#strong[筛法求约数个数] \
记a\[i\]为i的最小质因子的次数，d\[i\]为i的约数个数。 若 $i$ 是质数， \
$ a \[ i \] = 1 \, quad d \[ i \] = 2 $ 在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。 \
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = p_j times i$ 筛掉。

#block[
#set enum(numbering: "(1)", start: 1)
+ 若 $i$ 能被 $p_j$ 整除，则 $p_j$ 一定是 $i$ 的最小质因子。 \
  $ a \[ m \] = a \[ i \] + 1 ; $ $ d \[ i \] = ( a \[ i \] + 1 ) times dots.h.c \, quad d \[ m \] = ( a \[ m \] + 1 ) times dots.h.c $
]

于是 $ d \[ m \] = d \[ i \] times frac(a \[ m \] + 1, a \[ i \] + 1) $ (2) 若 $i$ 不能被 $p_j$ 整除，则 $i$ 不包含质因子 $p_j$。 \
$ a \[ m \] = 1 \, quad d \[ m \] = d \[ i \] times ( 1 + 1 ) $

```cpp
//O(n)求1-n的约数个数
vector<int> d()
{
    vector<int> a(n+1),d(n+1);
    vector<int> primes;
    vector<bool>v(n+1,0);
    for(int i=2;i<=n;i++)
    {
        if(!v[i])
        {
            primes.push_back(i);
            a[i]=1,d[i]=2;
        }
        for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
        {
            int m=primes[j]*i;
            v[m]=1;
            if(i%primes[j]==0)
            {
                a[m]=a[i]+1;
                d[m]=d[i]/(a[i]+1)*(a[m]+1);
                break;
            }
            else
            {
                a[m]=1;
                d[m]=d[i]*2;
            }
        }
    }
}
```

=== 约数和定理
<约数和定理>
若 $n = product_(i = 1)^s p_i^(alpha_i)$，则 $f ( n ) = product_(i = 1)^s sum_(j = 0)^(alpha_i) p_i^j$

#strong[证明：] \
$p_i^(alpha_i)$ 的约数有 $p_i^0 \, p_i^1 \, dots.h.c \, p_i^(alpha_i)$ 共 $( alpha_i + 1 )$ 个，其约数和为 $sum_(j = 0)^(alpha_i) p_i^j$。

根据乘法原理， \
$ f ( n ) = product_(i = 1)^s sum_(j = 0)^(alpha_i) p_i^j $

#strong[例：] \
$ 12 = 2^2 times 3^1 \, $ $ f ( 12 ) = ( 1 + 2 + 4 ) times ( 1 + 3 ) = 7 times 4 = 28 $

#line(length: 100%, stroke: 0.5pt + luma(180))

=== 筛法求约数和
<筛法求约数和>
记g\[i\]为i的最小质因子的幂和 1 + p^1 +p^2 + … + p^k，f\[i\]为i的约数和。 若 $i$ 是质数， \
$ g \[ i \] = f \[ i \] = i + 1 $

在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = i times p_j$ 筛掉。

#block[
#set enum(numbering: "(1)", start: 1)
+ 若 $i$ 能被 $p_j$ 整除，则 $p_j$ 一定也是 $i$ 的最小质因子 \
  $ g \[ i \] = p_j^0 + p_j^1 + dots.h.c + p_j^(alpha_j) \, quad g \[ m \] = p_j^0 + p_j^1 + dots.h.c + p_j^(alpha_j + 1) $ $ f \[ i \] = g \[ i \] times dots.h.c \, quad f \[ m \] = g \[ m \] times dots.h.c $ 于是 $ f \[ m \] = f \[ i \] times frac(g \[ m \], g \[ i \]) $

+ 若 $i$ 不能被 $p_j$ 整除，则 $i$ 不包含质因子 $p_j$。 \
  $ g \[ m \] = 1 + p_j $ $ f \[ m \] = g \[ m \] times f \[ i \] $
]

```cpp
//O(n)求1-n的约数和
vector<int> sumd()
{
    vector<int> g(n+1),f(n+1);
    vector<int> primes;
    vector<bool>v(n+1,0);
    g[1]=f[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!v[i])
        {
            primes.push_back(i);
            f[i]=g[i]=i+1;
        }
        for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
        {
            int m=primes[j]*i;
            v[m]=1;
            if(i%primes[j]==0)
            {
                g[m]=g[i]*primes[j]+1;
                f[m]=f[i]*g[m]/g[i];
                break;
            }
            else
            {
                g[m]=primes[j]+1;
                f[m]=f[i]*g[m];
            }
        }
    }
}
```

=== 唯一分解定理
<唯一分解定理>
$ n = product_(i = 1)^s p_i^(alpha_i) = p_1^(alpha_1) p_2^(alpha_2) dots.h.c p_s^(alpha_s) $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 莫比乌斯函数定义
<莫比乌斯函数定义>
莫比乌斯函数记作 $mu ( n )$，它是一个经典的数论函数，定义如下：

- $mu ( 1 ) = 1$

- 如果 $n$ 含有平方因子（即存在某个质数 $p$，使得 $p^2 divides n$），则 $mu ( n ) = 0$

- 如果 $n$ 是 $k$ 个#strong[互不相同的质数];的乘积（即 $n = p_1 p_2 dots.h.c p_k$），则：

$ mu ( n ) = ( - 1 )^k $

#line(length: 100%, stroke: 0.5pt + luma(180))

=== 筛法求莫比乌斯函数
<筛法求莫比乌斯函数>
若 $i$ 是质数，$mu \[ i \] = - 1$。 \
在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。 \
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = i times p_j$ 筛掉。

#block[
#set enum(numbering: "(1)", start: 1)
+ 若 $i$ 能被 $p_j$ 整除，则 $i$ 也包含质因子 $p_j$。 \
  $ mu \[ m \] = 0 $

+ 若 $i$ 不能被 $p_j$ 整除，则 $m$ 比 $i$ 多一个不同的质因子 $p_j$ \
]

- 若 $mu \[ i \] = - 1$, 则 $mu \[ m \] = 1$ \
- 若 $mu \[ i \] = 1$, 则 $mu \[ m \] = - 1$ \
- 若 $mu \[ i \] = 0$, 则 $mu \[ m \] = 0$ \
  综上，$mu \[ m \] = - mu \[ i \]$。


== 数论笔记(线性逆元)

#strong[O(n)求阶乘和阶乘逆元]

==== 📘 推导目标
<推导目标>
给定质数 $p$，我们希望在线性时间内计算 $1$ 到 $n$ 的所有数在模 $p$ 意义下的乘法逆元，即： \
$ upright("求 ") forall 1 lt.eq i lt.eq n \, quad upright("使得 ") r_i dot.op i equiv 1 med (mod p) upright(" 的 ") r_i $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 🧮 推导公式
<推导公式>
我们设 $r_i = i^(- 1) #h(0em) mod med p$，有： - $r_1 = 1$ - 对于 $i > 1$，我们可以利用如下递推式求出 $r_i$：

$ r_i = ( p - ⌊p / i⌋ ) dot.op r_(p #h(0em) mod med i) #h(0em) mod med p $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== ✏️ 证明过程
<证明过程>
考虑：

$ p = i dot.op ⌊p / i⌋ + ( p #h(0em) mod med i ) arrow.r.double p #h(0em) mod med i = p - i dot.op ⌊p / i⌋ $

两边模p:

$ i dot.op (⌊p / i⌋) equiv - ( p #h(0em) mod med i ) med (mod p) arrow.r.double i dot.op (⌊p / i⌋) dot.op ( p #h(0em) mod med i )^(- 1) equiv - 1 med (mod p) $

两边都乘上 $- 1$：

$ i dot.op (- ⌊p / i⌋) dot.op ( p #h(0em) mod med i )^(- 1) equiv 1 med (mod p) $

于是我们得出：

$ #box(stroke: black, inset: 3pt, [$ upright("inv") \[ i \] equiv - (⌊p / i⌋) dot.op upright("inv") \[ p #h(0em) mod med i \] med (mod p) $]) $

再化简成#strong[无负数形式];：

$ #box(stroke: black, inset: 3pt, [$ upright("inv") \[ i \] = \( p - p \/ i \) dot.op upright("inv") \[ p % i \] #h(0em) mod med p $]) $

这就是我们要用的递推式！

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 🧪 C++ 实现示例
<c-实现示例>
```cpp
void preC()
{
    inv[1]=1;
    for(int i=2;i<=n;i++)
    {
        inv[i]=(mod-mod/i)*inv[mod%i]%mod;
    }
    fac[0]=invfac[0]=1;
    for(int i=1;i<=n;i++)
    {
        fac[i]=fac[i-1]*i%mod;
        invfac[i]=invfac[i-1]*inv[i]%mod;
    }
}
```

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 🧠 时间复杂度
<时间复杂度>
- 时间：$cal(O) ( n )$
- 空间：$cal(O) ( n )$
- 要求 $p$ 是质数（否则不存在乘法逆元）


== 数论笔记(不定方程与同余方程组)

#strong[前置芝士] exgcd求解不定方程 ax+by=gcd(a,b) /线性同余方程 ax≡b(mod m) 的解 #strong[exgcd求解不定方程 ax+by=gcd(a,b)] 设 $ a x_1 + b y_1 = g c d ( a \, b ) $

$ b x_2 + ( a % b ) y_2 = g c d ( b \, a % b ) $

由欧几里得定理可得 $ g c d ( a \, b ) = g c d ( b \, a % b ) $

于是 $ a x_1 + b y_1 = b x_2 + ( a % b ) y_2 $

$ a x_1 + b y_1 = b x_2 + ( a - floor.l a \/ b floor.r \* b ) y_2 $

整理 $ a x_1 + b y_1 = a y_2 + b ( x_2 - floor.l a \/ b floor.r \* y_2 ) $

于是 $ x_1 = y_2 $

$ y_1 = x_2 - floor.l a \/ b floor.r \* y_2 $ ax+by=gcd(a,b)即可通过最初的x,y求解 于是我们可以通过递归求解

```cpp
int exgcd(int a,int b,int &x,int &y)//扩展欧几里得
{
    if(b==0)
    {
        //从x=1,y=0开始向上
        x=1;y=0;
        //ax+by=gcd(a,b)->ax=0->x=1,y=0
        return a;
    }
    //先求解gcd(a,b)
    int d=exgcd(b,a%b,x,y),t=x;
    x=y;y=t-a/b*y;
    return d;
}
```

#strong[exgcd求解线性同余方程 ax≡b(mod m) 的解] $ a x equiv b med (mod m) $

可写成 $ a x + m k = b $

于是我们先求解不定方程 $ a x + m k = g c d ( a \, m ) $

若gcd(a,m)!=1则无解，否则得到解 $ x = x_0 $

$ k = k_0 $ 于是我们得到原方程的解为 $ x_1 = x_0 \* b \/ g c d ( a \, m ) $

$ k_1 = k_0 \* b \/ g c d ( a \, m ) $ 方程的任意解(对任意整数t成立)为 $ x = x_1 + m t $

$ k = k_1 - a t $ 求最小的正整数解 $ x = ( x_1 med mod med t + t ) med mod med t $ 其中 $ t = m \/ g c d ( a \, m ) $ 为何？ 因为上述操作等价于 $ a \/ d \* x equiv b \/ d med (mod m \/ d) $ 把a/d 放到右边 会发现x是 模m/d的等价类

要用exgcd求解逆元的话，需要保证gcd(a,m)=1 代入exgcd(a,m,x,y)中,对x值域变换即可 其实就是 $ a x equiv 1 med (mod m) $

可写成 $ a x + m k = g c d ( a \, m ) = 1 $ 罢了

#strong[中国剩余定理] 求解同余方程组 $ {x equiv a_1 med (mod m_1)\
x equiv a_2 med (mod m_2)\
. . .\
x equiv a_k med (mod m_k) $ 其中 $ m_1 \, m_2 \, . . . \, m_k 两 两 互 质 $ 过程: 求 $ M = m_1 \* m_2 \* . . . \* m_k $ 对每个m\_i求 $ M_i = M \/ m_i $

$ M_i^(- 1) equiv 1 med (mod m_i) $

$ c_i = M_i^(- 1) \* M_i $ 于是 $ x = sum_(i = 1)^k a_i \* c_i med (mod M) $

很显然的证明，对任意一个方程组： $ x equiv sum_(i = 1)^k a_i \* c_i med (mod m_i) $

$ x equiv a_i \* M_i \* M_i^(- 1) med (mod m_i) $

$ x equiv a_i med (mod m_i) \* ( M_i \* M_i^(- 1) med (mod m_i) ) $

按定义

$ x equiv a_i med (mod m_i) $ 代码：

```cpp
int CRT()
{
    int mul=accumulate(m.begin(),m.end(),1LL,
    [](int a,int b){return a*b;}),ans=0;
    for(int i=0;i<n;i++)
    {
        int M=mul/m[i],b,y;
        exgcd(M,m[i],b,y);//求M的逆元
        ans=(ans+nums[i]*M%mul*b%mul+mul)%mul;
    }
    return (ans%mul+mul)%mul;
}
```

#strong[扩展中国剩余定理] 求解同余方程组 $ {x equiv a_1 med (mod m_1)\
x equiv a_2 med (mod m_2)\
. . .\
x equiv a_k med (mod m_k) $ 其中 $ m_1 \, m_2 \, . . . \, m_k 不 两 两 互 质 $ 过程: 考虑合并两个同余方程 $ {x equiv a_1 med (mod m_1)\
x equiv a_2 med (mod m_2) $ 可写成不定方程 $ {x = a_1 + k_1 \* m_1\
x = a_2 + k_2 \* m_2 $ 消去x $ a_1 + k_1 \* m_1 = a_2 + k_2 \* m_2 $ 于是我们得到了一个不定方程 $ k_1 \* m_1 + - k_2 \* m_2 = a_2 - a_1 $ 可通过exgcd求解 $ K_1 \* m_1 + - K_2 \* m_2 = g c d ( m_1 \, m_2 ) $ 于是 $ k_1 = frac(a_2 - a_1, g c d ( m_1 \, m_2 )) \* K_1 $

$ k_2 = frac(a_1 - a_2, g c d ( m_1 \, m_2 )) \* K_2 $ 得到x的一个解 $ x_0 = a_1 + k_1 \* m_1 = a_1 + frac(a_2 - a_1, g c d ( m_1 \, m_2 )) \* K_1 \* m_1 $ 窝们很显然可以构造x的通解 $ x = x_0 + t \* l c m ( m_1 \, m_2 ) $ 于是进行形式转化 $ x equiv x_0 med (mod l c m ( m_1 \, m_2) ) $ 于是我们得到了两个同余方程的合并

```cpp
int _exCRT()
{
    int M=m[0],ans=nums[0];
    //M：合并后的模数，ans:合并后的余数
    for(int i=1;i<n;i++)
    {
        //当前方程：
        //x≡nums[i] (mod \ m[i])
        //x≡ans (mod \ M)
        //不定方程 ax+by=gcd(a,b)
        int a=M,b=m[i];
        int c=((nums[i]-ans)%b+b)%b;
        int x,y;
        int gcd=exgcd(a,b,x,y);
        int bg=b/gcd;
        if(c%gcd!=0) return -1;//判断有无解
        x=(x%bg+bg)%bg;//对x值域变换变成正数
        x=(x*c/gcd%bg+bg)%bg;//对x值域变换
        ans+=x*M;
        M*=bg;//更新M=lcm(M,m[i])=m[i]*M/gcd(M,m[i])
        ans=(ans%M+M)%M;
    }
    return (ans%M+M)%M;
}
```


== 数论小结论

$upright("gcd")$ 的重要恒等式： $upright("gcd") ( x_1 \, x_2 \, dots.h \, x_n ) = upright("gcd") ( x_1 \, x_2 - x_1 \, x_3 - x_2 \, dots.h \, x_n - x_(n - 1) )$。

对于一个前缀 $upright("gcd")$ 序列 $g_i = upright("gcd") ( a_1 \, dots.h \, a_i )$，这个序列 $g_1 \, g_2 \, dots.h \, g_n$ 的不同取值最多只有 $O ( log V )$ 个。 因为下降是$l o g$的

gcd(x,y)=x xor y-\>gcd(x,y)=y-x

gcd(x,y)=x+y -\> gcd(x,y)=max(x,y)。

处理逆元不存在的方法总列： 考虑前后缀积处理 考虑处理乘0的个数 阶乘考虑列让德公式 考虑卢卡斯定理 考虑特殊性质，如k是9的倍数，要求k/9 mod mod 那么可以先求k%9mod然后/9然后%mod


= 卷积与反演

== 数论笔记(狄利克雷卷积与莫比乌斯反演 1)



== 数论笔记(狄利克雷卷积与莫比乌斯反演 2)

=== 狄利克雷卷积
<狄利克雷卷积>
==== 定义
<定义>
设 $f ( n )$, $g ( n )$ 是两个积性函数，其狄利克雷卷积定义为： $ ( f \* g ) ( n ) = sum_(d \| n) f ( d ) g (n / d) = sum_(d \| n) f (n / d) g ( d ) $

#quote(block: true)[
注意跟狄利克雷生成函数形式上的相似性 读作：$f$ 卷 $g$
]

===== 示例
<示例>
$ ( f \* g ) ( 4 ) = f ( 1 ) g ( 4 ) + f ( 2 ) g ( 2 ) + f ( 4 ) g ( 1 ) $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 运算规律
<运算规律>
+ #strong[交换律];：$f \* g = g \* f$ \
+ #strong[结合律];：$( f \* g ) \* h = f \* ( g \* h )$ \
+ #strong[分配律];：$( f + g ) \* h = f \* h + g \* h$

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 常用函数
<常用函数>
#figure(
  align(center)[#table(
    columns: (36%, 40%, 24%),
    align: (auto,auto,auto,),
    table.header([函数名称], [符号表示], [定义],),
    table.hline(),
    [元函数], [$epsilon.alt ( n )$], [$\[ n = 1 \] = cases(delim: "{", 1 & n = 1, 0 & n > 1)$],
    [常数函数], [$1 ( n )$], [$1$],
    [恒等函数], [$i d ( n )$], [$n$],
    [欧拉函数], [$phi ( n )$], [\<n且与n互质的数的个数],
    [莫比乌斯函数], [$mu ( n )$], [$cases(delim: "{", 1 & n = 1, ( - 1 )^k & n = p_1 p_2 . . . p_k, 0 & n 含 平 方 因 子)$],
  )]
  , kind: table
  )

#line(length: 100%, stroke: 0.5pt + luma(180))

#quote(block: true)[
注意符号的读法 $mu$ 读作“缪”，$phi$ 读作“phi”，$epsilon.alt$ 读作“一穆西隆”
]

==== 常用卷积关系
<常用卷积关系>
简记形式 1. $sum_(d \| n) mu ( d ) = \[ n = 1 \] quad arrow.l.r.double quad mu \* 1 = epsilon.alt$ \
\2. $sum_(d \| n) phi ( d ) = n quad arrow.l.r.double quad phi \* 1 = i d$ \
\3. $sum_(d \| n) mu ( d ) n / d = phi ( n ) quad arrow.l.r.double quad mu \* i d = phi$ \
\4. $f \* epsilon.alt = f$ \
\5. $f \* 1 eq.not f$

#line(length: 100%, stroke: 0.5pt + luma(180))

#quote(block: true)[
注意莫比乌斯函数是常数函数的逆元
]

==== 证明
<证明>
===== 1. $mu \* 1 = epsilon.alt$
<mu-1-epsilon>
$ ( mu \* 1 ) ( n ) = sum_(d \| n) mu ( d ) dot.op 1 (n / d) = sum_(d \| n) mu ( d ) = \[ n = 1 \] = epsilon.alt ( n ) $

===== 2. $phi \* 1 = i d$
<varphi-1-id>
$ ( phi \* 1 ) ( n ) = sum_(d \| n) phi ( d ) dot.op 1 (n / d) = sum_(d \| n) phi ( d ) = n = i d ( n ) $

===== 3. $mu \* i d = phi$
<mu-id-varphi>
$ ( mu \* i d ) ( n ) = sum_(d \| n) mu ( d ) dot.op i d (n / d) = sum_(d \| n) mu ( d ) dot.op n / d = phi ( n ) $

$ mu \* i d = mu \* phi \* 1 = ( phi \* 1 \* mu ) = epsilon.alt \* mu = phi $

===== 4. $f \* epsilon.alt = f$
<f-epsilon-f>
$ ( f \* epsilon.alt ) ( n ) = sum_(d \| n) f ( d ) dot.op epsilon.alt (n / d) = sum_(d \| n) f ( d ) dot.op [n / d = 1] = f ( n ) $

===== 5. $f \* 1 eq.not f$
<f-1-neq-f>
$ ( f \* 1 ) ( n ) = sum_(d \| n) f ( d ) dot.op 1 (n / d) = sum_(d \| n) f ( d ) eq.not f ( n ) $

=== 莫比乌斯反演
<莫比乌斯反演>
其实就是一下几个式子(条件式变成和式) $ sum_(d \| n) phi ( d ) = n $

$ sum_(d \| n) mu ( d ) = \[ n = 1 \] = cases(delim: "{", 1 & n = 1, 0 & n > 1) $

把$n$换成$g c d ( a \, b )$

$ sum_(d \| g c d ( a \, b )) mu ( d ) = \[ g c d ( a \, b ) = 1 \] cases(delim: "{", 1 & g c d ( a \, b ) = 1, 0 & g c d ( a \, b ) > 1) $

=== 莫比乌斯变换
<莫比乌斯变换>
==== 基本公式
<基本公式>
设 $f ( n )$, $g ( n )$ 均为积性函数，则： $ f ( n ) = sum_(d \| n) g ( d ) arrow.l.r.double g ( n ) = sum_(d \| n) mu ( d ) f (n / d) $

即 $ f = g \* 1 arrow.l.r.double g = mu \* f $

- $f ( n )$ 称为 $g ( n )$ 的莫比乌斯变换
- $g ( n )$ 称为 $f ( n )$ 的莫比乌斯逆变换

#line(length: 100%, stroke: 0.5pt + luma(180))

#quote(block: true)[
注意对于一些函数 f(n)，如果很难直接求出它的值，而容易求出其倍数和或约数和 g(n)，那么可以通过莫比乌斯反演简化运算，求得 f(n) 的值。
]

==== 证明方法一（卷积形式）
<证明方法一卷积形式>
===== 正向推导
<正向推导>
$ f & = g \* 1\
mu \* f & = mu \* ( g \* 1 )\
 & = g \* ( mu \* 1 )\
 & = g \* epsilon.alt\
 & = g $

===== 逆向推导
<逆向推导>
$ g & = mu \* f\
g \* 1 & = ( mu \* f ) \* 1\
 & = f \* ( mu \* 1 )\
 & = f \* epsilon.alt\
 & = f $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 证明方法二（双重求和）
<证明方法二双重求和>
$ sum_(d \| n) mu ( d ) f (n / d) & = sum_(d \| n) mu ( d ) sum_(k \| n / d) g ( k )\
 & = sum_(d \| n) sum_(k \| n / d) mu ( d ) g ( k )\
 & = sum_(k \| n) sum_(d \| n / k) mu ( d ) g ( k )\
 & = sum_(k \| n) g ( k ) (sum_(d \| n / k) mu ( d ))\
 & = sum_(k \| n) g ( k ) dot.op epsilon.alt (n / k)\
 & = g ( n ) $


== 数论笔记(炫酷反演魔术)

#quote(block: true)[
从广义莫比乌斯反演的角度思考各类反演魔术
]

此笔记主要总结之前的二项式反演/莫比乌斯反演 然后补充斯特林反演/mix-max容斥/容斥的相关技术

===== 一切的基石：广义莫比乌斯反演
<一切的基石广义莫比乌斯反演>
我们在数论中熟悉的 $sum_(d \| n) mu ( d ) = \[ n = 1 \]$ 其实只是冰山一角。所有的反演公式本质上都是在一个特定的偏序集( $D A G$ )上求解线性方程组。

====== 1.1 定义
<定义>
设 $P$ 是一个局部有限的偏序集。

Zeta 函数 (前缀和) $zeta ( x \, y )$： 表示 $x$ 是否小于等于 $y$。

$ zeta ( x \, y ) = cases(delim: "{", 1 & x lt.eq y, 0 & upright("otherwise")) $

#quote(block: true)[
$zeta ( x \, y ) = 1$ 等价于DAG上存在从 $x$ 到 $y$ 的路径。
]

我们现在知道

$ g ( y ) = sum_(x lt.eq y) zeta ( x \, y ) f ( x ) $

我们现在要求解

$ f ( y ) = sum_(x lt.eq y) mu ( x \, y ) g ( x ) $

于是我们有这个式子

Möbius 函数 (差分系数) $mu ( x \, y )$：

$ mu ( x \, y ) = cases(delim: "{", 1 & x = y, - sum_(x lt.eq z < y) mu ( x \, z ) & x < y, 0 & upright("otherwise")) $

或者说

$ sum_(y prec.curly.eq z prec.curly.eq x) mu ( z \, x ) = \[ x = y \] = delta ( x \, y ) $

#quote(block: true)[
你想求 $x$ 到 $y$ 的莫比乌斯函数值？ 你就把从 $x$ 出发，到 $y$ 之前所有中间节点 $z$ 的 $mu ( x \, z )$ 加起来，取个负号，就是 $mu ( x \, y )$。
]

怎么推导呢？

定义两个双变量函数 $A \, B$ 的#strong[狄利克雷卷积];（Dirichlet Convolution）为：

$ ( A \* B ) ( x \, y ) = sum_(x lt.eq z lt.eq y) A ( x \, z ) B ( z \, y ) $

这本质上就是#strong[矩阵乘法];。如果你把 $A ( x \, z )$ 看作矩阵 $A$ 的第 $x$ 行第 $z$ 列元素，把 $B ( z \, y )$ 看作矩阵 $B$ 的第 $z$ 行第 $y$ 列元素，上述公式就是 $( A B )_(x y)$。

#strong[推导单位元 $delta$]

我们想要求解 $f ( y )$，这需要逆运算。在做逆运算之前，必须先知道在这个乘法规则下，什么东西相当于数字 $1$（保持原值不变）。

设单位元函数为 $delta ( x \, y )$。 对于任意函数 $A$，必须满足 $A \* delta = A$。 即： $ sum_(x lt.eq z lt.eq y) A ( x \, z ) delta ( z \, y ) = A ( x \, y ) $

观察这个等式： 1. #strong[当 $x = y$ 时];： 式子变为 $A ( x \, x ) delta ( x \, x ) = A ( x \, x )$。 为了对任意 $A$ 成立，必须有 #strong[$delta ( x \, x ) = 1$];。

#block[
#set enum(numbering: "1.", start: 2)
+ #strong[当 $x < y$ 时];： 式子展开为 $A ( x \, x ) delta ( x \, y ) + dots.h + A ( x \, y ) delta ( y \, y ) = A ( x \, y )$。 我们在上一步已经确定了 $delta ( y \, y ) = 1$，所以式子最后那一项 $A ( x \, y ) delta ( y \, y )$ 已经等于等号右边的 $A ( x \, y )$ 了。 这意味着，前面所有的项加起来必须等于 0： $ A ( x \, x ) delta ( x \, y ) + A ( x \, x' ) delta ( x' \, y ) + dots.h = 0 $ 为了让它对任意 $A$ 都恒成立（无论 $A$ 取什么值），必须要求所有系数 #strong[$delta ( z \, y ) = 0$];（其中 $z < y$）。
]

#strong[结论 1];：要在这种卷积下保持不变，必须使用函数： $ delta ( x \, y ) = \[ x = y \] = cases(delim: "{", 1 & x = y, 0 & x eq.not y) $

#strong[形式化已知条件]

已知条件是 $g ( y ) = sum_(x lt.eq y) f ( x )$。 引入常函数 $zeta ( x \, y ) = 1$ （当 $x lt.eq y$ 时）。 原式可以写成卷积形式（视 $f \, g$ 为向量，$A \, B$ 为矩阵）： $ g ( y ) = sum_(x lt.eq y) f ( x ) zeta ( x \, y ) $ 在代数层面上，这等价于： $ g = f \* zeta $

#strong[推导 $mu$]

我们的目标是求 $f$。 既然 $g = f \* zeta$，我们在两边同乘一个 $zeta$ 的逆元（记作 $mu$）： $ g \* mu = ( f \* zeta ) \* mu $ 利用结合律： $ g \* mu = f \* ( zeta \* mu ) $ 为了消掉 $zeta$，我们要求 $mu$ 满足： $ zeta \* mu = delta $ 这样就有： $ g \* mu = f \* delta = f $ 即 #strong[$f ( y ) = sum_(x lt.eq y) g ( x ) mu ( x \, y )$];。

现在的问题核心变成了：#strong[如何找到这个 $mu$，使得 $zeta \* mu = delta$？]

根据 $zeta \* mu = delta$ 的定义，展开卷积公式： $ sum_(x lt.eq z lt.eq y) zeta ( x \, z ) mu ( z \, y ) = delta ( x \, y ) $

因为对于 $x lt.eq z$，$zeta ( x \, z )$ 恒等于 1，所以式子简化为： $ sum_(x lt.eq z lt.eq y) mu ( z \, y ) = delta ( x \, y ) $

这也是 $mu$ 定义里是 $mu ( x \, z )$ 还是 $mu ( z \, y )$ 的区别。由于左逆元等于右逆元（因为我们定义的卷积的形式），我们通常习惯计算 $mu \* zeta = delta$，即： $ sum_(x lt.eq z lt.eq y) mu ( x \, z ) zeta ( z \, y ) = delta ( x \, y ) $ 简化后（因为 $zeta ( z \, y ) = 1$）： $ sum_(x lt.eq z lt.eq y) mu ( x \, z ) = delta ( x \, y ) $

我们对区间 $\[ x \, y \]$ 的大小进行分类讨论：

+ #strong[当 $x = y$ 时（区间长度为1）：] $ mu ( x \, x ) = delta ( x \, x ) $ #strong[$mu ( x \, x ) = 1$]

+ #strong[当 $x < y$ 时（区间长度大于1）：] $ sum_(x lt.eq z lt.eq y) mu ( x \, z ) = delta ( x \, y ) $ 因为 $x eq.not y$，所以 $delta ( x \, y ) = 0$。 $ sum_(x lt.eq z lt.eq y) mu ( x \, z ) = 0 $ 我们将末项（$z = y$ 的项）分离出来： $ (sum_(x lt.eq z < y) mu ( x \, z )) + mu ( x \, y ) = 0 $ 移项得到 $mu ( x \, y )$ 的递推式： #strong[$ mu ( x \, y ) = - sum_(x lt.eq z < y) mu ( x \, z ) $] $Q . E . D$

====== 1.2 从矩阵视角看待这个反演
<从矩阵视角看待这个反演>
所有的反演公式，都可以抽象为两个数列（或函数） $f$ 和 $g$ 之间的线性关系： $ g ( y ) = sum_(x prec.curly.eq y) zeta ( x \, y ) f ( x ) $ 这写成矩阵形式就是： $ upright(bold(g)) = upright(bold(Z)) times upright(bold(f)) $其中 $upright(bold(Z))$ 是一个变换矩阵。 所谓的“反演”，就是求逆矩阵 $upright(bold(M)) = upright(bold(Z))^(- 1)$：$ upright(bold(f)) = upright(bold(Z))^(- 1) times upright(bold(g)) = upright(bold(M)) times upright(bold(g)) $ 对应的形式就是 $ f ( x ) = sum_(y prec.curly.eq x) mu ( y \, x ) g ( y ) $

====== 1.3. 通用反演定理
<通用反演定理>
于是我们得到了这个很好看的式子

若有两个定义在偏序集 $P$ 上的函数 $f \, g$ 满足“和”的关系：

$ g ( x ) = sum_(y lt.eq x) zeta ( y \, x ) f ( y ) $

则可以通过 $mu$ 函数反解出 $f$：

$ f ( x ) = sum_(y lt.eq x) mu ( y \, x ) g ( y ) $

====== 1.4. 这暗示我们啥东西？
<这暗示我们啥东西>
我们通过定义一个矩阵乘法状物的卷积，把所有在偏序集上的反演公式都统一了起来。

其中 $zeta \* mu = delta$ 是一个核心方程，$mu$ 是 $zeta$ 的逆元。

===== 容斥原理与子集/超集反演
<容斥原理与子集超集反演>
====== 2.1 容斥原理及其补集形式
<容斥原理及其补集形式>
有一个集合的集合 $A = { S_i }$ ，那么：

$ lr(|union.big_(i = 1)^n S_i|) = sum_(i = 1)^n ( - 1 )^(i - 1) sum_({ a_k } \, a_k < a_(k + 1)) lr(|sect.big_(j = 1)^i S_(a_j)|) $

#quote(block: true)[
这是显然的，要证也可以通过二项式定理证明
]

容斥原理的补集形式

$ lr(|sect.big_(i = 1)^n S_i|) = \| U \| - lr(|union.big_(i = 1)^n overline(S_i)|) $

#quote(block: true)[
其实画个venn图就能看出来
]

====== 2.2 子集/超集反演
<子集超集反演>
$ f ( S ) = sum_(T subset.eq S) g ( T ) arrow.l.r.double g ( S ) = sum_(T subset.eq S) ( - 1 )^(\| S \| - \| T \|) f ( T ) $

$ f ( S ) = sum_(S subset.eq T) g ( T ) arrow.l.r.double g ( S ) = sum_(S subset.eq T) ( - 1 )^(\| T \| - \| S \|) f ( T ) $

证明： #strong[引理] $mu$ 具有积性，具体来说，若偏序集 $P = P_1 times P_2$，则 $mu_P ( ( x_1 \, x_2 ) \, ( y_1 \, y_2 ) ) = mu_(P_1) ( x_1 \, y_1 ) dot.op mu_(P_2) ( x_2 \, y_2 )$。

证：$mu$ 函数的唯一定义特征是： 对于任意区间 $\[ S t a r t \, E n d \]$，所有子元素的 $mu$ 值之和必须等于1 （当 $S t a r t = E n d$ 时） 值之和必须等于 0 （当 $S t a r t < E n d$ 时） 只要能证明 $mu_P ( x ) dot.op mu_Q ( y )$ 满足这个特征，那它就是 $mu_(P times Q)$。

设 $S = ( x_1 \, y_1 )$ 是起点，$E = ( x_2 \, y_2 )$ 是终点。我们需要计算区间 $\[ S \, E \]$ 里所有元素的“分量 $mu$ 乘积”之和：$ upright("Sum") = sum_(S lt.eq ( u \, v ) lt.eq E) mu_P ( x_1 \, u ) dot.op mu_Q ( y_1 \, v ) $

因为积偏序集中，$u$ 和 $v$ 的选取是独立的（$x_1 lt.eq u lt.eq x_2$ 且 $y_1 lt.eq v lt.eq y_2$），我们可以把双重求和拆成两个括号相乘：$ upright("Sum") = (sum_(x_1 lt.eq u lt.eq x_2) mu_P ( x_1 \, u )) times (sum_(y_1 lt.eq v lt.eq y_2) mu_Q ( y_1 \, v )) $

根据 $mu$ 的定义，括号里的求和结果只能是 $1$ 或 $0$： 左括号 $= delta ( x_1 \, x_2 )$ （只有 $x_1 = x_2$ 时是 1，否则是 0） 右括号 $= delta ( y_1 \, y_2 )$ （只有 $y_1 = y_2$ 时是 1，否则是 0）

情况 A：起点 = 终点即 $x_1 = x_2$ 且 $y_1 = y_2$。此时 $upright("Sum") = 1 times 1 = 1$。 $arrow.r.double.long$ 满足定义！ 情况 B：起点 \< 终点这意味着 $x$ 和 $y$ 中至少有一个不等（例如 $x_1 < x_2$）。那么对应的那个括号求和就是 $0$。$ upright("Sum") = 0 times ( dots.h ) = 0 quad upright("或") quad ( dots.h ) times 0 = 0 $ \$$arrow.r.double.long$ 满足定义！

$Q . E . D$

布尔格（所有子集构成的格）同构于 $k$ 个 链（Chain） $L_2 = { 0 \, 1 }$ 的直积。 即：$cal(P) ( { 1 \, dots.h \, k } ) tilde.equiv L_2 times L_2 times dots.h times L_2$。（每个元素选或不选对应 0 或 1）。 Möbius 函数具有积性：若偏序集 $P = P_1 times P_2$，则 $mu_P ( ( x_1 \, x_2 ) \, ( y_1 \, y_2 ) ) = mu_(P_1) ( x_1 \, y_1 ) dot.op mu_(P_2) ( x_2 \, y_2 )$。 对于单个链 $0 lt.eq 1$：$mu ( 0 \, 0 ) = 1$ $mu ( 0 \, 1 ) = - 1$ （因为 $mu ( 0 \, 0 ) + mu ( 0 \, 1 ) = 0$） 对于集合 $A subset.eq B$，设 $\| B \| - \| A \| = k$。这相当于在 $k$ 个维度上从 $0$ 变到了 $1$，而在其他维度上保持不变。因此：$ mu ( A \, B ) = underbrace(( - 1 ) times ( - 1 ) times dots.h times ( - 1 ), k upright(" 次")) = ( - 1 )^k $

#quote(block: true)[
超集反演同理，证明留作作业
]

===== 二项式反演 (Binomial Inversion)
<二项式反演-binomial-inversion>
二项式反演是集合包含偏序集（即布尔格）上的反演在计数问题中的投影。它处理的是“恰好满足 $k$ 个性质”与“至少满足 $k$ 个性质”之间的转换。

====== 3.1 形式一（对称形式）
<形式一对称形式>
这是最对称的一种形式，常用于证明其他等式。

$ f ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i ) $

====== 3.2 形式二（常用形式/多退少补）
<形式二常用形式多退少补>
通常 $f ( i )$ 表示“钦定选 $i$ 个满足性质，其余随意的方案数”，$g ( i )$ 表示“恰好 $i$ 个满足性质的方案数”。

$ f ( n ) = sum_(i = 0)^n binom(n, i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i ) $

+ 恰好 → 至多型

设 g(m) 代表 #strong[恰好] 满足条件 (= m) 的方案数（即答案） \
设 f(m) 代表 #strong[至多] 满足条件 (≤ m) 的方案数

先求出 f(m)，再反演求 g(m)

$ f ( m ) = sum_(i = 0)^m binom(m, i) g ( i ) quad arrow.r.double quad g ( m ) = sum_(i = 0)^m ( - 1 )^(m - i) binom(m, i) f ( i ) $

#block[
#set enum(numbering: "1.", start: 2)
+ 恰好 → 至少型
]

设 g(m) 代表 #strong[恰好] 满足条件 (= m) 的方案数（即答案） \
设 f(m) 代表 #strong[至少] 满足条件 (≥ m) 的方案数

先求出 f(m)，再反演求 g(m) $ f ( m ) = sum_(i = m)^n binom(i, m) g ( i ) quad arrow.r.double quad g ( m ) = sum_(i = m)^n ( - 1 )^(i - m) binom(i, m) f ( i ) $

我们证明一下形式二：

直接套用子集反演公式： $ f ( n ) = sum_(i = 0)^n binom(n, i) g ( i ) arrow.l.r.double f ( S ) = sum_(T subset.eq S) g ( T ) $ $ g ( S ) = sum_(S subset.eq T) ( - 1 )^(\| T \| - \| S \|) f ( T ) $ 我们只关心大小，所以把大小为i的集合合并 $ g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i ) $

===== 莫比乌斯反演
<莫比乌斯反演>
$ f ( n ) = sum_(d \| n) g ( d ) arrow.l.r.double g ( n ) = sum_(d \| n) mu ( d ) f (n / d) $

证明：

我们取偏序集为 #strong[正整数整除格] $D = ( bb(Z)^(+) \, divides )$。 关系定义为：$a lt.eq b arrow.l.r.double a divides b$。

此时，我们需要证明：#strong[关联代数 $I ( D )$ 可以退化为狄利克雷代数。]

====== 核心引理：区间的结构同构
<核心引理区间的结构同构>
#strong[命题];：对于任意 $d divides n$，偏序区间 $\[ d \, n \]$ 与区间 $\[ 1 \, n \/ d \]$ 是#strong[序同构];的。

#quote(block: true)[
#strong[序同构];：两个偏序集 $A$ 和 $B$ 是序同构的，当且仅当存在双射 $phi.alt : A arrow.r B$，使得 $phi.alt$ 和 $phi.alt^(- 1)$ 都保持偏序关系。
]

#quote(block: true)[
其实就是两个偏序集的dag相同
]

#strong[证明];： 1. 定义映射 $phi.alt : \[ d \, n \] arrow.r \[ 1 \, n \/ d \]$，法则为 $phi.alt ( z ) = z \/ d$。 2. #strong[双射性];： \* 若 $z in \[ d \, n \]$，则 $d divides z divides n$，故 $1 divides ( z \/ d ) divides ( n \/ d )$，即 $phi.alt ( z ) in \[ 1 \, n \/ d \]$。显然这是可逆的。 3. #strong[保序性];： \* 对于任意 $a \, b in \[ d \, n \]$，我们需要证明 $a divides b arrow.l.r.double phi.alt ( a ) divides phi.alt ( b )$。 \* 根据整除定义：$a divides b arrow.l.r.double exists k in bb(Z) \, b = a k$。 \* 等式两边同除以 $d$：$b \/ d = ( a \/ d ) k arrow.l.r.double phi.alt ( b ) = phi.alt ( a ) k$。 \* 这等价于 $phi.alt ( a ) divides phi.alt ( b )$。

#strong[结论];：区间 $\[ d \, n \]$ 的偏序结构完全等同于 $\[ 1 \, n \/ d \]$。

由于 $mu ( x \, y )$ 的值仅由区间 $\[ x \, y \]$ 的#strong[拓扑结构];决定（由 $mu$ 的递归定义可证，同构的偏序集具有相同的 $mu$ 值）。

由核心引理可知，区间 $\[ d \, n \]$ 的结构仅依赖于商 $n \/ d$。 因此，我们可以定义#strong[约化函数];。

对于关联代数中的任意函数 $h ( x \, y )$，若其满足 $h ( x \, y ) = h ( k x \, k y )$（即具有平移不变性），我们可以定义对应的一元算术函数 $tilde(h)$： $ tilde(h) ( k ) equiv h ( 1 \, k ) $ 从而有： $ h ( d \, n ) = tilde(h) (n / d) $

#strong[检查 $zeta$ 和 $mu$ 是否符合此条件：] 1. #strong[Zeta];: $zeta ( d \, n ) = 1 arrow.l.r.double d divides n$。 $zeta ( 1 \, n \/ d ) = 1 arrow.l.r.double 1 divides ( n \/ d )$。 显然恒成立，故 $zeta ( d \, n ) = tilde(zeta) ( n \/ d ) = 1$。 这里 $tilde(zeta)$ 就是数论中的常数函数 $upright(bold(1)) ( n ) = 1$。

#block[
#set enum(numbering: "1.", start: 2)
+ #strong[Mu];: 由于 $mu$ 是 $zeta$ 的逆元，且 $zeta$ 具有平移不变性，由代数性质可知 $mu$ 也必然继承此性质（同构区间的逆元结构也相同）。 定义数论莫比乌斯函数 $mu_(N T) ( k ) = mu ( 1 \, k )$。 则 $mu ( d \, n ) = mu_(N T) ( n \/ d )$。
]

现在我们将广义反演公式代入整除格环境。

#strong[广义形式];： $ f ( n ) = sum_(d divides n) g ( d ) mu ( d \, n ) $ #emph[(注：这里 $d lt.eq n$ 即 $d divides n$)]

#strong[代入降维后的 $mu$];： $ f ( n ) = sum_(d divides n) g ( d ) mu_(N T) (n / d) $

为了验证这个 $mu_(N T)$ 确实是我们熟知的那个函数，我们检查它的一元卷积性质。 在广义代数中：$mu \* zeta = delta$。 对应的一元卷积（狄利克雷卷积）为： $ ( tilde(mu) \* tilde(zeta) ) ( n ) = sum_(d divides n) tilde(mu) ( d ) tilde(zeta) (n / d) = sum_(d divides n) mu_(N T) ( d ) dot.op 1 $

而在广义代数中，$delta ( x \, y )$ 降维后对应单位函数 $epsilon.alt ( n ) = \[ n = 1 \]$。 \>为啥？此时$d = n$,$n \/ d = 1$

因此： $ sum_(d divides n) mu_(N T) ( d ) = \[ n = 1 \] $

这正是数论中莫比乌斯函数的#strong[定义式];。

====== 最终结论
<最终结论>
通过#strong[整除格区间 $\[ d \, n \] tilde.equiv \[ 1 \, n \/ d \]$ 的序同构];，我们将定义在 $I ( D )$ 上的广义莫比乌斯反演： $ f ( n ) = sum_(d divides n) g ( d ) mu ( d \, n ) $

严格推导为数论形式： $ f ( n ) = sum_(d divides n) g ( d ) mu_(N T) (n / d) $

证毕。

以下是常用式子

$ sum_(d \| n) phi ( d ) = n $

$ sum_(d \| n) mu ( d ) = \[ n = 1 \] = cases(delim: "{", 1 & n = 1, 0 & n > 1) $

把$n$换成$g c d ( a \, b )$

$ sum_(d \| g c d ( a \, b )) mu ( d ) = \[ g c d ( a \, b ) = 1 \] cases(delim: "{", 1 & g c d ( a \, b ) = 1, 0 & g c d ( a \, b ) > 1) $

初等证明前已述及

===== Min-Max 容斥
<min-max-容斥>
Min-Max 容斥用于将“全集中最大值（最后出现）的问题”转化为“子集中最小值（最先出现）的问题”。在概率期望中，$min$ 通常比 $max$ 好算得多。

====== 公式
<公式>
对于全序集合 $S$ 和其上的值 ${ x_i }$：

$ max_(i in S) zws_k x_i = sum_(T subset.eq S \, T eq.not nothing) ( - 1 )^(\| T \| - k) binom(\| T \| - 1, k - 1) min_(j in T) x_j $

$ min_(i in S) zws_k x_i = sum_(T subset.eq S \, T eq.not nothing) ( - 1 )^(\| T \| - k) binom(\| T \| - 1, k - 1) max_(j in T) x_j $

其实 #strong[min\_max 容斥] 在期望意义下也满足：

$ E ( max_(i in S) zws_k x_i ) = sum_(T subset.eq S) ( - 1 )^(\| T \| - k) binom(\| T \| - 1, k - 1) E ( min_(j in T) x_j ) $

$ E ( min_(i in S) zws_k x_i ) = sum_(T subset.eq S) ( - 1 )^(\| T \| - k) binom(\| T \| - 1, k - 1) E ( max_(j in T) x_j ) $

由期望线性性可知。

#quote(block: true)[
期望的线性性告诉我们：和的期望等于期望的和。 即 $E \[ sum c_i dot.op V_i \] = sum c_i dot.op E \[ V_i \]$，无论这些随机变量 $V_i$ 之间是否独立。 $ E \[ Y \] & = E [sum_(T subset.eq S) a ( \| T \| ) dot.op Z_T]\
 & = sum_(T subset.eq S) a ( \| T \| ) dot.op E \[ Z_T \] quad ( upright("常数提出，求和拆开") ) $
]

特殊情况 ($k = 1$, 最大值):

$ max ( S ) = sum_(T subset.eq S \, T eq.not nothing) ( - 1 )^(\| T \| - 1) min ( T ) $

====== 初等证明
<初等证明>
我们需要构造一个关于集合大小的系数函数 $a ( \| T \| )$，使得等式成立：

$ upright("第 ") k upright(" 大值") = sum_(T subset.eq S) a ( \| T \| ) min ( T ) $

1.分析元素的贡献 考虑 $S$ 中第 $r$ 大的那个元素，记为 $x_(( r ))$。 在右边的求和式 $sum a ( \| T \| ) min ( T )$ 中，$x_(( r ))$ 何时会被计入？ 只有当 $x_(( r ))$ 是集合 $T$ 的最小值时，$min ( T )$ 才会取到 $x_(( r ))$。

如果 $x_(( r ))$ 是 $T$ 的最小值，那么 $T$ 中的其他元素必须都比 $x_(( r ))$ 大。 在 $S$ 中，比 $x_(( r ))$ 大的元素一共有 $r - 1$ 个。 假设我们构造的集合 $T$ 大小为 $m$。除去 $x_(( r ))$ 本身，我们需要从那 $r - 1$ 个比它大的数中选出 $m - 1$ 个。 方案数为 $binom(r - 1, m - 1)$。

所以，第 $r$ 大的数 $x_(( r ))$ 在右边的总贡献系数是：

$ C ( r ) = sum_m binom(r - 1, m - 1) a ( m ) $

(这里枚举的是集合大小 $m$)

2.建立目标方程 我们希望左边计算出来的结果恰好是第 $k$ 大的值。 这意味着：

当 $r = k$ 时（即它是第 $k$ 大），总系数 $C ( r ) = 1$。

当 $r eq.not k$ 时，总系数 $C ( r ) = 0$。

令 $N = r - 1$ (表示比当前数大的数的个数)，目标方程为：

$ sum_m binom(N, m - 1) a ( m ) = \[ N = k - 1 \] $

令 $j = m - 1$，则 $a ( m ) = a ( j + 1 )$。方程化为：

$ sum_(j = 0)^N binom(N, j) a ( j + 1 ) = \[ N = k - 1 \] $

3.使用二项式反演求解系数 观察上式，形如 $F ( N ) = sum binom(N, j) G ( j )$。 其中 $F ( N ) = \[ N = k - 1 \]$ 是已知结果，$G ( j ) = a ( j + 1 )$ 是待求系数。 根据二项式反演形式二：

$ G ( N ) = sum_(j = 0)^N ( - 1 )^(N - j) binom(N, j) F ( j ) $

代入 $F ( j ) = \[ j = k - 1 \]$：

$ a ( N + 1 ) & = sum_(j = 0)^N ( - 1 )^(N - j) binom(N, j) \[ j = k - 1 \]\
 $

由于求和式中只有 $j = k - 1$ 这一项非零（前提是 $N gt.eq k - 1$），直接提取该项：

$ a ( N + 1 ) = ( - 1 )^(N - ( k - 1 )) binom(N, k - 1) $

4.还原变量 代入上式：

$ a ( \| T \| ) & = ( - 1 )^(( \| T \| - 1 ) - ( k - 1 )) binom(\| T \| - 1, k - 1)\
 & = ( - 1 )^(\| T \| - k) binom(\| T \| - 1, k - 1) $

#quote(block: true)[
max和min在全序上是对称的,所以第二个式子就不证了
]

#quote(block: true)[
从广义莫反证明min-max容斥是有点难度的，留作读者证明
]

===== 斯特林反演
<斯特林反演>
斯特林反演揭示了普通幂与下降幂/上升幂之间的基底变换关系。

第二类斯特林数 (子集数) $mat(delim: "{", n; k)$：将 $n$ 个不同元素放入 $k$ 个无区别盒子（非空）的方案数。

第一类斯特林数 (轮换数) $mat(delim: "[", n; k)$：将 $n$ 个不同元素排成 $k$ 个轮换（非空）的方案数。

斯特林反演

$ f ( n ) = sum_(i = 0)^n mat(delim: "{", n; i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) mat(delim: "[", n; i) f ( i ) $

====== 前置知识
<前置知识>
#strong[上升幂和下降幂]

上升幂 $x^(overline(n)) = product_(i = 0)^(n - 1) ( x + i )$ 下降幂 \$x^{} = = \_{i=0}^{n-1} (x-i) =n! \$

上升幂转普通幂

$ x^(overline(n)) = sum_(i = 0)^n mat(delim: "[", n; i) x^i $

证明： 考虑归纳证明： $n = 0$ 时，等式两边皆为 $1$，等式成立 $n = 1$ 时，等式左边等于 $x$ ，等式右边等于 $x$ ，等式成立。

$ x^(overline(n + 1)) & = ( x + n ) x^(overline(n))\
 & = ( x + n ) sum_(i = 0)^n mat(delim: "[", n; i) x^i\
 & = x sum_(i = 0)^n mat(delim: "[", n; i) x^i + n sum_(i = 0)^n mat(delim: "[", n; i) x^i\
 & = sum_(i = 1)^(n + 1) mat(delim: "[", n; i - 1) x^i + n sum_(i = 0)^(n + 1) mat(delim: "[", n; i) x^i\
 & = sum_(i = 0)^(n + 1) (mat(delim: "[", n; i - 1) + n mat(delim: "[", n; i)) x^i\
 & = sum_(i = 0)^(n + 1) mat(delim: "[", n + 1; i) x^i $

下降幂转普通幂

$ x^(underline(n)) = sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i $

证明： 同样考虑归纳证明： $n = 0$ 时，等式两边皆为 $1$ ，等式成立 $n = 1$ 时，等式两边皆为 $x$ ，等式成立

$ x^(underline(n + 1)) & = ( x - n ) x^(underline(n))\
 & = ( x - n ) sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i\
 & = x sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i - n sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i\
 & = sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^(i + 1) - n sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i\
 & = sum_(i = 1)^(n + 1) mat(delim: "[", n; i - 1) ( - 1 )^(n - i + 1) x^i - n sum_(i = 0)^(n + 1) mat(delim: "[", n; i) ( - 1 )^(n - i) x^i\
 & = sum_(i = 1)^(n + 1) mat(delim: "[", n; i - 1) ( - 1 )^(n - i + 1) x^i + n sum_(i = 0)^(n + 1) mat(delim: "[", n; i) ( - 1 )^(n - i + 1) x^i\
 & = sum_(i = 0)^(n + 1) (mat(delim: "[", n; i - 1) + n mat(delim: "[", n; i)) ( - 1 )^(n - i + 1) x^i\
 & = sum_(i = 0)^(n + 1) mat(delim: "[", n + 1; i) ( - 1 )^(( n + 1 ) - i) x^i $

普通幂转为下降幂

$ x^n = sum_(i = 0)^n mat(delim: "{", n; i) x^(underline(i)) quad $

证明：考虑组合意义 假设我们有：$n$ 个不同的球（标号 $1 \, 2 \, dots.h \, n$）$x$ 个不同的盒子（标号 $1 \, 2 \, dots.h \, x$） 我们要计算：把这 $n$ 个球任意放入这 $x$ 个盒子中，允许盒子为空，一共有多少种方案？ 左边 对于每一个球，我们都可以从 $x$ 个盒子中任选一个放入。所以总方案数为 $x^n$。 右边 假设最后放完球后，恰好有 $i$ 个盒子 里有球（即非空盒子数为 $i$） 1.选出这 $i$ 个盒子我们在 $x$ 个不同的盒子中，选出 $i$ 个将被使用的盒子。方案数为组合数：$binom(x, i)$ 2.把 $n$ 个球分给这 $i$ 个盒子（暂时不区分盒子的顺序） 这相当于把 $n$ 个不同的元素，划分成 $i$ 个非空的集合（堆）。 这正是第二类斯特林数 $mat(delim: "{", n; i)$ 的定义！ 3.把这 $i$ 堆球放入刚才选好的 $i$ 个盒子里因为盒子是不同的，我们需要决定哪一堆球放进哪一个盒子。这相当于 $i$ 个元素的全排列。方案数为：$i !$ 方案总数为： $ med binom(x, i) times mat(delim: "{", n; i) times i ! = mat(delim: "{", n; i) x^(underline(i)) $

普通幂转上升幂

$ x^n = sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^(n - i) x^(overline(i)) $

我们会发现一个关于负数的重要性质： $ ( - x )^(underline(n)) = ( - 1 )^n x^(overline(n)) $ 推导引理： $ ( - x )^(underline(n)) & = ( - x ) ( - x - 1 ) ( - x - 2 ) dots.h ( - x - n + 1 )\
 & = [( - 1 ) ( x )] dot.op [( - 1 ) ( x + 1 )] dot.op [( - 1 ) ( x + 2 )] dots.h [( - 1 ) ( x + n - 1 )]\
 & = ( - 1 )^n dot.op underbrace(x ( x + 1 ) ( x + 2 ) dots.h ( x + n - 1 ), x^(overline(n)))\
 & = ( - 1 )^n x^(overline(n)) $

我们已知普通幂转下降幂的公式： $ x^n = sum_(i = 0)^n mat(delim: "{", n; i) x^(underline(i)) $ 在这个恒等式中，将 $x$ 替换为 $- x$： $ ( - x )^n = sum_(i = 0)^n mat(delim: "{", n; i) ( - x )^(underline(i)) $ 左边展开为 $( - 1 )^n x^n$，右边利用引理代换： $ ( - 1 )^n x^n = sum_(i = 0)^n mat(delim: "{", n; i) [( - 1 )^i x^(overline(i))] $ 两边同时除以 $( - 1 )^n$（或者乘以 $( - 1 )^n$）： $ x^n & = frac(1, ( - 1 )^n) sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^i x^(overline(i))\
 & = sum_(i = 0)^n mat(delim: "{", n; i) frac(( - 1 )^i, ( - 1 )^n) x^(overline(i))\
 & = sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^(i - n) x^(overline(i)) $ 由于 $( - 1 )^(i - n) = ( - 1 )^(n - i)$（指数差偶数倍不影响符号），整理得： $ x^n = sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^(n - i) x^(overline(i)) $

#strong[反转公式]

$ sum_(i = m)^n ( - 1 )^(n - i) mat(delim: "{", n; i) mat(delim: "[", i; m) = \[ m = n \] $

$ sum_(i = m)^n ( - 1 )^(m - i) mat(delim: "[", n; i) mat(delim: "{", i; m) = \[ m = n \] $

反转公式（1）证明(利用下降幂转普通幂/普通幂转下降幂)：

$ x^(underline(n)) & = sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) x^i\
 & = sum_(i = 0)^n mat(delim: "[", n; i) ( - 1 )^(n - i) sum_(j = 0)^i mat(delim: "{", i; j) x^(underline(j))\
 & = sum_(i = 0)^n x^(underline(i)) sum_(j = i)^n ( - 1 )^(n - j) mat(delim: "[", n; j) mat(delim: "{", j; i) $ 当 $i = n$ 时，等式右边为 $x^(underline(n))$，等式成立 当 $i < n$ 时，等式右边为 要为$0$，等式成立

反转公式（2）证明：

$ x^n & = sum_(i = 0)^n mat(delim: "{", n; i) x^(underline(i))\
 & = sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^i ( - x )^(overline(i))\
 & = sum_(i = 0)^n mat(delim: "{", n; i) ( - 1 )^i sum_(j = 0)^i mat(delim: "[", i; j) ( - x )^j\
 & = sum_(i = 0)^n x^i sum_(j = i)^n ( - 1 )^(i - j) mat(delim: "{", n; j) mat(delim: "[", j; i) $

同理

斯特林反演

$ f ( n ) = sum_(i = 0)^n mat(delim: "{", n; i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) mat(delim: "[", n; i) f ( i ) $

上式中一二类斯特林数可以互换位置。

证明： 已知： $g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) mat(delim: "[", n; i) f ( i )$

$ f ( n ) & = sum_(i = 0) \[ i = n \] f ( i )\
 & = sum_(i = 0)^n sum_(j = i)^n mat(delim: "{", n; j) mat(delim: "[", j; i) ( - 1 )^(j - i) f ( i )\
 & = sum_(i = 0)^n mat(delim: "{", n; i) sum_(j = 0)^i ( - 1 )^(i - j) mat(delim: "[", i; j) f ( j )\
 & = sum_(i = 0)^n mat(delim: "{", n; i) g ( i ) $

====== 另一个视角
<另一个视角>
#strong[斯特林反演本质上是“集合分划格”（Partition Lattice）上的广义莫比乌斯反演。]

就像： \* #strong[二项式反演] 对应 #strong[子集格（布尔格）] 上的莫比乌斯反演。 \* #strong[数论莫比乌斯反演] 对应 #strong[整除格] 上的莫比乌斯反演。 \* #strong[斯特林反演] 对应 #strong[集合分划格] 上的莫比乌斯反演。

======= 1. 什么是集合分划格 $Pi_n$？
<什么是集合分划格-pi_n>
我们定义一个偏序集（Poset） $P = Pi_n$。 \* #strong[元素];：集合 $S = { 1 \, 2 \, dots.h \, n }$ 的所有#strong[分划];。 \* 例如 $n = 3$，${ { 1 \, 2 } \, { 3 } }$ 就是一个分划。 \* #strong[偏序关系 $lt.eq$];： \* 对于两个分划 $sigma$ 和 $tau$，如果 $sigma$ 中的每一个块（block）都完全包含在 $tau$ 的某一个块中，我们称 $sigma lt.eq tau$（$sigma$ 比 $tau$ 更“细”，或者 $tau$ 比 $sigma$ 更“粗”）。 \* #strong[最小元 $hat(0)$];：${ { 1 } \, { 2 } \, dots.h \, { n } }$（全部分开，最细）。 \* #strong[最大元 $hat(1)$];：${ { 1 \, 2 \, dots.h \, n } }$（全部在一起，最粗）。

======= 2. 这个格上的 Zeta 和 Möbius 函数
<这个格上的-zeta-和-möbius-函数>
在 $Pi_n$ 上定义广义莫比乌斯反演的两个核心函数：

Zeta 函数 $zeta ( sigma \, tau )$ 定义为：若 $sigma lt.eq tau$，则为 $1$，否则为 $0$。 它对应着#strong[求和];操作，也就是我们熟悉的“第二类斯特林数”方向。

Möbius 函数 $mu ( sigma \, tau )$ 它是 $zeta$ 的逆。在集合分划格中，$mu$ 的取值非常特殊： 对于区间 $\[ hat(0) \, hat(1) \]$（即从全分散到全聚合），有著名的定理： $ mu ( hat(0) \, hat(1) ) = ( - 1 )^(n - 1) ( n - 1 ) ! $

#quote(block: true)[
咋证？ 我们要证的目标是 $M_n = ( - 1 )^(n - 1) ( n - 1 ) !$，其中 $M_n$ 简记为 $n$ 个元素分划格最顶端的 $mu ( hat(0) \, hat(1) )$。 利用 $sum mu = 0$ 构造递推 #strong[\1. 莫比乌斯函数的定义] 对于任意 $n > 1$，分划格 $Pi_n$ 中所有元素的 $mu$ 值之和必须为 0： $ sum_(sigma in Pi_n) mu ( hat(0) \, sigma ) = 0 $ 换句话说： $ M_n = - sum_(sigma eq.not hat(1)) mu ( hat(0) \, sigma ) $ #emph[(最顶上的那一项，等于底下所有项之和的相反数)] #strong[\2. 巧妙的分组] 我们根据 #strong[“元素 $n$ 所在的块的大小”] 来对所有的分划 $sigma$ 进行归类。 设元素 $n$ 所在的块为 $B$，设 $\| B \| = i$。 $i$ 的取值范围是 $1$ 到 $n$。 选出这个块 $B$ 的方案数是 $binom(n - 1, i - 1)$（因为 $n$ 必须在里面，从剩下 $n - 1$ 个里选 $i - 1$ 个陪它）。 #strong[\3. 结构的乘积] 对于一个固定的块 $B$（大小为 $i$），剩下的 $n - i$ 个元素组成了某种分划 $tau$。 因为块之间互不干扰，$mu$ 值具有乘积性质： $ mu ( hat(0) \, sigma ) = underbrace(M_i, upright("块") B upright("的贡献")) times underbrace(mu ( upright("剩余部分的") tau ), upright("剩下的贡献")) $ #strong[\4. 所有的项求和] 把刚才的 $sum mu = 0$ 按照 $i$ 展开： $ 0 = sum_(i = 1)^n binom(n - 1, i - 1) M_i times underbrace((sum_(tau in Pi_(n - i)) mu ( hat(0) \, tau )), upright("剩余部分的所有情况之和")) $ #strong[\5. 奇迹发生了] 请盯着最后那个括号里的部分：$sum_(tau in Pi_(n - i)) mu ( hat(0) \, tau )$。 这是一个大小为 $n - i$ 的全部分划格的 $mu$ 之和。 根据莫比乌斯函数的定义，只要 $n - i > 1$，这个和就是 #strong[0];！ 这个和不为 0 的只有两种情况： #strong[$n - i = 1$];：只剩 1 个元素，和为 $M_1 = 1$。 #strong[$n - i = 0$];：没有剩余元素（即 $i = n$），定义为 1。 #strong[\6. 最后的递推] 因为只有 $n - i = 0$ 和 $n - i = 1$ 有值，长长的求和式瞬间只剩下两项： #strong[当 $i = n$ 时];（全在一起）： 系数是 $binom(n - 1, n - 1) M_n times 1 = M_n$ #strong[当 $i = n - 1$ 时];（剩一个孤立点）： 系数是 $binom(n - 1, n - 2) M_(n - 1) times M_1 = ( n - 1 ) M_(n - 1)$ 方程变成了： $ M_n + ( n - 1 ) M_(n - 1) = 0 $ $ arrow.r.double M_n = - ( n - 1 ) M_(n - 1) $ #strong[\7. 求解] $M_1 = 1$ $M_2 = - 1 dot.op M_1 = - 1$ $M_3 = - 2 dot.op M_2 = 2$ … $M_n = ( - 1 )^(n - 1) ( n - 1 ) !$
]

更一般地，对于任意 $sigma lt.eq tau$，$mu ( sigma \, tau )$ 的值只与它们之间的结构差异有关，其数值形式就是#strong[带符号的第一类斯特林数];的乘积形式。

======= 3. 如何连接到斯特林反演？
<如何连接到斯特林反演>
我们通常看到的斯特林反演公式是关于 $n$ 和 $k$ 的，而不是关于具体分划 $sigma$ 的。这是因为斯特林反演是集合分划格反演的 #strong[“约化”（Reduced）形式];。

第一步：从组合意义看 $x^n$ (Zeta 变换)

考虑映射 $f : { 1 \, dots.h \, n } arrow.r { 1 \, dots.h \, x }$。总共有 $x^n$ 种映射。 我们可以根据这个映射的 #strong[核（Kernel）] 来分类。核是一个分划 $sigma$，也就是把映射到同一个值的元素归为一个块。

如果我们设 $N ( sigma )$ 是“恰好以 $sigma$ 为核”的映射数量（即 $sigma$ 的每个块映射到不同的值）。 那么总数 $x^n$ 就是对所有可能的核 $sigma$ 求和（这就是 Zeta 变换的体现）： $ x^n = sum_(sigma in Pi_n) N ( sigma ) $

注意到，如果分划 $sigma$ 有 $k$ 个块（记为 $\| sigma \| = k$），那么 $N ( sigma ) = x^(underline(k))$（从 $x$ 个值选 $k$ 个排列）。 而这也正好对应了有多少个分划具有 $k$ 个块------这正是#strong[第二类斯特林数] $mat(delim: "{", n; k)$。

所以上面的式子变成了我们熟悉的： $ x^n = sum_(k = 0)^n mat(delim: "{", n; k) x^(underline(k)) $ #strong[(这对应 $zeta$ 变换，也就是 $g = zeta \* f$)]

#quote(block: true)[
听不懂？见A3.普通幂转下降幂的组合意义
]

第二步：反演 (Möbius 变换)

根据广义莫比乌斯反演原理，如果我们想求 $x^(underline(n))$（即强行要求核为 $hat(0)$ 且全部映射到不同值的方案数），我们需要用 $mu$ 函数倒推回去。

在“约化”的代数视角下，$Pi_n$ 的 Möbius 函数系数对应正是 #strong[第一类斯特林数];。

具体来说，由于 $mu ( hat(0) \, hat(1) ) = ( - 1 )^(n - 1) ( n - 1 ) !$，这其实是第一类斯特林数 $s ( n \, 1 )$。 推广到从 $n$ 个元素的块反演回 $k$ 个元素的块，系数正是： $ s ( n \, k ) = ( - 1 )^(n - k) mat(delim: "[", n; k) $

#quote(block: true)[
咋推广？ 对于 $n$ 个元素的#strong[全部分划格] $Pi_n$，从最底（全散）到最顶（全聚）的莫比乌斯函数是： $ mu_(Pi_n) ( hat(0) \, hat(1) ) = ( - 1 )^(n - 1) ( n - 1 ) ! $ #strong[那如果不是到最顶 $hat(1)$，而是到一个中间状态 $sigma$ 呢？] 假设分划 $sigma$ 将 $n$ 个元素分成了 $k$ 个块，块的大小分别为 $b_1 \, b_2 \, dots.h \, b_k$（显然 $sum b_i = n$）。 #strong[关键性质：] 区间 $\[ hat(0) \, sigma \]$ 在结构上等价于 #strong[$k$ 个独立的小分划格的直积];。 $ \[ hat(0) \, sigma \] tilde.equiv Pi_(b_1) times Pi_(b_2) times dots.h times Pi_(b_k) $ 因为莫比乌斯函数在直积上具有#strong[积性];（Multiplicative），所以： $ mu ( hat(0) \, sigma ) = product_(i = 1)^k mu_(Pi_(b_i)) ( hat(0) \, hat(1) ) $ 代入基础结论： $ mu ( hat(0) \, sigma ) & = product_(i = 1)^k [( - 1 )^(b_i - 1) ( b_i - 1 ) !]\
 & = ( - 1 )^(sum ( b_i - 1 )) product_(i = 1)^k ( b_i - 1 ) !\
 & = ( - 1 )^(n - k) product_(i = 1)^k ( b_i - 1 ) ! $ 这是一个非常重要的中间结论：#strong[任意分划 $sigma$ 的莫比乌斯函数值，取决于它的块大小的阶乘积。] 我们在做斯特林反演时，不是针对某一个特定的 $sigma$，而是把#strong[所有块数（Rank）为 $k$ 的分划];归为一类。 所谓的“反演系数” $s ( n \, k )$，本质上就是把所有“这就只有 $k$ 个块”的分划的 $mu$ 值加起来： $ s ( n \, k ) = sum_(sigma in Pi_n\
\| sigma \| = k) mu ( hat(0) \, sigma ) $ 代入刚才推导的公式： $ s ( n \, k ) = sum_(sigma in Pi_n\
\| sigma \| = k) (( - 1 )^(n - k) product_(i = 1)^k ( b_i - 1 ) !) $ 提公因式 $( - 1 )^(n - k)$： $ s ( n \, k ) = ( - 1 )^(n - k) underbrace(sum_(sigma in Pi_n\
\| sigma \| = k) (product_(i = 1)^k ( b_i - 1 ) !), upright("这是什么？")) $ #strong[让我们看看这个求和部分的组合意义：] #strong[外部求和];：枚举所有把 $n$ 个元素分成 $k$ 个块的方案（分划 $sigma$）。 #strong[内部乘积] $product ( b_i - 1 ) !$：对于每一个块（大小为 $b_i$），$( b_i - 1 ) !$ 正好是#strong[圆排列];（Circle Permutation）的方案数。 #strong[合起来的意思是：] 先选定一种分块方式，然后把每个块里的元素排成一个圆环（Cycle）。 把所有分块方式对应的圆环方案加起来，这不就是： #strong[“把 $n$ 个元素排成 $k$ 个圆环（轮换）的总方案数”吗？] 这正是#strong[无符号第一类斯特林数] $mat(delim: "[", n; k)$ 的定义！ 所以： $ s ( n \, k ) = ( - 1 )^(n - k) mat(delim: "[", n; k) $ 这就是为什么斯特林反演的系数里藏着圆排列的阶乘，也是它“推广”的内在逻辑。
]

所以反演公式为： $ x^(underline(n)) = sum_(k = 0)^n ( - 1 )^(n - k) mat(delim: "[", n; k) x^k $ #strong[(这对应 $f = mu \* g$)]

===== 总结
<总结>
#figure(
  align(center)[#table(
    columns: (16.67%, 16.67%, 16.67%, 16.67%, 16.67%, 16.67%),
    align: (left,left,left,left,left,left,),
    table.header([反演名称], [基础偏序集 (Poset)], [偏序关系 $lt.eq$], [$zeta ( x \, y )$ (正向覆盖系数)], [$mu ( x \, y )$ (反演容斥系数)], [卷积单位元 $delta ( x \, y )$],),
    table.hline(),
    [#strong[通用定义];], [#strong[任意局部有限偏序集] $P$], [$x lt.eq y$], [$1$ 若 $x lt.eq y$$0$ 若不满足], [$zeta^(- 1)$ (递归定义)], [$\[ x = y \]$],
    [#strong[子集反演] (SOS DP)], [#strong[布尔格] $B_n$ (Power Set)], [$S subset.eq T$], [$1$], [$( - 1 )^(parallel T parallel - parallel S parallel)$], [$\[ S = T \]$],
    [#strong[超集反演] (高维后缀和)], [#strong[布尔格] $B_n$ (Dual)], [$S supset.eq T$], [$1$], [$( - 1 )^(parallel S parallel - parallel T parallel)$], [$\[ S = T \]$],
    [#strong[二项式反演] (PIE)], [#strong[布尔格] (投影到大小)], [大小 $k$ 归类], [$binom(n, k)$], [$( - 1 )^(n - k) binom(n, k)$], [$\[ n = k \]$],
    [#strong[数论莫比乌斯];], [#strong[整除格] $D_n$], [$a divides b$], [$1$], [$mu ( b \/ a )$], [$\[ a = b \]$],
    [#strong[斯特林反演];], [#strong[分划格] $Pi_n$], [加细 (Refinement)], [$mat(delim: "{", n; k)$], [$( - 1 )^(n - k) mat(delim: "[", n; k)$], [$\[ n = k \]$],
  )]
  , kind: table
  )


== 数论笔记(和式变换)

=== 和式变换规则与技术
<和式变换规则与技术>
==== 基本变换规则
<基本变换规则>
+ #strong[分配律] \
  $ sum_(k in K) c a_k = c sum_(k in K) a_k $

+ #strong[结合律] \
  $ sum_(k in K) ( a_k + b_k ) = sum_(k in K) a_k + sum_(k in K) b_k $

+ #strong[交换律] \
  $ sum_(k in K) a_k = sum_(p ( k ) in K) a_(p ( k )) $ 其中 $p ( k )$ 是指标集的任意排列 \
  #strong[示例];： \
  $a_1 + a_2 + a_3 + a_6 = a_6 + a_3 + a_2 + a_1$

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 高级变换技术
<高级变换技术>
===== 1. 替换条件式
<替换条件式>
$ sum_(i = 1)^n sum_(j = 1)^m sum_(d \| gcd ( i \, j )) d = sum_(i = 1)^n sum_(j = 1)^m sum_(d = 1)^(min ( n \, m )) \[ d \| i \] \[ d \| j \] d $

===== 2. 替换指标变量
<替换指标变量>
$ sum_(i = 1)^n sum_(j = 1)^m \[ gcd ( i \, j ) = k \] = sum_(i' = 1)^(floor.l n \/ k floor.r) sum_(j' = 1)^(floor.l m \/ k floor.r) \[ gcd ( i' \, j' ) = 1 \] $ 其中 $i' = i \/ k \, j' = j \/ k$

===== 3. 交换求和次序
<交换求和次序>
$ sum_(i = 1)^n sum_(j = 1)^m A ( i ) B ( j ) = sum_(j = 1)^m sum_(i = 1)^n A ( i ) B ( j ) $

===== 4. 分离变量
<分离变量>
$ sum_(i = 1)^n sum_(j = 1)^m A ( i ) B ( j ) = (sum_(i = 1)^n A ( i )) (sum_(j = 1)^m B ( j )) $

==== 技巧
<技巧>
===== 1. 区间整除条件式的封闭形式
<区间整除条件式的封闭形式>
$ sum_(i = 1)^n \[ k \| i \] = floor.l n / k floor.r $ 扩展 $ floor.l frac(floor.l n / k floor.r, m) floor.r = floor.l frac(n, k m) floor.r $ \>人话：在1到n的整数中，能被k整除的数的个数是 $floor.l n / k floor.r$

===== 2. $\[ g c d ( i \, j ) = 1 \]$的进一步变换
<gcdij1的进一步变换>
$ sum_(i = 1)^n sum_(j = 1)^m \[ gcd ( i \, j ) = 1 \] = sum_(i = 1)^n sum_(j = 1)^m sum_(d \| gcd ( i \, j )) mu ( d ) $ $ = sum_(i = 1)^n sum_(j = 1)^m sum_(d = 1)^(min ( n \, m )) mu ( d ) \[ d \| i \] \[ d \| j \] $ \>人话：变换枚举顺序，d能整除i和j，则d能整除gcd(i,j)

===== 3. 有序对和无序对求和的互相转换
<有序对和无序对求和的互相转换>
$ sum_(i = 1)^n sum_(j = 1)^n A ( i \, j ) = 2 \* sum_(i = 1)^n sum_(j = i)^n A ( i \, j ) - sum_(i = 1)^n A ( i \, i ) $ 当且仅当 $A ( i \, j ) = A ( j \, i )$ 时成立 \>人话：将有序对转换为无序对，注意枚举顺序

===== 4. 通过$g c d ( i \, j ) = 1$构造条件式$\[ g c d ( i \, j ) = 1 \]$
<通过gcdij1构造条件式gcdij1>
令 $ d = gcd ( i \, j ) \, i = i' d \, j = j' d $ 注意，当且仅当 $ gcd ( i' \, j' ) = 1 $ 时，此变换成立，于是 $ sum_(i = 1)^n sum_(j = 1)^m f ( gcd ( i \, j ) ) $ $ = sum_(d = 1)^(min ( n \, m )) sum_(i' d = 1)^n sum_(j' d = 1)^m f ( d ) \[ gcd ( i' \, j' ) = 1 \] $ $ = sum_(d = 1)^(min ( n \, m )) sum_(i' = 1)^(floor.l n \/ d floor.r) sum_(j' = 1)^(floor.l m \/ d floor.r) f ( d ) \[ gcd ( i' \, j' ) = 1 \] $ 变量换名 $ = sum_(d = 1)^(min ( n \, m )) sum_(i = 1)^(floor.l n \/ d floor.r) sum_(j = 1)^(floor.l m \/ d floor.r) f ( d ) \[ gcd ( i \, j ) = 1 \] $ \>人话：没有人话

===== 5. $g c d ( x \, y )$的变换
<gcdxy的变换>
$ n = sum_(d \| n) phi ( d ) $ $ g c d ( x \, y ) = sum_(d \| gcd ( x \, y )) phi ( d ) = sum_(d \| x \, d \| y) phi ( d ) $


= 组合与生成函数

== 数论笔记(排列组合)

=== 圆排列
<圆排列>
$n$ 个不同元素围成一圈的#strong[圆排列数];，记作 $Q_n^n$。

考虑其中已经排好的一圈，从不同位置断开，会变成 $n$ 个不同的线排列： \
$ Q_n^n times n = A_n^n $ 则 \
$ Q_n^n = A_n^n / n = ( n - 1 ) ! $

例如，3 个不同元素的圆排列数为 $( 3 - 1 ) ! = 2$ 种：

从 $n$ 个不同元素中选 $m$ 个围成一圈的#strong[圆排列数];，记作 $Q_n^m$： \
$ Q_n^m = C_n^m dot.op Q_m^m = frac(n !, m dot.op ( n - m ) !) $ \>其实就是全排列固定了一个数

=== 错位排列
<错位排列>
错位排列是没有任何元素出现在其有序位置的排列。对于 $1 tilde.op n$ 的排列 $P$，如果满足 $P_i eq.not i$，则称 $P$ 是 $n$ 的错位排列。

==== 错位排列数
<错位排列数>
- $D_1 = 0$
- $D_2 = 1$（即 ${ 2 \, 1 }$）
- $D_3 = 2$（即 ${ 2 \, 3 \, 1 } \, { 3 \, 1 \, 2 }$）
- $D_4 = 9$（即 ${ 2 \, 1 \, 4 \, 3 } \, { 2 \, 3 \, 4 \, 1 } \, { 2 \, 4 \, 1 \, 3 } \, { 3 \, 1 \, 4 \, 2 } \, { 3 \, 4 \, 1 \, 2 } \, { 3 \, 4 \, 2 \, 1 } \, { 4 \, 1 \, 2 \, 3 } \, { 4 \, 3 \, 1 \, 2 } \, { 4 \, 3 \, 2 \, 1 }$）

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 递推关系 $D_n = ( n - 1 ) ( D_(n - 1) + D_(n - 2) )$
<递推关系-d_n-n-1d_n-1-d_n-2>
#strong[边界条件];： $ D_1 = 0 \, quad D_2 = 1 $

#figure(
  align(center)[#table(
    columns: 9,
    align: (auto,auto,auto,auto,auto,auto,auto,auto,auto,),
    table.header([$n$], [1], [2], [3], [4], [5], [6], [7], [8],),
    table.hline(),
    [$D_n$], [0], [1], [2], [9], [44], [265], [1854], [14833],
  )]
  , kind: table
  )

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 信封问题
<信封问题>
$n$ 封不同的信（编号 $1 \, 2 \, dots.h \, n$）放入 $n$ 个编号对应的信封中，要求每个信封的编号与信的编号都不相同。有多少种放置方法？

===== 递推关系分析
<递推关系分析>
考虑第 $n$ 封信的放置： 1. #strong[情况1];：前 $n - 1$ 封信已全错排 \
\- 第 $n$ 封信只需与前面任一封信交换位置 \
\- 方法数：$( n - 1 ) dot.op D_(n - 1)$

#block[
#set enum(numbering: "1.", start: 2)
+ #strong[情况2];：前 $n - 1$ 封信恰好有 1 封位置正确
  - 第 $n$ 封信必须与位置正确的信交换 \
  - 方法数：$( n - 1 ) dot.op D_(n - 2)$
]

#quote(block: true)[
其他情况无法通过一次交换变成全错排
]

=== 第一类斯特林数（斯特林轮换数）
<第一类斯特林数斯特林轮换数>
将 $n$ 个不同元素划分为 $m$ 个非空圆排列的方案数， \
记作 $S ( n \, m )$ 或： $ mat(delim: "[", n; m) $

==== 递推关系
<递推关系>
$ mat(delim: "[", n; m) = mat(delim: "[", n - 1; m - 1) + ( n - 1 ) mat(delim: "[", n - 1; m) $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 组合解释（圆桌问题）
<组合解释圆桌问题>
$n$ 个人坐 $m$ 张圆桌的方案数，考虑第 $n$ 个人的两种坐法：

+ #strong[单独坐一桌]
  - 前 $n - 1$ 人坐满剩余的 $m - 1$ 张桌 \
  - 方案数：$mat(delim: "[", n - 1; m - 1)$
+ #strong[与其他人同坐]
  - 前 $n - 1$ 人先坐满 $m$ 张桌 \
  - 第 $n$ 人可坐到任意 $n - 1$ 个人的左侧 \
  - 方案数：$( n - 1 ) dot.op mat(delim: "[", n - 1; m)$

=== 第二类斯特林数（斯特林子集数）
<第二类斯特林数斯特林子集数>
将 $n$ 个不同元素划分为 $m$ 个非空子集的方案数， \
记作 $S ( n \, m )$ 或： $ mat(delim: "{", n; m) $

==== 递推关系
<递推关系-1>
$ mat(delim: "{", n; m) = mat(delim: "{", n - 1; m - 1) + m mat(delim: "{", n - 1; m) $

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 组合解释（房间分配问题）
<组合解释房间分配问题>
$n$ 个人进入 $m$ 个房间的方案数（每个房间非空），考虑第 $n$ 个人的两种选择：

+ #strong[单独进入新房间]
  - 前 $n - 1$ 人进入剩余的 $m - 1$ 个房间 \
  - 方案数：$mat(delim: "{", n - 1; m - 1)$
+ #strong[进入已有人的房间]
  - 前 $n - 1$ 人先进入所有 $m$ 个房间 \
  - 第 $n$ 人可选择进入任意一个已有人的房间 \
  - 方案数：$m dot.op mat(delim: "{", n - 1; m)$

=== Catalan数 通项公式
<catalan数-通项公式>
#block[
#set enum(numbering: "(1)", start: 1)
+ $H_n = C_(2 n)^n - C_(2 n)^(n - 1)$ \
+ $H_n = frac(1, n + 1) C_(2 n)^n$ \
+ $H_n = frac(4 n - 2, n + 1) H_(n - 1)$
]

==== 证明
<证明>
#figure(image("../../数论/md.img/卡特兰数.png"),
  caption: [
    本地图片
  ]
)

==== Catalan 数列
<catalan-数列>
#figure(
  align(center)[#table(
    columns: (11.11%, 11.11%, 11.11%, 11.11%, 11.11%, 11.11%, 11.11%, 11.11%, 11.11%),
    align: (auto,auto,auto,auto,auto,auto,auto,auto,auto,),
    table.header([$H_0$], [$H_1$], [$H_2$], [$H_3$], [$H_4$], [$H_5$], [$H_6$], [$H_7$], [$H_8$],),
    table.hline(),
    [1], [1], [2], [5], [14], [42], [132], [429], [1430],
  )]
  , kind: table
  )

==== Catalan 特征
<catalan-特征>
从 (0,0) 到 (n,n)，不越过对角线，即任何时候，向上走的步数不能超过向右走的步数。 \
一种操作数不能超过另外一种操作数，或者两种操作不能有交集，这些操作的合法方案数，通常是卡特兰数。

==== Catalan 应用
<catalan-应用>
+ 一个有 $n$ 个 0 和 $n$ 个 1 组成的字串，且所有的前缀字串皆满足 1 的个数不超过 0 的个数。这样的字串个数有多少？ \
+ 包含 $n$ 组括号的合法运算式的个数有多少？ \
+ 一个栈的进栈序列为 1,2,3,···, $n$，有多少个不同的出栈序列？ \
  合法性：任何时刻不能空栈出栈 ⇒ 任意前缀“)”不多于“(”；最后入栈、出栈各n次，栈空。
+ $n$ 个结点可构造多少个不同的二叉树？ \
+ 在圆上选择 $2 n$ 个点，将这些点成对连接起来使得所得的 $n$ 条弦不相交的方法数？ \
+ 通过连结顶点而将 $n + 2$ 边的凸多边形分成 $n$ 个三角形的方法数？

==== 说明
<说明>
这些都是卡特兰数，因为它们都与#strong[合法括号序列 / Dyck 路径];存在天然的双射，或者都满足同一个#strong[卡特兰递推] \
$ C_0 = 1 \, #h(2em) C_n = sum_(i = 0)^(n - 1) C_i thin C_(n - 1 - i) med ( n gt.eq 1 ) \, $ 并且初值一致，所以计数相同。分别说------

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 3) 单栈出栈序列的个数 = $C_n$
<单栈出栈序列的个数-c_n>
把栈的操作写成长度 $2 n$ 的串： \
\- 入栈记“(”，出栈记“)”。 \
合法性：任何时刻不能空栈出栈 ⇒ 任意前缀“)”不多于“(”；最后入栈、出栈各 $n$ 次。 \
这正是#strong[合法括号序列];的定义，因此个数为 $C_n = frac(1, n + 1) binom(2 n, n)$。

（同说法：栈可生成的排列=231-避免排列，其数目为卡特兰数。）

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 4) $n$ 个结点的二叉树个数 = $C_n$
<n-个结点的二叉树个数-c_n>
设 $T_n$ 为有 $n$ 个结点（有序、无标号）的二叉树数。以根为界：左子树 $i$ 个结点、右子树 $n - 1 - i$ 个结点，二者独立： $ T_n = sum_(i = 0)^(n - 1) T_i thin T_(n - 1 - i) \, quad T_0 = 1 . $ 这恰是卡特兰递推，因此 $T_n = C_n$。 \
（等价双射：对二叉树做先序/中序边走访，“向下”记“(”，“返回”记“)”，得到 Dyck 串，反之亦然。）

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 5) 圆上 $2 n$ 点配对且弦不相交的配法数 = $C_n$
<圆上-2n-点配对且弦不相交的配法数-c_n>
固定点 1，它必须与某个点 $2 k$ 相连（顺时针计）。这一条弦把圆分成两侧： \
\- 一侧有 $2 k - 2$ 个点，可不交配对数 $C_(k - 1)$； \
\- 另一侧有 $2 n - 2 k$ 个点，可不交配对数 $C_(n - k)$。 \
枚举 $k = 1 . . n$ 得 $ M_n = sum_(k = 1)^n C_(k - 1) C_(n - k) = sum_(i = 0)^(n - 1) C_i C_(n - 1 - i) . $ 同初值 $M_0 = 1$，故 $M_n = C_n$。

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 6) 将凸 $( n + 2 )$-边形三角剖分的方法数 = $C_n$
<将凸-n2-边形三角剖分的方法数-c_n>
固定顶点 1，选一条对角线 $( 1 \, j )$（$j = 3 . . n + 2$）。它把多边形分成： \
\- 一个 $( j - 1 )$-边形（可三角剖分数 $C_(j - 3)$）； \
\- 一个 $( n + 3 - j )$-边形（可三角剖分数 $C_(n - ( j - 2 ))$）。 \
求和得同一递推： $ T_n = sum_(j = 3)^(n + 2) C_(j - 3) C_(n - ( j - 2 )) = sum_(i = 0)^(n - 1) C_i C_(n - 1 - i) \, quad T_0 = 1 \, $ 故 $T_n = C_n$。

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 递推来源
<递推来源>
===== 从合法括号序列（Dyck 路径）推导
<从合法括号序列dyck-路径推导>
考虑长度 $2 n$ 的合法括号序列：

- 第一个符号必然是“(”。 \
- 找到与之匹配的 “)” 位置。假设它在第 $2 k + 2$ 个位置（即括号包住了 $k$ 对括号）。

于是序列可以分成三部分： \
$ upright("(") thin underbrace(S, 2 k med upright("长度的合法串")) thin upright(")") thin underbrace(T, 2 ( n - k - 1 ) med upright("长度的合法串")) $

- 内部 $S$ 是一个合法串，长度 $2 k$，个数 $C_k$； \
- 外部 $T$ 也是一个合法串，长度 $2 ( n - k - 1 )$，个数 $C_(n - 1 - k)$。

所以 $ C_n = sum_(k = 0)^(n - 1) C_k dot.op C_(n - 1 - k) . $

这就是递推的来源。


== 数论笔记(排列组合进阶)

===== 组合意义天地灭 代数推导保平安
<组合意义天地灭-代数推导保平安>
#quote(block: true)[
主要记录一些好玩的小玩意 特别感谢#link("https://www.cnblogs.com/cbdsopa/p/16319395.html")[cbdsopa的博客-OI中基本的组合数学公式];/#link("https://oi-wiki.org/math/combinatorics/combination/")[oi\_wiki组合数学]
]

比较深刻的是 如果两个计数过程本质相同，那么答案相同，即使过程不同

==== 插板法
<插板法>
插板法是用于求一类给相同元素分组的方案数的一种技巧，也可以用于求一类线性不定方程的解的组数。

===== 正整数和的数目
<正整数和的数目>
问题一：现有 $n$ 个 #strong[完全相同] 的元素，要求将其分为 $k$ 组，保证每组至少有一个元素，一共有多少种分法？

考虑拿 $k - 1$ 块板子插入到 $n$ 个元素两两形成的 $n - 1$ 个空里面。

因为元素是完全相同的，所以答案就是 $binom(n - 1, k - 1)$。

本质是求 $x_1 + x_2 + dots.h.c + x_k = n$ 的正整数解的组数。

===== 非负整数和的数目
<非负整数和的数目>
问题二：现有 $n$ 个 #strong[完全相同] 的元素，要求将其分为 $k$ 组，每组可以为空，一共有多少种分法？

我们考虑创造条件转化成有限制的问题一，先借 $k$ 个元素过来，在这 $n + k$ 个元素形成的 $n + k - 1$ 个空里面插板，答案为

$ binom(n + k - 1, k - 1) = binom(n + k - 1, n) $

组合意义：

开头我们借来了 $k$ 个元素，用于保证每组至少有一个元素，插完板之后再把这 $k$ 个借来的元素从 $k$ 组里面拿走。因为元素是相同的，所以转化过的情况和转化前的情况可以一一对应，答案也就是相等的。

由此可以推导出插板法的公式：$binom(n + k - 1, n)$。

本质是求 $x_1 + x_2 + dots.h.c + x_k = n$ 的非负整数解的组数（即要求 $x_i gt.eq 0$）。

===== 不同下界整数和的数目
<不同下界整数和的数目>
问题三：现有 $n$ 个 #strong[完全相同] 的元素，要求将其分为 $k$ 组，对于第 $i$ 组，至少要分到 $a_i \, sum a_i lt.eq n$ 个元素

本质是求 $x_1 + x_2 + dots.h.c + x_k = n$ 的解的数目，其中 $x_i gt.eq a_i$。

类比无限制的情况，我们借 $sum a_i$ 个元素过来，保证第 $i$ 组至少能分到 $a_i$ 个。也就是令

$ x_i' = x_i - a_i $

得到新方程：

$ ( x_1' + a_1 ) + ( x_2' + a_2 ) + dots.h.c + ( x_k' + a_k ) & = n\
x_1' + x_2' + dots.h.c + x_k' & = n - a_1 - a_2 - dots.h.c - a_k\
x_1' + x_2' + dots.h.c + x_k' & = n - sum a_i $

其中

$ x_i' gt.eq 0 $

然后问题三就转化成了问题二，直接用插板法公式得到答案为

$ binom(n - sum a_i + k - 1, n - sum a_i) $

===== 不相邻的排列
<不相邻的排列>
$1 tilde.op n$ 这 $n$ 个自然数中选 $k$ 个，这 $k$ 个数中任何两个数都不相邻的组合有 $binom(n - k + 1, k)$ 种。

组合意义：

假设我们已经完成了选择。那么这 $n$ 个数就被分成了两类： - 被选中的数 - 没被选中的数

我们考虑把$n - k$没被选中的数分别排成一列,产生了 $n - k + 1$ 个空,然后在这 $n - k + 1$ 个空里面插入 $k$ 个板子 ,答案就是 $binom(n - k + 1, k)$ 种。

构造双射：

问题等价于 我们需要从 $1 \, 2 \, dots.h \, n$ 中选出 $k$ 个数，设这就 $k$ 个数从小到大排序后为 $a_1 \, a_2 \, dots.h \, a_k$。 满足： $ 1 lt.eq a_1 < a_2 < dots.h < a_k lt.eq n $ 且 $ a_(i + 1) - a_i gt.eq 2 quad ( 1 lt.eq i < k ) $ 考虑构造 $ b_i = a_i - ( i - 1 ) $ 原式变为 $ b_i < b_(i + 1) $ $b_i$ 上界 $ b_k = a_k - ( k - 1 ) lt.eq n - ( k - 1 ) = n - k + 1 $ 经过变换后，原问题等价于： 从整数集合 ${ 1 \, 2 \, dots.h \, n - k + 1 }$ 中选出 $k$ 个互不相同的整数 $b_1 < b_2 < dots.h < b_k$ 这个问题是简单的，请读者自己算

==== 组合恒等式
<组合恒等式>
===== 1. 对称式
<对称式>
$ binom(n, m) = binom(n, n - m) $ 组合意义: 你 $n$ 中拿 $m$ 个等价于有 $n - m$ 个不拿。

===== 2. 二项式定理/多项式定理
<二项式定理多项式定理>
$ ( x + y )^n = sum_(i = 0)^n binom(n, i) x^i y^(n - i) $ 组合意义： 我们把 $( x + y )^n$ 看作是有 $n$ 个 $( x + y )$ 相乘，那么得到一个 $x^a y^(n - a)$ 相当于是从 $n$ 个 $( x + y )$ 中选出 $a$ 个 $( x + y )$ 中的 $x$ 相乘，那么结果的多项式中就有一项是 $binom(n, a) x^a y^(n - a)$ 。所有的这种项都满足这种情况，那么公式可得。

其实不一定要求必须是两元的，多元的也是同理。 然后我们可以得到#strong[多项式定理];：

$ ( x_1 + x_2 + dots.h + x_m )^n = sum_(n_1 + n_2 + dots.h + n_m = n) binom(n, n_1 \, n_2 \, dots.h \, n_m) x_1^(n_1) x_2^(n_2) dots.h.c x_m^(n_m) $

证明： 同二项式定理证明，对于 $x_1^(n_1) x_2^(n_2) dots.h.c x_m^(n_m)$ 这一项的系数为 $binom(n, n_1) binom(n - n_1, n_2) dots.h.c binom(n - n_1 - n_2 - dots.h - n_(m - 1), n_m)$ 。 展开发现可以抵消一些东西，于是上面的系数就等于：

$ frac(n !, n_1 ! n_2 ! dots.h.c n_m !) = binom(n, n_1 \, n_2 \, dots.h \, n_m) $

其实这个式子就是多重集排列数

===== 3. 帕斯卡定理
<帕斯卡定理>
$ binom(n, k) = binom(n - 1, k - 1) + binom(n - 1, k) $ 我们知道 $( x + y )^n$ 得到的多项式是系数满足杨辉三角的，我们知道了二项式定理的话，这个东西就是杨辉三角的递推式。

组合意义： 考虑已经选了$n - 1$个数，现在要选第$n$个数，那么第$n$个数选了的话，就相当于从$n - 1$个数中选$k - 1$个数，第$n$个数没选的话，就相当于从$n - 1$个数中选$k$个数。

===== 4. 特殊的二项式定理/第一类二项式反演
<特殊的二项式定理第一类二项式反演>
$ binom(n, 0) + binom(n, 1) + dots.h + binom(n, n) = 2^n $

实际上是 $( 1 + 1 )^n$ ，当然也可以从组合意义上证明 ，当然这是不必要的。

$ sum_(i = 0)^n ( - 1 )^i binom(n, i) = \[ n = 0 \] $

实际上是 $( 1 - 1 )^n$ ，我称之为第一类二项式反演。

===== 5. 递推式 1
<递推式-1>
$ binom(n, k) = n / k binom(n - 1, k - 1) $

按定义来就没了，简单提一下。

代数推导：

$ binom(n, k) & = frac(n !, k ! ( n - k ) !)\
 & = frac(n !, ( k - 1 ) ! ( n - k ) !) dot.op n / k\
 & = n / k binom(n - 1, k - 1) $

这个式子比较深刻 比如，要求解这个经典求和：$ S = sum_(k = 0)^n k binom(n, k) $直接算很难，因为系数里有个 $k$。利用 $k binom(n, k) = n binom(n - 1, k - 1)$，我们可以把 $k$ “吸”进组合数里，把常数 $n$ “吐”出来：$ sum_(k = 1)^n k binom(n, k) & = sum_(k = 1)^n n binom(n - 1, k - 1)\
 & = n sum_(k = 1)^n binom(n - 1, k - 1) quad ( upright("令 ") j = k - 1 )\
 & = n sum_(j = 0)^(n - 1) binom(n - 1, j)\
 & = n dot.op 2^(n - 1) $ \>这个式子正是7.变下项求和式中的第一个式子。

===== 6. 积式
<积式>
$ binom(n, r) binom(r, k) = binom(n, k) binom(n - k, r - k) $ 定义展开左边上下分子分母同乘 $( n - k ) !$ 即可证明。

代数推导： $ binom(n, r) binom(r, k) & = frac(n !, r ! ( n - r ) !) dot.op frac(r !, k ! ( r - k ) !)\
 & = frac(n !, k ! ( n - k ) !) dot.op frac(( n - k ) !, ( r - k ) ! ( n - r ) !)\
 & = binom(n, k) binom(n - k, r - k) $

===== 7. 变下项求和式
<变下项求和式>
$ sum_(k = 0)^n k binom(n, k) = n 2^(n - 1) $

代数推导：

$ sum_(k = 0)^n k binom(n, k) & = sum_(k = 1)^n k binom(n, k)\
 & = sum_(k = 1)^n n binom(n - 1, k - 1)\
 & = n sum_(i = 0)^(n - 1) binom(n - 1, i)\
 & = n 2^(n - 1) $

从二项式定理的求导也可以推出 利用二项式定理：$ ( 1 + x )^n = sum_(k = 0)^n binom(n, k) x^k $两边对 $x$ 求导：$ n ( 1 + x )^(n - 1) = sum_(k = 0)^n k binom(n, k) x^(k - 1) $令 $x = 1$，代入得：$ n ( 1 + 1 )^(n - 1) = sum_(k = 0)^n k binom(n, k) ( 1 )^(k - 1) $ $ n 2^(n - 1) = sum_(k = 0)^n k binom(n, k) $

$ sum_(k = 0)^n k^2 binom(n, k) = n ( n + 1 ) 2^(n - 2) $

证明和上面差不多，就不证了。

===== 8. 变上项求和式
<变上项求和式>
$ sum_(l = 0)^n binom(l, k) = binom(n + 1, k + 1) $

组合意义： 指定 $n + 1$ 个数的集合 $S = { a_1 \, a_2 \, dots.h \, a_(n + 1) }$。 先考虑右边的组合意义，即从 $n + 1$ 个数中选出 $k + 1$ 个。 左边的组合意义：相当于是总共 $n + 1$ 种不累加的情况的加法原理。

第一种：指定必须选择 $a_1$，然后从剩余的 $n$ 个数中选择 $k$ 个，方案数 $binom(n, k)$。

第二种：指定必须不选择 $a_1$ 而选择 $a_2$ ，然后从剩余的 $n - 1$ 个数中选择 $k$ 个，方案数 $binom(n - 1, k)$。

第三种：指定必须不选择 $a_1 \, a_2$ 而选择 $a_3$ ，然后从剩余的 $n - 2$ 个数中选择 $k$ 个，方案数 $binom(n - 2, k)$。

$ dots.v $

第 $n + 1$ 种：指定必须不选择 $a_1 \, a_2 \, dots.h \, a_n$ 而选择 $a_(n + 1)$ ，然后从剩余的 $0$ 个数中选择 $k$ 个，方案数 $binom(0, k)$。

总体的组合意义 $sum_(i = 0)^n binom(i, k)$ 等价于从 $n + 1$ 个数中选出 $k + 1$ 个，那么等式左右两边组合意义相同，等式成立。

===== 8.1 变上项求和式 (扩展)
<变上项求和式-扩展>
$ sum_(i = 0)^n binom(i + m, m) = binom(n + m + 1, m + 1) $

另外的一种常用的形式是上下项共变的。 证明和上面的式子是类似的。 但是我们采取代数推导： $ S = underbrace(binom(m + 1, m + 1), upright("原") binom(m, m)) + binom(m + 1, m) + binom(m + 2, m) + dots.h + binom(n + m, m) $

$ S = underbrace(binom(m + 2, m + 1) + binom(m + 2, m), upright("合并这两项")) + dots.h + binom(n + m, m) $

一直合并 Q.E.D.

===== 9. 积和式 (范德蒙德卷积)
<积和式-范德蒙德卷积>
$ sum_(k = 0)^r binom(m, k) binom(n, r - k) = binom(m + n, r) \, quad r lt.eq min { n \, m } $

组合意义： 指定集合 $A = { a_1 \, a_2 \, dots.h \, a_m }$ 和 $B = { b_1 \, b_2 \, dots.h \, b_n }$。 右边问题等价于从 $A \, B$ 两个集合中选出 $r$ 个的方案数。 左边问题等价于： 对于 $k in \[ 0 \, r \]$，先在 $A$ 中先取出 $k$ 个，然后在 $B$ 中选出 $r - k$ 个的总方案数。 即 $A \, B$ 中总共选出 $r$ 个的方案数。 所以左右两个问题组合意义等价，等式成立。

$ sum_(i = 0)^n binom(n, i)^2 = binom(2 n, n) $

这是 #strong[9.积和式] 的一种特殊情况，代入 $m = n$ 即可。

$ sum_(k = 0)^r binom(k, a) binom(r - k, b) = binom(r + 1, a + b + 1) $

几何意义： 指定集合 $S = { a_1 \, a_2 \, dots.h \, a_(r + 1) }$。 右边问题等价于从 $S$ 中选出 $a + b + 1$ 个数。 左边问题等价于： 第 $i$ 种，指定选出 $a_i$ ，然后再在 $\[ 1 \, i - 1 \]$ 和 $\[ i + 1 \, r \]$ 中分别选出 $a$ 个和 $b$ 个。 于是这几种合并起来就等价于 $binom(r + 1, a + b + 1)$ 。 因此左右两边组合意义等价，等式成立。

为啥不重复？ 我们考虑 $S$ 是升序的 $a_i$ ，即为第 $a + 1$ 小，我们钦定 $i$ 的时候保证了独立性

===== 10. 第二类二项式反演
<第二类二项式反演>
$ f ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i ) $

代数推导： 已知：$g ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i )$ 则：

$ sum_(i = 0)^n ( - 1 )^i binom(n, i) g ( i ) & = sum_(i = 0)^n ( - 1 )^i binom(n, i) sum_(j = 0)^i ( - 1 )^j binom(i, j) f ( j )\
 & = sum_(i = 0)^n sum_(j = 0)^i ( - 1 )^(i + j) binom(n, i) binom(i, j) f ( j )\
 & = sum_(i = 0)^n sum_(j = 0)^i ( - 1 )^(i + j) binom(n, j) binom(n - j, i - j) f ( j )\
 & = sum_(j = 0)^n ( - 1 )^j binom(n, j) f ( j ) sum_(i = j)^n ( - 1 )^i binom(n - j, i - j)\
 & = sum_(j = 0)^n ( - 1 )^j binom(n, j) f ( j ) sum_(i = 0)^(n - j) ( - 1 )^(i + j) binom(n - j, i)\
 & = sum_(j = 0)^n binom(n, j) f ( j ) sum_(i = 0)^(n - j) ( - 1 )^i ( - 1 )^(2 j) binom(n - j, i)\
 & = sum_(j = 0)^n binom(n, j) f ( j ) sum_(i = 0)^(n - j) ( - 1 )^i binom(n - j, i)\
 & = sum_(j = 0)^n binom(n, j) f ( j ) \[ n - j = 0 \]\
 & = f ( n ) $

积式-\>变换求和顺序-\>第一类二项式反演

然后我们还可以得到一个扩展：

$ f ( n ) = sum_(i = 0)^n binom(n, i) g ( i ) arrow.l.r.double g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i ) $

证明： 令 $G ( n ) = ( - 1 )^n g ( n )$ ，有： 若 $f ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) G ( i )$ 成立，有 $G ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i )$。 即

$ G ( n ) = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i ) $

$ therefore g ( n ) ( - 1 )^n = sum_(i = 0)^n ( - 1 )^i binom(n, i) f ( i ) $

当 $n$ 为偶数，则 $n - i$ 与 $i$ 奇偶性相同，有 $g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i )$ 当 $n$ 为奇数，则 $n - i$ 与 $i$ 奇偶性相反，同样有 $g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i )$ 由此有 $g ( n ) = sum_(i = 0)^n ( - 1 )^(n - i) binom(n, i) f ( i )$

===== 11.浅对角线求和
<浅对角线求和>
$ sum_(i = 0)^n binom(n - i, i) = F_(n + 1) $ 组合意义：爬楼梯 (Climbing Stairs)问题： 假设你要爬一个 $n$ 级台阶的楼梯。你每次只能走 1阶 或 2阶。问有多少种不同的爬法？ 角度 A：动态规划（对应右边 $F_(n + 1)$）设 $f ( n )$ 为爬 $n$ 级台阶的方法数。 最后一步可能是迈了 1 阶（前一步在 $n - 1$），或者是迈了 2 阶（前一步在 $n - 2$）。 递推公式：$f ( n ) = f ( n - 1 ) + f ( n - 2 )$。初始值：$f ( 0 ) = 1$ (不动也是一种), $f ( 1 ) = 1$。这是标准的斐波那契数列定义。 所以总方案数对应 $F_(n + 1)$。 角度 B：枚举“迈2阶”的次数（对应左边 $sum$）我们换一种数法：按“一共迈了几次 2 阶”来分类。假设我们在整个过程中，一共迈了 $i$ 次 2阶。消耗台阶数：这 $i$ 次 2 阶共消耗了 $2 i$ 级台阶。剩余台阶数：剩下的 $n - 2 i$ 级台阶，必须全部由 1阶 走完（共 $n - 2 i$ 次 1 阶）。 总步数： $ upright("总步数") = ( upright("2阶的次数") ) + ( upright("1阶的次数") ) = i + ( n - 2 i ) = n - i $排列组合：我们要在这 $n - i$ 步中，选出哪 $i$ 步是走“2阶”的。这相当于从 $n - i$ 个位置中选 $i$ 个位置。方案数为：$binom(n - i, i)$。结论既然所有的方案就是枚举 $i$ 从 $0$ 到最大可能值（$floor.l n \/ 2 floor.r$），把这些情况加起来，就是总的爬楼梯方案数。$ sum_i binom(n - i, i) = upright("爬n阶楼梯的总方案数") = F_(n + 1) $

==== 多重集的组合数 1
<多重集的组合数-1>
设 $S = { n_1 dot.op a_1 \, n_2 dot.op a_2 \, dots.h.c \, n_k dot.op a_k }$ 表示由 $n_1$ 个 $a_1$，$n_2$ 个 $a_2$，…，$n_k$ 个 $a_k$ 组成的多重集。那么对于整数 $r ( r < n_i \, forall i in \[ 1 \, k \] )$，从 $S$ 中选择 $r$ 个元素组成一个多重集的方案数就是 #strong[多重集的组合数];。这个问题等价于 $x_1 + x_2 + dots.h.c + x_k = r$ 的非负整数解的数目，可以用插板法解决，答案为

$ binom(r + k - 1, k - 1) $

==== 多重集的组合数 2
<多重集的组合数-2>
考虑这个问题：设 $S = { n_1 dot.op a_1 \, n_2 dot.op a_2 \, dots.h.c \, n_k dot.op a_k \, }$ 表示由 $n_1$ 个 $a_1$，$n_2$ 个 $a_2$，…，$n_k$ 个 $a_k$ 组成的多重集。那么对于正整数 $r$，从 $S$ 中选择 $r$ 个元素组成一个多重集的方案数。

这样就限制了每种元素的取的个数。同样的，我们可以把这个问题转化为带限制的线性方程求解：

$ forall i in \[ 1 \, k \] \, med x_i lt.eq n_i \, med sum_(i = 1)^k x_i = r $

于是很自然地想到了容斥原理。容斥的模型如下：

+ 全集：$sum_(i = 1)^k x_i = r$ 的非负整数解。
+ 属性：$x_i lt.eq n_i$。

于是设满足属性 $i$ 的集合是 $S_i$，$overline(S_i)$ 表示不满足属性 $i$ 的集合，即满足 $x_i gt.eq n_i + 1$ 的集合（转化为上面插板法的问题三）。

怎么转化的？ e.g. 我们强制分配 $n_i + 1$ 个 $a_i$ ，剩下的 $r - ( n_i + 1 )$ 个 $a_i$ 可以任意取，所以方案数为 $binom(r - ( n_i + 1 ) + k - 1, k - 1)$

那么答案即为

$ lr(|sect.big_(i = 1)^k S_i|) = \| U \| - lr(|union.big_(i = 1)^k overline(S_i)|) $

根据容斥原理，有：

$ lr(|union.big_(i = 1)^k overline(S_i)|) = & sum_i lr(|overline(S_i)|) - sum_(i \, j) lr(|overline(S_i) sect overline(S_j)|) + sum_(i \, j \, k) lr(|overline(S_i) sect overline(S_j) sect overline(S_k)|) - dots.h.c\
 & + ( - 1 )^(k - 1) lr(|sect.big_(i = 1)^k overline(S_i)|)\
=== & sum_i binom(k + r - n_i - 2, k - 1) - sum_(i \, j) binom(k + r - n_i - n_j - 3, k - 1) + sum_(i \, j \, k) binom(k + r - n_i - n_j - n_k - 4, k - 1) - dots.h.c\
 & + ( - 1 )^(k - 1) binom(k + r - sum_(i = 1)^k n_i - k - 1, k - 1) $

拿全集 $\| U \| = binom(k + r - 1, k - 1)$ 减去上式，得到多重集的组合数

$ A n s = sum_(p = 0)^k ( - 1 )^p sum_A binom(k + r - 1 - sum_A n_(A_i) - p, k - 1) $

其中 $A$ 是充当枚举子集的作用，满足 $\| A \| = p \, med A_i < A_(i + 1)$。


== 数论笔记(生成函数)

序列 $a$ 的普通生成函数（ordinary generating function，OGF）定义为形式幂级数（其实就是一个多项式()）：

$ F ( x ) = sum_n a_n x^n $

$a$ 既可以是有穷序列，也可以是无穷序列。常见的例子（假设 $a$ 以 $0$ 为起点）：

+ 序列 $a = angle.l 1 \, 2 \, 3 angle.r$ 的普通生成函数是 $1 + 2 x + 3 x^2$。
+ 序列 $a = angle.l 1 \, 1 \, 1 \, dots.h.c angle.r$ 的普通生成函数是 $sum_(n gt.eq 0) x^n$。
+ 序列 $a = angle.l 1 \, 2 \, 4 \, 8 \, 16 \, dots.h.c angle.r$ 的生成函数是 $sum_(n gt.eq 0) 2^n x^n$。
+ 序列 $a = angle.l 1 \, 3 \, 5 \, 7 \, 9 \, dots.h.c angle.r$ 的生成函数是 $sum_(n gt.eq 0) ( 2 n + 1 ) x^n$。

换句话说，如果序列 $a$ 有通项公式，那么它的普通生成函数的系数就是通项公式。

==== 基本运算
<基本运算>
考虑两个序列 $a \, b$ 的普通生成函数，分别为 $F ( x ) \, G ( x )$。那么有

$ F ( x ) plus.minus G ( x ) = sum_n ( a_n plus.minus b_n ) x^n $

因此 $F ( x ) plus.minus G ( x )$ 是序列 $angle.l a_n plus.minus b_n angle.r$ 的普通生成函数。

考虑乘法运算，也就是卷积：

$ F ( x ) G ( x ) = sum_n x^n sum_(i = 0)^n a_i b_(n - i) $

因此 $F ( x ) G ( x )$ 是序列 $angle.l sum_(i = 0)^n a_i b_(n - i) angle.r$ 的普通生成函数。

==== 封闭形式
<封闭形式>
在运用生成函数的过程中，我们不会一直使用形式幂级数的形式，而会适时地转化为封闭形式以更好地化简。

例如 $angle.l 1 \, 1 \, 1 \, dots.h.c angle.r$ 的普通生成函数 $F ( x ) = sum_(n gt.eq 0) x^n$，我们可以发现

$ F ( x ) x + 1 = F ( x ) $

那么解这个方程得到

$ F ( x ) = frac(1, 1 - x) $

这就是 $sum_(n gt.eq 0) x^n$ 的封闭形式。

考虑等比数列 $angle.l 1 \, p \, p^2 \, p^3 \, p^4 \, dots.h.c angle.r$ 的生成函数 $F ( x ) = sum_(n gt.eq 0) p^n x^n$，有

$ F ( x ) p x + 1 & = F ( x )\
F ( x ) & = frac(1, 1 - p x) $

等比数列的封闭形式与展开形式是常用的变换手段。

==== 应用
<应用>
接下来给出一些例题，来介绍生成函数在 OI 中的具体应用。

======= 普通生成函数可以用来解决#strong[多重集合组合数];问题。
<普通生成函数可以用来解决多重集合组合数问题>

#line(length: 100%, stroke: 0.5pt + luma(180))

问题：有 $n$ 种物品，每种物品有 $a_i$ 个，问取 $m$ 个物品的组合数？

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 多重集合组合数
<多重集合组合数>
设从每种物品中取 $b_i$ 个，$0 lt.eq b_i lt.eq a_i$， \
$m = sum_(i = 1)^n b_i$，对于一组选定的 $b_i$ 进行组合的方案数为 #strong[1];。 \
例如，取 3 个 A，1 个 B 的方案就是 {AAAB}；取 2 个 A、2 个 B 的方案就是 {AABB}。 \
那么，所有满足 \
#emph[#strong[b₁ + b₂ + ⋯ + bₙ = m];] 的方案之和，即答案。

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 构造普通生成函数
<构造普通生成函数>
第 1 种物品的生成函数为 $( 1 + x^1 + x^2 + dots.h.c + x^(a_1) )$， \
第 $n$ 种物品的生成函数为 $( 1 + x^1 + x^2 + dots.h.c + x^(a_n) )$。

即 \
$ ( 1 + x^1 + x^2 + dots.h.c + x^(a_1) ) ( 1 + x^1 + x^2 + dots.h.c + x^(a_2) ) dots.h.c ( 1 + x^1 + x^2 + dots.h.c + x^(a_n) ) $ 求 $x^m$ 的系数。

#quote(block: true)[
#strong[注意：] 指数即物品个数，系数即组合数。
]

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 例如：
<例如>
有三种物品，分别有 3、2、1 个，问取 4 个物品的组合数？ \
枚举的话，有 {AAAB, AAAC, AABB, AABC, ABBC}，5 个方案。

构造 \
$ ( 1 + x + x^2 + x^3 ) ( 1 + x + x^2 ) ( 1 + x ) $

逐步展开：

$  & = ( 1 + x + x^2 + x^3 ) ( 1 + x + x^2 ) ( 1 + x )\
 & = ( 1 + x + x^2 + x^3 + x^2 + x^3 + x^4 + x^3 + x^4 + x^5 ) ( 1 + x )\
 & = ( 1 + 2 x + 3 x^2 + 3 x^3 + 3 x^4 + 2 x^5 + x^6 ) ( 1 + x )\
 & = 1 + 3 x + 5 x^2 + 6 x^3 + 5 x^4 + 3 x^5 + x^6 $

$x^4$ 的系数为 5，即答案。

===== HDU - 1085 Holding Bin-Laden Captive!
<hdu---1085-holding-bin-laden-captive>
面值为 1, 2, 5 的硬币分别有 $a_1 \, a_2 \, a_3$ 枚， \
问用这些硬币#strong[不能];组成的最小面值是多少？

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 思路
<思路>
构造生成函数：

$ ( 1 + x^1 + x^2 + dots.h.c + x^(a_1) ) times ( 1 + x^2 + x^4 + dots.h.c + x^(2 a_2) ) times ( 1 + x^5 + x^10 + dots.h.c + x^(5 a_3) ) $

从小到大遍历系数，#strong[为 0 的那一项];就是答案。

#line(length: 100%, stroke: 0.5pt + luma(180))

===== 例如：
<例如-1>
1 分有 1 枚，2 分有 1 枚，5 分有 1 枚：

$  & = ( 1 + x^2 + x^1 + x^3 ) ( 1 + x^5 )\
 & = ( 1 + x^1 + x^2 + x^3 ) ( 1 + x^5 )\
 & = ( 1 + x^5 + x^1 + x^6 + x^2 + x^7 + x^3 + x^8 )\
 & = 1 + x^1 + x^2 + x^3 + x^5 + x^6 + x^7 + x^8 $

最小不能组成的面值是 4。

===== 食物
<食物>
```
在许多不同种类的食物中选出 $n$ 个，每种食物的限制如下：

1.  承德汉堡：偶数个
2.  可乐：0 个或 1 个
3.  鸡腿：0 个，1 个或 2 个
4.  蜜桃多：奇数个
5.  鸡块：4 的倍数个
6.  包子：0 个，1 个，2 个或 3 个
7.  土豆片炒肉：不超过一个。
8.  面包：3 的倍数个

每种食物都是以「个」为单位，只要总数加起来是 n 就算一种方案。对于给出的 n 你需要计算出方案数，对 10007 取模。
```

这是一道经典的生成函数题。对于一种食物，我们可以设 $a_n$ 表示这种食物选 $n$ 个的方案数，并求出它的生成函数。而两种食物一共选 $n$ 个的方案数的生成函数，就是它们生成函数的卷积。多种食物选 $n$ 个的方案数的生成函数也是它们生成函数的卷积。

在理解了方案数可以用卷积表示以后，我们就可以构造生成函数（标号对应题目中食物的标号）：

+ $sum_(n gt.eq 0) x^(2 n) = frac(1, 1 - x^2)$。
+ $1 + x$。
+ $1 + x + x^2 = frac(1 - x^3, 1 - x)$。
+ $sum_(n gt.eq 0) x^(2 n + 1) = sum_(n gt.eq 0) x^n - sum_(n gt.eq 0) x^(2 n) = frac(x, 1 - x^2)$。
+ $sum_(n gt.eq 0) x^(4 n) = frac(1, 1 - x^4)$。
+ $1 + x + x^2 + x^3 = frac(1 - x^4, 1 - x)$。
+ $1 + x$。
+ $sum_(n gt.eq 0) x^(3 n) = frac(1, 1 - x^3)$。

那么全部乘起来，得到答案的生成函数：

$ F ( x ) = frac(( 1 + x ) ( 1 - x^3 ) x ( 1 - x^4 ) ( 1 + x ), ( 1 - x^2 ) ( 1 - x ) ( 1 - x^2 ) ( 1 - x^4 ) ( 1 - x ) ( 1 - x^3 )) = frac(x, ( 1 - x )^4) $

#strong[广义二项式定理] $ frac(1, ( 1 - x )^n) = sum_(i = 0)^oo C_(n + i - 1)^i x^i $

然后将它转化为展开形式（使用广义二项式定理）：

$ F ( x ) & = x sum_(i = 0)^oo (C_(4 + i - 1)^i) x^i\
 & = sum_(i = 0)^oo (C_(4 + i - 1)^i) x^(i + 1)\
 & = sum_(k = 1)^oo (C_(k + 2)^(k - 1)) x^k\
 & = sum_(k = 1)^oo (C_(k + 2)^3) x^k $

因此答案为

$ C_(n + 2)^3 $

==== 指数生成函数
<指数生成函数>
指数生成函数：

$ F ( x ) = sum_(n gt.eq 0) a_n frac(x^n, n !) $

序列 $< 1 \, 1 \, 1 \, dots.h.c >$ 的指数生成函数是 $ 1 + frac(x, 1 !) + frac(x^2, 2 !) + frac(x^3, 3 !) + dots.h.c = sum_(n gt.eq 0) frac(x^n, n !) = e^x $

序列 $< 1 \, p \, p^2 \, dots.h.c >$ 的指数生成函数是 $ 1 + p frac(x, 1 !) + p^2 frac(x^2, 2 !) + p^3 frac(x^3, 3 !) + dots.h.c = sum_(n gt.eq 0) p^n frac(x^n, n !) = e^(p x) $

===== 基本运算
<基本运算-1>
加减运算

$ F ( x ) plus.minus G ( x ) = sum_(i gt.eq 0) a_i frac(x^i, i !) plus.minus sum_(j gt.eq 0) b_j frac(x^j, j !) = sum_(n gt.eq 0) ( a_n plus.minus b_n ) frac(x^n, n !) $

因此 $F ( x ) plus.minus G ( x )$ 是序列 $< a_n plus.minus b_n >$ 的指数生成函数。

乘法运算（卷积）

$ F ( x ) G ( x ) = sum_(i gt.eq 0) a_i frac(x^i, i !) sum_(j gt.eq 0) b_j frac(x^j, j !) = sum_(n gt.eq 0) x^n sum_(i = 0)^n a_i b_(n - i) frac(1, i ! ( n - i ) !) = sum_(n gt.eq 0) frac(x^n, n !) sum_(i = 0)^n frac(n !, i ! ( n - i ) !) a_i b_(n - i) = sum_(n gt.eq 0) frac(x^n, n !) sum_(i = 0)^n C_n^i a_i b_(n - i) $

因此 $F ( x ) G ( x )$ 是序列 $< sum_(i = 0)^n C_n^i a_i b_(n - i) >$ 的指数生成函数。

===== 封闭形式
<封闭形式-1>
我们同样考虑指数生成函数的封闭形式。

序列 $angle.l 1 \, 1 \, 1 \, dots.h.c angle.r$ 的指数生成函数是：

$ hat(F) ( x ) = sum_(n gt.eq 0) frac(x^n, n !) = upright(e)^x $

因为你将 $upright(e)^x$ 在 $x = 0$ 处泰勒展开就得到了它的无穷级数形式。

类似地，等比数列 $angle.l 1 \, p \, p^2 \, dots.h.c angle.r$ 的指数生成函数是：

$ hat(F) ( x ) = sum_(n gt.eq 0) frac(p^n x^n, n !) = upright(e)^(p x) $

===== 指数生成函数可以用来解决多重集排列数问题。
<指数生成函数可以用来解决多重集排列数问题>
HDU - 1521 排列组合 题意：有 $n$ 种物品，每种物品有 $a_i$ 个，问取 $m$ 个物品的排列数？

多重集排列数 设从每种物品中取 $b_i$ 个，$0 lt.eq b_i lt.eq a_i$，$m = sum_(i = 1)^n b_i$，对于一组选定的 $b_i$ 进行排列的方案数为 $frac(m !, b_1 ! b_2 ! dots.h.c b_n !)$。若 $m$ 个物品互不相同，其排列数为 $m !$，分母就是对每种相同物品的排列数去重。 例如，取3个A、1个B的排列数为 $frac(4 !, 3 ! 1 !) = 24 / 6 = 4$，即 {AAAA, AABA, ABAA, BAAA}。

取2个A、2个B的排列数为 $frac(4 !, 2 ! 2 !) = 24 / 4 = 6$，即 {AABB, ABAB, ABBA, BAAB. BABA, BBAA}。 那么，所有满足 $b_1 + b_2 + dots.h.c + b_n = m$ 的排列数之和，即答案。

构造指数生成函数 第1种物品的生成函数为 $( 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + dots.h.c + frac(x^(a_1), a_1 !) )$，第n种物品的生成函数为 $( 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + dots.h.c + frac(x^(a_n), a_n !) )$。 即 $( 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + dots.h.c + frac(x^(a_1), a_1 !) ) ( 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + dots.h.c + frac(x^(a_2), a_2 !) ) dots.h.c ( 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + dots.h.c + frac(x^(a_n), a_n !) )$，求 $frac(x^m, m !)$ 的系数。

做乘法，$frac(x^(b_1), b_1 !) times frac(x^(b_2), b_2 !) times dots.h.c times frac(x^(b_n), b_n !) = frac(x^(b_1 + b_2 + dots.h.c + b_n), b_1 ! b_2 ! dots.h.c b_n !) = frac(x^m, b_1 ! b_2 ! dots.h.c b_n !) = frac(m !, b_1 ! b_2 ! dots.h.c b_n !) dot.op frac(x^m, m !)$。 做卷积，所有满足 $b_1 + b_2 + dots.h.c + b_n = m$ 的项的系数之和，再乘以 $m !$，即答案。

==== 一点小结论（前已述及）
<一点小结论前已述及>
#block[
#set enum(numbering: "(1)", start: 1)
+ 序列 $a$ 的普通生成函数：$F ( x ) = sum a_n x^n$

+ 序列 $a$ 的指数生成函数：$F ( x ) = sum a_n frac(x^n, n !)$
]

#line(length: 100%, stroke: 0.5pt + luma(180))

#strong[泰勒展开式]

普通生成函数： $ frac(1, 1 - x) = 1 + x + x^2 + x^3 + dots.h.c = sum_(n = 0)^oo x^n $ $ frac(1, 1 - x^2) = 1 + x^2 + x^4 + dots.h.c $ $ frac(1, 1 - x^3) = 1 + x^3 + x^6 + dots.h.c $ $ frac(1, ( 1 - x )^2) = 1 + 2 x + 3 x^2 + dots.h.c $

指数生成函数： $ e^x = 1 + frac(x^1, 1 !) + frac(x^2, 2 !) + frac(x^3, 3 !) + dots.h.c = sum_(n = 0)^oo frac(x^n, n !) $ $ e^(- x) = 1 - frac(x^1, 1 !) + frac(x^2, 2 !) - frac(x^3, 3 !) + dots.h.c $ $ frac(e^x + e^(- x), 2) = 1 + frac(x^2, 2 !) + frac(x^4, 4 !) + dots.h.c $ $ frac(e^x - e^(- x), 2) = x + frac(x^3, 3 !) + frac(x^5, 5 !) + dots.h.c $

#line(length: 100%, stroke: 0.5pt + luma(180))

#strong[有穷序列的生成函数] $ 1 + x + x^2 = frac(1 - x^3, 1 - x) $ $ 1 + x + x^2 + x^3 = frac(1 - x^4, 1 - x) $

#line(length: 100%, stroke: 0.5pt + luma(180))

#strong[广义二项式定理] $ frac(1, ( 1 - x )^n) = sum_(i = 0)^oo C_(n + i - 1)^i x^i $

#strong[证明：]

二项式定理： $ ( 1 + x )^n = sum_(i = 0)^n C_n^i x^i $

#block[
#set enum(numbering: "(1)", start: 1)
+ 扩展域： $ ( 1 + x )^n = sum_(i = 0)^oo C_n^i x^i \, quad upright("当 ") i > n upright(" 时 ") C_n^i = 0 $

+ 扩展指数为负数： $ C_(- n)^i = ( - n ) ( - n - 1 ) dots.h.c ( - n - i + 1 ) $ $ = ( - 1 )^i dot.op frac(n ( n + 1 ) dots.h.c ( n + i - 1 ), i !) = ( - 1 )^i C_(n + i - 1)^i $ $ ( 1 + x )^(- n) = sum_(i = 0)^oo C_(- n)^i x^i $ $ = sum_(i = 0)^oo ( - 1 )^i C_(n + i - 1)^i x^i $

+ 括号内的加号变减号： $ ( 1 - x )^(- n) = sum_(i = 0)^oo ( - 1 )^i C_(n + i - 1)^i ( - x )^i $ $ = sum_(i = 0)^oo C_(n + i - 1)^i x^i $
]

证毕。


= 变换与多项式

== FFT笔记

#figure(image("../../数论/md.img/FFT1.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT2.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT3.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT4.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT5.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT6.png"),
  caption: [
    本地图片
  ]
)

#figure(image("../../数论/md.img/FFT7.png"),
  caption: [
    本地图片
  ]
)


== 数论笔记(sosdp&fmt&fwt)

===== sosdp(高维前缀和)
<sosdp高维前缀和>
我们要求 $d p \[ i \]$ 的所有子集 $j subset.eq i$ 的 $A \[ j \]$ 之和。

考虑二维前缀和的另外一种求法 想象一个二维矩阵，我们怎么用“笨办法”求前缀和？ 我们可以分两步走：

先只处理行（x轴）： 对每一行单独做 1D 前缀和。

再只处理列（y轴）： 基于第一步的结果，对每一列单独做 1D 前缀和。

====== 推广到 $N$ 维（比特位）
<推广到-n-维比特位>
在 SOS DP 中，我们把一个数字的二进制表示看作维度。 比如数字 $5 ( 101_2 )$，可以看作是一个 3 维坐标 $( 1 \, 0 \, 1 )$。 求“子集和”（即 $i$ 的所有子集 $j subset.eq i$ 的 $A \[ j \]$ 之和），本质上就是在求这个 $N$ 维超立方体的“高维前缀和”。

我们只需要：

+ 先处理第 0 位（维度 0）：把所有第 0 位是 0 的数，累加到第 0 位是 1 的数上。
+ 再处理第 1 位（维度 1）：基于上一步，把所有第 1 位是 0 的数，累加到第 1 位是 1 的数上。
+ …
+ 直到处理完第 $N - 1$ 位。

#quote(block: true)[
如果泥玩过2048的话，应该能理解这个方法
]

```cpp
// dp[mask] 初始存的是单点的值 A[mask]
// n 是二进制的位数（维度）

for (int j = 0; j < n; ++j) { // 枚举每一个维度（每一位）
    for (int i = 0; i < (1 << n); ++i) { // 枚举所有状态
        if ((i >> j) & 1) { // 如果当前状态 i 在第 j 维上是 1
            // 那么它包含“第 j 维是 0”的那个状态（i ^ (1 << j)）
            dp[i] += dp[i ^ (1 << j)];
        }
    }
}
```

#quote(block: true)[
我们追踪一下dp\[7(111)\]
]

假设 $N = 3$，我们要算 $d p \[ 111_2 \]$ (即 $d p \[ 7 \]$)。

+ #strong[初始：] $d p \[ 7 \]$ 只包含 $A \[ 7 \]$。
+ #strong[`j=0` (处理第0位)：] $d p \[ . . .1 \] + = d p \[ . . .0 \]$。 此时 $d p \[ 111 \]$ 加上了 $d p \[ 110 \]$。 #emph[意义：现在 $d p \[ 111 \]$ 包含了 ${ 111 \, 110 }$ 的和。]
+ #strong[`j=1` (处理第1位)：] $d p \[ . .1 . \] + = d p \[ . .0 . \]$。 此时 $d p \[ 111 \]$ 加上了 $d p \[ 101 \]$。 #emph[注意：] 这里的 $d p \[ 101 \]$ 在上一轮已经加上了 $d p \[ 100 \]$。 #emph[意义：现在 $d p \[ 111 \]$ 包含了 ${ 111 \, 110 \, 101 \, 100 }$ 的和。]
+ #strong[`j=2` (处理第2位)：] $d p \[ 1 . . \] + = d p \[ 0 . . \]$。 此时 $d p \[ 111 \]$ 加上了 $d p \[ 011 \]$。 #emph[注意：] $d p \[ 011 \]$ 在前两轮已经包含了 ${ 011 \, 010 \, 001 \, 000 }$。 #emph[意义：现在 $d p \[ 111 \]$ 包含了所有 8 个子集。]

====== 超集和
<超集和>
换个符号即可

```cpp
for (int j = 0; j < n; ++j) {
    for (int i = 0; i < (1 << n); ++i) {
        if (!((i >> j) & 1)) { // 如果第 j 位是 0
            // 它的超集包括第 j 位是 1 的那个状态
            dp[i] += dp[i ^ (1 << j)]; 
        }
    }
}
```

#quote(block: true)[
如果做高维差分 反着做前缀和即可
]

====== 例题：luogu P5495 【模板】Dirichlet 前缀和
<例题luogu-p5495-模板dirichlet-前缀和>
唯一分解定理：$N = product p_i^(c_i)$，其中 $p_i$ 是质数，$c_i$ 是次数。

我们把 $p_i$ 看作维度，$c_i$ 看作第 $p_i$ 维的值。 $e . g .$ $12 = 2^2 times 3^1$，那么 $12$ 可以看作是 $( 2 \, 1 \, 0 . . . )$。

考虑先枚举 $p_i$， 再在每个维度上做偏序关系转移即可

====== 例题：CF449D
<例题cf449d>
考虑答案是st的超集的方案数是好求的：

我们首先统计`a[i]`是`st`的超集的个数 这里用下sosdp求

答案即为$2^(c n t \[ s t \]) - 1$

现在我们有至少 考虑恰好的关系

用一下超集反演： $ f ( S ) = sum_(S subset.eq T) g ( T ) arrow.l.r.double g ( S ) = sum_(S subset.eq T) ( - 1 )^(\| T \| - \| S \|) f ( T ) $

我们知道 $ f ( 0 ) = sum_(S subset.eq T) g ( T ) $ \>这里的f函数含义是 答案是0的超集的方案数 \>右边是 对所有0的超集 恰好为t的方案数

所以 $ g ( 0 ) = sum_(S subset.eq T) ( - 1 )^(\| T \|) f ( T ) $

#quote(block: true)[
其实这是简单容斥
]

容斥求解即可

===== FMT(快速莫比乌斯变换)
<fmt快速莫比乌斯变换>
回忆多项式乘法 $ C_k = sum_(i + j = k) A_i times B_j $

考虑把下标 换成按位或 $ C_k = sum_(i or j = k) A_i times B_j $

#quote(block: true)[
你如果把下标看成集合意义下的状压的话 可能比较好理解一点 e.g.~$i or j = k$ 就是说 $i$ 和 $j$ 的并集是 $k$
]

这就是所谓的“按位或卷积”

我们知道,FFT的原理是把系数乘法换成点值乘法，以达到加速的目的

or卷积也能这样加速

定义： $ F M T ( A ) \[ k \] = sum_(i subset.eq k) A \[ i \] $

#quote(block: true)[
注意这是子集和的形式
]

考虑： $ F M T ( A ) \[ k \] times F M T ( B ) \[ k \] = ( sum_(i subset.eq k) A \[ i \] ) times ( sum_(j subset.eq k) B \[ j \] ) $ $ = sum_(i subset.eq k) sum_(j subset.eq k) A \[ i \] B \[ j \] $

注意条件：如果 $i$ 是 $k$ 的子集，且 $j$ 也是 $k$ 的子集，那么 $i or j$ 一定也是 $k$ 的子集。 所以上面那坨式子其实等于： $ sum_(( i or j ) subset.eq k) A \[ i \] B \[ j \] $

按定义，我们知道： $ F M T ( C ) \[ k \] = sum_(i subset.eq k) C \[ i \] = sum_(( i or j ) subset.eq k) A \[ i \] B \[ j \] $

所以： $ F M T ( A ) \[ k \] times F M T ( B ) \[ k \] = F M T ( C ) \[ k \] $

所以我们可以把多项式乘法变成 $F M T$ 乘法，然后再 $I F M T$ 回去。

#quote(block: true)[
这里的or卷积的FMT就是高维前缀和，IMFT就是高维前缀差分
]

同样的我们有and卷积

$C_k = sum_(i and j = k) A_i times B_j$

推导基本从上，恕我从略，只要把高维前缀和变成高维后缀和，高维前缀差分变成高维后缀差分即可

===== FWT(快速沃尔什变换)
<fwt快速沃尔什变换>
FWT除了可以处理and/or卷积，还可以处理异或卷积

异或卷积： $ C_k = sum_(i xor j = k) A_i times B_j $

下面详细推导一下xor卷积的FWT

构造： $ F W T ( A )_k = sum_(i = 0)^(2^n - 1) ( - 1 )^(\| i sect k \|) A_i $

其中 $\| x \|$ 表示 $x$ 二进制表示中 $1$ 的个数（popcount）。 $\| i sect k \|$ 表示 $i$ 和 $k$ 按位与之后，包含 $1$ 的个数。 $( - 1 )^(\| i sect k \|)$ 实际上是在检测 $i$ 和 $k$ 的“相关性”奇偶。

我们要证明：$F W T ( C )_k = F W T ( A )_k dot.op F W T ( B )_k$。 推导左边 (LHS)：根据定义，把 $C_p = sum_(i xor j = p) A_i B_j$ 代入： $ F W T ( C )_k & = sum_p ( - 1 )^(\| p sect k \|) C_p\
 & = sum_p ( - 1 )^(\| p sect k \|) (sum_(i xor j = p) A_i B_j) $ 由于 $p = i xor j$，我们可以把求和号展开，直接枚举 $i$ 和 $j$： $ F W T ( C )_k = sum_i sum_j ( - 1 )^(\| ( i xor j ) sect k \|) A_i B_j $ 关键性质：$ \| ( i xor j ) sect k \| equiv \| i sect k \| + \| j sect k \| med (mod 2) $ 解释：$i$ 和 $j$ 在第 $m$ 位异或为 1，当且仅当它们在该位上一个为 0 一个为 1。与 $k$ 做 AND 后，只有当 $k$ 在该位也是 1 时才会产生贡献。这一位的贡献要么是 $1 + 0 = 1$，要么是 $0 + 1 = 1$，奇偶性都和加法一致。因为 $( - 1 )^x$ 只关心 $x$ 的奇偶性，所以： $ ( - 1 )^(\| ( i xor j ) sect k \|) = ( - 1 )^(\| i sect k \| + \| j sect k \|) = ( - 1 )^(\| i sect k \|) dot.op ( - 1 )^(\| j sect k \|) $ 代回原式： $ F W T ( C )_k & = sum_i sum_j (( - 1 )^(\| i sect k \|) dot.op ( - 1 )^(\| j sect k \|)) A_i B_j\
 & = (sum_i ( - 1 )^(\| i sect k \|) A_i) dot.op (sum_j ( - 1 )^(\| j sect k \|) B_j)\
 & = F W T ( A )_k dot.op F W T ( B )_k $ 得证。

现在我们推导FWT的分治算法

假设数组 $A$ 的长度为 $2^n$。我们将 $A$ 分成前半部分 $A_0$（下标最高位为 0）和后半部分 $A_1$（下标最高位为 1）。 即： \* $A_0$ 包含下标 $0 tilde.op 2^(n - 1) - 1$ \* $A_1$ 包含下标 $2^(n - 1) tilde.op 2^n - 1$

现在我们要计算 $F W T ( A )$。 对于任意结果下标 $k$，我们把它写成 $( 0 \, k' )$ 或者 $( 1 \, k' )$ 的形式，其中 $k'$ 是去掉最高位剩下的部分。

====== 情况 1：结果下标最高位为 0 ($k < 2^(n - 1)$)
<情况-1结果下标最高位为-0-k-2n-1>
$ F W T ( A )_k & = sum_(i = 0)^(2^n - 1) ( - 1 )^(\| i sect k \|) A_i\
 & = sum_(i = 0)^(2^(n - 1) - 1) ( - 1 )^(\| i sect k \|) A_i + sum_(i = 2^(n - 1))^(2^n - 1) ( - 1 )^(\| i sect k \|) A_i $ \* #strong[左半边和 ($A_0$)];：$i$ 的最高位是 0，$k$ 的最高位是 0。$i sect k$ 不涉及最高位。这部分就是 $F W T ( A_0 )_k$。 \* #strong[右半边和 ($A_1$)];：$i$ 的最高位是 1，$k$ 的最高位是 0。$i sect k$ #strong[依然不涉及最高位];（因为 $1 med & med 0 = 0$）。这部分就是 $F W T ( A_1 )_k$。

#strong[结论 1：] $ F W T ( A )_(l e f t) = F W T ( A_0 ) + F W T ( A_1 ) $

====== 情况 2：结果下标最高位为 1 ($k gt.eq 2^(n - 1)$)
<情况-2结果下标最高位为-1-k-ge-2n-1>
令实际计算的下标为 $k + 2^(n - 1)$（其中 $k < 2^(n - 1)$）。 $ F W T ( A )_(k + 2^(n - 1)) & = sum_(i = 0)^(2^(n - 1) - 1) ( - 1 )^(\| i sect ( k + 2^(n - 1) ) \|) A_i + sum_(i = 2^(n - 1))^(2^n - 1) ( - 1 )^(\| i sect ( k + 2^(n - 1) ) \|) A_i $ \* #strong[左半边和 ($A_0$)];：$i$ 最高位 0。$i sect ( k + 2^(n - 1) )$ 的最高位是 $0 med & med 1 = 0$。最高位无贡献。这部分依然是 $F W T ( A_0 )_k$。 \* #strong[右半边和 ($A_1$)];：$i$ 最高位 1。$i sect ( k + 2^(n - 1) )$ 的最高位是 $1 med & med 1 = 1$。#strong[最高位贡献了 1 个 1];。 所以 $( - 1 )^(\| . . . \|)$ 会多乘一个 $( - 1 )^1 = - 1$。 这部分变成了 $- F W T ( A_1 )_k$。

#strong[结论 2：] $ F W T ( A )_(r i g h t) = F W T ( A_0 ) - F W T ( A_1 ) $

综合上面的推导，我们得到了递归式：$ F W T ( A ) = upright("merge") ( F W T ( A_0 ) \, F W T ( A_1 ) ) $其中 merge 操作为： $ {A'_(l e f t) = A_(0') + A_(1')\
A'_(r i g h t) = A_(0') - A_(1') $

这正是FWT的分治算法。

#quote(block: true)[
IFWT对反解上述过程即可
]

同理 我们可以推导

====== and卷积的FWT
<and卷积的fwt>
$ F W T ( A )_k = sum_(i subset.eq k) A_i $

假设数组 $A$ 长度为 $2^n$。我们将 $A$ 一分为二，同样可以推导 正变换: $ {A'_(l e f t) = A'_0\
A'_(r i g h t) = A'_0 + A'_1 $

逆变换:解上面的方程组求 $A'_0 \, A'_1$： $ {A'_0 = A'_(l e f t)\
A'_1 = A'_(r i g h t) - A'_(l e f t) $

====== or卷积的FWT
<or卷积的fwt>
$ F W T ( A )_k = sum_(k subset.eq i) A_i $

正变换:$ {A'_(l e f t) = A'_0 + A'_1\
A'_(r i g h t) = A'_1 $ 逆变换:解方程组：$ {A'_1 = A'_(r i g h t)\
A'_0 = A'_(l e f t) - A'_(r i g h t) $

====== 例题： luogu P5387 \[Cnoi2019\] 人形演舞
<例题-luogu-p5387-cnoi2019-人形演舞>
观察到游戏是独立的，打表出sg函数 sg(x)=x-highbit(x)+1

问题变成：计算方案数，使得 $S G ( v_1 ) xor S G ( v_2 ) xor dots.h xor S G ( v_n ) eq.not 0$

考虑正难则反 用总方案数 $m^n$减去 $S G ( v_1 ) xor S G ( v_2 ) xor dots.h xor S G ( v_n ) = 0$ 的方案数

我们用一个cnt数组cnt\[k\]统计sg=k的数的个数

答案就是这个数组自卷v次的cnt\[0\]

#quote(block: true)[
为啥？ 假设只有两轮游戏，或者只选了两个数。 第一个数选出的 SG 值为 $i$ 的方案数是 $c n t \[ i \]$。 第二个数选出的 SG 值为 $j$ 的方案数是 $c n t \[ j \]$。 现在我想知道：这两个数的异或和 $i xor j = k$ 的方案数是多少？根据乘法原理，对于固定的 $i$ 和 $j$，方案数是 $c n t \[ i \] times c n t \[ j \]$。我们要找所有满足 $i xor j = k$ 的情况，把它们加起来：$ upright("Total") \[ k \] = sum_(i xor j = k) c n t \[ i \] times c n t \[ j \] $ 这就是xor卷积
]

先fwt一下 然后对点值求v次方 最后ifwt回去即可


= Trick 与杂项

== 数学相关trick

我们在处理某些计数问题时 总是有一下思路 1.定1移1 考虑某个前缀对r的贡献 2.考虑某个点对区间的贡献

gcd(k,n)=1-\>gcd(n-k,n)=1 simga \[gcd(i,n)=1\]#emph[i=phi(n)];n/2 https:\/\/ac.nowcoder.com/acm/contest/view-submission?submissionId=79094134&returnHomeType=1&uid=719203876

原来aj\*2\>ai的时候aj一定不整除ai https:\/\/ac.nowcoder.com/acm/contest/view-submission?submissionId=79086887&returnHomeType=1&uid=719203876

考虑算\[l,r\]中被p整除的数，一个好的实现是先算\[1,r\]中被p整除的数，再减去\[1,l-1\]中被p整除的数。


== 数论笔记(杂项)

==== \# 勒让德公式
<勒让德公式>
==== 勒让德公式是什么
<勒让德公式是什么>
勒让德公式用来算#strong[质数 $p$ 在阶乘 $n !$ 的质因子分解中的指数];。 \
公式是：

$ v_p ( n ! ) = sum_(k = 1)^oo ⌊n / p^k⌋ \, $

其中 $v_p ( n ! )$ 表示 $p$ 在 $n !$ 里出现的次数（指数）。

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 为什么成立
<为什么成立>
考虑 $n ! = 1 dot.op 2 dot.op 3 dots.h.c n$。

- 每隔 $p$ 个数里有一个能被 $p$ 整除，所以至少有 $floor.l n \/ p floor.r$ 个因子 $p$；
- 每隔 $p^2$ 个数里有一个能被 $p^2$ 整除，它会额外贡献一个 $p$，所以再加 $floor.l n \/ p^2 floor.r$；
- 每隔 $p^3$ 个数里有一个能被 $p^3$ 整除，它会再额外贡献一个 $p$…… \
  如此类推，直到 $p^k > n$ 为止。

所以总和就是上面的式子。

#line(length: 100%, stroke: 0.5pt + luma(180))

==== 一个例子
<一个例子>
算 $v_2 ( 10 ! )$：

$ floor.l 10 \/ 2 floor.r + floor.l 10 \/ 4 floor.r + floor.l 10 \/ 8 floor.r = 5 + 2 + 1 = 8 . $

验证： \
$10 ! = 3628800 = 2^8 dot.op 3^4 dot.op 5^2 dot.op 7$.

#figure(
  align(center)[#table(
    columns: 1,
    align: (auto,),
    [在这题里的作用],
  )]
  , kind: table
  )

我们要计算：

$ C_n = frac(1, n + 1) binom(2 n, n) . $

其中涉及到阶乘： \
$binom(2 n, n) = frac(( 2 n ) !, ( n ! )^2)$。

为了在#strong[任意模数 $p$] 下算出结果（尤其是 $p$ 不是质数时），不能用逆元，要直接做#strong[质因子分解];。 \
于是对每个质数 $q$，用勒让德公式求它在 $( 2 n ) !$、$n !$ 中的指数差，得到 $q$ 在组合数中的幂次。

再减去 $n + 1$ 的质因子指数，就是卡特兰数的质因子分解。最后用快速幂拼起来，就得到答案 $#h(0em) mod med p$。

