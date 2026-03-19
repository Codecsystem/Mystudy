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
class TreeEinf{
    public:
    vector<vector<array<int,2>>> tr;
    vector<vector<int>> fa,inf;
    vector<int> dep;
    int n,k,INF,op;
    int nop(int a,int b){
        if(op==1) return max(a,b);
        else return min(a,b);
    }
    TreeEinf(int n,vector<vector<array<int,2>>> &g,int op):
        tr(g),n(n),k(__lg(n)+1),dep(n+1,0),op(op),
        fa(k+1,vector<int>(n+1,0)){
            //inf j点向上2^i步的最值
            if(op==1) INF=-1e9;
            else INF=1e9;
            inf.assign(k+1,vector<int>(n+1,INF));
            dfs(1,0,INF);
    }
    void dfs(int u,int f,int fw){
        dep[u]=dep[f]+1;
        fa[0][u]=f;
        inf[0][u]=fw;
        for(int i=1;i<=k;i++){
            fa[i][u]=fa[i-1][fa[i-1][u]];
            inf[i][u]=nop(inf[i-1][u],inf[i-1][fa[i-1][u]]);
        }
        for(auto [v,w]:tr[u]){
            if(v!=f){
                dfs(v,u,w);
            }
        }
    }
    int q(int u,int v){
        int ans=INF;
        if(dep[u]<dep[v]) swap(u,v);
        for(int i=k;i>=0;i--){
            if(dep[fa[i][u]]>=dep[v]){
                ans=nop(ans,inf[i][u]);
                u=fa[i][u];
            }
        }
        if(u==v) return ans;
        for(int i=k;i>=0;i--){
            if(fa[i][u]!=fa[i][v]){
                ans=nop(ans,inf[i][u]);
                ans=nop(ans,inf[i][v]);
                u=fa[i][u],v=fa[i][v];
            }
        }
        ans=nop(ans,inf[0][u]);
        ans=nop(ans,inf[0][v]);
        return ans;
    }
};
class TreeDinf{
    public:
    vector<vector<int>> tr;
    vector<vector<int>> fa,inf;
    vector<int> dep,val;
    int n,k,INF,op;
    int nop(int a,int b){
        if(op==1) return max(a,b);
        else return min(a,b);
    }
    TreeDinf(int n,vector<vector<int>> &g,
        int op,vector<int> &val):
        tr(g),n(n),k(__lg(n)+1),dep(n+1,0),op(op),
        fa(k+1,vector<int>(n+1,0)),val(val){
            //inf j点向上2^i步的最值
            if(op==1) INF=-1e9;
            else INF=1e9;
            inf.assign(k+1,vector<int>(n+1,INF));
            dfs(1,0);
    }
    void dfs(int u,int f){
        dep[u]=dep[f]+1;
        fa[0][u]=f;
        inf[0][u]=(f==0?val[u]:nop(val[f],val[u]));
        for(int i=1;i<=k;i++){
            fa[i][u]=fa[i-1][fa[i-1][u]];
            inf[i][u]=nop(inf[i-1][u],inf[i-1][fa[i-1][u]]);
        }
        for(auto v:tr[u]){
            if(v!=f){
                dfs(v,u);
            }
        }
    }
    int q(int u,int v){
        int ans=INF;
        if(dep[u]<dep[v]) swap(u,v);
        for(int i=k;i>=0;i--){
            if(dep[fa[i][u]]>=dep[v]){
                ans=nop(ans,inf[i][u]);
                u=fa[i][u];
            }
        }
        if(u==v) return nop(ans,val[u]);
        for(int i=k;i>=0;i--){
            if(fa[i][u]!=fa[i][v]){
                ans=nop(ans,inf[i][u]);
                ans=nop(ans,inf[i][v]);
                u=fa[i][u],v=fa[i][v];
            }
        }
        ans=nop(ans,inf[0][u]);
        ans=nop(ans,inf[0][v]);
        return ans;
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