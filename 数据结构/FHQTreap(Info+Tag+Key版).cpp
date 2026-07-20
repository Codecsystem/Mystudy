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
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class Key,class Info,class kTag,class iTag>
class FHQTreap{
    public:
    struct Node{
        Key key;
        Info info;
        kTag ktag;
        iTag itag;
        int sz,rnd,l,r,id;
    };
    vector<Node> tr;
    int rt,tot;
    FHQTreap(int n){
        tr.resize(n+5);
        tr[0]={Key(),Info(),kTag(),iTag(),0,0,0,0,0};
        rt=tot=0;
    }
    int nw(Key kval,Info ival,int id=0){
        tr[++tot]={kval,ival,kTag(),iTag(),1,(int)rng(),0,0,id};
        return tot;
    }
    inline void pushup(int u){
        tr[u].sz=1+tr[tr[u].l].sz+tr[tr[u].r].sz;
        tr[u].key.pushup(tr[tr[u].l].key,tr[tr[u].r].key);
        tr[u].info.pushup(tr[tr[u].l].info,tr[tr[u].r].info,tr[u].key);
    }
    inline void apply(int u,const kTag &kt,const iTag &it){
        if(!u)return;
        if(kt.has_tag()){
            tr[u].key.apply(kt,tr[u].sz);
            tr[u].ktag.apply(kt);
        }
        if(it.has_tag()){
            tr[u].info.apply(it,tr[u].sz);
            tr[u].itag.apply(it);
        }
    }
    inline void pushdown(int u){
        if(!tr[u].ktag.has_tag()&&!tr[u].itag.has_tag())return;
        apply(tr[u].l,tr[u].ktag,tr[u].itag);
        apply(tr[u].r,tr[u].ktag,tr[u].itag);
        tr[u].ktag.clear();tr[u].itag.clear();
    }
    void merge(int &u,int a,int b){
        if(!a||!b){u=a|b;return;}
        pushdown(a);pushdown(b);
        if(tr[a].rnd>tr[b].rnd){
            u=a;merge(tr[a].r,tr[a].r,b);
        }else{
            u=b;merge(tr[b].l,a,tr[b].l);
        }
        pushup(u);
    }
    void split(int u,int &a,int &b,Key val,bool strict=false){
        if(!u){a=b=0;return;}
        pushdown(u);
        bool go_right=strict?(tr[u].key<val):!(val<tr[u].key);
        if(go_right){
            a=u;split(tr[u].r,tr[a].r,b,val,strict);
        }else{
            b=u;split(tr[u].l,a,tr[b].l,val,strict);
        }
        pushup(u);
    }
    void insert(Key val,Info ival=Info(),int id=0){
        int a,b;split(rt,a,b,val,false);
        merge(a,a,nw(val,ival,id));
        merge(rt,a,b);
    }
    void erase(Key val){
        int a,b,c;
        split(rt,a,b,val,false); // a存<=val
        split(a,a,c,val,true);   // a存<val,c存==val
        if(c){
            merge(a,a,tr[c].r);
            merge(a,a,tr[c].l);
        }
        merge(rt,a,b);
    }
    void range_add_key(Key lim,const kTag &t){
        int x,y;split(rt,x,y,lim,true); // x存<lim, y存>=lim
        if(y)apply(y,t,iTag());
        merge(rt,x,y);
    }
    void range_add_info(Key l,Key r,const iTag &t){
        int x,y,z;
        split(rt,x,y,l,true); // x存<l
        split(y,y,z,r,false); // y存[l,r], z存>r
        if(y)apply(y,kTag(),t);
        merge(y,y,z);merge(rt,x,y);
    }
    Key findMax(int u){
        if(!u)return tr[0].key;
        pushdown(u);
        while(tr[u].r){u=tr[u].r;pushdown(u);}
        return tr[u].key;
    }
    Key findMin(int u){
        if(!u)return tr[0].key;
        pushdown(u);
        while(tr[u].l){u=tr[u].l;pushdown(u);}
        return tr[u].key;
    }
    Key pre(Key val){
        int a,b;split(rt,a,b,val,true); // 按<分裂
        Key res=findMax(a);
        merge(rt,a,b);return res;
    }
    Key next(Key val){
        int a,b;split(rt,a,b,val,false); //按<=分裂，b中自然就是>的部分
        Key res=findMin(b);
        merge(rt,a,b);return res;
    }
    int rank(Key val){
        int a,b;split(rt,a,b,val,true);
        int res=(a?tr[a].sz:0)+1;
        merge(rt,a,b);return res;
    }
    Key QueryKth(int k){return KthQuery(rt,k);}
    Key KthQuery(int u,int k){
        if(!u)return tr[0].key;
        pushdown(u);
        int lsz=tr[tr[u].l].sz;
        if(k<=lsz)return KthQuery(tr[u].l,k);
        if(k==lsz+1)return tr[u].key;
        return KthQuery(tr[u].r,k-lsz-1);
    }
    bool find(Key val){
        int a,b,c;
        split(rt,a,b,val,false);
        split(a,a,c,val,true);
        bool res=(c!=0); 
        merge(a,a,c);merge(rt,a,b);
        return res;
    }
    void get_ans(int u,vector<ll>& ans){
        if(!u)return;
        pushdown(u);
        get_ans(tr[u].l,ans);
        if(tr[u].id)ans[tr[u].id]=tr[u].key.val;
        get_ans(tr[u].r,ans);
    }
    vector<ll> get_ans(){
        vector<ll> ans(tot+1);
        get_ans(rt,ans);return ans;
    }
};

struct kTag{
    ll add;
    kTag():add(0){}
    void apply(const kTag &v){add+=v.add;}
    bool has_tag()const{return add!=0;}
    void clear(){add=0;}
};
struct iTag{
    ll add;
    iTag():add(0){}
    void apply(const iTag &v){add+=v.add;}
    bool has_tag()const{return add!=0;}
    void clear(){add=0;}
};
struct Key{
    ll val,sum;
    Key(ll v=0):val(v),sum(v){}
    bool operator<(const Key &o)const{return val<o.val;} 
    void pushup(const Key &l,const Key &r){sum=val+l.sum+r.sum;}
    void apply(const kTag &t,int sz){val+=t.add,sum+=t.add*sz;}
};
struct Info{
    ll ex_val,ex_sum;
    Info(ll v=0):ex_val(v),ex_sum(v){}
    void pushup(const Info &l,const Info &r,const Key &k){
        ex_sum=ex_val+l.ex_sum+r.ex_sum;
    }
    void apply(const iTag &t,int sz){
        ex_val+=t.add,ex_sum+=t.add*sz;
    }
};
//可以额外维护 key的sum,>=lim的key整体加正值(保证单调性) 
//以及 Key范围[l,r]内额外的Info区间加 info的子树和 之类的
/*
struct kTag{
    void apply(const kTag &){}
    bool has_tag()const{return false;}
    void clear(){}
};
struct iTag{
    void apply(const iTag &){}
    bool has_tag()const{return false;}
    void clear(){}
};
struct Info{
    Info(ll v=0){}
    void pushup(const Info &,const Info &,const Key &){}
    void apply(const iTag &,int){}
};
struct Key{
    ll val,sum;
    Key(ll v=0):val(v),sum(v){}
    bool operator<(const Key &o)const{return val<o.val;} 
    void pushup(const Key &l,const Key &r){sum=val+l.sum+r.sum;}
    void apply(const kTag &,int){}
};
//只需维护key的模式（将其他结构体置空即可无缝套用）。
*/
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}