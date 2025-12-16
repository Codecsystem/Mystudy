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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
using ll=long long;
const int mod=998244353;
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
const Z inv2=Z(2).inv();
class FWT{
public:
    FWT(){}
    void trans(vector<Z> &a,int typ,int op){
        // typ: 1 FWT -1 IFWT
        // op: 0 or 1 and 2 xor
        int len=a.size();
        for(int mid=1;mid<len;mid<<=1){
            //枚举块长
            for(int i=0;i<len;i+=(mid<<1)){
                //枚举块
                for(int j=0;j<mid;j++){
                    //枚举偏移
                    Z x=a[i+j],y=a[i+j+mid];
                    //x是y的子集
                    if(op==0){
                        //把子集数据给超集
                        if(typ==1) a[i+j+mid]=x+y;
                        else a[i+j+mid]=y-x;
                    }
                    else if(op==1){
                        //把超集数据给子集
                        if(typ==1) a[i+j]=x+y;
                        else a[i+j]=x-y;
                    }
                    else{
                        if(typ==1) a[i+j]=x+y,a[i+j+mid]=x-y;
                        else a[i+j]=(x+y)*inv2,a[i+j+mid]=(x-y)*inv2;
                    }
                }
            }
        }
    }
    //返回c作为结果 a,b 0-based
    vector<Z> conv(vector<Z> &a,vector<Z> &b,int op){
        int n=max(a.size(),b.size());
        int len=1;
        while(len<n) len<<=1;
        vector<Z> c(len,0);
        a.resize(len,0),b.resize(len,0);
        trans(a,1,op),trans(b,1,op);
        for(int i=0;i<len;i++) c[i]=a[i]*b[i];
        trans(a,-1,op),trans(b,-1,op),trans(c,-1,op);
        return c;
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;cin>>n;
    vector<Z> a(1<<n),b(1<<n);
    for(int i=0;i<(1<<n);i++) cin>>a[i];
    for(int i=0;i<(1<<n);i++) cin>>b[i];
    FWT fwt;
    auto c1=fwt.conv(a,b,0);
    auto c2=fwt.conv(a,b,1);
    auto c3=fwt.conv(a,b,2);
    for(auto i:c1) cout<<i<<" ";
    cout<<endl;
    for(auto i:c2) cout<<i<<" ";
    cout<<endl;
    for(auto i:c3) cout<<i<<" ";
    cout<<endl;
    return 0;
}