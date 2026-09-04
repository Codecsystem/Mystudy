#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using ld=long double;

//在闭整数区间上寻找单峰函数最优点；默认求最小值，求最大值传greater<>。
template<class F,class Better=less<>>
ll ternaryInt(ll l,ll r,F f,Better better={}){
    assert(l<=r);
    while((i128)r-l>6){
        ll d=((i128)r-l)/3;
        ll m1=l+d,m2=r-d;
        auto v1=f(m1),v2=f(m2);
        if(better(v2,v1))l=m1+1;
        else r=m2-1;
    }
    ll ans=l;
    auto val=f(l);
    for(ll x=l;;x++){
        auto cur=f(x);
        if(better(cur,val))ans=x,val=cur;
        if(x==r)break;
    }
    return ans;
}

//在实数区间上寻找连续单峰函数最优点；固定轮数规避eps导致的死循环。
template<class F,class Better=less<>>
ld ternaryReal(ld l,ld r,F f,int rounds=200,Better better={}){
    assert(l<=r&&rounds>=0);
    while(rounds--){
        ld m1=l+(r-l)/3,m2=r-(r-l)/3;
        auto v1=f(m1),v2=f(m2);
        if(better(v2,v1))l=m1;
        else r=m2;
    }
    return (l+r)/2;
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
