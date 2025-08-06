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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
const int mod=1e9+7;
template <int MOD>
struct SMC {
    int64_t val;
    constexpr SMC(int64_t v=0){
        val=(v%MOD+MOD)%MOD;
    }
    SMC& operator=(int64_t v){
        val=(v%MOD+MOD)%MOD;
        return *this;
    }
    SMC& operator+=(const SMC &rhs){
        val+=rhs.val;
        if(val>=MOD) val-=MOD;
        return *this;
    }
    SMC& operator-=(const SMC &rhs){
        val-=rhs.val;
        if(val<0) val+=MOD;
        return *this;
    }
    SMC& operator*=(const SMC &rhs){
        val=1LL*val*rhs.val%MOD;
        return *this;
    }
    static int64_t qpow(int64_t a,int64_t b){
        int64_t res=1;
        while(b){
            if(b&1) res=res*a%MOD;
            a=a*a%MOD;
            b>>=1;
        }
        return res;
    }
    SMC pow(int64_t k) const{
        return SMC(qpow(val,k));
    }
    SMC inv() const{
        return pow(MOD-2);
    }
    SMC& operator/=(const SMC &rhs){
        return *this*=rhs.inv();
    }
    friend SMC operator+(SMC a,const SMC &b){ return a+=b;}
    friend SMC operator-(SMC a,const SMC &b){ return a-=b;}
    friend SMC operator*(SMC a,const SMC &b){ return a*=b;}
    friend SMC operator/(SMC a,const SMC &b){ return a/=b;}
    SMC& operator++() { return *this += 1; }
	SMC& operator--() { return *this -= 1; }
	SMC operator++(int32_t dummy) { SMC t=*this; ++*this; return t; }
	SMC operator--(int32_t dummy) { SMC t=*this; --*this; return t; }
    friend bool operator==(const SMC &a,const SMC &b){ return a.val==b.val;}
	friend bool operator<(const SMC &a,const SMC &b){ return a.val<b.val;}
    friend bool operator>(const SMC &a,const SMC &b){ return a.val>b.val;}
    friend bool operator<=(const SMC &a,const SMC &b){ return a.val<=b.val;}
    friend bool operator>=(const SMC &a,const SMC &b){ return a.val>=b.val;}
    friend bool operator!=(const SMC &a,const SMC &b){ return a.val!=b.val;}

    friend std::istream& operator>>(std::istream &in,SMC &a){
        int64_t v;
        in>>v,a=SMC(v);
        return in;
    }

    friend std::ostream& operator<<(std::ostream &out,const SMC &a){
        out<<a.val;
        return out;
    }
    explicit operator long long() const{
        return val;
    }
    SMC operator-() const{
        return SMC(-val);
    }
	SMC& operator+=(int64_t x) { return *this+=SMC(x); }
	SMC& operator-=(int64_t x) { return *this-=SMC(x); }
	SMC& operator*=(int64_t x) { return *this*=SMC(x); }
	SMC& operator/=(int64_t x) { return *this/=SMC(x); }

	friend SMC operator+(SMC a, int64_t b) { return a+=b; }
	friend SMC operator-(SMC a, int64_t b) { return a-=b; }
	friend SMC operator*(SMC a, int64_t b) { return a*=b; }
	friend SMC operator/(SMC a, int64_t b) { return a/=b; }

	friend SMC operator+(int64_t a, SMC b) { return b+a; }
	friend SMC operator-(int64_t a, SMC b) { return SMC(a)-b; }
	friend SMC operator*(int64_t a, SMC b) { return b*a; }
	friend SMC operator/(int64_t a, SMC b) { return SMC(a)/b; }
};
using Z=SMC<mod>;
class Pre{
    public:
    int n,m;
    vector<vector<Z>> s,s2;
    vector<Z> inv,fac,invfac,d;
    //c组合数,s第一类斯特林数
    Pre(int n,int m):n(n),m(m){
        preS();
        preC();
        preD();
        preS2();
    }
    void preS(){
        s.resize(n+1,vector<Z>(m+1));
        s[0][0]=1;
        for(int i=1;i<=n;i++){
            s[i][0]=0;
            if(i<=m) s[i][i]=1;
            for(int j=1;j<=min(m,i);j++){
                s[i][j]=s[i-1][j]*(i-1)+s[i-1][j-1];
            }
        }
    }
    void preC()
    {
        fac.resize(n+1);
        invfac.resize(n+1);
        inv.resize(n+1);
        inv[1]=1;
        for(int i=2;i<=n;i++)
        {
            inv[i]=(mod-mod/i)*inv[mod%i];
        }
        fac[0]=invfac[0]=1;
        for(int i=1;i<=n;i++)
        {
            fac[i]=fac[i-1]*i;
            invfac[i]=invfac[i-1]*inv[i];
        }
    }
    void preD(){
        d.resize(n+1);
        d[1]=0,d[2]=1;
        for(int i=3;i<=n;i++){
            d[i]=(i-1)*(d[i-1]+d[i-2]);
        }
    }
    void preS2(){
        s2.resize(n+1,vector<Z>(m+1));
        s2[0][0]=1;
        for(int i=1;i<=n;i++){
            s2[i][0]=0;
            if(i<=m) s2[i][i]=1;
            for(int j=1;j<=min(m,i);j++){
                s2[i][j]=s2[i-1][j]*j+s2[i-1][j-1];
            }
        }
    }
    //第一类斯特林数S(n,m) n个不同元素划分为m个非空圆排列的方案数
    Z S(int i,int j){
        return s[i][j];
    }
    //第二类斯特林数S2(n,m) n个不同元素划分为m个非空子集的方案数
    Z S2(int i,int j){
        return s2[i][j];
    }
    //排列数 A(n,m) n个数中选m个的排列
    Z A(int n,int m){
        if(n<0||m<0||n<m)return 0;
        return fac[n]*invfac[n-m];
    }
    //组合数C(n,m) n个数中选m个
    Z C(int n,int m)
    {
        if(n<0||m<0||n<m)return 0;
        return fac[n]*invfac[m]*invfac[n-m];
    }
    //圆排列数 Q(n,m) n个数中选m个，m个数的圆排列
    //Q(n,n)=(n-1)!,n个数的圆排列
    Z Q(int n,int m)
    {
        if(n<0||m<0||n<m)return 0;
        return fac[n]*invfac[n-m]*inv[m];
    }
    //错位排列数 D(n,m) n个数中选m个，m个数的错位排列
    //D(n,n)=d[n],n个数的错位排列
    Z D(int n,int m)
    {
        if(n<0||m<0||n<m)return 0;
        return d[n]*C(n,m);
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}