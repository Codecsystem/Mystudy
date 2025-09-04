p1:
给定一个无向联通图 染色 1,2,3
Alice 选某个未被染色的点和两个数
bob 确定两个数中的一个染色
若不存在染色相同的边 则bob胜
else 则alice胜
判断必胜态

思路 deg>=3 i-a1,i-a2,i-a3  a1:1,2 a2:1,3 a3:2,3 ->a1,a2,a3必定有两个不同 alice必胜
    deg<=2 dot>=4 a1-a2-a3-a4
    a1:1,3 a4:1,3 
    -> case1 a1=a4  a2,a3 1,3 alice必胜
    -> case2 a1!=a4 e.g a1=1,a4=3->a3:2,3 -> 1 ? 2 3 alice必胜
    dot<=3 bob必胜 expect 三元环
1200*

