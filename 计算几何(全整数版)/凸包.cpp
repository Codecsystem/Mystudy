#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using ld=long double;

//整数向量用于精确计算叉积、点积和距离平方。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点提供Andrew排序和几何判定所需的基本运算。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
    bool operator<(const pit& o)const{return x!=o.x?x<o.x:y<o.y;}
};

//返回三点的有向二倍三角形面积，用于所有转向判断。
i128 crossVal(const pit& a,const pit& b,const pit& c){return (b-a)*(c-a);}
//返回三点转向符号，逆时针、顺时针、共线分别为1、-1、0。
int cross(const pit& a,const pit& b,const pit& c){i128 v=crossVal(a,b,c);return (v>0)-(v<0);}
//判断点p是否在闭线段ab上，供退化凸包和边界查询使用。
bool onSeg(const pit& a,const pit& b,const pit& p){return cross(a,b,p)==0&&((a-p)&(b-p))<=0;}
//返回两点距离，只有周长需要真实长度时才开方。
ld dis(const pit& a,const pit& b){return sqrtl((ld)((b-a)&(b-a)));}
//Andrew算法返回闭合边界周长和逆时针凸包，不保留边上的共线中间点。
pair<ld,vector<pit>> Andrew(vector<pit> p){
    sort(p.begin(),p.end());
    p.erase(unique(p.begin(),p.end()),p.end());
    if(p.empty())return {0,{}};
    if(p.size()==1)return {0,p};
    vector<pit> lo,up;
    for(const auto& x:p){
        while(lo.size()>1&&crossVal(lo[lo.size()-2],lo.back(),x)<=0)lo.pop_back();
        lo.push_back(x);
    }
    for(int i=(int)p.size()-1;i>=0;i--){
        const pit& x=p[i];
        while(up.size()>1&&crossVal(up[up.size()-2],up.back(),x)<=0)up.pop_back();
        up.push_back(x);
    }
    lo.pop_back();up.pop_back();
    lo.insert(lo.end(),up.begin(),up.end());
    ld peri=0;
    for(int i=0;i<(int)lo.size();i++)peri+=dis(lo[i],lo[(i+1)%lo.size()]);
    return {peri,lo};
}
//返回多边形有向面积的两倍，逆时针为正且结果完全精确。
i128 TA2(const vector<pit>& p){
    i128 res=0;
    for(int i=0;i<(int)p.size();i++)res+=(i128)p[i].x*p[(i+1)%p.size()].y-(i128)p[i].y*p[(i+1)%p.size()].x;
    return res;
}
//返回多边形面积；除以2时才转换为浮点数。
ld TA(const vector<pit>& p){i128 s=TA2(p);return (ld)(s<0?-s:s)/2;}
//若多边形为顺时针则翻转，使后续凸包算法统一按逆时针处理。
void rev(vector<pit>& p){if(TA2(p)<0)reverse(p.begin(),p.end());}
//判断点p是否在逆时针三角形abc内部或边界上。
bool isCon(const pit& a,const pit& b,const pit& c,const pit& p){return crossVal(a,b,p)>=0&&crossVal(b,c,p)>=0&&crossVal(c,a,p)>=0;}
//在逆时针凸包中二分判断点是否在内部或边界，复杂度O(logn)。
bool isConvex(const vector<pit>& p,const pit& a){
    int n=p.size();
    if(n==0)return false;
    if(n==1)return a==p[0];
    if(n==2)return onSeg(p[0],p[1],a);
    i128 lcr=crossVal(p[0],p[1],a),rcr=crossVal(p[0],p[n-1],a);
    if(lcr<0||rcr>0)return false;
    if(lcr==0)return onSeg(p[0],p[1],a);
    if(rcr==0)return onSeg(p[0],p[n-1],a);
    int l=1,r=n-1;
    while(r-l>1){
        int mid=(l+r)>>1;
        if(crossVal(p[0],p[mid],a)>=0)l=mid;
        else r=mid;
    }
    return crossVal(p[l],p[r],a)>=0;
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
