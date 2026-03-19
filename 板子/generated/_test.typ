#set page(paper: "a4")
#set text(size: 9pt)
= 基础与工具

== 数论笔记(筛法)

===== 欧拉函数的定义

$1 tilde n$ 中与 $n$ 互质的数的个数称为欧拉函数，记为 $phi.alt(n)$  
*例*：$phi.alt(1) = 1,  phi.alt(2) = 1,  phi.alt(3) = 2,  phi.alt(4) = 2,  phi.alt(5) = 4$  

===== 欧拉函数的性质

+ 若 $p$ 是质数，则 $phi.alt(p) = p - 1$  
+ 若 $p$ 是质数，则 $phi.alt(p^k) = (p - 1)p^(k-1)$  
+ *积性函数*：若 $gcd(m,n) = 1$，则 $phi.alt(m n) = phi.alt(m)phi.alt(n)$  

===== 欧拉函数的计算公式

由唯一分解定理 $n = product_(i=1)^s p_i^(alpha_i) = p_1^(alpha_1)p_2^(alpha_2)dots.c p_s^(alpha_s)$，  

$phi.alt(n) 
&= product_(i=1)^s phi.alt(p_i^(alpha_i))  
&= product_(i=1)^s p_i^(alpha_i-1)(p_i - 1)  
&= product_(i=1)^s p_i^(alpha_i) (1 - (1)/(p_i))  
&= ( product_(i=1)^s p_i^(alpha_i) ) times ( product_(i=1)^s (1 - (1)/(p_i)) )  
&= n times product_(i=1)^s (p_i-1)/(p_i)  
&= n times (p_1-1)/(p_1) times (p_2-1)/(p_2) times dots.c times (p_s-1)/(p_s)$

#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[欧拉函数仅由 $n$ 和质因子决定，与次数无关。  ]
*例*：$phi.alt(12) = 12 times (2-1)/(2) times (3-1)/(3) = 4$

==== 筛法求欧拉函数


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[若 $i$ 是质数，$phi.alt[i] = i - 1$。]

*在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的*。  
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = p_j times i$ 筛掉。

===== 分两种情况计算：

+ *若 $i$ 能被 $p_j$ 整除*（即 $i equiv 0 (mod p_j)$），则 $i$ 包含了 $m$ 的所有质因子：  

$   phi.alt(m) 
   &= m times product_(k=1)^s (p_k-1)/(p_k)  
   &= p_j times i times product_(k=1)^s (p_k-1)/(p_k)  
   &= p_j times phi.alt(i)$

   *例*：$phi.alt(12) = phi.alt(2 times 6) = 2 times phi.alt(6)$

+ *若 $i$ 不能被 $p_j$ 整除*（即 $gcd(i, p_j)=1$），则 $i$ 和 $p_j$ 互质：  

$   phi.alt(m) 
   &= phi.alt(p_j times i)  
   &= phi.alt(p_j) times phi.alt(i)  
   &= (p_j - 1) times phi.alt(i)$

   *例*：$phi.alt(75) = phi.alt(3 times 25) = (3 - 1) times phi.alt(25)$
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


==== 问题

给定整数 $n$ ($n lt.eq 10^6$)，输出 $1 tilde n$ 中每个数的约数个数。

==== 约数个数定理

若正整数 $n$ 有质因数分解 $n = product_(i=1)^s p_i^(alpha_i)$，则约数个数为：

$d(n) = product_(i=1)^s (alpha_i + 1)$


===== 证明

- 对每个质因子 $p_i^(alpha_i)$，其约数可取 $p_i^0, p_i^1, dots.c, p_i^(alpha_i)$ 共 $(alpha_i + 1)$ 种选择
- 根据乘法原理，总约数个数为各质因子选择数的乘积：

$d(n) = (alpha_1 + 1) times (alpha_2 + 1) times dots.c times (alpha_s + 1)$



*筛法求约数个数*  
记a[i]为i的最小质因子的次数，d[i]为i的约数个数。
若 $i$ 是质数，  

$a[i] = 1, quad d[i] = 2$

在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。  
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = p_j times i$ 筛掉。  

(1) 若 $i$ 能被 $p_j$ 整除，则 $p_j$ 一定是 $i$ 的最小质因子。  

$a[m] = a[i] + 1;$


$d[i] = (a[i] + 1) times dots.c, quad d[m] = (a[m] + 1) times dots.c$


于是

$d[m] = d[i] times (a[m] + 1)/(a[i] + 1)$

(2) 若 $i$ 不能被 $p_j$ 整除，则 $i$ 不包含质因子 $p_j$。  

$a[m] = 1, quad d[m] = d[i] times (1 + 1)$


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


若 $n = product_(i=1)^s p_i^(alpha_i)$，则 $f(n) = product_(i=1)^s sum_(j=0)^(alpha_i) p_i^j$

*证明：*  
$p_i^(alpha_i)$ 的约数有 $p_i^0, p_i^1, dots.c, p_i^(alpha_i)$ 共 $(alpha_i + 1)$ 个，其约数和为 $sum_(j=0)^(alpha_i) p_i^j$。

根据乘法原理，  

$f(n) = product_(i=1)^s sum_(j=0)^(alpha_i) p_i^j$


*例：*  

$12 = 2^2 times 3^1,$


$f(12) = (1 + 2 + 4) times (1 + 3) = 7 times 4 = 28$


#line(length: 100%, stroke: 0.5pt + luma(180))


=== 筛法求约数和

记g[i]为i的最小质因子的幂和 1 + p^1 +p^2 + ... + p^k，f[i]为i的约数和。
若 $i$ 是质数，  

$g[i] = f[i] = i + 1$


在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = i times p_j$ 筛掉。

(1) 若 $i$ 能被 $p_j$ 整除，则 $p_j$ 一定也是 $i$ 的最小质因子  

$g[i] = p_j^0 + p_j^1 + dots.c + p_j^(alpha_j), quad g[m] = p_j^0 + p_j^1 + dots.c + p_j^(alpha_j+1)$


$f[i] = g[i] times dots.c, quad f[m] = g[m] times dots.c$

于是

$f[m] = f[i] times (g[m])/(g[i])$


(2) 若 $i$ 不能被 $p_j$ 整除，则 $i$ 不包含质因子 $p_j$。  

$g[m] = 1 + p_j$


$f[m] = g[m] times f[i]$


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



$n = product_(i=1)^(s) p_i^(alpha_i) = p_1^(alpha_1) p_2^(alpha_2) dots.c p_s^(alpha_s)$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 莫比乌斯函数定义


莫比乌斯函数记作 $mu(n)$，它是一个经典的数论函数，定义如下：

- $mu(1) = 1$

- 如果 $n$ 含有平方因子（即存在某个质数 $p$，使得 $p^2 mid n$），则 $mu(n) = 0$

- 如果 $n$ 是 $k$ 个*互不相同的质数*的乘积（即 $n = p_1 p_2 dots.c p_k$），则：


$mu(n) = (-1)^k$


#line(length: 100%, stroke: 0.5pt + luma(180))




=== 筛法求莫比乌斯函数


若 $i$ 是质数，$mu[i] = -1$。  
在线性筛中，每个合数 $m$ 都是被最小的质因子筛掉的。  
设 $p_j$ 是 $m$ 的最小质因子，则 $m$ 通过 $m = i times p_j$ 筛掉。  

(1) 若 $i$ 能被 $p_j$ 整除，则 $i$ 也包含质因子 $p_j$。  

$mu[m] = 0$


(2) 若 $i$ 不能被 $p_j$ 整除，则 $m$ 比 $i$ 多一个不同的质因子 $p_j$  
- 若 $mu[i] = -1$, 则 $mu[m] = 1$  
- 若 $mu[i] = 1$, 则 $mu[m] = -1$  
- 若 $mu[i] = 0$, 则 $mu[m] = 0$  
综上，$mu[m] = -mu[i]$。

== 数论笔记(线性逆元)

*O(n)求阶乘和阶乘逆元*
==== 📘 推导目标


给定质数 $p$，我们希望在线性时间内计算 $1$ 到 $n$ 的所有数在模 $p$ 意义下的乘法逆元，即：  

$upright("求 ") forall 1 l e i l e n, quad upright("使得 ") r_i dot.c i equiv 1 (mod p) upright(" 的 ") r_i$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 🧮 推导公式


我们设 $r_i = i^(-1) b m o d p$，有：
- $r_1 = 1$
- 对于 $i > 1$，我们可以利用如下递推式求出 $r_i$：


$r_i = (p - floor( (p)/(i) )) dot.c r_(p b m o d i) b m o d p$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== ✏️ 证明过程




考虑：


$p = i dot.c floor( (p)/(i) ) + (p b m o d i) arrow.r.double p b m o d i = p - i dot.c floor( (p)/(i) )$


两边模p:


$i dot.c ( floor( (p)/(i) ) ) equiv - (p b m o d i) (mod p)
arrow.r.double i dot.c ( floor( (p)/(i) ) ) dot.c (p b m o d i)^(-1) equiv -1 (mod p)$


两边都乘上 $-1$：


$i dot.c ( -floor( (p)/(i) ) ) dot.c (p b m o d i)^(-1) equiv 1 (mod p)$


于是我们得出：


$b o x e d{
upright("i n v")[i] equiv - ( floor( (p)/(i) ) ) dot.c upright("i n v")[p b m o d i] (mod p)
}$


再化简成*无负数形式*：


$b o x e d{
upright("i n v")[i] = (p - p / i) dot.c upright("i n v")[p \% i] b m o d p
}$


这就是我们要用的递推式！


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 🧪 C++ 实现示例


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


- 时间：$m a t h c a l{O}(n)$
- 空间：$m a t h c a l{O}(n)$
- 要求 $p$ 是质数（否则不存在乘法逆元）



== 数论笔记(不定方程与同余方程组)

*前置芝士*
exgcd求解不定方程 ax+by=gcd(a,b) /线性同余方程 ax≡b(mod m) 的解
*exgcd求解不定方程 ax+by=gcd(a,b)*
设

```
ax_1+by_1=gcd(a,b)
```



```
bx_2+(a\%b)y_2=gcd(b,a\%b)
```


由欧几里得定理可得

$gcd(a,b)=gcd(b,a\%b)$


于是

```
ax_1+by_1=bx_2+(a\%b)y_2
```



```
ax_1+by_1=bx_2+(a-\lfloor a/b \rfloor*b)y_2
```


整理

```
ax_1+by_1=ay_2+b(x_2-\lfloor a/b \rfloor*y_2)
```


于是

$x_1=y_2$



$y_1=x_2-floor( a/b )*y_2$

ax+by=gcd(a,b)即可通过最初的x,y求解
于是我们可以通过递归求解
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


*exgcd求解线性同余方程 ax≡b(mod m) 的解*

$a x equiv b (mod  m)$


可写成

$a x+m k=b$


于是我们先求解不定方程

$a x+m k=gcd(a,m)$


若gcd(a,m)!=1则无解，否则得到解

$x=x_0$



$k=k_0$

于是我们得到原方程的解为

$x_1=x_0*b/gcd(a,m)$



$k_1=k_0*b/gcd(a,m)$

方程的任意解(对任意整数t成立)为

$x=x_1+m t$



$k=k_1-a t$

求最小的正整数解

$x=(x_1  mod  t +t)  mod  t$

其中

$t=m/gcd(a,m)$

为何？
因为上述操作等价于

$a/d *x equiv b/d (mod  m/d)$

把a/d 放到右边 会发现x是 模m/d的等价类

要用exgcd求解逆元的话，需要保证gcd(a,m)=1
代入exgcd(a,m,x,y)中,对x值域变换即可
其实就是

$a x equiv 1 (mod  m)$


可写成

$a x+m k=gcd(a,m)=1$

罢了

*中国剩余定理*
求解同余方程组

$x equiv a_1 (mod  m_1) 
x equiv a_2 (mod  m_2) 
... 
x equiv a_k (mod  m_k)$

其中

```
m_1,m_2,...,m_k两两互质
```

过程:
求

$M=m_1*m_2*...*m_k$

对每个m_i求

$M_i=M/m_i$



$M_i^(-1) equiv 1 (mod  m_i)$



$c_i=M_i^(-1) * M_i$

于是

$x=sum_(i=1)^(k)a_i*c_i (mod  M)$


很显然的证明，对任意一个方程组：

$x equiv sum_(i=1)^(k)a_i*c_i (mod  m_i)$



$x equiv a_i*M_i*M_i^(-1) (mod  m_i)$



$x equiv a_i (mod  m_i) * (M_i * M_i^(-1) (mod  m_i))$


按定义


$x equiv a_i (mod  m_i)$

代码：
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


*扩展中国剩余定理*
求解同余方程组

$x equiv a_1 (mod  m_1) 
x equiv a_2 (mod  m_2) 
... 
x equiv a_k (mod  m_k)$

其中

```
m_1,m_2,...,m_k不两两互质
```

过程:
考虑合并两个同余方程

$x equiv a_1 (mod  m_1) 
x equiv a_2 (mod  m_2)$

可写成不定方程

$x=a_1+k_1*m_1 
x=a_2+k_2*m_2$

消去x

$a_1+k_1*m_1=a_2+k_2*m_2$

于是我们得到了一个不定方程

$k_1*m_1+-k_2*m_2=a_2-a_1$

可通过exgcd求解

$K_1*m_1+-K_2*m_2=gcd(m_1,m_2)$

于是

$k_1=(a_2-a_1)/(gcd(m_1,m_2))*K_1$



$k_2=(a_1-a_2)/(gcd(m_1,m_2))*K_2$

得到x的一个解

$x_0=a_1+k_1*m_1=a_1+(a_2-a_1)/(gcd(m_1,m_2))*K_1*m_1$

窝们很显然可以构造x的通解

$x=x_0+t*lcm(m_1,m_2)$

于是进行形式转化

$x equiv x_0 (mod  lcm(m_1,m_2))$

于是我们得到了两个同余方程的合并
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

$upright("gcd")$ 的重要恒等式：
$upright("gcd")(x_1, x_2, dots, x_n) = upright("gcd")(x_1, x_2 - x_1, x_3 - x_2, dots, x_n - x_(n-1))$。

对于一个前缀 $upright("gcd")$ 序列 $g_i = upright("gcd")(a_1, dots, a_i)$，这个序列 $g_1, g_2, dots, g_n$ 的不同取值最多只有 $O(log V)$ 个。
因为下降是$log$的

gcd(x,y)=x xor y-\>gcd(x,y)=y-x

gcd(x,y)=x+y -\> gcd(x,y)=max(x,y)。

处理逆元不存在的方法总列：
考虑前后缀积处理
考虑处理乘0的个数
阶乘考虑列让德公式
考虑卢卡斯定理
考虑特殊性质，如k是9的倍数，要求k/9 mod mod 那么可以先求k%9mod然后/9然后%mod

= 卷积与反演

== 数论笔记(狄利克雷卷积与莫比乌斯反演 1)

===== 狄利克雷生成函数


数列 $angle.l a_1, a_2, a_3, dots.c angle.r$ 的狄利克雷生成函数定义为：

$F(x) = (a_1)/(1^x) + (a_2)/(2^x) + (a_3)/(3^x) + dots.c = sum_(n=1)^(infinity) (a_n)/(n^x)$


#line(length: 100%, stroke: 0.5pt + luma(180))


====== 乘法运算（Dirichlet 卷积）


$sum_(i=1)^(infinity) (a_i)/(i^x) sum_(j=1)^(infinity) (b_j)/(j^x)
&= ( (a_1)/(1^x) + (a_2)/(2^x) + (a_3)/(3^x) + (a_4)/(4^x) + dots.c ) ( (b_1)/(1^x) + (b_2)/(2^x) + (b_3)/(3^x) + (b_4)/(4^x) + dots.c )  
&= (a_1 b_1)/(1^x) + (a_1 b_2)/(2^x) + (a_2 b_1)/(3^x) + (a_1 b_3)/(4^x) + dots.c  
&= sum_(n=1)^(infinity) (1)/(n^x) sum_(d|n) a_d b_(n)/(d)$


====== 系数计算规则

$(1)/(n^x)$ 项的系数等于所有满足 $d|n$（$d$ 整除 $n$）的项 $a_d b_(n/d)$ 之和：
- *$4^x$ 的系数*：$a_1 b_4 + a_2 b_2 + a_4 b_1$  
  （枚举 4 的约数 $d=1,2,4$）
- *$6^x$ 的系数*：$a_1 b_6 + a_2 b_3 + a_3 b_2 + a_6 b_1$  
  （枚举 6 的约数 $d=1,2,3,6$）

===== 一点和式的小结论


=== 欧拉函数


==== 1. 定义

欧拉函数 $phi.alt(n)$ 表示小于等于 $n$ 且与 $n$ 互质的正整数个数：

$phi.alt(n) = sum_(i=1)^n [gcd(i,n) = 1]$


===== 欧拉函数值表

| $n$   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|-------|---|---|---|---|---|---|---|---|---|---|----|----|
| $phi.alt(n)$ | 1 | 1 | 2 | 2 | 4 | 2 | 6 | 4 | 6 | 4  | 10 | 4  |

==== 2. 性质

===== 欧拉函数求和定理

对于任意正整数 $n$，其所有因子的欧拉函数值之和等于 $n$：

$sum_(d|n) phi.alt(d) = n$


===== 验证示例


$phi.alt(1) &= 1  
phi.alt(1) + phi.alt(2) &= 1 + 1 = 2  
phi.alt(1) + phi.alt(3) &= 1 + 2 = 3  
phi.alt(1) + phi.alt(2) + phi.alt(4) &= 1 + 1 + 2 = 4  
phi.alt(1) + phi.alt(5) &= 1 + 4 = 5  
phi.alt(1) + phi.alt(2) + phi.alt(3) + phi.alt(6) &= 1 + 1 + 2 + 2 = 6$




==== 证明思路

考虑以 $n$ 为分母的真分数 $[0,1)$ 区间：

$(0)/(n), (1)/(n), (2)/(n), dots.c, (n-1)/(n)$

将这些分数化简为最简形式后，根据分母分组，可证明结论。

#line(length: 100%, stroke: 0.5pt + luma(180))


==== 示例演示（$n=12$）

===== 所有真分数


$(0)/(12), (1)/(12), (2)/(12), (3)/(12), (4)/(12), (5)/(12), (6)/(12), (7)/(12), (8)/(12), (9)/(12), (10)/(12), (11)/(12)$


===== 化简后分组

| 分母 $d$ | 最简分数                                                                 | 个数 $phi.alt(d)$ |
|---------|--------------------------------------------------------------------------|------------------|
| 1       | $(0)/(1)$                                                           | $phi.alt(1)=1$  |
| 2       | $(1)/(2)$                                                           | $phi.alt(2)=1$  |
| 3       | $(1)/(3),(2)/(3)$                                              | $phi.alt(3)=2$  |
| 4       | $(1)/(4),(3)/(4)$                                              | $phi.alt(4)=2$  |
| 6       | $(1)/(6),(5)/(6)$                                              | $phi.alt(6)=2$  |
| 12      | $(1)/(12),(5)/(12),(7)/(12),(11)/(12)$               | $phi.alt(12)=4$ |

===== 验证等式


$sum_(d|12)phi.alt(d) = phi.alt(1)+phi.alt(2)+phi.alt(3)+phi.alt(4)+phi.alt(6)+phi.alt(12) = 1+1+2+2+2+4=12$


#line(length: 100%, stroke: 0.5pt + luma(180))

注意到窝们的证明思路天然满足定义

==== 一般性证明

+ 考虑所有分母为 $n$ 的真分数：

$(k)/(n) quad (0 lt.eq k < n)$

   共有 $n$ 个分数。

+ 将每个分数化简为最简形式 $(a)/(d)$，其中 $d mid n$ 且 $gcd(a,d)=1$。

+ 对 $n$ 的每个约数 $d$ 分组：
  - 分母为 $d$ 的分数个数 = $phi.alt(d)$
  - 因为分子 $a$ 需满足 $1 lt.eq a lt.eq d$ 且 $gcd(a,d)=1$

+ 总和为：

$sum_(d|n) phi.alt(d) = n$

   即得证。

=== 莫比乌斯函数


==== 1. 定义

莫比乌斯函数 $mu(n)$ 定义如下：

$mu(n) = 
1 & upright("若 ") n = 1   
(-1)^s & upright("若 ") n = p_1p_2 dots.c p_s upright("（无平方因子的整数）")   
0 & upright("若 ") n upright(" 包含平方因子")$


===== 莫比乌斯函数值表

| $n$   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|-------|---|---|---|---|---|---|---|---|---|---|----|----|
| $mu(n)$ | 1 | -1 | -1 | 0 | -1 | 1 | -1 | 0 | 0 | 1  | -1 | 0  |

#line(length: 100%, stroke: 0.5pt + luma(180))


==== 2. 核心性质


$sum_(d|n) mu(d) = [n = 1] = 
1 & n=1  
0 & n>1 $


===== 验证示例


$n=1:&quad mu(1) = 1  
n=2:&quad mu(1) + mu(2) = 1 + (-1) = 0  
n=3:&quad mu(1) + mu(3) = 1 + (-1) = 0  
n=4:&quad mu(1) + mu(2) + mu(4) = 1 + (-1) + 0 = 0  
n=6:&quad mu(1) + mu(2) + mu(3) + mu(6) = 1 + (-1) + (-1) + 1 = 0$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 3. 证明（$n>1$ 时和为 0）


===== 证明思路

设 $n = p_1^(a_1)p_2^(a_2) dots.c p_s^(a_s)$，定义 $n'$ 为 $n$ 的平方自由部分：

$n' = p_1p_2 dots.c p_s$

则：

$sum_(d|n) mu(d) = sum_(d|n') mu(d)$


===== 组合证明

考虑 $n$ 的质因子集合 $S$，其大小为 $s$：
- $mu(d) eq.not 0$ 的 $d$ 对应 $S$ 的子集
- $d$ 的质因子个数为 $k$ 时，$mu(d) = (-1)^k$
- 由二项式定理


$sum_(d|n') mu(d) = sum_(k=0)^s (-1)^k binom{s}{k} = (1+(-1))^s = 0$


===== 示例说明（$n=6$）

$6 = 2^1 times 3^1$，$S = \{2,3\}$：

$mu(1) &= (-1)^0 = 1 quad & (upright("取 0 个质因子"))  
mu(2) &= (-1)^1 = -1 quad & (upright("取质因子 ")2)  
mu(3) &= (-1)^1 = -1 quad & (upright("取质因子 ")3)  
mu(6) &= (-1)^2 = 1 quad & (upright("取质因子 ")2,3)$

和为 $1 + (-1) + (-1) + 1 = 0$。

== 数论笔记(狄利克雷卷积与莫比乌斯反演 2)

=== 狄利克雷卷积


==== 定义

设 $f(n)$, $g(n)$ 是两个积性函数，其狄利克雷卷积定义为：

$(f * g)(n) = sum_(d|n) f(d)g((n)/(d)) = sum_(d|n) f((n)/(d))g(d)$


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[注意跟狄利克雷生成函数形式上的相似性]
#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[读作：$f$ 卷 $g$ ]

===== 示例


$(f * g)(4) = f(1)g(4) + f(2)g(2) + f(4)g(1)$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 运算规律

+ *交换律*：$f * g = g * f$  
+ *结合律*：$(f * g) * h = f * (g * h)$  
+ *分配律*：$(f + g) * h = f * h + g * h$  

#line(length: 100%, stroke: 0.5pt + luma(180))


==== 常用函数

| 函数名称 | 符号表示 | 定义 |
|---------|----------|------|
| 元函数 | $epsilon(n)$ | $[n=1] =  1 & n=1   0 & n>1 $ |
| 常数函数 | $1(n)$ | $1$ |
| 恒等函数 | $id(n)$ | $n$ |
| 欧拉函数 | $phi.alt(n)$ |  \<n且与n互质的数的个数 |
| 莫比乌斯函数 | $mu(n)$ |  `$\begin{cases} 1 & n=1 \\ (-1)^k & n=p_1p_2...p_k \\ 0 & n含平方因子 \end{cases}$` |

#line(length: 100%, stroke: 0.5pt + luma(180))


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[注意符号的读法 $mu$ 读作“缪”，$phi.alt$ 读作“phi”，$epsilon$ 读作“一穆西隆”]

==== 常用卷积关系


简记形式
+ $sum_(d|n) mu(d) = [n = 1] quad arrow.l.r.double quad mu * 1 = epsilon$  
+ $sum_(d|n) phi.alt(d) = n quad arrow.l.r.double quad phi.alt * 1 = i d$  
+ $sum_(d|n) mu(d) (n)/(d) = phi.alt(n) quad arrow.l.r.double quad mu * i d = phi.alt$  
+ $f * epsilon = f$  
+ $f * 1 eq.not f$

#line(length: 100%, stroke: 0.5pt + luma(180))


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[注意莫比乌斯函数是常数函数的逆元]

==== 证明

===== 1. $mu * 1 = epsilon$


$(mu * 1)(n) = sum_(d|n) mu(d) dot.c 1((n)/(d)) = sum_(d|n) mu(d) = [n = 1] = epsilon(n)$


===== 2. $phi.alt * 1 = i d$


$(phi.alt * 1)(n) = sum_(d|n) phi.alt(d) dot.c 1((n)/(d)) = sum_(d|n) phi.alt(d) = n = i d(n)$


===== 3. $mu * i d = phi.alt$


$(mu * i d)(n) = sum_(d|n) mu(d) dot.c i d((n)/(d)) = sum_(d|n) mu(d) dot.c (n)/(d) = phi.alt(n)$



$mu * i d =mu *phi.alt *1 =(phi.alt *1 * mu) = epsilon * mu = phi.alt$


===== 4. $f * epsilon = f$


$(f * epsilon)(n) = sum_(d|n) f(d) dot.c epsilon((n)/(d)) = sum_(d|n) f(d) dot.c [(n)/(d) = 1] = f(n)$


===== 5. $f * 1 eq.not f$


$(f * 1)(n) = sum_(d|n) f(d) dot.c 1((n)/(d)) = sum_(d|n) f(d) eq.not f(n)$


=== 莫比乌斯反演


其实就是一下几个式子(条件式变成和式)

$sum_(d|n) phi.alt(d) = n$



$sum_(d|n) mu(d) = [n = 1] = 
1 & n=1  
0 & n>1 $


把$n$换成$gcd(a,b)$



$sum_(d|gcd(a,b)) mu(d) = [gcd(a,b) = 1]
1 & gcd(a,b)=1  
0 & gcd(a,b)>1 $



=== 莫比乌斯变换


==== 基本公式

设 $f(n)$, $g(n)$ 均为积性函数，则：

$f(n) = sum_(d|n) g(d) arrow.l.r.double g(n) = sum_(d|n) mu(d) f((n)/(d))$


即

$f=g * 1 arrow.l.r.double g = mu * f$


- $f(n)$ 称为 $g(n)$ 的莫比乌斯变换
- $g(n)$ 称为 $f(n)$ 的莫比乌斯逆变换

#line(length: 100%, stroke: 0.5pt + luma(180))


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[注意对于一些函数 f(n)，如果很难直接求出它的值，而容易求出其倍数和或约数和 g(n)，那么可以通过莫比乌斯反演简化运算，求得 ]
f(n) 的值。

==== 证明方法一（卷积形式）

===== 正向推导


$f &= g * 1  
mu * f &= mu * (g * 1)  
&= g * (mu * 1)  
&= g * epsilon  
&= g$


===== 逆向推导


$g &= mu * f  
g * 1 &= (mu * f) * 1  
&= f * (mu * 1)  
&= f * epsilon  
&= f$


#line(length: 100%, stroke: 0.5pt + luma(180))


==== 证明方法二（双重求和）


$sum_(d|n) mu(d) f((n)/(d)) 
&= sum_(d|n) mu(d) sum_(k|(n)/(d)) g(k)  
&= sum_(d|n) sum_(k|(n)/(d)) mu(d) g(k)  
&= sum_(k|n) sum_(d|(n)/(k)) mu(d) g(k)  
&= sum_(k|n) g(k) ( sum_(d|(n)/(k)) mu(d) )  
&= sum_(k|n) g(k) dot.c epsilon((n)/(k))  
&= g(n)$



== 数论笔记(炫酷反演魔术)

#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[从广义莫比乌斯反演的角度思考各类反演魔术]

此笔记主要总结之前的二项式反演/莫比乌斯反演
然后补充斯特林反演/mix-max容斥/容斥的相关技术

===== 一切的基石：广义莫比乌斯反演



我们在数论中熟悉的 $sum_(d|n)mu(d) = [n=1]$ 其实只是冰山一角。所有的反演公式本质上都是在一个特定的偏序集( `$DAG$` )上求解线性方程组。

====== 1.1 定义


设 $P$ 是一个局部有限的偏序集。

Zeta 函数 (前缀和) $zeta(x, y)$：
表示 $x$ 是否小于等于 $y$。



$zeta(x, y) =  1 & x l e y   0 & upright("o t h e r w i s e") $


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[$zeta(x, y) = 1$ 等价于DAG上存在从 $x$ 到 $y$ 的路径。]

我们现在知道


$g(y) = sum_(x l e y) zeta(x, y) f(x)$

 
我们现在要求解


$f(y) = sum_(x l e y) mu(x, y) g(x)$


于是我们有这个式子

Möbius 函数 (差分系数) $mu(x, y)$：


$mu(x, y) =  1 & x = y   -sum_(x l e z < y) mu(x, z) & x < y   0 & upright("o t h e r w i s e") $


或者说


$sum_(y p r e c e q z p r e c e q x) mu(z, x) = [x=y] = delta(x,y)$


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[你想求 $x$ 到 $y$ 的莫比乌斯函数值？]
你就把从 $x$ 出发，到 $y$ 之前所有中间节点 $z$ 的 $mu(x, z)$ 加起来，取个负号，就是 $mu(x, y)$。


怎么推导呢？


定义两个双变量函数 $A, B$ 的*狄利克雷卷积*（Dirichlet Convolution）为：


$(A * B)(x, y) = sum_(x l e z l e y) A(x, z) B(z, y)$


这本质上就是*矩阵乘法*。如果你把 $A(x, z)$ 看作矩阵 $A$ 的第 $x$ 行第 $z$ 列元素，把 $B(z, y)$ 看作矩阵 $B$ 的第 $z$ 行第 $y$ 列元素，上述公式就是 `$(AB)_{xy}$`。

*推导单位元 $delta$*

我们想要求解 $f(y)$，这需要逆运算。在做逆运算之前，必须先知道在这个乘法规则下，什么东西相当于数字 $1$（保持原值不变）。

设单位元函数为 $delta(x, y)$。
对于任意函数 $A$，必须满足 $A * delta = A$。
即：

$sum_(x l e z l e y) A(x, z) delta(z, y) = A(x, y)$


观察这个等式：
+ *当 $x=y$ 时*：
    式子变为 $A(x, x)delta(x, x) = A(x, x)$。
    为了对任意 $A$ 成立，必须有 *$delta(x, x) = 1$*。

+ *当 $x < y$ 时*：
    式子展开为 $A(x, x)delta(x, y) + dots + A(x, y)delta(y, y) = A(x, y)$。
    我们在上一步已经确定了 $delta(y, y)=1$，所以式子最后那一项 $A(x, y)delta(y, y)$ 已经等于等号右边的 $A(x, y)$ 了。
    这意味着，前面所有的项加起来必须等于 0：

$A(x, x)delta(x, y) + A(x, x')delta(x', y) + dots = 0$

    为了让它对任意 $A$ 都恒成立（无论 $A$ 取什么值），必须要求所有系数 *$delta(z, y) = 0$*（其中 $z < y$）。

*结论 1*：要在这种卷积下保持不变，必须使用函数：

$delta(x, y) = [x = y] =  1 & x = y   0 & x n e y $


*形式化已知条件*

已知条件是 $g(y) = sum_(x l e y) f(x)$。
引入常函数 $zeta(x, y) = 1$ （当 $x l e y$ 时）。
原式可以写成卷积形式（视 $f, g$ 为向量，$A, B$ 为矩阵）：

$g(y) = sum_(x l e y) f(x) zeta(x, y)$

在代数层面上，这等价于：

$g = f * zeta$


*推导 $mu$*

我们的目标是求 $f$。
既然 $g = f * zeta$，我们在两边同乘一个 $zeta$ 的逆元（记作 $mu$）：

$g * mu = (f * zeta) * mu$

利用结合律：

$g * mu = f * (zeta * mu)$

为了消掉 $zeta$，我们要求 $mu$ 满足：

$zeta * mu = delta$

这样就有：

$g * mu = f * delta = f$

即 *$f(y) = sum_(x l e y) g(x)mu(x, y)$*。

现在的问题核心变成了：*如何找到这个 $mu$，使得 $zeta * mu = delta$？*

根据 $zeta * mu = delta$ 的定义，展开卷积公式：

$sum_(x l e z l e y) zeta(x, z) mu(z, y) = delta(x, y)$


因为对于 $x l e z$，$zeta(x, z)$ 恒等于 1，所以式子简化为：

$sum_(x l e z l e y) mu(z, y) = delta(x, y)$


这也是 $mu$ 定义里是 $mu(x, z)$ 还是 $mu(z, y)$ 的区别。由于左逆元等于右逆元（因为我们定义的卷积的形式），我们通常习惯计算 $mu * zeta = delta$，即：

$sum_(x l e z l e y) mu(x, z) zeta(z, y) = delta(x, y)$

简化后（因为 $zeta(z, y)=1$）：

$sum_(x l e z l e y) mu(x, z) = delta(x, y)$


我们对区间 $[x, y]$ 的大小进行分类讨论：

+ *当 $x = y$ 时（区间长度为1）：*

$mu(x, x) = delta(x, x)$

    *$mu(x, x) = 1$*

+ *当 $x < y$ 时（区间长度大于1）：*

$sum_(x l e z l e y) mu(x, z) = delta(x, y)$

    因为 $x n e y$，所以 $delta(x, y) = 0$。

$sum_(x l e z l e y) mu(x, z) = 0$

    我们将末项（$z=y$ 的项）分离出来：

$( sum_(x l e z < y) mu(x, z) ) + mu(x, y) = 0$

    移项得到 $mu(x, y)$ 的递推式：
    *$mu(x, y) = - sum_(x l e z < y) mu(x, z)$*
$Q.E.D$


====== 1.2 从矩阵视角看待这个反演


所有的反演公式，都可以抽象为两个数列（或函数） $f$ 和 $g$ 之间的线性关系：

$g(y) = sum_(x p r e c e q y) zeta(x, y) f(x)$

这写成矩阵形式就是：
$bold("g") = bold("Z") times bold("f")$其中 $bold("Z")$ 是一个变换矩阵。
所谓的“反演”，就是求逆矩阵 $bold("M") = bold("Z")^(-1)$：$bold("f") = bold("Z")^(-1) times bold("g") = bold("M") times bold("g")$
对应的形式就是

$f(x) = sum_(y p r e c e q x) mu(y, x) g(y)$


====== 1.3. 通用反演定理


于是我们得到了这个很好看的式子

若有两个定义在偏序集 $P$ 上的函数 $f, g$ 满足“和”的关系：


$g(x) = sum_(y l e x) zeta(y, x) f(y)$


则可以通过 $mu$ 函数反解出 $f$：


$f(x) = sum_(y l e x) mu(y, x) g(y)$


====== 1.4. 这暗示我们啥东西？

 
我们通过定义一个矩阵乘法状物的卷积，把所有在偏序集上的反演公式都统一了起来。

其中 $zeta * mu = delta$ 是一个核心方程，$mu$ 是 $zeta$ 的逆元。

===== 容斥原理与子集/超集反演


====== 2.1 容斥原理及其补集形式


有一个集合的集合 $A=\{S_i\}$ ，那么：


$l v e r t union.big_(i=1)^n S_i r v e r t = sum_(i=1)^n (-1)^(i-1) sum_{\{a_k\}, a_k < a_(k+1)} l v e r t sect.big_(j=1)^i S_(a_j) r v e r t$


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[这是显然的，要证也可以通过二项式定理证明]

容斥原理的补集形式


$l v e r t sect.big_(i=1)^n S_i r v e r t = l v e r t U r v e r t - l v e r t union.big_(i=1)^n overline{S_i} r v e r t$


#block(inset: (left: 12pt), stroke: (left: 2pt + luma(180)))[其实画个venn图就能看出来]

====== 2.2 子集/超集反演



$f(S) = sum_(T s u b s e t e q S) g(T) arrow.l.r.double g(S) = sum_(T s u b s e t e q S) (-1)^(|S|-|T|) f(T)$



$f(S) = sum_(S s u b s e t e q T) g(T) arrow.l.r.double g(S) = sum_(S s u b s e t e q T) (-1)^(l v e r t T r v e r t - l v e r t S r v e r t) f(T)$


证明：
*引理*  $mu$ 具有积性，具体来说，若偏序集 $P = P_1 times P_2$，则 $mu_P((x_1, x_2), (y_1, y_2)) = mu_(P_1)(x_1, y_1) dot.c mu_(P_2)(x_2, y_2)$。

证：$mu$ 函数的唯一定义特征是：
对于任意区间 `$[Start, End]$`，所有子元素的 $mu$ 值之和必须等于1 （当 `$Start = End$` 时）
值之和必须等于 0 （当 `$Start < End$` 时）
只要能证明 $mu_P(x) dot.c mu_Q(y)$ 满足这个特征，那它就是 $mu_(P times Q)$。

设 $S = (x_1, y_1)$ 是起点，$E = (x_2, y_2)$ 是终点。我们需要计算区间 $[S, E]$ 里所有元素的“分量 $mu$ 乘积”之和：$upright("Sum") = sum_(S l e (u, v) l e E) mu_P(x_1, u) dot.c mu_Q(y_1, v)$

因为积偏序集中，$u$ 和 $v$ 的选取是独立的（$x_1 l e u l e x_2$ 且 $y_1 l e v l e y_2$），我们可以把双重求和拆成两个括号相乘：$upright("Sum") = ( sum_(x_1 l e u l e x_2) mu_P(x_1, u) ) times ( sum_(y_1 l e v l e y_2) mu_Q(y_1, v) )$

根据 $mu$ 的定义，括号里的求和结果只能是 $1$ 或 $0$：
左括号 $= delta(x_1, x_2)$ （只有 $x_1=x_2$ 时是 1，否则是 0）
右括号 $= delta(y_1, y_2)$ （只有 $y_1=y_2$ 时是 1，否则是 0）

情况 A：起点 = 终点即 $x_1=x_2$ 且 $y_1=y_2$。此时 $upright("Sum") = 1 times 1 = 1$。 $arrow.r.double$ 满足定义！
情况 B：起点 \< 终点这意味着 $x$ 和 $y$ 中至少有一个不等（例如 $x_1 < x_2$）。那么对应的那个括号求和就是 $0$。$upright("Sum") = 0 times (dots) = 0 quad upright("或") quad (dots) times 0 = 0$$\implies$ 满足定义！
