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
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    REDSU rsu(n);
    vector<array<int,3>> op(m+1);
    vector<vector<int>> optr(m+1);
    op[0]={2,-1,-1};
    for(int i=1;i<=m;i++){
        int x;cin>>x;
        if(x!=2){
            int a,b;cin>>a>>b;
            op[i]={x,a,b};
            optr[i].push_back(i-1);
            optr[i-1].push_back(i);
        }
        else{
            int a;cin>>a;
            op[i]={x,a,-1};
            optr[i].push_back(a);
            optr[a].push_back(i);
        }
    }
    vector<int> ans(m+1,-1);
    auto dfs=[&](this auto&& dfs,int u,int f)->void{
        if(op[u][0]!=2){
            if(op[u][0]==1) rsu.merge(op[u][1],op[u][2]);
            else ans[u]=rsu.same(op[u][1],op[u][2]);
        }
        for(int y:optr[u]) 
            if(y!=f) dfs(y,u);
        if(op[u][0]==1) rsu.back();
    };
    dfs(0,0);
    for(int i=1;i<=m;i++){
        if(op[i][0]==3) cout<<ans[i]<<endl;
    }
    return 0;
}