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

signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    vector<vector<int>> mp(n+1);
    vector<int> deg(n+1,0),vis(n+1,0);
    for(int i=0;i<m;i++){
        int u,v;cin>>u>>v;
        mp[u].push_back(v);
        mp[v].push_back(u);
        deg[u]++,deg[v]++;
    }
    vector<vector<int>> g(n+1);
    for(int u=1;u<=n;u++){
        for(int v:mp[u]){
            if(deg[u]>deg[v]||(deg[u]==deg[v]&&u<v)){
                g[u].push_back(v);
            }
        }
    }
    int ans=0;
    for(int u=1;u<=n;u++){
        for(auto v:mp[u]) vis[v]=1;
        for(auto v:g[u]){
            for(auto w:g[v]){
                if(vis[w]) ans++;
            }
        }
        for(auto v:mp[u]) vis[v]=0;
    }
    cout<<ans<<endl;
    return 0;
}
//无向图三元环计数 O(msqrt(m))
//由 无向图把边定向：度数小的指向度数大的点，然后每个点的出度<=sqrt(2m) 保证
//可用根号分治证明
//至于不重不漏的原因，某个三元环定向后一定是算法所示的形态，按度数最小的分类就是对的