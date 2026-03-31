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
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;

constexpr int P1=998244353;  
constexpr int P2=1004535809; 
constexpr int P3=469762049; 
constexpr int P4=167772161;  
constexpr ll P5=4179340454199820289LL;   
//大质数可用于答案不超过1e18的多项式乘法
//g=3
const int mod=998244353;
//防爆ll的模运算
template <ll MOD>
struct SMC {
    ll val;
    SMC(ll v=0) : val(v%MOD) { if (val<0) val+=MOD; }
    SMC& operator+=(const SMC &r) { val+=r.val; if (val>=MOD) val-=MOD; return *this; }
    SMC& operator-=(const SMC &r) { val-=r.val; if (val<0) val+=MOD; return *this; }
    SMC& operator*=(const SMC &r) { val=(ll)((__int128_t)val*r.val%MOD); return *this; }
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
//Montgomery 模乘，快很多
template<uint32_t MOD>
struct Mont {
    static constexpr uint32_t M_PRIME = []() {
        uint32_t x = MOD;
        for (int i = 0; i < 4; ++i) x *= 2u - MOD * x;
        return ~x + 1;
    }();
    static constexpr uint32_t R2 = []() {
        uint64_t x = 1ull << 32;
        x %= MOD;
        return (x * x) % MOD;
    }();

    uint32_t val; 
    static constexpr uint32_t reduce(uint64_t T) {
        uint32_t m = uint32_t(T) * M_PRIME;
        uint32_t res = (T + (uint64_t)m * MOD) >> 32;
        return res >= MOD ? res - MOD : res;
    }
    constexpr Mont(long long x = 0) : val(reduce((uint64_t)(x % MOD + MOD) % MOD * R2)) {}
    constexpr uint32_t get() const { return reduce(val); }
    constexpr Mont& operator+=(const Mont& rhs) { val += rhs.val; if (val >= MOD) val -= MOD; return *this; }
    constexpr Mont& operator-=(const Mont& rhs) { if (val < rhs.val) val += MOD; val -= rhs.val; return *this; }
    constexpr Mont& operator*=(const Mont& rhs) {
        val = reduce((uint64_t)val * rhs.val);
        return *this;
    }
    friend constexpr Mont operator+(Mont lhs, const Mont& rhs) { return lhs += rhs; }
    friend constexpr Mont operator-(Mont lhs, const Mont& rhs) { return lhs -= rhs; }
    friend constexpr Mont operator*(Mont lhs, const Mont& rhs) { return lhs *= rhs; }
    constexpr Mont pow(uint64_t n) const {
        Mont res(1), a(*this);
        while (n) { if (n & 1) res *= a; a *= a; n >>= 1; }
        return res;
    }
    constexpr Mont inv() const { return pow(MOD - 2); }
    constexpr Mont operator/(const Mont& rhs) const { return *this * rhs.inv(); }
    friend istream& operator>>(istream &in, Mont &a) { 
        long long v; 
        in >> v; 
        a = Mont(v); 
        return in; 
    }
    friend ostream& operator<<(ostream &out, const Mont &a) { 
        return out << a.get(); 
    }
};
using Z=SMC<mod>;
class NTT{
public:
    const int G=3;
    vector<int> R; vector<Z> rt;
    NTT(int len=0){ 
        int n=1; 
        while(n<len*2) n<<=1; 
        init(n); 
    }
    void init(int n){
        if(rt.empty()) rt={0,1};
        if(rt.size()>=n) return;
        for(int i=rt.size();i<n;i<<=1){
            rt.resize(i<<1); Z w=Z(G).pow((mod-1)/(i<<1)); rt[i]=1;
            for(int j=1;j<i;j++) rt[i+j]=rt[i+j-1]*w;
        }
    }
    void ntt(vector<Z>& a,int n,int op){
        for(int i=0;i<n;i++) R[i]=(R[i>>1]>>1)|((i&1)*(n>>1));
        for(int i=0;i<n;i++) if(i<R[i]) swap(a[i],a[R[i]]);
        for(int i=2;i<=n;i<<=1)
            for(int m=i>>1,j=0;j<n;j+=i)
                for(int k=j;k<j+m;k++){
                    Z x=a[k],y=rt[m+k-j]*a[k+m];
                    a[k]=x+y,a[k+m]=x-y;
                }
        if(op==-1){
            reverse(a.begin()+1,a.end()); Z in=Z(n).inv();
            for(int i=0;i<n;i++) a[i]*=in;
        }
    }
    vector<Z> calc(vector<Z> a,vector<Z> b){
        if(a.empty()||b.empty()) return {};
        int sz=a.size()+b.size()-1,len=1;
        while(len<sz) len<<=1;
        init(len); R.resize(len); a.resize(len); b.resize(len);
        ntt(a,len,1); ntt(b,len,1);
        for(int i=0;i<len;i++) a[i]*=b[i];
        ntt(a,len,-1); a.resize(sz);
        return a;
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    vector<Z> a(n+1),b(m+1);
    for(int i=0;i<=n;i++) cin>>a[i];
    for(int i=0;i<=m;i++) cin>>b[i];
    NTT ntt; vector<Z> c=ntt.calc(a,b);
    for(int i=0;i<=n+m;i++) cout<<c[i]<<' ';
    return 0;
}