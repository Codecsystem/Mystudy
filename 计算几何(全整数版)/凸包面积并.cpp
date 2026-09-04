#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;
using ld=long double;
const ld eps=1e-18L;

//整数向量承担半平面裁剪中的叉积和点积判定。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点保存各凸多边形顶点，交点参数才转换为浮点数。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
};

//返回向量叉积，供边对原点的有向面积贡献使用。
i128 crossVal(const vec& a,const vec& b){return a*b;}
//返回三点有向面积的两倍，用于精确的半平面方向判定。
i128 crossVal(const pit& a,const pit& b,const pit& c){return (b-a)*(c-a);}
//返回多边形有向面积的两倍，正值表示逆时针。
i128 area2(const vector<pit>& p){
    i128 res=0;
    for(int i=0;i<(int)p.size();i++)res+=(i128)p[i].x*p[(i+1)%p.size()].y-(i128)p[i].y*p[(i+1)%p.size()].x;
    return res;
}
//清理重复首点并统一为逆时针，退化多边形会被丢弃。
vector<pit> normalizePoly(vector<pit> p){
    if(p.size()>1&&p.front()==p.back())p.pop_back();
    vector<pit> q;
    for(const auto& x:p)if(q.empty()||!(q.back()==x))q.push_back(x);
    if(q.size()>1&&q.front()==q.back())q.pop_back();
    if(q.size()<3||area2(q)==0)return {};
    if(area2(q)<0)reverse(q.begin(),q.end());
    return q;
}
//求若干凸多边形的面积并；半平面判断为整数，只有边上交点参数使用浮点数。
ld area_union(vector<vector<pit>> polys){
    vector<vector<pit>> valid;
    for(auto& p:polys){
        p=normalizePoly(move(p));
        if(!p.empty())valid.push_back(move(p));
    }
    polys=move(valid);
    ld ans=0;
    int n=polys.size();
    for(int i=0;i<n;i++){
        int ni=polys[i].size();
        for(int e=0;e<ni;e++){
            pit A=polys[i][e],B=polys[i][(e+1)%ni];
            vec AB=B-A;
            vector<pair<ld,ld>> cov;
            for(int j=0;j<n;j++){
                if(i==j)continue;
                ld L=0,R=1;
                bool ok=true;
                int nj=polys[j].size();
                for(int k=0;k<nj;k++){
                    pit C=polys[j][k],D=polys[j][(k+1)%nj];
                    vec DC=D-C;
                    i128 c1=DC*(A-C),c2=DC*AB;
                    if(c2>0)L=max(L,-(ld)c1/(ld)c2);
                    else if(c2<0)R=min(R,-(ld)c1/(ld)c2);
                    else if(c1<0||(c1==0&&(AB&DC)>0&&i>j)){ok=false;break;}
                }
                if(ok&&L<=R+eps)cov.push_back({max(0.0L,L),min(1.0L,R)});
            }
            sort(cov.begin(),cov.end());
            ld cur=0,edge=(ld)((vec(A.x,A.y))*vec(B.x,B.y));
            for(auto [l,r]:cov){
                if(r<cur+eps)continue;
                if(l>cur+eps)ans+=(l-cur)*edge;
                cur=max(cur,r);
                if(cur>=1-eps)break;
            }
            if(cur<1-eps)ans+=(1-cur)*edge;
        }
    }
    return ans/2;
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
