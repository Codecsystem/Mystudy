#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <map>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <functional>
#include <ranges>
#include <iomanip>
#include <chrono>
#include <random>
#include <cstdint>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
template<class Info>
class zkw{
    public:
    int n,m;
    vector<Info> info;
    zkw(const vector<Info> &a):n(a.size()-1){
        //a 1-Based
        for(m=1;m<=n+1;m<<=1);
        info.resize((m<<1)+5);
        for(int i=1;i<=n;++i) info[m+i]=a[i];
        for(int i=m-1;i;--i) pushup(i);
    }
    inline void pushup(int p){
        info[p]=info[p<<1]+info[p<<1|1];
    }
    void mdf(int x,const Info &v){
        for(info[x+=m]=v,x>>=1;x;x>>=1) pushup(x);
    }
    Info qry(int l,int r){
        Info rl=Info(),rr=Info();
        for(l+=m-1,r+=m+1;l^r^1;l>>=1,r>>=1){
            if(~l&1) rl=rl+info[l^1];
            if(r&1) rr=info[r^1]+rr;
        }
        return rl+rr;
    }
};
// Info 结构体：定义节点信息
// 需要实现:
// 1. 成员变量: 存储节点维护的信息
// 2. 默认构造函数: Info 的单位元 (例如求和的0, 求积的1, 最大值的-INF)
// 3. operator+(const Info& other): 合并两个子节点的信息
struct Info{
    //...
    int info;
    Info():info(0){}
    Info(int x):info(x){}
};

Info operator+(const Info &a,const Info &b){
    //... (注意非交换律运算时，顺序为 左子节点+右子节点)
    Info c;
    c.info=a.info+b.info;
    return c;
}
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}