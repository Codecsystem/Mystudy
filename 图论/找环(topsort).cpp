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

signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    vector<vector<int>> mp(n+1);
    vector<int> deg(n+1,0);
    for(int i=1;i<=m;i++)
    {
        int u,v;cin>>u>>v;
        mp[u].push_back(v);
        mp[v].push_back(u);
        deg[u]++,deg[v]++;
    }
    queue<int> q;
    for(int i=1;i<=n;i++)
    {
        if(deg[i]==1) q.push(i);
    }
    while(!q.empty())
    {
        int u=q.front();q.pop();
        for(int v:mp[u])
        {
            deg[v]--;
            if(deg[v]==1) q.push(v);
        }
    }
    
    return 0;
}
//无向图,跑完topsort后,度数>1的点一定在环上
//有向图只要把入度=0的入队,跑完topsort后,入度>0的点一定在环上
//同理topsort可以用来判环