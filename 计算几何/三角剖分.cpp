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
struct pit;struct vec;
const double eps=1e-8;
const double pi=acos(-1);
double R;
struct pit;
pit CO;//圆心 
struct vec{
    double x,y;
    vec(double x=0,double y=0):x(x),y(y){}
    vec(pit a) {x=a.x;y=a.y;}//点转向量(OA向量)
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
//向量逆时针旋转theta弧度
vec rotate(const vec& o,double theta){
    return vec(o.x*cos(theta)-o.y*sin(theta),o.x*sin(theta)+o.y*cos(theta));
} 
//单位向量
vec norm(vec a){
    return a/len(a);
}
//用单位圆证明
//向量夹角 dot(a,b)=len(a)*len(b)*cos(θ)
double angle(vec a,vec b){
    double val=(a&b)/len(a)/len(b);
    val=max(-1.0,min(1.0,val));
    return acos(val);
} 
//向量围成的平行四边形面积,b在a的逆时针方向为正，否则为负
double area(vec a,vec b){return a*b;} 
//点线关系(点c,直线ab)
int cross(pit a,pit b,pit c){
    if((b-a)*(c-a)>eps) return 1; //c在ab的逆时针方向
    else if((b-a)*(c-a)<-eps) return -1; //c在ab的顺时针方向
    return 0; //c,a,b共线
}
//判断点在线段上(p在ab上)
bool onSeg(pit a,pit b,pit p){
    return cross(a,b,p)==0&&((a-p)&(b-p))<=eps;
}
//OA OB扇形面积
double sector(vec a,vec b){
    double angle=acos((a&b)/len(a)/len(b)); //[0,pi]
    if(a*b<=-eps) angle=-angle; 
    return angle*R*R/2;
}
//求两直线ab,cd的交点(两点式)
pit getNode(pit a,pit b,pit c,pit d){
    vec u=b-a,v=d-c;
    //assert(fabs(u*v)<=eps); //平行 无交点
    //if(fabs(u*v)<=eps) return pit(NAN,NAN); //平行 无交点
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//求两直线ab,cd的交点(点向式) a起点u方向向量 c起点v方向向量
pit getNode(pit a,vec u,pit c,vec v){
    //assert(fabs(u*v)<=eps); //平行 无交点
    //if(fabs(u*v)<=eps) return pit(NAN,NAN); //平行 无交点
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//计算线段ab与圆的交点和距离(此处的距离是有意义的距离 即线段离圆心的距离)
double getDP2(pit a,pit b,pit& pa,pit &pb){
    pit e=getNode(a,b-a,CO,rotate(b-a,pi/2));
    //圆心到线段的垂足
    double d=dis(e,CO);
    if(!onSeg(a,b,e)) d=min(dis(a,CO),dis(b,CO)); //垂足不在线段上
    if(R-d<=-eps) return d; //线段在圆外 0个交点
    double h=sqrt(max(0.0,R*R-d*d));
    pa=e+norm(a-b)*h;
    pb=e+norm(b-a)*h;//计算两个交点
    return d;
}
//计算线段ab与圆心构成的三角形与圆的面积交
double getS(pit a,pit b){
    if(cross(a,b,CO)==0) return 0; //case1:三点共线
    double da=dis(a,CO),db=dis(b,CO);
    if(R-da>=-eps&&R-db>=-eps) return (vec(a))*(vec(b))/2; //case2:线段在圆内 构成一个三角形
    pit pa,pb;
    double d=getDP2(a,b,pa,pb); 
    if(R-d<=-eps) return sector(vec(a),vec(b)); //case3:线段在圆外 构成一个扇形
    if(R-da>=-eps) return (vec(a))*(vec(pb))/2+sector(vec(pb),vec(b)); //case4.1:a在圆内 一个三角形+扇形
    if(R-db>=-eps) return (vec(pa))*(vec(b))/2+sector(vec(a),vec(pa)); //case4.2:b在圆内 一个三角形+扇形
    return (vec(pa))*(vec(pb))/2+sector(vec(a),vec(pa))+sector(vec(b),vec(pb)); //case5:两个端点都在圆内 一个三角形+两个扇形
}
//极角排序
void psort(vector<pit>& a)
{
    pit cen(0,0);
    for(auto& i:a) cen=cen+i;
    cen=cen/a.size();
    sort(a.begin(),a.end(),[&](pit a,pit b){
        double angA=atan2(a.y-cen.y,a.x-cen.x);
        double angB=atan2(b.y-cen.y,b.x-cen.x);
        return angA<angB;
    });
}
double S(pit o,double r,vector<pit> a)
{
    for(auto& i:a) i.x-=o.x,i.y-=o.y;
    R=r;CO=pit(0,0);
    //psort(a); //当且仅当多边形为凸多边形且传入顺序不是正/逆时针时
    double res=0;
    for(int i=0;i<a.size();i++)
        res+=getS(a[i],a[(i+1)%a.size()]);
    return fabs(res);
}
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}
//三角剖分：将多边形分割成若干边 求边与圆心构成的三角形
//通过这个三角形，求多边形与圆的面积交
//传多边形的时候要逆时针传参(极角排序,当且仅当多边形为凸多边形时成立),同时圆心要平移到(0,0)