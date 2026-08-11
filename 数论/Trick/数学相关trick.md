我们在处理某些计数问题时 总是有一下思路
1.定1移1 考虑某个前缀对r的贡献
2.考虑某个点对区间的贡献
3.移项，解耦变量后考虑区间内相同性质点的贡献

数数题 最重要的思想是按什么东西进行分类，是好做的，而且能做到不重不漏

gcd(k,n)=1->gcd(n-k,n)=1 simga [gcd(i,n)=1]*i=phi(n)*n/2
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79094134&returnHomeType=1&uid=719203876

原来aj*2>ai的时候aj一定不整除ai
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79086887&returnHomeType=1&uid=719203876

考虑算[l,r]中被p整除的数，一个好的实现是先算[1,r]中被p整除的数，再减去[1,l-1]中被p整除的数。

常系数线性递推式可以用矩阵快速幂优化。
注意$dp[i] = W[1]dp[i-1] + W[2]dp[i-2] + \dots + W[d]dp[i-d]$。
构造的矩阵为第一行为系数矩阵，然后是从(2,1)开始的类似于单位矩阵的矩阵(d行d列)。
可以把$$(dp[i-1],dp[i-2],\dots,dp[i-d])$$看作一个列向量，然后乘上构造的矩阵，就可以得到$$(dp[i],dp[i-1],\dots,dp[i-d+1])$$。
有些时候能把状态压缩成一个等价类，缩减矩阵的大小

ntt如果有多次intt，有可能可以不用先intt然后答案相加，而是直接在ntt后把点值加起来，最后做一次intt就行

质数间距是小的，根据x附近的质数密度是1/lnx，那么x附近的质数间距是lnx

筛积性函数的时候 这个积性函数在质数和质数幂时候的值是好算的话 这样他的值是好求的。

Raney 引理：如果一个整数序列 $x_1, x_2, \dots, x_m$ 的每一项都不超过 $1$，且总和为 $S > 0$，那么在这个序列的 $m$ 个循环移位中，恰好有 $S$ 个移位满足“所有非空前缀和都严格大于 $0$”。