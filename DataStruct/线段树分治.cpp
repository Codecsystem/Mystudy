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
#define lc(x) (x<<1)
#define rc(x) (x<<1|1)
class REDSU{
public:
    vector<int> fa,sz;
    int n,tag;vector<array<int,3>> st;
    REDSU(int n):n(n){
        tag=1;
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
            st.push_back({0,-1,tag});
            return;
        }
        if(sz[x]<sz[y]) swap(x,y); //sz[x]>=sz[y]
        st.push_back({1,y,tag});sz[x]+=sz[y];fa[y]=x;
    }
    int size(int x){return sz[find(x)];}
    void back(){
        if(!st.empty()){
            auto [fg,y,prvtag]=st.back();st.pop_back();
            tag=prvtag;
            if(!fg) return;
            sz[fa[y]]-=sz[y];fa[y]=y;
        }
    }
    void back_k(int k){
        while(k--) back();
    }
};
class SegDiv{
public:
    struct info
    {
        vector<array<int,2>> ed;
    };
    int n;vector<info> tr;
    vector<int> ans;
    SegDiv(int n):n(n){
        tr.resize((n<<2)+5);
        ans.resize(n+5);
    }
    void ins(int p,int l,int r,int ql,int qr,array<int,2> d){
        if(ql<=l&&r<=qr){
            tr[p].ed.push_back(d);
            return;
        }
        int mid=(l+r)>>1;
        if(ql<=mid) ins(lc(p),l,mid,ql,qr,d);
        if(mid<qr) ins(rc(p),mid+1,r,ql,qr,d);
    }
    void q(int p,int l,int r,REDSU &dsu){
        int k=0;
        for(const auto &[u,v]:tr[p].ed){
            if(dsu.same(u,v)){
                dsu.st.push_back({0,-1,dsu.tag}),k++;
                if(dsu.tag==1) dsu.tag=0;
            }
            dsu.merge(u,v+n);
            dsu.merge(v,u+n);
            k+=2;
        }
        if(l==r){
            ans[l]=dsu.tag;
        }
        else{
            int mid=(l+r)>>1;
            q(lc(p),l,mid,dsu);
            q(rc(p),mid+1,r,dsu);
        }
        dsu.back_k(k);
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m,k;cin>>n>>m>>k;
    REDSU dsu(2*n+5);
    SegDiv segdiv(k+5);
    for(int i=1;i<=m;i++){
        int u,v,l,r;cin>>u>>v>>l>>r;
        //[l,r) 0-> [l+1,r+1)->[l+1,r]
        segdiv.ins(1,1,k,l+1,r,{u,v});
    }
    segdiv.q(1,1,k,dsu);
    for(int i=1;i<=k;i++) cout<<(segdiv.ans[i]==1?"Yes":"No")<<'\n';
    return 0;
}
//线段树分治：把区间信息挂到线段树的点上 然后dfs线段树，维护一些东西
//挂信息的复杂度是logn的