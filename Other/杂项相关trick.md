括号序列合法的充要是 +1 -1 trick-> 所有前缀和>=0 sum=0
-->能推导出 这样的串里一定存在() 子串

sqrt 对longlong精度不够  要用sqrtl

某些区间问题 可以利用前后缀处理的思想
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79677851&returnHomeType=1&uid=719203876

某些题可以转化成二进制来做
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79675702&returnHomeType=1&uid=719203876
eg x <- floor(x/2) = x>>1

位运算计数某些经典技巧是拆位，每位独立贡献
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79091623&returnHomeType=1&uid=719203876

压位 ull st=((ull)a<<32)|b;


xorhash
通常存在于某些2/1性质 奇偶性质的题里面

有些题有奇偶不变量/其他的不变量

压维转化 0-base x+m*y 1base (x-1)+m*(y-1)+1

曼哈顿距离 dis(x1,y1,x2,y2)=abs(x1-x2)+abs(y1-y2)
切比雪夫距离 dis(x1,y1,x2,y2)=max(abs(x1-x2),abs(y1-y2))
互转 (x,y) -> ((x+y)/2,(x-y)/2) 原坐标的曼哈顿距离等于新坐标的切比雪夫距离

max(a,b)=(a+b+abs(a-b))/2
min(a,b)=(a+b-abs(a-b))/2

diliworth定理
dilworth定理：一个有向无环图的最小链划分等于其最大反链大小

hall定理
一个二分图是完美匹配的充要条件是，对于任意一个X的子集，其邻居数大于等于该子集大小

排序不等式
simga ai*bi最大的充要为 即对于所有 $i, j$，若 $a_i < a_j$ 则 $b_i < b_j$
即 $a$ 和 $b$ 是同序的。
最小的充要为 即对于所有 $i, j$，若 $a_i < a_j$ 则 $b_i > b_j$
即 $a$ 和 $b$ 是反序的。
--同理 max(ai-bi) 最小 -> $a$ 和 $b$ 是同序的。
https://www.luogu.com.cn/problem/AT_arc201_d

注意%m的形式有时候可以破环为链

排列诸结论
1.交换两个数的顺序，会使排列的逆序对数+1/-1 奇偶性变换
2.一个长度为 n 的排列，其逆序对数的奇偶性等于n-c c是置换环数
置换环：下标[l,r]包含了[l,r]的所有元素，称为置换环
3.交换大小为x，y的两个块 对逆序对的贡献为x*y-2k,k为原来块间的逆序对数
4.最优交换次数 $K$：将排列 $P$ 分解为若干个不相交的置换环。设环的个数为 $C$，则将 $P$ 排序为 $(1, 2, \ldots, N)$ 的最小交换次数为：$$K = N - C$$
5.置换环内交换：如果你交换同一个环内的两个元素，这个环一定会分裂成两个更小的环。
6.置换环间交换：如果你交换不同环的两个元素，这两个环会合并成一个大环。

恰好的方案数难求的时候 考虑至少的方案数然后转化
经典的 k=simga_i<=k {1} 然后我们能把一个东西变成对每个>=k都计算一遍
式子很像的时候考虑求出整体贡献然后对每个i取消贡献

有时候建出操作树可以降维打击！

二分图染色可以使用拓展域并查集
具体来说是拆点 x->x,x+n x代表黑色 x+n代表白色
(u,v) 连边的时候 merge(u,v+n) merge(u+n,v) 如果same(u,v) 则说明两个点同色 不成立

float 有效位数是6-7位
double 有效位数是15-16位
long double 有效位数是18-19位

记得sqrtl和atanl
acos(-1)=pi

完全图最小生成树可以用boruvka算法
也可能可以用数据结构优化prim算法

