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
class REDSU{
public:
    vector<int> fa,sz;
    int n;vector<array<int,2>> st;
    REDSU(int n):n(n){
        fa.resize(n+5);
        sz.resize(n+5);
        st.reserve(n+5);
        for(int i=1;i<=n;i++){
            fa[i]=i;
            sz[i]=1;
        }
    }
    int find(int x){
        while(x!=fa[x]) x=fa[x];
        return x;
    }
    bool same(int x,int y){
        return find(x)==find(y);
    }
    void merge(int x,int y){
        x=find(x),y=find(y);
        if(x==y) 
        {
            st.push_back({0,y});
            return;
        }
        if(sz[x]<sz[y]) swap(x,y); //sz[x]>=sz[y]
        st.push_back({1,y});sz[x]+=sz[y];fa[y]=x;
    }
    int size(int x){return sz[find(x)];}
    void back(){
        if(!st.empty()){
            auto [fg,y]=st.back();st.pop_back();
            if(!fg) return;
            sz[fa[y]]-=sz[y];fa[y]=y;
        }
    }
    void back_k(int k){
        while(k--) back();
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    REDSU dsu(n+5);
    vector<vector<array<int,2>>> mp(n+1);
    vector<array<int,2>> ed(m+5);
    for(int i=1;i<=m;i++)
    {
        int u,v,w=i;cin>>u>>v;
        mp[u].push_back({v,w});
        mp[v].push_back({u,w});
        ed[w]={u,v};
    }
    int q;cin>>q;
    vector<array<int,4>> qr;
    for(int i=1;i<=q;i++)
    {
        int x,y,z;cin>>x>>y>>z;
        qr.push_back({x,y,z,i});
    }
    vector<int> ans(q+1);
    auto sol=[&](this auto&& sol,int l,int r,vector<array<int,4>>& qx)->void{
        int mid=(l+r)>>1;
        if(l==r){
            for(auto &[_,__,___,id]:qx){
                ans[id]=l;
            }
            return ;
        }
        for(int i=l;i<=mid;i++){
            dsu.merge(ed[i][0],ed[i][1]);
        }
        vector<array<int,4>> q1,q2;
        for(auto &[x,y,z,id]:qx){
            int now=0;
            if(dsu.same(x,y)) now=dsu.size(x);
            else now=dsu.size(x)+dsu.size(y);
            if(now>=z) q1.push_back({x,y,z,id});
            else q2.push_back({x,y,z,id});
        }
        sol(mid+1,r,q2);
        dsu.back_k(mid-l+1);
        sol(l,mid,q1);
    };
    sol(1,m,qr);
    for(int i=1;i<=q;i++)
    {
        cout<<ans[i]<<'\n';
    }
    return 0;
}
//整体二分(离线算法) agc2 d
//把q次二分操作放在一起 对询问整体做二分
//一般的二分是考虑区间[l,r],每次二分后变成[l,mid]和[mid+1,r],对单个询问
//整体二分是考虑区间[l,r],每次二分后变成[l,mid]和[mid+1,r],对多个询问 把询问分到左右区间
//时间复杂度分析我们可以把他变成一颗线段树来看
//通常 我们在一层递归的时候我们要保证[1,l-1]的信息存在
//考虑同层的区间修改是常数的 所以修改操作是O(nlogn)的