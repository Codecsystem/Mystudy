树上任意一点v，距离其最远的点一定是该树某条直径的两个端点之一。
考虑反证法即可

联通块
https://ac.nowcoder.com/acm/contest/view-submission?submissionId=79644138&returnHomeType=1&uid=719203876

有时候想想菊花图呢

每个点只有一个出边是内向基环树，可以倍增
每个点只有一个入边是外向基环树

最小瓶颈路：一个点到一个点所有路径中最大点权的最小值，可以用dijk算
多次询问的话可以求出mst然后跑树剖/rmq