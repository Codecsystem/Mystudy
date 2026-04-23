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
using ull=unsigned long long;
using ui128=__uint128_t;
using namespace std;
class SHASH{
    public:
        ull b;
        vector<ull> h,p;
        const ull mod=(1ull<<61)-1;
        inline ull add(ull a,ull b){ a+=b;return a>=mod?a-mod:a;}
        inline ull sub(ull a,ull b){ return a>=b?a-b:a+mod-b;}
        inline ull mul(ull a,ull b){ ui128 c=ui128(a)*b; return (add(c>>61,c&mod));}
        SHASH(int m):p(m+1,1),h(m+1,0){
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            b=rng()%(mod-1313131)+1313131;
            for(int i=1;i<=m;i++) p[i]=mul(p[i-1],b);
        }
        void calc(const string& s,int n){
            for(int i=1;i<=n;i++) h[i]=add(mul(h[i-1],b),s[i]);
        }
        ull get(int l,int r){
            return sub(h[r],mul(h[l-1],p[r-l+1]));
        }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}