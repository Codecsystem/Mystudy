有时候想想 这个状态是否完备？ 状态数是不是可以通过某些手段缩减？

多位dp的时候能把小的项放在前面 会快一点(?)

状压子集枚举 3^n
st的子集 for(int i=st;i;i=(i-1)&st)
st的超集 for(int i=st;i<(1<<n);i=(i+1)|st)

退背包
一般用于我要统计不包含一个物品的方案数
就是反着做背包就行
->
```cpp
    auto add=[&](int cnt,int siz,int val)->void{
        for(int i=cnt;i>=1;i--){
            for(int j=siz;j>=val;j--){
                dp[i][j]+=dp[i-1][j-val];
            }
        }
    };
    auto del=[&](int cnt,int siz,int val)->void{
        for(int i=1;i<=cnt;i++){
            for(int j=val;j<=siz;j++){
                dp[i][j]-=dp[i-1][j-val];
            }
        }
    };
```