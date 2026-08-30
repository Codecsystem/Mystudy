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
const int mod=1e9+7;
template <int MOD>
struct SMC {
    int val;
    SMC(ll v=0) : val(v%MOD) { if (val<0) val+=MOD; }
    SMC& operator+=(const SMC &r) { val+=r.val; if (val>=MOD) val-=MOD; return *this; }
    SMC& operator-=(const SMC &r) { val-=r.val; if (val<0) val+=MOD; return *this; }
    SMC& operator*=(const SMC &r) { val=1LL*val*r.val%MOD; return *this; }
    SMC& operator/=(const SMC &r) { return *this*=r.inv(); }
    friend SMC operator+(SMC a,const SMC &b) { return a+=b; }
    friend SMC operator-(SMC a,const SMC &b) { return a-=b; }
    friend SMC operator*(SMC a,const SMC &b) { return a*=b; }
    friend SMC operator/(SMC a,const SMC &b) { return a/=b; }
    SMC pow(ll k) const {
        SMC res=1,a=*this;
        for (;k;k>>=1,a*=a) if(k&1) res*=a;
        return res;
    }
    SMC inv() const { return pow(MOD-2); }
    friend istream& operator>>(istream &in,SMC &a) { ll v; in>>v; a=v; return in; }
    friend ostream& operator<<(ostream &out,const SMC &a) { return out<<a.val; }
};
using Z=SMC<mod>;
//给定n个x不相同的点，求次数<=n-1,过这些点的函数f(x),求f(k)
//$$P(x)=\sum_{i=1}^n y_i \prod_{j \neq i} \frac{x-x_j}{x_i-x_j}$$
//O(n^2)
Z clac(vector<array<int,2>> fun,int n,int k){
    Z res=0;
    for(int i=1;i<=n;i++){
        Z now=fun[i][1];
        for(int j=1;j<=n;j++){
            if(i==j) continue;
            now*=Z(k-fun[j][0])/(Z(fun[i][0]-fun[j][0]));
        }
        res+=now;
    }
    return res;
};
//连续点值优化：当xi是连续的，例如1,2,3,...n
//可以用前缀积和后缀积来优化
//$$P(x)=\sum_{i=1}^n y_i \frac{pre_{i-1} \cdot suf_{i+1}}{(i-1)! (n-i)! (-1)^{n-i}}$$
//$$pre_i=\prod_{j=1}^i(x-j)$$,$$suf_i=\prod_{j=i}^n(x-j)$$
//O(n),
Z clac2(vector<array<Z,2>> fun,int n,int k){
    vector<Z> pre(n+1,1),suf(n+2,1),inv(n+1,1),invfac(n+1,1);
    for(int i=1;i<=n;i++) pre[i]=pre[i-1]*Z(k-i);
    for(int i=n;i>=1;i--) suf[i]=suf[i+1]*Z(k-i);
    for(int i=2;i<=n;i++) inv[i]=(mod-mod/i)*inv[mod%i];
    for(int i=1;i<=n;i++) invfac[i]=invfac[i-1]*inv[i];
    Z res=0;
    for(int i=1;i<=n;i++){
        res+=(((n-i)&1)?-1:1)*fun[i][1]*pre[i-1]*suf[i+1]*invfac[i-1]*invfac[n-i];
    }
    return res;
}
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,k;cin>>n>>k;
    vector<array<Z,2>> fun(k+3);
    fun[0]={0,0};
    for(int i=1;i<=k+2;i++){
        fun[i]={i,Z(i).pow(k)+fun[i-1][1]};
    }
    cout<<clac2(fun,k+2,n)<<endl;
    return 0;
}