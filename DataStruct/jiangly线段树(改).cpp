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
#define lc(p) (p<<1)
#define rc(p) (p<<1|1)
template<class Info,class Tag>
class SegTree{
    public:
    int n;
    vector<Info> info;
    vector<Tag> tag;
    SegTree(int n):n(n),info((n<<2)+5),tag((n<<2)+5){}
    SegTree(const vector<Info> &a):n(a.size()-1){
        //a 1-Based
        info.resize((n<<2)+5);
        tag.resize((n<<2)+5);
        bd(1,1,n,a);
    }
    inline void pushup(int p){
        info[p]=info[lc(p)]+info[rc(p)];
    }
    inline void apply(int p,int l,int r,const Tag &v){
        info[p].apply(l,r,v);
        tag[p].apply(v);
    }
    inline void pushdown(int p,int l,int r){
        if(!tag[p].has_tag()) return;
        int m=(l+r)>>1;
        apply(lc(p),l,m,tag[p]);
        apply(rc(p),m+1,r,tag[p]);
        tag[p]=Tag();
    }
    void bd(int p,int l,int r,const vector<Info> &a){
        if(l==r){
            info[p]=a[l];
            return;
        }
        int m=(l+r)>>1;
        bd(lc(p),l,m,a);
        bd(rc(p),m+1,r,a);
        pushup(p);
    }
    void upd(int p,int l,int r,int x,int y,const Tag &v){
        if(x<=l&&r<=y){
            apply(p,l,r,v);
            return;
        }
        pushdown(p,l,r);
        int m=(l+r)>>1;
        if(x<=m) upd(lc(p),l,m,x,y,v);
        if(m<y) upd(rc(p),m+1,r,x,y,v);
        pushup(p);
    }
    void mdf(int p,int l,int r,int x,const Info &v){
        if(l==r){
            info[p]=v;
            return;
        }
        pushdown(p,l,r);
        int m=(l+r)>>1;
        if(x<=m) mdf(lc(p),l,m,x,v);
        else mdf(rc(p),m+1,r,x,v);
        pushup(p);
    }
    Info qry(int p,int l,int r,int x,int y){
        if(x<=l&&r<=y) return info[p];
        pushdown(p,l,r);
        int m=(l+r)>>1;
        Info res=Info();
        if(x<=m) res=res+qry(lc(p),l,m,x,y);
        if(m<y) res=res+qry(rc(p),m+1,r,x,y);
        return res;
    }
    int findfirst(int p,int l,int r,int x,int y,
        Info &v,const function<bool(const Info&)> &chk){
        if(r<x||y<l) return n+1;
        if(x<=l&&r<=y){
            Info cmb=v+info[p];
            if(!chk(cmb)) {
                v=cmb;
                return n+1;
            }
            if(l==r) return l;
            pushdown(p,l,r);
            int m=(l+r)>>1;
            int res=findfirst(lc(p),l,m,x,y,v,chk);
            if(res!=n+1) return res;
            return findfirst(rc(p),m+1,r,x,y,v,chk);
        }
        pushdown(p,l,r);
        int m=(l+r)>>1;
        int res=findfirst(lc(p),l,m,x,y,v,chk);
        if(res!=n+1) return res;
        return findfirst(rc(p),m+1,r,x,y,v,chk);
    }
    int findlast(int p,int l,int r,int x,int y,
        Info &v,const function<bool(const Info&)> &chk){
            if(r<x||y<l) return 0;
            if(x<=l&&r<=y){
                Info cmb=v+info[p];
                if(!chk(cmb)) {
                    v=cmb;
                    return 0;
                }
                if(l==r) return l;
                pushdown(p,l,r);
                int m=(l+r)>>1;
                int res=findlast(rc(p),m+1,r,x,y,v,chk);
                if(res!=0) return res;
                return findlast(lc(p),l,m,x,y,v,chk);
            }
            pushdown(p,l,r);
            int m=(l+r)>>1;
            int res=findlast(rc(p),m+1,r,x,y,v,chk);
            if(res!=0) return res;
            return findlast(lc(p),l,m,x,y,v,chk);
    }
    int _findfirst(int p,int l,int r,int x,int y,
        const function<bool(const Info&)> &chk){
            if(r<x||y<l) return n+1;
            if(!chk(info[p])) return n+1;
            if(l==r) return l;
            pushdown(p,l,r);
            int m=(l+r)>>1;
            int res=_findfirst(lc(p),l,m,x,y,chk);
            if(res!=n+1) return res;
            return _findfirst(rc(p),m+1,r,x,y,chk);
    }
    int _findlast(int p,int l,int r,int x,int y,
        const function<bool(const Info&)> &chk){
            if(r<x||y<l) return 0;
            if(!chk(info[p])) return 0;
            if(l==r) return l;
            pushdown(p,l,r);
            int m=(l+r)>>1;
            int res=_findlast(rc(p),m+1,r,x,y,chk);
            if(res!=0) return res;
            return _findlast(lc(p),l,m,x,y,chk);
    }
    void upd(int l,int r,const Tag &v){
        upd(1,1,n,l,r,v);
    }
    void mdf(int x,const Info &v){
        mdf(1,1,n,x,v);
    }
    Info qry(int l,int r){
        return qry(1,1,n,l,r);
    }
    //寻找在[l,r]的第一个[l,k] 满足Info{l,k}满足chk e.g.[1,4]的[1,2]满足sum(1,2)<10
    //异常值: n+1
    int findfirst(int l,int r,const function<bool(const Info&)> &chk){
        Info tp=Info();
        return findfirst(1,1,n,l,r,tp,chk);
    }
    //寻找在[l,r]的最后一个[k,r] 满足Info{k,r}满足chk e.g.[1,4]的[3,4]满足sum(3,4)<10
    //异常值: 0
    int findlast(int l,int r,const function<bool(const Info&)> &chk){
        Info tp=Info();
        return findlast(1,1,n,l,r,tp,chk);
    }
    //寻找在[l,r]的第一个k 满足Info k满足chk e.g.[1,4]的第一个k=2满足info k<10
    //异常值: n+1
    int _findfirst(int l,int r,const function<bool(const Info&)> &chk){
        return _findfirst(1,1,n,l,r,chk);
    }
    //寻找在[l,r]的最后一个k 满足Info k满足chk e.g.[1,4]的最后一个k=3满足info k<10
    //异常值: 0
    int _findlast(int l,int r,const function<bool(const Info&)> &chk){
        return _findlast(1,1,n,l,r,chk);
    }
};
// Tag 结构体：定义懒标记
// 需要实现:
// 1. 成员变量: 存储懒标记信息
// 2. 默认构造函数: 表示无标记状态
// 3. apply(const Tag& v): 将另一个标记 v 合并到当前标记
// 4. has_tag(): 判断当前是否是无标记状态
struct Tag{
    int tag;
    Tag():tag(0){}
    void apply(const Tag &v){
        
    }
    bool has_tag(){
        return tag!=0;
    }
};
// Info 结构体：定义节点信息
// 需要实现:
// 1. 成员变量: 存储节点维护的信息
// 2. 默认构造函数: Info 的单位元 (例如求和的0, 求积的1)
// 3. apply(int l, int r, const Tag& v): 将懒标记 v 应用到当前节点信息上
// 4. operator+(const Info& other): 合并两个子节点的信息
struct Info{
    //...
    int info;
    Info():info(0){}
    void apply(int l,int r,const Tag &v){
        
    }
};
Info operator+(const Info &a,const Info &b){
    //...
    Info c;
    return c;
}
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}