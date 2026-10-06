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
class tree{
    public:
    vector<vector<int>> tr;
    vector<vector<int>> fa;
    vector<int> dfn,dep,siz,in,out,eul;
    int n,k,tot,tim;
    tree(int n,vector<vector<int>>& tr):
    tr(tr),n(n),k(__lg(n)+1),tim(0),dfn(n+1,0),dep(n+1,0),tot(0),siz(n+1,0),
    fa(n+1,vector<int>(__lg(n)+2,0)),in(n+1,0),out(n+1,0),eul(2*n+1,0){
        dfs(1,0);
    }

    int dfs(int u,int f){
        dfn[u]=++tot,in[u]=++tim;eul[tim]=u;
        dep[u]=dep[f]+1;
        fa[u][0]=f;siz[u]=1;
        for(int i=1;i<=k;i++){ 
            fa[u][i]=fa[fa[u][i-1]][i-1]; 
        }
        for(auto v:tr[u]){
            if(v==f) continue;
            siz[u]+=dfs(v,u);
        }
        out[u]=++tim,eul[tim]=u;
        return siz[u];
    }
    //求u,v的最近公共祖先
    int lca(int u,int v){
        if(dep[u]<dep[v]) swap(u,v);
        for(int i=k;i>=0;i--){
            if(dep[fa[u][i]]>=dep[v]) u=fa[u][i];
        }
        if(u==v) return u;
        for(int i=k;i>=0;i--){
            if(fa[u][i]!=fa[v][i]){
                u=fa[u][i];
                v=fa[v][i];
            }
        }
        return fa[u][0];
    }
    //求u,v距离
    int dis(int u,int v){
        return dep[u]+dep[v]-2*dep[lca(u,v)];
    }
    //判断x是否在s,t的路径上
    bool con(int s,int t,int x){
        return dis(s,x)+dis(t,x)==dis(s,t);
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,q;cin>>n>>q;
    vector<int> a(n+1),vis(n+1,0),g(n+5,0),cnt(n+1,0);
    for(int i=1;i<=n;i++) cin>>a[i];
    vector<vector<int>> tr(n+1);
    for(int i=1;i<n;i++){
        int u,v;cin>>u>>v;
        tr[u].push_back(v);
        tr[v].push_back(u);
    }
    tree t(n,tr);
    vector<array<int,6>> qry(q);
    vector<int> ans(q);
    for(int i=0;i<q;i++){
        int u,v,qa,qb;cin>>u>>v>>qa>>qb;
        if(t.in[u]>t.in[v]) swap(u,v);
        if(t.lca(u,v)==u){
            qry[i]={t.in[u],t.in[v],qa,qb,0,i};
        }
        else{
            qry[i]={t.in[u]+1,t.in[v],qa,qb,t.lca(u,v),i};
        }
    }
    const int B=2*n/static_cast<int>(sqrt(q));
    auto cmp=[&](array<int,6> a,array<int,6> b){
        if(a[0]/B!=b[0]/B) return a[0]/B<b[0]/B;
        else{
            if((a[0]/B)&1) return a[1]>b[1];
            else return a[1]<b[1];
        }
    };
    sort(qry.begin(),qry.end(),cmp);
    int l=1,r=0;
    auto f=[&](int x){
        int c=a[x];
        if(vis[x]){
            g[cnt[c]]--;
            cnt[c]--;
        }
        else{
            cnt[c]++;
            g[cnt[c]]++;
        }
        vis[x]^=1;
    };
    for(auto [ql,qr,qa,qb,lca,i]:qry){
        while(r<qr) f(t.eul[++r]);
        while(l>ql) f(t.eul[--l]);
        while(r>qr) f(t.eul[r--]);
        while(l<ql) f(t.eul[l++]);
        if(lca) f(lca);
        ans[i]=g[qa]-g[qb+1];
        if(lca) f(lca);
    }
    for(int i=0;i<q;i++) cout<<ans[i]<<'\n';
    return 0;
}