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
#define double long double
const double eps=1e-12;
const double pi=acos(-1);
using namespace std;
//const double eps=1e-8;
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
//向量逆时针旋转theta弧度
vec rotate(const vec& o,double theta){
    return vec(o.x*cos(theta)-o.y*sin(theta),o.x*sin(theta)+o.y*cos(theta));
} 
//向量单位化
vec norm(vec a){
    return a/len(a);
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
//线线关系
//case1:直线ab与线段cd
bool lcross(pit a,pit b,pit c,pit d){
    if(cross(a,b,c)*cross(a,b,d)>0) return 0;//c,d在ab的同一侧 无交点
    return 1; //有交点
}
//case2:线段ab与线段cd
bool scross(pit a,pit b,pit c,pit d){
    if(cross(a,b,c)*cross(a,b,d)>0||cross(c,d,a)*cross(c,d,b)>0) return 0;//c,d在ab 或 a,b在cd 的同一侧 无交点
    return 1; //有交点
}
//case3:直线ab与直线cd
bool pcross(pit a,pit b,pit c,pit d){
    if(fabs((b-a)*(d-c))<=eps) return 0; //平行 无交点
    return 1; //有交点
}
//求两直线ab,cd的交点(两点式)
pit getNode(pit a,pit b,pit c,pit d){
    vec u=b-a,v=d-c;
    //assert(fabs(u*v)<=eps);
    //if(fabs(u*v)<=eps) return pit(NAN,NAN); //平行 无交点
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//求两直线ab,cd的交点(点向式) a起点u方向向量 c起点v方向向量
pit getNode(pit a,vec u,pit c,vec v){
    //assert(fabs(u*v)<=eps);
    //if(fabs(u*v)<=eps) return pit(NAN,NAN); //平行 无交点
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
signed main()
{
    int T_start=clock();
    freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    while(t--){
        int x1,y1,x2,y2;cin>>x1>>y1>>x2>>y2;
        int x3,y3,x4,y4;cin>>x3>>y3>>x4>>y4;
        pit s1(x1,y1),t1(x2,y2),s2(x3,y3),t2(x4,y4);
        double T1=min(dis(s1,t1),dis(s2,t2));
        vec v1=norm(t1-s1),v2=norm(t2-s2);
        double l=0,r=T1;
        while(r-l>eps){
            double lmid=l+(r-l)/3,rmid=lmid+(r-l)/3;
            double lans=dis(s1+v1*lmid,s2+v2*lmid);
            double rans=dis(s1+v1*rmid,s2+v2*rmid);
            if(rans-lans>eps) r=rmid;
            else l=lmid;
        }
        //不过应该注意的是
        double ans=dis(s1+v1*l,s2+v2*l);
        if(dis(s1,t1)-dis(s2,t2)>eps) swap(s1,s2),swap(t1,t2),swap(v1,v2);
        // now t1, s2->t2
        s2=s2+v2*T1;
        //cout<<s2.x<<' '<<s2.y<<endl;
        vec v_rot=norm(rotate(v2,pi/2));
        //cout<<v_rot.x<<' '<<v_rot.y<<endl;
        //cout<<t1.x<<' '<<t1.y<<endl;
        pit cropit=getNode(t1,v_rot,s2,v2);
        //cout<<v2.x<<' '<<v2.y<<endl;
        //cout<<cropit.x<<' '<<cropit.y<<endl;
        if(onSeg(s2,t2,cropit)) 
        {
            //cout<<"Y"<<endl;
            ans=min(ans,dis(t1,cropit));
        }
        ans=min(ans,dis(t1,s2)),ans=min(ans,dis(t1,t2));
        cout<<fixed<<setprecision(12)<<ans<<endl;
    }
    return 0;
}
//注意我们通常不用浮点数三分 而是固定次数 t=100
/*
int l = 1,r = 100;
while(l < r) {
    int lmid = l + (r - l) / 3;
    int rmid = r - (r - l) / 3;
    lans = f(lmid),rans = f(rmid);
    // 求凹函数的极小值
    if(lans <= rans) r = rmid - 1;
    else l = lmid + 1;
    // 求凸函数的极大值
    if(lasn >= rans) l = lmid + 1;
    else r = rmid - 1;
}
// 求凹函数的极小值
cout << min(lans,rans) << endl;
// 求凸函数的极大值
cout << max(lans,rans) << endl;
*/


/*
const double EPS = 1e-9;
while(r - l > EPS) {
    double lmid = l + (r - l) / 3;
    double rmid = r - (r - l) / 3;
    lans = f(lmid),rans = f(rmid);
    // 求凹函数的极小值
    if(lans <= rans) r = rmid;
    else l = lmid;
    // 求凸函数的极大值
    if(lans >= rans) l = lmid;
    else r = rmid;
}
// 输出 l 或 r 都可
cout << l << endl;
*/

