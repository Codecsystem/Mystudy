#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;

//整数向量保存凸包边，叉积和点积均在i128中精确完成。
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    vec operator+(const vec& o)const{return {x+o.x,y+o.y};}
    i128 operator*(const vec& o)const{return (i128)x*o.y-(i128)y*o.x;}
    i128 operator&(const vec& o)const{return (i128)x*o.x+(i128)y*o.y;}
};
//整数点用于输入、平移和输出闵可夫斯基和的顶点。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return {x-o.x,y-o.y};}
    pit operator+(const vec& o)const{return {x+o.x,y+o.y};}
    pit operator+(const pit& o)const{return {x+o.x,y+o.y};}
    bool operator==(const pit& o)const{return x==o.x&&y==o.y;}
    bool operator<(const pit& o)const{return x!=o.x?x<o.x:y<o.y;}
};

//返回三点有向面积的两倍，用于检查凸性和删除共线冗余点。
i128 crossVal(const pit& a,const pit& b,const pit& c){return (b-a)*(c-a);}
//返回多边形有向面积的两倍，正值表示逆时针。
i128 area2(const vector<pit>& p){
    i128 res=0;
    for(int i=0;i<(int)p.size();i++)res+=(i128)p[i].x*p[(i+1)%p.size()].y-(i128)p[i].y*p[(i+1)%p.size()].x;
    return res;
}
//在线性时间内规范凸多边形：去重、转为逆时针并删除边上的中间点。
vector<pit> normalizeConvex(vector<pit> p){
    if(p.size()>1&&p.front()==p.back())p.pop_back();
    vector<pit> q;
    for(const auto& x:p)if(q.empty()||!(q.back()==x))q.push_back(x);
    if(q.size()>1&&q.front()==q.back())q.pop_back();
    if(q.size()<3){
        sort(q.begin(),q.end());
        q.erase(unique(q.begin(),q.end()),q.end());
        return q;
    }
    i128 s=area2(q);
    if(s==0){
        auto [mn,mx]=minmax_element(q.begin(),q.end());
        if(*mn==*mx)return {*mn};
        return {*mn,*mx};
    }
    if(s<0)reverse(q.begin(),q.end());
    vector<pit> r;
    for(const auto& x:q){
        while(r.size()>1&&crossVal(r[r.size()-2],r.back(),x)==0)r.pop_back();
        r.push_back(x);
    }
    while(r.size()>2&&crossVal(r[r.size()-2],r.back(),r[0])==0)r.pop_back();
    while(r.size()>2&&crossVal(r.back(),r[0],r[1])==0)r.erase(r.begin());
    return r;
}
//把方向按[0,2pi)分到两个半平面，供边向量归并使用。
int half(const vec& a){return a.y>0||(a.y==0&&a.x>=0)?0:1;}
//严格比较两个非零边向量的极角，不调用atan2。
bool angleLess(const vec& a,const vec& b){
    int ha=half(a),hb=half(b);
    if(ha!=hb)return ha<hb;
    i128 cr=a*b;
    if(cr!=0)return cr>0;
    return false;
}
//在线性时间求两个凸集的闵可夫斯基和，输入可顺/逆时针且无需重复首点。
vector<pit> minkowski(vector<pit> a,vector<pit> b){
    a=normalizeConvex(move(a));b=normalizeConvex(move(b));
    if(a.empty()||b.empty())return {};
    auto cmp=[](const pit& A,const pit& B){return A.y!=B.y?A.y<B.y:A.x<B.x;};
    rotate(a.begin(),min_element(a.begin(),a.end(),cmp),a.end());
    rotate(b.begin(),min_element(b.begin(),b.end(),cmp),b.end());
    vector<vec> ea,eb;
    if(a.size()>1)for(int i=0;i<(int)a.size();i++)ea.push_back(a[(i+1)%a.size()]-a[i]);
    if(b.size()>1)for(int i=0;i<(int)b.size();i++)eb.push_back(b[(i+1)%b.size()]-b[i]);
    vector<pit> res={a[0]+b[0]};
    int i=0,j=0;
    while(i<(int)ea.size()||j<(int)eb.size()){
        vec e;
        if(j==(int)eb.size()||(i<(int)ea.size()&&angleLess(ea[i],eb[j])))e=ea[i++];
        else if(i==(int)ea.size()||angleLess(eb[j],ea[i]))e=eb[j++];
        else e=ea[i++]+eb[j++];
        res.push_back(res.back()+e);
    }
    if(res.size()>1&&res.back()==res.front())res.pop_back();
    return res;
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
