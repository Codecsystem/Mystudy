#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using ld=long double;
const ld eps=1e-18L;

//整数向量保存平移到圆心后的多边形端点，并精确完成叉积和点积。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    vec operator+(const vec& o)const{return {x+o.x,y+o.y};}
    vec operator-(const vec& o)const{return {x-o.x,y-o.y};}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点保存圆心和原多边形坐标。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
};
//浮点向量仅用于线段与圆交点之间的面积积分。
struct dvec{
    ld x,y;
    dvec(ld x=0,ld y=0):x(x),y(y){}
};

//返回整数向量长度平方，端点是否在圆内用它精确判断。
i128 len2(const vec& a){return a&a;}
//返回两个浮点向量的叉积，计算分段三角形面积时使用。
ld crossVal(const dvec& a,const dvec& b){return a.x*b.y-a.y*b.x;}
//返回两个浮点向量的点积，计算有向圆心角时使用。
ld dotVal(const dvec& a,const dvec& b){return a.x*b.x+a.y*b.y;}
//返回圆心到两向量的有向扇形面积，角度由atan2稳定计算。
ld sector(const dvec& a,const dvec& b,ll r){return (ld)r*r*atan2l(crossVal(a,b),dotVal(a,b))/2;}
//求线段a->b与圆的内部交点参数，返回值包含0和1并保持递增。
vector<ld> cutParams(const vec& a,const vec& b,ll r){
    vec d=b-a;
    i128 A=d&d;
    vector<ld> ts={0,1};
    if(A==0)return ts;
    i128 ad=a&d,cr=a*d;
    i128 det=(i128)r*r*A-cr*cr;
    if(det>0){
        ld mid=-(ld)ad/(ld)A,off=sqrtl((ld)det)/(ld)A;
        ld t1=mid-off,t2=mid+off;
        if(t1>eps&&t1<1-eps)ts.push_back(t1);
        if(t2>eps&&t2<1-eps)ts.push_back(t2);
    }
    sort(ts.begin(),ts.end());
    ts.erase(unique(ts.begin(),ts.end(),[](ld x,ld y){return fabsl(x-y)<=eps;}),ts.end());
    return ts;
}
//计算有向三角形Oab与圆的面积交贡献，直线交点和扇形角处才使用浮点数。
ld getS(const vec& a,const vec& b,ll r){
    if(r==0||(a.x==b.x&&a.y==b.y))return 0;
    i128 rr=(i128)r*r;
    if(len2(a)<=rr&&len2(b)<=rr)return (ld)(a*b)/2;
    vec d=b-a;
    vector<ld> ts=cutParams(a,b,r);
    ld ans=0;
    for(int i=0;i+1<(int)ts.size();i++){
        ld l=ts[i],h=ts[i+1],m=(l+h)/2;
        dvec u={a.x+d.x*l,a.y+d.y*l};
        dvec v={a.x+d.x*h,a.y+d.y*h};
        ld mx=a.x+d.x*m,my=a.y+d.y*m;
        if(mx*mx+my*my<=(ld)rr+eps)ans+=crossVal(u,v)/2;
        else ans+=sector(u,v,r);
    }
    return ans;
}
//三角剖分累加简单多边形与整数圆的面积交，顶点顺/逆时针均可。
ld S(const pit& o,ll r,const vector<pit>& p){
    assert(r>=0);
    if(p.size()<3||r==0)return 0;
    ld ans=0;
    for(int i=0;i<(int)p.size();i++)ans+=getS(p[i]-o,p[(i+1)%p.size()]-o,r);
    return fabsl(ans);
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
