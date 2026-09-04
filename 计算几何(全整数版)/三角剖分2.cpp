#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;

//整数向量为耳切法提供精确叉积和点积。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点保存简单多边形及剖分得到的三角形顶点。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
};

//返回三点有向面积的两倍，耳朵判定全程使用该精确值。
i128 crossVal(const pit& a,const pit& b,const pit& c){return (b-a)*(c-a);}
//返回多边形有向面积的两倍，用于统一输入方向。
i128 area2(const vector<pit>& p){
    i128 res=0;
    for(int i=0;i<(int)p.size();i++)res+=(i128)p[i].x*p[(i+1)%p.size()].y-(i128)p[i].y*p[(i+1)%p.size()].x;
    return res;
}
//判断点p是否位于闭线段ab上，用于清理多边形边上的冗余点。
bool onSeg(const pit& a,const pit& b,const pit& p){return crossVal(a,b,p)==0&&((a-p)&(b-p))<=0;}
//去掉连续重复点和共线中间点，并把有效简单多边形调整为逆时针。
vector<pit> simplifyPolygon(vector<pit> p){
    if(p.size()>1&&p.front()==p.back())p.pop_back();
    vector<pit> q;
    for(const auto& x:p)if(q.empty()||!(q.back()==x))q.push_back(x);
    if(q.size()>1&&q.front()==q.back())q.pop_back();
    bool changed=true;
    while(changed&&q.size()>2){
        changed=false;
        vector<pit> r;
        int n=q.size();
        for(int i=0;i<n;i++){
            if(onSeg(q[(i-1+n)%n],q[(i+1)%n],q[i]))changed=true;
            else r.push_back(q[i]);
        }
        q.swap(r);
    }
    if(q.size()>2&&area2(q)<0)reverse(q.begin(),q.end());
    return q;
}
//判断点p是否在逆时针三角形abc内部或边界上。
bool inTriangle(const pit& a,const pit& b,const pit& c,const pit& p){return crossVal(a,b,p)>=0&&crossVal(b,c,p)>=0&&crossVal(c,a,p)>=0;}
//用耳切法把简单多边形剖分为三角形；非法或退化输入返回空集，复杂度O(n^2)。
vector<vector<pit>> triangulate(vector<pit> p){
    p=simplifyPolygon(move(p));
    int n=p.size();
    if(n<3||area2(p)==0)return {};
    if(n==3)return {p};
    vector<int> pre(n),nxt(n),alive(n,1),ear(n);
    for(int i=0;i<n;i++)pre[i]=(i-1+n)%n,nxt[i]=(i+1)%n;
    auto isEar=[&](int u){
        if(!alive[u])return false;
        int a=pre[u],b=nxt[u];
        if(crossVal(p[a],p[u],p[b])<=0)return false;
        for(int v=nxt[b];v!=a;v=nxt[v])if(inTriangle(p[a],p[u],p[b],p[v]))return false;
        return true;
    };
    queue<int> q;
    for(int i=0;i<n;i++){
        ear[i]=isEar(i);
        if(ear[i])q.push(i);
    }
    vector<vector<pit>> res;
    int cnt=n;
    while(cnt>3&&!q.empty()){
        int u=q.front();q.pop();
        if(!alive[u])continue;
        ear[u]=isEar(u);
        if(!ear[u])continue;
        int a=pre[u],b=nxt[u];
        res.push_back({p[a],p[u],p[b]});
        alive[u]=0;nxt[a]=b;pre[b]=a;cnt--;
        ear[a]=isEar(a);ear[b]=isEar(b);
        if(ear[a])q.push(a);
        if(ear[b])q.push(b);
    }
    if(cnt!=3)return {};
    int a=find(alive.begin(),alive.end(),1)-alive.begin(),b=nxt[a],c=nxt[b];
    res.push_back({p[a],p[b],p[c]});
    return res;
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
