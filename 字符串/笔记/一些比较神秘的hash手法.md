#### 一些比较神秘的 hash 手法

以下的 $Hash$ 均采用这个生成：

```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()^(ull)(new char));
vector<ull> h(n+1);
for(int i=1;i<=n;i++) h[i]=Xhash::splitmix64(rng())|((1ull<<50)+1);
```

> XHash 类见 防hack的umap,gp_hash_table.cpp

对字符集/数集中的每个元素都随机分配一个 $Hash$ 值（大奇数）。

采取自然溢出 $Hash$。

给定数组/字符串。

##### 1.1. 考虑求解每个元素的出现次数为偶数的子数组/子串个数

老生常谈，维护前缀异或和就行，区间 $[l,r]$ 合法的条件是 $sum[r]\oplus sum[l-1]=0$，即 $sum[r]=sum[l-1]$。

> 如果他要你求区间能重排成回文串，做一些适当转化可以变成这个问题。

##### 1.2. 更一般的，考虑求解每个元素的出现次数为 $k$ 的倍数的子数组/子串个数

做一个前缀 $Hash$ 和，维护一个 $cnt$ 数组，当数组中的某个元素达到 $k$ 时候，在前缀和上减去 $(k-1)*h[i]$，这样每个前缀的意义其实是一个长度为 $n$ 的状态向量，每个位置是模 $k$ 的。区间 $[l,r]$ 合法的条件是 $sum[r]=sum[l-1]$。

实现：

```cpp
for(int i=0;i<26;i++) w[i]=rng()|1;
for(int i=1;i<=n;i++){
    cnt[s[i]-'a']++;
    if(cnt[s[i]-'a']==k){
        ph[i]=ph[i-1]-w[s[i]-'a']*(k-1);
        cnt[s[i]-'a']=0;
    }
    else ph[i]=ph[i-1]+w[s[i]-'a'];
}
```

例题：https://codeforces.com/gym/105911/problem/E

##### 1.3. 更特殊的，考虑求解每个元素的出现次数为 $k$ 的子数组/子串个数

注意到我们维护一个滑动窗口，窗口里面的元素个数 $\le k$，然后问题就转化成 $1.2$ 了。

例题：https://atcoder.jp/contests/abc455/tasks/abc455_g 子问题1

##### 2.1. 考虑求解字母个数相同的子数组/子串个数

对我们的 $Hash$ 做一个小小的修改，对前 $n-1$ 个元素保持不变，最后一个元素取前 $n-1$ 个元素哈希值和的相反数，做前缀 $Hash$，区间 $[l,r]$ 合法的条件是 $sum[r]=sum[l-1]$。

例题：https://atcoder.jp/contests/abc455/tasks/abc455_e

##### 3.1 考虑判断一个子区间能够进行重排后变成另一个子区间

那么其实就是我们普通的前缀 $Hash$，判断 $[l1,r1]$ 和 $[l2,r2]$ 是否相等，即 $sum[r1]-sum[l1-1]=sum[r2]-sum[l2-1]$。

> 那其实说明我们前缀hash能代表某个多重集。

> 以上的 $Hash$ 概率都是没有问题的，防 $hack$ 也是对的。

##### 4.1 考虑判断区间中的所有数的个数是否能整除其本身

直接取 $Hash(x)=1/x (mod p)$，然后做前缀 $Hash$，多模几个大质数，判断在这些质数下区间和是否相等，并且是一个小值。

##### 5.1 有根树同构(树hash)

$$H(u) = 1 + \sum_{v \in son(u)} f(H(v)) \pmod{2^{64}}$$

其中 $f$ 是一个非线性映射函数，比如 $splitmix64$。

使用例子：
```cpp
Xhash64 H;
ull h[N];
void dfs(int u,int fa){
    h[u]=1;
    for(int v:g[u])
        if(v!=fa) dfs(v,u),h[u]+=H(h[v]);
}
```

##### 5.2 无根树同构

对重心跑一边有根树同构，如果有两个重心，都跑一遍取 $max$ 即可

##### 5.3 有标号图同构

对每个标号随一个 $hash$ 值，然后每个标号的 $hash$ 值是他邻居的 $hash$ 值的和(自然溢出)，对每个标号检查即可。


