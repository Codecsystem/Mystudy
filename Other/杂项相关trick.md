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

xorhash
通常存在于某些2/1性质 奇偶性质的题里面

有些题有奇偶不变量/其他的不变量

压维转化 0-base x+m*y 1base (x-1)+m*(y-1)+1

曼哈顿距离 dis(x1,y1,x2,y2)=abs(x1-x2)+abs(y1-y2)
切比雪夫距离 dis(x1,y1,x2,y2)=max(abs(x1-x2),abs(y1-y2))
互转 (x,y) -> ((x+y)/2,(x-y)/2) 原坐标的曼哈顿距离等于新坐标的切比雪夫距离

max(a,b)=(a+b+abs(a-b))/2
min(a,b)=(a+b-abs(a-b))/2

dillworth


