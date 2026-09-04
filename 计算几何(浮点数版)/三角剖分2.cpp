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
#include <cassert>
//#define int long long //赫赫 要不要龙龙呢
//#define double long double
//const double eps=1e-12;
using namespace std;
const double eps=1e-8;
struct vec{
    double x,y;
    vec(double x=0,double y=0):x(x),y(y){}
    vec operator+(const vec& o)const{return vec(x+o.x,y+o.y);}
    vec operator-(const vec& o)const{return vec(x-o.x,y-o.y);}
    vec operator/(const double& o)const{return vec(x/o,y/o);} //数除
    vec operator*(const double& o)const{return vec(x*o,y*o);} //数乘
    double operator*(const vec& o)const{return x*o.y-y*o.x;} //叉积
    double operator&(const vec& o)const{return x*o.x+y*o.y;} //点积
};
struct pit
{
    double x,y;
    pit(double x=0,double y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return vec(x-o.x,y-o.y);}
    pit operator+(const vec& o)const{return pit(x+o.x,y+o.y);}
    pit operator+(const pit& o)const{return pit(x+o.x,y+o.y);}
    pit operator/(const double& o)const{return pit(x/o,y/o);}
};
double len(const vec& o){return sqrt(o.x*o.x+o.y*o.y);} //向量模长
double dis(const pit& a,const pit& b){return len(b-a);} //两点距离
vec rotate(const vec& o,double theta){
    return vec(o.x*cos(theta)-o.y*sin(theta),o.x*sin(theta)+o.y*cos(theta));
} 
//向量单位化；要求a为非零向量，否则会产生NaN
vec norm(vec a){
    return a/len(a);
}
double angle(vec a,vec b){
    double val=(a&b)/len(a)/len(b);
    val=max(-1.0,min(1.0,val));
    return acos(val);
} 
double area(vec a,vec b){return a*b;} 
int cross(pit a,pit b,pit c){
    if((b-a)*(c-a)>eps) return 1; 
    else if((b-a)*(c-a)<-eps) return -1; 
    return 0; 
}
bool onSeg(pit a,pit b,pit p){
    return cross(a,b,p)==0&&((a-p)&(b-p))<=eps;
}
bool lcross(pit a,pit b,pit c,pit d){
    if(cross(a,b,c)*cross(a,b,d)>0) return 0;
    return 1; 
}
bool scross(pit a,pit b,pit c,pit d){
    int c1=cross(a,b,c),c2=cross(a,b,d),c3=cross(c,d,a),c4=cross(c,d,b);
    if(c1==0&&onSeg(a,b,c)) return 1;
    if(c2==0&&onSeg(a,b,d)) return 1;
    if(c3==0&&onSeg(c,d,a)) return 1;
    if(c4==0&&onSeg(c,d,b)) return 1;
    return c1*c2<0&&c3*c4<0; 
}
bool pcross(pit a,pit b,pit c,pit d){
    if(fabs((b-a)*(d-c))<=eps) return 0; 
    return 1; 
}
//求两条不平行直线ab、cd的交点；要求两条方向向量非零且不平行
pit getNode(pit a,pit b,pit c,pit d){
    vec u=b-a,v=d-c;
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//点向式求两条不平行直线的交点；要求u、v非零且不平行
pit getNode(pit a,vec u,pit c,vec v){
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//O(n^2) 三角剖分 把逆时针传入的一个多边形剖分成on个三角形 
vector<vector<pit>> triangulate(vector<pit> p){
    int n=p.size();
    if(n==3) return {p};
    vector<vector<pit>> res;
    vector<int> nxt(n),pre(n),in(n,1),ear(n,0);//in 是否被删除
    for(int i=0;i<n;++i) nxt[i]=(i+1)%n,pre[i]=(i-1+n)%n;
    auto chk=[&](int u){
        int pu=pre[u],nu=nxt[u];
        if(cross(p[pu],p[u],p[nu])!=1) return 0;
        for(int j=nxt[nu];j!=pu;j=nxt[j])
            if(cross(p[pu],p[u],p[j])>=0&&cross(p[u],p[nu],p[j])>=0&&cross(p[nu],p[pu],p[j])>=0) return 0;
        return 1;
    };//判断p是否是耳朵点 即和前驱和后继是否能形成一个凸三角形 且没有点在其内部
    queue<int> q; //维护耳朵点
    for(int i=0;i<n;++i) if(ear[i]=chk(i)) q.push(i);
    int cnt=n;
    while(cnt>2&&!q.empty()){
        int u=q.front();q.pop();
        if(!in[u]||!ear[u])continue;
        int pu=pre[u],nu=nxt[u];
        res.push_back({p[pu],p[u],p[nu]});
        in[u]=0;nxt[pu]=nu;pre[nu]=pu;cnt--; //删除这个点
        if(ear[pu]=chk(pu)) q.push(pu); //判断前驱是否是耳朵点
        if(ear[nu]=chk(nu)) q.push(nu); //判断后继是否是耳朵点
    }
    return res;
}
signed main()
{
    int T_start=clock();
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}
