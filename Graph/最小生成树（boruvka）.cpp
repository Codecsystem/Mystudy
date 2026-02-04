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
using ll=long long;
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
    int t;cin>>t;
    while(t--)
    {
        int n,m;cin>>n>>m;
        vector<int> a(n+1);
        vector<vector<int>> g(n+1);
        for(int i=1;i<=n;i++) cin>>a[i];
        for(int i=1;i<=m;i++){
            int u,v;cin>>u>>v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        set<array<int,2>> s;
        for(int i=1;i<=n;i++)
            s.insert({a[i],i});
        ll ans=0;
        DSU dsu(n);
        auto boruvka=[&](){
            int cnt=0;
            vector<vector<int>> now(n+1);
            for(int i=1;i<=n;i++) now[dsu.find(i)].push_back(i);
            for(auto &vec:now){
                int nm=2e9+5,idx=-1;
                for(auto &x:vec) s.erase({a[x],x});
                for(auto &u:vec){
                    for(auto &v:g[u]){
                        if(dsu.find(v)==dsu.find(u)) continue;
                        s.erase({a[v],v});
                    }
                    if(s.size()){
                        auto [val,id]=*s.begin();
                        if(val+a[u]<nm) nm=val+a[u],idx=id;
                    }
                    for(auto &v:g[u]){
                        if(dsu.find(v)==dsu.find(u)) continue;
                        s.insert({a[v],v});
                    }
                }
                if(idx!=-1&&!dsu.same(idx,vec[0])){
                    cnt++;
                    ans+=nm;
                    dsu.merge(idx,vec[0]);
                }
                for(auto &x:vec) s.insert({a[x],x});
            }
            return cnt;
        };
        while(boruvka());
        if(dsu.size(1)!=n) cout<<"-1\n";
        else cout<<ans<<'\n';
    };
    return 0;
}
//boruvka算法
//考虑对多个联通块找最短的出边，然后合并，每次合并能使联通块个数/2，
//所以时间复杂度是O(nlogn*p),其中p是每次找出边的复杂度
//处理完全图好
//此处是对每一轮边合并边找，标准的写法是对每一轮先找出所有边，然后合并边（这个不容易有错）