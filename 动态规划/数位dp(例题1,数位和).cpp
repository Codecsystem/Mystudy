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
const int mod=1e9+7;;
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
signed main()
{
	int T_start=clock();
	//freopen("in.txt","r",stdin);
	//freopen("out.txt","w",stdout);
	//ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
	int t;cin>>t;
	vector<vector<Z>> dp(20,vector<Z>(18*9+5,-1));
	//dp[i][j]表示[i+1,len](除低i位的高位)数位和=j时,[1,i]任选的所有方案的数位和
	while(t--)
	{
	    int l,r;cin>>l>>r;
		vector<int> bit;
		auto work=[&](int x)->int{
			bit.clear();
			bit.push_back(0);//1-based
			while(x) bit.push_back(x%10),x/=10;
			return bit.size()-1;
		};
		auto dfs=[&](this auto&& dfs,int pos,bool lim,int sum)->Z{
			//从len位填到pos+1位,lim表示是否受上界限制,sum表示当前数位和
			//现在填pos位 也就是说dfs的含义是[pos+1,len]数位和=sum时,pos位受lim限制的方案数
			if(pos==0) return sum;//第0位,直接返回sum
			if(!lim&&dp[pos][sum]!=-1) return dp[pos][sum];
			int up=lim?bit[pos]:9;
			Z res=0;
			for(int i=0;i<=up;i++)
			{
				res+=dfs(pos-1,lim&&i==up,sum+i);
				//传递受上界限制的状态
			}
			if(!lim) dp[pos][sum]=res;
			return res;
		};
		auto solve=[&](int x)->Z{
			int len=work(x);
			return dfs(len,1,0);
		};
		cout<<solve(r)-solve(l-1)<<endl;
	}
	return 0;
}
//数位dp 计算[l,r]内所有数的数位和
//dfs 形参总结
//