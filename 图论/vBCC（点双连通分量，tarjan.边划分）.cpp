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
vector<vector<int>> tarjan(const vector<vector<array<int,2>>>&g,int n){
    vector<int> dfn(n+1,0),low(n+1,0);
    vector<vector<int>> bccs;
    stack<int> st;int cnt=0;
    auto dfs=[&](auto&& dfs,int u,int in)->void{
        dfn[u]=low[u]=++cnt;
        for(auto[v,id]:g[u]){
            if(!dfn[v]){
                st.push(id);
                dfs(dfs,v,id);
                low[u]=min(low[u],low[v]);
                if(low[v]>=dfn[u]){
                    vector<int> bcc;
                    while(true){
                        int e=st.top();st.pop();
                        bcc.push_back(e);
                        if(e==id)break;
                    }
                    bccs.push_back(bcc);
                }
            }else if(id!=in&&dfn[v]<dfn[u]){
                st.push(id);
                low[u]=min(low[u],dfn[v]);
            }
        }
    };
    for(int i=1;i<=n;i++){
        if(!dfn[i]) dfs(dfs,i,0);
    }
    return bccs;
}
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    vector<vector<array<int,2>>> g(n+1);
    for(int i=1;i<=m;i++){
        int u,v;cin>>u>>v;
        g[u].push_back({v,i});
        g[v].push_back({u,i});
    }
    auto bccs=tarjan(g,n);
    cout<<bccs.size()<<"\n";
    for(const auto&bcc:bccs){
        cout<<bcc.size()<<" ";
        for(int id:bcc)cout<<id<<" ";
        cout<<"\n";
    }
    return 0;
}