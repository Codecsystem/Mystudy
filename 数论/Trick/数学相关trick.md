我们在处理某些计数问题时 总是有一下思路
1.定1移1 考虑某个前缀对r的贡献
2.考虑某个点对区间的贡献

gcd(k,n)=1->gcd(n-k,n)=1 simga [gcd(i,n)=1]*i=phi(n)*n/2
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79094134&returnHomeType=1&uid=719203876

原来aj*2>ai的时候aj一定不整除ai
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79086887&returnHomeType=1&uid=719203876

考虑算[l,r]中被p整除的数，一个好的实现是先算[1,r]中被p整除的数，再减去[1,l-1]中被p整除的数。

常系数线性递推式可以用矩阵快速幂优化。
注意$dp[i] = W[1]dp[i-1] + W[2]dp[i-2] + \dots + W[d]dp[i-d]$。
构造的矩阵为第一行为系数矩阵，然后是从(2,1)开始的类似于单位矩阵的矩阵(d行d列)。
可以把$$(dp[i-1],dp[i-2],\dots,dp[i-d])$$看作一个列向量，然后乘上构造的矩阵，就可以得到$$(dp[i],dp[i-1],\dots,dp[i-d+1])$$。