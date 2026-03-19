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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
#define lc(x) tr[x].l
#define rc(x) tr[x].r
class HJTree{
    public:
    struct node
    {
        int l,r,minn;
        //左右儿子，区间minn
    };
    vector<node> tr;
    vector<int> rt,rt_k;
    int tot,n;
    HJTree(int n,vector<vector<array<int,2>>>& q,int& maxdep):tot(0),n(n)
    {
        //a 1-based
        rt.resize(n+5);
        tr.resize((log2(n)+4)*n+5);
        rt_k.resize(maxdep+5);
        bd(rt[0],1,n);
        int cur=1;
        for(int i=1;i<=maxdep;i++)
        {
            for(auto [pos,val]:q[i])
            {
                ins(rt[cur-1],rt[cur],1,n,pos,val);
                cur++;
            }
            rt_k[i]=cur-1;
        }
    };
    void bd(int &x,int l,int r)
    {
        x=++tot; tr[x].minn=2e9;
        if(l==r) return ;
        int m=(l+r)>>1;
        bd(lc(x),l,m);
        bd(rc(x),m+1,r);
    }
    void ins(int x,int &y,int l,int r,int p,int val)
    {
        y=++tot; tr[y]=tr[x]; tr[y].minn=min(tr[y].minn,val);
        if(l==r) return ;
        int m=(l+r)>>1;
        if(p<=m) ins(lc(x),lc(y),l,m,p,val);
        else ins(rc(x),rc(y),m+1,r,p,val);
    }
    int qry(int rt,int l,int r,int s,int e){
        if(l>e||r<s) return 2e9;
        if(l>=s&&r<=e) return tr[rt].minn;
        int m=(l+r)>>1;
        return min(qry(lc(rt),l,m,s,e),qry(rc(rt),m+1,r,s,e));
    }
    int qry(int k,int s,int e)
    {
        return qry(rt[rt_k[k]],1,n,s,e);
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,r;cin>>n>>r;
    vector<int> val(n+1);
    for(int i=1;i<=n;i++) cin>>val[i];
    vector<vector<int>> tre(n+1);
    for(int i=1;i<n;i++){
        int u,v;cin>>u>>v;
        tre[u].push_back(v);
        tre[v].push_back(u);
    }
    vector<int> dep(n+1,0),dfn(n+1,0),out(n+1,0);
    int idx=0,maxdep=0;
    auto dfs=[&](this auto&& dfs,int u,int fa)->void{
        dfn[u]=++idx;
        dep[u]=dep[fa]+1,maxdep=max(maxdep,dep[u]);
        for(auto v:tre[u]) 
            if(v!=fa) dfs(v,u);
        out[u]=idx;
    };
    dep[r]=1;
    dfs(r,0);
    //在[1,n]建主席树 维护dep<=k的版本
    vector<vector<array<int,2>>> q(maxdep+1);
    for(int i=1;i<=n;i++)
    {
        q[dep[i]].push_back({dfn[i],val[i]});
    }
    int last=0,m;cin>>m;
    HJTree hjt(n,q,maxdep);
    while(m--)
    {
        int p,q;cin>>p>>q;
        int x=(p+last)%n+1;
        int k=(q+last)%n;
        int ans=hjt.qry(min(maxdep,dep[x]+k),dfn[x],out[x]);
        cout<<ans<<'\n';
        last=ans;
    }
    return 0;
}
//https://codeforces.com/contest/893/problem/F
//主席树维护一个前缀结构 解决二维偏序