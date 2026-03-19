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
#include <unordered_set>
#include <numeric>
#include <functional>
#include <ranges>
#include <iomanip>
#include <chrono>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using ull=unsigned long long;
using namespace std;
class DSU{
    public:
        int n;vector<int> fa,sz;
        DSU(int n):n(n)
        {
            srand(time(NULL));
            fa.resize(n+1);
            sz.resize(n+1);
            for(int i=1;i<=n;i++)
            {
                fa[i]=i;
                sz[i]=1;
            }
        }
        int find(int u){
            return fa[u]==u?u:fa[u]=find(fa[u]);
        }
        void merge(int a,int b)
        {
            int u=find(a),v=find(b);
            if(u==v) return;
            fa[u]=v;
            sz[v]+=sz[u];
        }
        int same(int a,int b)
        {
            return find(a)==find(b);
        }
        int size(int u){
            return sz[find(u)];
        }
        vector<vector<int>> get(){
            vector<vector<int>> ans(n+1);
            for(int i=1;i<=n;i++)
            {
                ans[find(i)].push_back(i);
            }
            ans.erase(remove(ans.begin(),ans.end(),vector<int>()),ans.end());
            return ans;
        }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    const int INF=2e9+5;
    int t;cin>>t;
    while(t--)
    {
        int n,m;cin>>n>>m;
        vector<int> a(n+1),p(n+1),ip(n+1);
        for(int i=1;i<=n;i++) cin>>a[i];
        iota(p.begin()+1,p.end(),1);
        sort(p.begin()+1,p.end(),[&](int x,int y){return a[x]<a[y];});
        for(int i=1;i<=n;i++) ip[p[i]]=i;
        sort(a.begin()+1,a.end());

        vector<vector<int>> g(n+1);
        for(int i=1;i<=m;i++)
        {
            int u,v;cin>>u>>v;
            u=ip[u],v=ip[v];
            g[u].push_back(v);
            g[v].push_back(u);
        }
        for(int i=1;i<=n;i++) sort(g[i].begin(),g[i].end());
        auto find=[&](int u,int v){
            auto it=lower_bound(g[u].begin(),g[u].end(),v);
            if(it==g[u].end()||*it!=v) return 0;
            return 1;
        };

        DSU dsu(n);
        vector<int> cur(n+1,1);
        ll ans=0;
        auto boruvka=[&](){
            int cnt=0;
            vector<int> r(n+1); r[n]=n;
            for(int i=n-1;i>=1;i--){
                if(dsu.same(i,i+1)) r[i]=r[i+1];
                else r[i]=i;
            }
            auto now=dsu.get();
            vector<array<int,2>> best(now.size(),{INF,-1});
            for(int i=0;i<now.size();i++){
                auto &vec=now[i];
                for(auto x:vec){
                    int j=cur[x];
                    while(j<=n){
                        if(dsu.same(x,j)){
                            j=r[j]+1;
                            continue;
                        }
                        if(find(x,j)){
                            j+=1;
                            continue;
                        }
                        if(best[i][0]>a[x]+a[j]){
                            best[i]={a[x]+a[j],j};
                        }
                        break;
                    }
                    cur[x]=j;
                }
            }
            for(int i=0;i<now.size();i++){
                if(best[i][1]!=-1){
                    if(!dsu.same(now[i][0],best[i][1])){
                        ans+=best[i][0];
                        dsu.merge(now[i][0],best[i][1]);
                        cnt++;
                    }
                }
            }
            return cnt;
        };
        while(boruvka());
        if(dsu.size(1)!=n) cout<<"-1\n";
        else cout<<ans<<"\n";
    }
    return 0;
}

//比较标准的boruvka算法