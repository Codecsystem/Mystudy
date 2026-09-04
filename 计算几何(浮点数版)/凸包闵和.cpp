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
//O(n+m)凸包闵和；要求a、b非空、为凸多边形、按逆时针排列且不带重复首点
//同向边可能保留共线中间点；空输入会在后续访问a[0]或b[0]时崩溃
vector<pit> minkowski(vector<pit> a,vector<pit> b){ 
    auto cmp=[](const pit& p1,const pit& p2){
        if(p1.y!=p2.y)return p1.y<p2.y;
        return p1.x<p2.x;
    };
    rotate(a.begin(),min_element(a.begin(),a.end(),cmp),a.end());
    rotate(b.begin(),min_element(b.begin(),b.end(),cmp),b.end());//以最左下角的点为起点
    int n=a.size(),m=b.size();
    vector<vec> va(n),vb(m);
    for(int i=0;i<n;++i) va[i]=a[(i+1)%n]-a[i];
    for(int i=0;i<m;++i) vb[i]=b[(i+1)%m]-b[i];
    vector<pit> res;
    res.push_back(a[0]+b[0]);
    int i=0,j=0; //做归并排序
    while(i<n&&j<m)res.push_back(res.back()+(va[i]*vb[j]>=0?va[i++]:vb[j++]));
    while(i<n) res.push_back(res.back()+va[i++]);
    while(j<m) res.push_back(res.back()+vb[j++]);
    res.pop_back(); //闭合会回到起点，弹掉最后一个重复点
    return res;
}
signed main()
{
    
    return 0;
}
