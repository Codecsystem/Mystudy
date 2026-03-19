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
        int l,r,s;
        //左右儿子，区间数频次
    };
    vector<node> tr;
    vector<int> b,rt;
    int tot,n,bn;
    HJTree(int n,const vector<int>& a):tot(0),n(n){
        //a 1-based
        rt.resize(n+5);
        tr.resize((log2(n)+4)*n+5);
        b.resize(n);
        //注意空间是2*n+(ceil(log2(n))+1)*n
        b.assign(a.begin()+1,a.end());
        sort(b.begin(),b.end());
        b.erase(unique(b.begin(),b.end()),b.end());
        bn=b.size();
        bd(rt[0],1,bn);
        for(int i=1;i<=n;i++) ins(rt[i-1],rt[i],1,bn,getid(a[i]));
    };
    int getid(int x){
        //离散化 ->[1,n]
        return lower_bound(b.begin(),b.end(),x)-b.begin()+1;
    }
    void bd(int &x,int l,int r)
    {
        x=++tot; tr[x].s=0;
        if(l==r) return ;
        int m=(l+r)>>1;
        bd(lc(x),l,m);
        bd(rc(x),m+1,r);
    }
    void ins(int x,int &y,int l,int r,int tar)
    {
        y=++tot; tr[y]=tr[x]; tr[y].s++;
        if(l==r) return ;
        int m=(l+r)>>1;
        if(tar<=m) ins(lc(x),lc(y),l,m,tar);
        else ins(rc(x),rc(y),m+1,r,tar);
    }
    int qry(int x,int y,int l,int r,int tar){
        if(l==r) return l;
        int m=(l+r)>>1;
        int s=tr[lc(y)].s-tr[lc(x)].s;
        if(tar<=s) return qry(lc(x),lc(y),l,m,tar);
        else return qry(rc(x),rc(y),m+1,r,tar-s);
    }
    int qry(int l,int r,int k){
        return b[qry(rt[l-1],rt[r],1,bn,k)-1];
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    HJTree hjt(n,a);
    while(m--){
        int l,r,k;cin>>l>>r>>k;
        cout<<hjt.qry(l,r,k)<<'\n';
    }
    return 0;
}
//主席树 (静态区间第k小)
//利用权值线段树,维护a[1]-a[n] n次插入的历史版本 
//于是可以利用前缀和思想,求出任意区间第k小
//时间复杂度qry(logn) 空间复杂度nlogn+2*n
//本质上是做了一个单点更新 保存历史版本 维护一个前缀结构 以此可以处理很多二维偏序问题