#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using ld=long double;

//整数向量负责精确的加减、叉积和点积，乘积用i128避免常见坐标范围下溢出。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    vec operator+(const vec& o)const{return {x+o.x,y+o.y};}
    vec operator-(const vec& o)const{return {x-o.x,y-o.y};}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点用于所有拓扑判定，只有构造非整数结果时才转换为dpit。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    pit operator+(const vec& o)const{return {x+o.x,y+o.y};}
    pit operator+(const pit& o)const{return {x+o.x,y+o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
};
//浮点向量仅承接单位化、旋转等无法保持整数坐标的结果。
struct dvec{
    ld x,y;
    dvec(ld x=0,ld y=0):x(x),y(y){}
    dvec operator+(const dvec& o)const{return {x+o.x,y+o.y};}
    dvec operator-(const dvec& o)const{return {x-o.x,y-o.y};}
    dvec operator*(ld k)const{return {x*k,y*k};}
    dvec operator/(ld k)const{return {x/k,y/k};}
};
//浮点点用于表示直线交点等一般为有理数的坐标。
struct dpit{
    ld x,y;
    dpit(ld x=0,ld y=0):x(x),y(y){}
    dpit operator+(const dvec& o)const{return {x+o.x,y+o.y};}
};

//返回向量长度平方，比较距离时优先使用它以保持全整数。
i128 len2(const vec& o){return o&o;}
//返回向量模长，仅在确实需要长度值时进行开方。
ld len(const vec& o){return sqrtl((ld)len2(o));}
//返回两点距离平方，适合最近点和大小关系判断。
i128 dis2(const pit& a,const pit& b){return len2(b-a);}
//返回两点欧氏距离，输出真实距离时使用。
ld dis(const pit& a,const pit& b){return sqrtl((ld)dis2(a,b));}
//将整数向量逆时针旋转theta弧度，结果通常不再是整数。
dvec rotate(const vec& o,ld theta){return {o.x*cosl(theta)-o.y*sinl(theta),o.x*sinl(theta)+o.y*cosl(theta)};}
//返回同方向单位向量，调用者需保证原向量非零。
dvec norm(const vec& o){
    assert(len2(o)!=0);
    ld l=len(o);
    return {(ld)o.x/l,(ld)o.y/l};
}
//返回两个非零向量的较小夹角，范围为[0,pi]。
ld angle(const vec& a,const vec& b){
    assert(len2(a)!=0&&len2(b)!=0);
    ld val=(ld)(a&b)/len(a)/len(b);
    val=max(-1.0L,min(1.0L,val));
    return acosl(val);
}
//返回有向平行四边形面积，正值表示b在a逆时针方向。
i128 area(const vec& a,const vec& b){return a*b;}
//精确判断点c相对有向直线ab的位置，依次返回1、-1、0。
int cross(const pit& a,const pit& b,const pit& c){
    i128 val=(b-a)*(c-a);
    return (val>0)-(val<0);
}
//判断点p是否位于闭线段ab上，端点也计入。
bool onSeg(const pit& a,const pit& b,const pit& p){return cross(a,b,p)==0&&((a-p)&(b-p))<=0;}
//判断非退化直线ab是否与闭线段cd有公共点。
bool lcross(const pit& a,const pit& b,const pit& c,const pit& d){return cross(a,b,c)*cross(a,b,d)<=0;}
//判断两条闭线段是否相交，覆盖端点接触、重合和退化成点的情况。
bool scross(const pit& a,const pit& b,const pit& c,const pit& d){
    int c1=cross(a,b,c),c2=cross(a,b,d),c3=cross(c,d,a),c4=cross(c,d,b);
    if(c1==0&&onSeg(a,b,c))return true;
    if(c2==0&&onSeg(a,b,d))return true;
    if(c3==0&&onSeg(c,d,a))return true;
    if(c4==0&&onSeg(c,d,b))return true;
    return c1*c2<0&&c3*c4<0;
}
//判断两条非退化直线是否有唯一交点，即方向向量不平行。
bool pcross(const pit& a,const pit& b,const pit& c,const pit& d){return (b-a)*(d-c)!=0;}
//求两条非平行直线ab、cd的交点，除法不可避免所以返回浮点点。
dpit getNode(const pit& a,const pit& b,const pit& c,const pit& d){
    vec u=b-a,v=d-c;
    i128 den=u*v;
    assert(den!=0);
    ld t=(ld)((c-a)*v)/(ld)den;
    return {a.x+u.x*t,a.y+u.y*t};
}
//用点向式求两条非平行直线的交点，u、v分别是两直线方向。
dpit getNode(const pit& a,const vec& u,const pit& c,const vec& v){
    i128 den=u*v;
    assert(den!=0);
    ld t=(ld)((c-a)*v)/(ld)den;
    return {a.x+u.x*t,a.y+u.y*t};
}
//返回点p到闭线段ab的最短欧式距离
ld disPtSeg(const pit& p,const pit& a,const pit& b){
    vec ab=b-a,ap=p-a,bp=p-b;
    if(len2(ab)==0)return dis(p,a); //退化成点
    if((ab&ap)<=0)return dis(p,a); //投影在a的外侧
    if(((a-b)&bp)<=0)return dis(p,b); //投影在b的外侧
    i128 cr=ab*ap; //投影在线段上，计算平行四边形面积求高
    return (ld)(cr<0?-cr:cr)/len(ab);
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
