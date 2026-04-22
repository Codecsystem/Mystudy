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
using namespace std;
class SHASH{
    public:
        const int m1=1e9+7,m2=1e9+9;
        int b1,b2,m;
        vector<int> h1,h2;
        vector<int> p1,p2;
        //m 预估的最长字符串长度
        SHASH(int m):p1(m+1,1),p2(m+1,1),h1(m+1,0),h2(m+1,0){
            mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
            b1=rng()%(int)1e9+1e6,b2=rng()%(int)1e9+1e6;
            //b1=131,b2=13331;
            for(int i=1;i<=m;i++){
                p1[i]=(1ll*p1[i-1]*b1)%m1;
                p2[i]=(1ll*p2[i-1]*b2)%m2;
            }
        }
        //s 1based
        void calc(const string& s,int n){
            for(int i=1;i<=n;i++){
                h1[i]=(1ll*h1[i-1]*b1+s[i])%m1;
                h2[i]=(1ll*h2[i-1]*b2+s[i])%m2;
            }
        }
        ull get(int l,int r){
            int len=r-l+1;
            int r1=h1[r]-1ll*h1[l-1]*p1[len]%m1;
            int r2=h2[r]-1ll*h2[l-1]*p2[len]%m2;
            r1<0?r1+=m1:0,r2<0?r2+=m2:0;
            return ((ull)r1<<32)|r2;
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