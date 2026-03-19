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
class SegGraph{
public:
    struct node
    {
        int lc,rc;
    };
    int n,m;
    int rtin,rtout;

    vector<node> tr;
    vector<vector<int>> adj;
    vector<int> deg,val;
    int nw(){
        tr.push_back({0,0});
        adj.push_back({});
        deg.push_back(0);
        return tr.size()-1;
    }
    void add(int u,int v){
        adj[u].push_back(v);
        deg[v]++;
    }
    int build(int l,int r,int typ){
        int u=nw();
        if(l==r) return u;
        int mid=(l+r)>>1;
        tr[u].lc=build(l,mid,typ);
        tr[u].rc=build(mid+1,r,typ);
        if(!typ) add(u,tr[u].lc),add(u,tr[u].rc); //向下的树
        else add(tr[u].lc,u),add(tr[u].rc,u); //向上的树
        return u;
    }
    SegGraph(int n,int m):n(n),m(m){
        //根据实际情况计算：tr,deg,adj:点数
        tr.reserve(4*m+n+5);
        adj.reserve(4*m+n+5);
        deg.reserve(4*m+n+5);
        for(int i=0;i<=n;i++) nw(); //原始点 1-based 使用
        rtin=build(1,m,0); //向下的树
        rtout=build(1,m,1); //向上的树
        //初始化完了只初始化了两棵线段树
    };
    //区间分解
    void qry(int u,int l,int r,int ql,int qr,int tar,int typ){
        if(ql<=l&&r<=qr){
            if(!typ) add(tar,u); //向下树连边
            else add(u,tar);
            return;
        }
        int mid=(l+r)>>1;
        if(ql<=mid) qry(tr[u].lc,l,mid,ql,qr,tar,typ);
        if(mid<qr) qry(tr[u].rc,mid+1,r,ql,qr,tar,typ);
    }
    //找叶子
    int getleaf(int u,int l,int r,int rk){
        if(l==r) return u;
        int mid=(l+r)>>1;
        if(rk<=mid) return getleaf(tr[u].lc,l,mid,rk);
        else return getleaf(tr[u].rc,mid+1,r,rk);
    }
    //下树叶子->图的原始点
    void set1(int u,int p){
        add(getleaf(rtin,1,m,p),u);
    }
    //图的原始点->上树叶子
    void set2(int u,int p){
        add(u,getleaf(rtout,1,m,p));
    }
    //点向[l,r]连边
    void dot_range(int u,int l,int r){
        qry(rtin,1,m,l,r,u,0);
    }
    //[l,r]向点连边
    void range_dot(int u,int l,int r){
        qry(rtout,1,m,l,r,u,1);
    }
    //[l1,r1]向[l2,r2]连边
    void range_range(int l1,int r1,int l2,int r2){
        int vt=nw();
        range_dot(vt,l1,r1);
        dot_range(vt,l2,r2);
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;cin>>n;
    vector<int> s(n+1),t(n+1),p;
    for(int i=1;i<=n;i++) {
        cin>>s[i]>>t[i];
        p.push_back(s[i]);
        p.push_back(t[i]);
    }
    sort(p.begin(),p.end());
    p.erase(unique(p.begin(),p.end()),p.end());
    auto getid=[&](int x){
        return lower_bound(p.begin(),p.end(),x)-p.begin()+1;
    };
    SegGraph sg(n,2*n);
    for(int i=1;i<=n;i++){
        sg.set2(i,getid(s[i]));
        sg.set1(i,getid(t[i]));
    }
    for(int i=1;i<=n;i++){
        int l=getid(min(s[i],t[i])),r=getid(max(s[i],t[i]));
        if(l+1<=r-1){
            sg.dot_range(i,l+1,r-1);
            sg.range_dot(i,l+1,r-1);
        }
    }
    queue<int> q;
    vector<int> ans;
    for(int i=1;i<sg.deg.size();i++){
        if(sg.deg[i]==0) q.push(i);
    }
    while(!q.empty()){
        int u=q.front();q.pop();
        if(u<=n&&u>0) ans.push_back(u);
        for(int v:sg.adj[u]){
            sg.deg[v]--;
            if(sg.deg[v]==0) q.push(v);
        }
    }
    if(ans.size()!=n) cout<<"No\n";
    else{
        cout<<"Yes\n";
        for(int i=0;i<n;i++) cout<<ans[i]<<" \n"[i==n-1];
    }
    return 0;
}
//线段树优化建图：边数爆炸的时候可以用
//要求：存在点->区间/区间->点/区间->区间的关系