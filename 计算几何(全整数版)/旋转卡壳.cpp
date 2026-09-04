#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using u128=__uint128_t;
using ld=long double;
const ld eps=1e-12L;

//384位无符号整数只用于比较三个非负i128因子的乘积。
struct u384{
    array<uint32_t,12> d{};
};

//整数向量用于卡壳指针移动时的精确叉积、点积比较。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点保存输入点集和凸包，所有最优性判定均不依赖eps。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
    bool operator<(const pit& o)const{return x!=o.x?x<o.x:y<o.y;}
};
//浮点点只用于返回最小外接矩形通常不是整数的四个顶点。
struct dpit{
    ld x,y;
    dpit(ld x=0,ld y=0):x(x),y(y){}
};

//返回三点有向面积的两倍，凸包和卡壳高度都用它计算。
i128 crossVal(const pit& a,const pit& b,const pit& c){return (b-a)*(c-a);}
//返回ab与ac的点积，用于寻找沿当前边方向的最远投影。
i128 dotVal(const pit& a,const pit& b,const pit& c){return (b-a)&(c-a);}
//返回向量与点位置向量的叉积，即该点在法向轴上的未归一化投影。
i128 crossOrigin(const vec& a,const pit& p){return (i128)a.x*p.y-(i128)a.y*p.x;}
//返回向量与点位置向量的点积，即该点在边轴上的未归一化投影。
i128 dotOrigin(const vec& a,const pit& p){return (i128)a.x*p.x+(i128)a.y*p.y;}
//精确计算三个非负i128的乘积，避免候选矩形面积比较时溢出。
u384 mul3(i128 a,i128 b,i128 c){
    assert(a>=0&&b>=0&&c>=0);
    array<uint32_t,4> A{},B{},C{};
    u128 x=a,y=b,z=c;
    for(int i=0;i<4;i++)A[i]=x>>(i*32),B[i]=y>>(i*32),C[i]=z>>(i*32);
    array<u128,13> sum{};
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)for(int k=0;k<4;k++)sum[i+j+k]+=(u128)A[i]*B[j]*C[k];
    u384 res;
    for(int i=0;i<12;i++)res.d[i]=sum[i],sum[i+1]+=sum[i]>>32;
    return res;
}
//按数值比较两个384位无符号整数，供分数交叉相乘后判优。
bool operator<(const u384& a,const u384& b){
    for(int i=11;i>=0;i--)if(a.d[i]!=b.d[i])return a.d[i]<b.d[i];
    return false;
}
//返回两点欧氏距离，计算凸包周长时使用。
ld dis(const pit& a,const pit& b){vec d=b-a;return sqrtl((ld)(d&d));}
//Andrew算法返回闭合边界周长和逆时针凸包，不保留共线中间点。
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
//返回多边形有向面积的两倍，用于统一卡壳遍历方向。
i128 TA2(const vector<pit>& p){
    i128 res=0;
    for(int i=0;i<(int)p.size();i++)res+=(i128)p[i].x*p[(i+1)%p.size()].y-(i128)p[i].y*p[(i+1)%p.size()].x;
    return res;
}
//返回多边形面积，只有最终除以2时转换为浮点数。
ld TA(const vector<pit>& p){i128 s=TA2(p);return (ld)(s<0?-s:s)/2;}
//若凸包为顺时针则翻转，保证内部始终位于每条边左侧。
void rev(vector<pit>& p){if(TA2(p)<0)reverse(p.begin(),p.end());}
//判断点p是否在逆时针三角形abc内部或边界上。
bool isCon(const pit& a,const pit& b,const pit& c,const pit& p){return crossVal(a,b,p)>=0&&crossVal(b,c,p)>=0&&crossVal(c,a,p)>=0;}
//在逆时针凸包内二分判点，边界计入且复杂度为O(logn)。
bool isConvex(const vector<pit>& p,const pit& a){
    int n=p.size();
    if(n==0)return false;
    if(n==1)return a==p[0];
    if(n==2)return crossVal(p[0],p[1],a)==0&&((p[0]-a)&(p[1]-a))<=0;
    i128 x=crossVal(p[0],p[1],a),y=crossVal(p[0],p[n-1],a);
    if(x<0||y>0)return false;
    if(x==0)return ((p[0]-a)&(p[1]-a))<=0;
    if(y==0)return ((p[0]-a)&(p[n-1]-a))<=0;
    int l=1,r=n-1;
    while(r-l>1){
        int mid=(l+r)>>1;
        if(crossVal(p[0],p[mid],a)>=0)l=mid;
        else r=mid;
    }
    return crossVal(p[l],p[r],a)>=0;
}
//由边轴投影u和法轴投影v还原矩形顶点，除法在此处才发生。
dpit fromProjection(const vec& e,i128 u,i128 v,i128 q){
    ld X=((ld)e.x*(ld)u-(ld)e.y*(ld)v)/(ld)q;
    ld Y=((ld)e.y*(ld)u+(ld)e.x*(ld)v)/(ld)q;
    return {X,Y};
}
//旋转卡壳求逆时针凸包的最小面积外接矩形，返回面积和逆时针四顶点。
pair<ld,vector<dpit>> rot(vector<pit> p){
    if(p.empty())return {0,{}};
    if(p.size()==1)return {0,vector<dpit>(4,{(ld)p[0].x,(ld)p[0].y})};
    rev(p);
    int n=p.size(),top=0,rgt=0,lft=0;
    vec firstEdge=p[1]-p[0];
    for(int i=1;i<n;i++){
        if(crossVal(p[0],p[1],p[i])>crossVal(p[0],p[1],p[top]))top=i;
        if(dotOrigin(firstEdge,p[i])>dotOrigin(firstEdge,p[rgt]))rgt=i;
        if(dotOrigin(firstEdge,p[i])<dotOrigin(firstEdge,p[lft]))lft=i;
    }
    bool first=true;
    i128 bestH=0,bestW=0,bestQ=1;
    vector<dpit> fin(4);
    for(int i=0;i<n;i++){
        int ni=(i+1)%n;
        vec e=p[ni]-p[i];
        i128 q=e&e;
        while(crossVal(p[i],p[ni],p[(top+1)%n])>crossVal(p[i],p[ni],p[top]))top=(top+1)%n;
        while(dotVal(p[i],p[ni],p[(rgt+1)%n])>dotVal(p[i],p[ni],p[rgt]))rgt=(rgt+1)%n;
        while(dotVal(p[i],p[ni],p[(lft+1)%n])<dotVal(p[i],p[ni],p[lft]))lft=(lft+1)%n;
        i128 h=crossVal(p[i],p[ni],p[top]);
        i128 w=dotOrigin(e,p[rgt])-dotOrigin(e,p[lft]);
        if(first||mul3(h,w,bestQ)<mul3(bestH,bestW,q)){
            first=false;bestH=h;bestW=w;bestQ=q;
            i128 u0=dotOrigin(e,p[lft]),u1=dotOrigin(e,p[rgt]);
            i128 v0=crossOrigin(e,p[i]),v1=crossOrigin(e,p[top]);
            fin[0]=fromProjection(e,u0,v0,q);
            fin[1]=fromProjection(e,u1,v0,q);
            fin[2]=fromProjection(e,u1,v1,q);
            fin[3]=fromProjection(e,u0,v1,q);
        }
    }
    return {(ld)bestH*(ld)bestW/(ld)bestQ,fin};
}
//把极小的输出坐标归零，避免打印出-0.00000。
void zero(dpit& a){if(fabsl(a.x)<eps)a.x=0;if(fabsl(a.y)<eps)a.y=0;}
//按x、y字典序比较输出点，用于固定矩形顶点输出起点。
bool dcmp(const dpit& a,const dpit& b){return fabsl(a.x-b.x)>eps?a.x<b.x:a.y<b.y-eps;}

//读取整数点集并输出最小外接矩形示例，顶点从最小点起按顺时针排列。
signed main(){
    ios::sync_with_stdio(false),cin.tie(nullptr);
    int n;
    if(!(cin>>n))return 0;
    vector<pit> p(n);
    for(auto& x:p)cin>>x.x>>x.y;
    auto hull=Andrew(p).second;
    auto [ans,fin]=rot(hull);
    cout<<fixed<<setprecision(5)<<ans<<'\n';
    if(fin.empty())return 0;
    reverse(fin.begin(),fin.end());
    int k=0;
    for(int i=1;i<4;i++)if(dcmp(fin[i],fin[k]))k=i;
    for(int i=0;i<4;i++){
        dpit& x=fin[(k+i)%4];zero(x);
        cout<<fixed<<setprecision(5)<<x.x<<' '<<x.y<<'\n';
    }
    return 0;
}
