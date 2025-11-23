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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
class TreeDiff{
    public:
    vector<int> cnt,dep,fa;
    vector<int> siz,hson,top;
    vector<vector<int>> tr;
    //cnt在点差分的时候代表点上的数值
    //cnt在边差分的时候代表该点和父亲连边上的数值
    int n;

    TreeDiff(int n,vector<vector<int>>& tr):
        n(n),cnt(n+1,0),dep(n+1,0),siz(n+1,0),
        hson(n+1,-1),top(n+1,-1),fa(n+1,0),tr(tr){ 
        //a 默认根为1
        dfs1(1,0,tr);
        top[1]=1;
        dfs2(1,tr);
    }
    void dfs1(int u,int f,vector<vector<int>>& tr){
        dep[u]=dep[f]+1;
        fa[u]=f;siz[u]=1;
        for(auto v:tr[u]){
            if(v==f)continue;
            dfs1(v,u,tr);
            siz[u]+=siz[v];
            if(hson[u]==-1||siz[hson[u]]<siz[v])
                hson[u]=v;
        }
    }
    void dfs2(int u,vector<vector<int>>& tr){
        if(hson[u]!=-1){
            top[hson[u]]=top[u];
            dfs2(hson[u],tr);
        }
        for(auto v:tr[u]){
            if(v==fa[u]||v==hson[u])
                continue;
            top[v]=v;
            dfs2(v,tr);
        }
    }
    int lca(int u,int v){
        while(top[u]!=top[v]){
            if(dep[top[u]]<dep[top[v]])
                swap(u,v);
            u=fa[top[u]];
        }
        return dep[u]<dep[v]?u:v;
    }
    void Dadd(int u,int v,int w){
        cnt[u]+=w,cnt[v]+=w;
        cnt[lca(u,v)]-=w;
        if(fa[lca(u,v)]!=0)
            cnt[fa[lca(u,v)]]-=w;
    }
    void Eadd(int u,int v,int w){
        cnt[u]+=w,cnt[v]+=w;
        cnt[lca(u,v)]-=2*w;
    }
    void q(int u,int f){
        for(auto v:tr[u]){
            if(v==f)continue;
            q(v,u);
            cnt[u]+=cnt[v];
        }
    }
    vector<int> dq() {return cnt;}
    vector<array<int,4>> eq(){
        vector<array<int,4>> res;
        auto dfs=[&](auto self,int u,int f,int w)->void{
            for(auto v:tr[u]){
                if(v==f)continue;
                self(self,v,u,w);
            }
            res.push_back({u,f,w,cnt[u]});
        };
        dfs(dfs,1,0,0);
        return res;
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}