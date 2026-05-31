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
class StLca{
    public:
        vector<vector<int>> dp,tr;
        int n,tim;
        vector<int> dep,eul,fir;
        int inf(int a,int b){return dep[a]<dep[b]?a:b;}
        void init(vector<int>& a,int m)
        {
            if(!m) return;
            int len=__lg(m)+1;
            dp.assign(len,vector<int>(m+1));
            for(int i=1;i<=m;i++) dp[0][i]=a[i];
            for(int j=1;j<len;j++)
                for(int i=1;i+(1<<j)-1<=m;i++)
                    dp[j][i]=inf(dp[j-1][i],dp[j-1][i+(1<<(j-1))]);
        }
        int query(int l,int r)
        {
            int k=__lg(r-l+1);
            return inf(dp[k][l],dp[k][r-(1<<k)+1]);
        }
        StLca(int n,const vector<vector<int>>& tr):tr(tr),n(n){
            dep.assign(n+1,0),fir.resize(n+1);
            eul.resize(2*n);tim=0;dfs(1,0);
            init(eul,2*n-1);
        }
        void dfs(int u,int fa){
            tim++,eul[tim]=u,fir[u]=tim;
            dep[u]=dep[fa]+1;
            for(auto v:tr[u]){
                if(v==fa) continue;
                dfs(v,u);
                eul[++tim]=u;
            }
        }
        int lca(int u,int v){
            if(fir[u]>fir[v]) swap(u,v);
            return query(fir[u],fir[v]);
        }
        int dis(int u,int v){
            return dep[u]+dep[v]-2*dep[lca(u,v)];
        }
};
class StLca{
    public:
        vector<vector<int>> dp;
        const vector<vector<int>>& tr;
        int n,tim;
        vector<int> dep,eul,fa,dfn,seq;
        int inf(int a,int b){return dep[a]<dep[b]?a:b;}
        void init(vector<int>& a,int m)
        {
            if(!m) return;
            int len=__lg(m)+1;
            dp.assign(len,vector<int>(m+1));
            for(int i=1;i<=m;i++) dp[0][i]=a[i];
            for(int j=1;j<len;j++)
                for(int i=1;i+(1<<j)-1<=m;i++)
                    dp[j][i]=inf(dp[j-1][i],dp[j-1][i+(1<<(j-1))]);
        }
        int query(int l,int r)
        {
            int k=__lg(r-l+1);
            return inf(dp[k][l],dp[k][r-(1<<k)+1]);
        }
        StLca(int n,const vector<vector<int>>& tr,int rt):tr(tr),n(n){
            dep.assign(n+1,0),dfn.resize(n+1),seq.resize(n+1);
            fa.resize(n+1);tim=0;dfs(rt,0);
            init(seq,n);
        }
        void dfs(int u,int f){
            dfn[u]=++tim;fa[u]=f;
            dep[u]=dep[f]+1;seq[tim]=u;
            for(auto v:tr[u]){
                if(v==f) continue;
                dfs(v,u);
            }
        }
        int lca(int u,int v){
            if(u==v) return u;
            if(dfn[u]>dfn[v]) swap(u,v);
            return fa[query(dfn[u]+1,dfn[v])];
        }
        int dis(int u,int v){
            return dep[u]+dep[v]-2*dep[lca(u,v)];
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
//O(nlogn)-O(1) lca 通过拍成欧拉序|dfs序然后st表得到
//dfs序常数更小