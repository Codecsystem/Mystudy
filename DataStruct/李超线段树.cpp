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
#define int long long //赫赫 要不要龙龙呢
using namespace std;
class LCSegTree{
public:
    const int L=1,R=1e9;
    const double eps=1e-9;
    struct Line{
        long double a,b;
        int id;
        Line(long double a=0,long double b=/*-1e18*/1e18,int id=0):a(a),b(b),id(id){}
        long double val(int x)const{return (long double)1.0*a*x+b;}
    };
    struct Node{
        Line l;
        Node *lc,*rc;
        Node():l(),lc(nullptr),rc(nullptr){}
    };
    Node *rt;
    LCSegTree(){
        rt=nullptr;
    }
    bool cmp(long double a,long double b){
		// return a-b>eps;//max
		return b-a>eps;//min
    }
    bool cmp1(Line a,Line b,int x){
		long double va=a.val(x),vb=b.val(x);
		return cmp(va,vb)||(fabs(va-vb)<eps&&a.id<b.id);
	}
    bool cmp2(pair<long double,int> a,long double b,int cid){
		return cmp(a.first,b)||(fabs(a.first-b)<eps&&a.second<cid);
    }
    void ins(Node*&o,int l,int r,int ql,int qr,Line v){
		if(qr<l||r<ql)return;
		if(!o)o=new Node();
		if(ql<=l&&r<=qr){
			int mid=(l+r)>>1;
			bool LB=cmp1(v,o->l,l),MB=cmp1(v,o->l,mid),RB=cmp1(v,o->l,r);
			if(MB)swap(o->l,v);
			if(l==r)return;
			if(LB!=MB)ins(o->lc,l,mid,ql,qr,v);
			else ins(o->rc,mid+1,r,ql,qr,v);
			return;
		}
		int mid=(l+r)>>1;
		ins(o->lc,l,mid,ql,qr,v);
		ins(o->rc,mid+1,r,ql,qr,v);
	}
	pair<long double,int> qry(Node*o,int l,int r,int x){
		if(!o)return {/*-1e18*/1e18,0};
		long double cur=o->l.val(x);
		int cid=o->l.id;
		int mid=(l+r)>>1;
		if(x<=mid){
			auto res=qry(o->lc,l,mid,x);
			if(cmp2(res,cur,cid))return res;
		}else{
			auto res=qry(o->rc,mid+1,r,x);
			if(cmp2(res,cur,cid))return res;
		}
		return {cur,cid};
	}
	void add(int x1,int y1,int x2,int y2,int id){
		if(x1==x2){
			int y=max(y1,y2);
			ins(rt,L,R,x1,x1,Line(0,y,id));
		}else{
			if(x1>x2)swap(x1,x2),swap(y1,y2);
			long double a=1.0*(y2-y1)/(x2-x1),b=1.0*y1-a*x1;
			ins(rt,L,R,x1,x2,Line(a,b,id));
		}
	}
    void add(Line v){
        ins(rt,L,R,L,R,v);
    }
	pair<long double,int> ask(int x){return qry(rt,L,R,x);}
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}
//李超线段树插入O(logDlogD)查询O(logD)
//解决：插入一条直线，查询某个点的最大/最小值
//插入先划分logn区间，再懒标记下放(logn)
//维护的是直线中点的最大/最小值
//证明，对每个局部，区间中值最大代表着这个局部最优，于是可以遍历获得全局最优
//空间复杂度O(nlogDlogD)