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
#include <random>
#define int long long //赫赫 要不要龙龙呢
#define ull unsigned long long
using namespace std;
class SHash{
    public:
    const int m1=1e9+7,m2=1e9+9;
    int b1,b2;
    SHash(){
        mt19937_64 rand(time(0));
        b1=rand()%(int)1e9+1e6,b2=rand()%(int)1e9+1e6;
    }
    ull get(string s){
        int h1=0,h2=0;
        for(auto c:s){
            h1=(h1*b1+c)%m1;
            h2=(h2*b2+c)%m2;
        }
        return ((ull)h1)<<32|(ull)(h2);
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    SHash hs;
    while(t--)
    {
        int n;cin>>n;
        vector<array<ull,2>> a(n+1);
        for(int i=1;i<=n;i++)
        {
            string s1,s2;cin>>s1>>s2;
            a[i]={hs.get(s1),hs.get(s2)};
        }
        vector<vector<int>> mp(n+1);
        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=n;j++)
            {
                if(i==j) continue;
                if(a[i][0]==a[j][0]||a[i][1]==a[j][1]) mp[i].push_back(j);
            }
        }
        int st=(1<<n);
        vector<vector<int>> dp(st,vector<int>(n+1,0));
        for(int i=1;i<=n;i++) dp[1<<(i-1)][i]=1;
        for(int i=0;i<st;i++)
        {
            for(int u=1;u<=n;u++)
            {
                if(!dp[i][u]) continue;
                for(int v:mp[u])
                {
                    if((i|(1<<(v-1)))!=i) 
                        dp[i|(1<<(v-1))][v]|=dp[i][u];
                }
            }
        }
        int ans=0;
        for(int i=0;i<st;i++)
        {
            for(int j=1;j<=n;j++)
            {
                if(dp[i][j])
                {
                    ans=max(ans,(int)__builtin_popcountll(i));
                }
            }
        }
        cout<<n-ans<<endl;
    }
    return 0;
}