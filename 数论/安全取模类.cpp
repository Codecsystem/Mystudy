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
#define int long long //赫赫 要不要龙龙呢
using namespace std;
const int mod=998244353;
template <int MOD>
struct SMC {
    long long val;
    constexpr SMC(long long v=0){
        val=(v%MOD+MOD)%MOD;
    }
    SMC& operator=(long long v){
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
    SMC pow(long long k) const{
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
    friend bool operator==(const SMC &a,const SMC &b){ return a.val==b.val;}
	friend bool operator<(const SMC &a,const SMC &b){ return a.val<b.val;}
    friend bool operator>(const SMC &a,const SMC &b){ return a.val>b.val;}
    friend bool operator<=(const SMC &a,const SMC &b){ return a.val<=b.val;}
    friend bool operator>=(const SMC &a,const SMC &b){ return a.val>=b.val;}
    friend bool operator!=(const SMC &a,const SMC &b){ return a.val!=b.val;}

    friend std::istream& operator>>(std::istream &in,SMC &a){
        long long v;
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
	SMC& operator+=(long long x) { return *this+=SMC(x); }
	SMC& operator-=(long long x) { return *this-=SMC(x); }
	SMC& operator*=(long long x) { return *this*=SMC(x); }
	SMC& operator/=(long long x) { return *this/=SMC(x); }

	friend SMC operator+(SMC a, long long b) { return a+=b; }
	friend SMC operator-(SMC a, long long b) { return a-=b; }
	friend SMC operator*(SMC a, long long b) { return a*=b; }
	friend SMC operator/(SMC a, long long b) { return a/=b; }

	friend SMC operator+(long long a, SMC b) { return b+a; }
	friend SMC operator-(long long a, SMC b) { return SMC(a)-b; }
	friend SMC operator*(long long a, SMC b) { return b*a; }
	friend SMC operator/(long long a, SMC b) { return SMC(a)/b; }
};
using Z=SMC<mod>;
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
	
    return 0;
}