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
pit getNode(pit a,pit b,pit c,pit d){
    vec u=b-a,v=d-c;
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
pit getNode(pit a,vec u,pit c,vec v){
    double t=((c-a)*v)/(u*v);
    return a+u*t;
}
//O(v^2+nvlogn) 凸包面积并(逆时针) v为凸包总点数
//做法是 找出外轮廓 然后用鞋带公式算
double area_union(const vector<vector<pit>>& polys){
    double ans=0;
    int n=polys.size();
    for(int i=0;i<n;++i){
        int szi=polys[i].size();
        for(int e=0;e<szi;++e){
            pit A=polys[i][e],B=polys[i][(e+1)%szi];
            vec AB=B-A;
            vector<pair<double,double>> cov;
            for(int j=0;j<n;++j){
                if(i==j)continue;
                int szj=polys[j].size();
                double L=0,R=1.0;
                bool valid=1;
                for(int k=0;k<szj;++k){
                    pit C=polys[j][k],D=polys[j][(k+1)%szj];
                    vec DC=D-C,CA=A-C;
                    double c1=DC*CA,c2=DC*AB;
                    if(c2>eps)L=max(L,-c1/c2);
                    else if(c2<-eps)R=min(R,-c1/c2);
                    else{
                        if(c1<-eps){valid=0;break;}
                        else if(c1<=eps&&(AB&DC)>eps&&i>j){valid=0;break;}
                    }
                }
                if(valid&&L<=R)cov.push_back({L,R});
            }
            sort(cov.begin(),cov.end());
            double cur=0;
            for(auto& p:cov){
                if(p.first>cur+eps){
                    pit P1=A+AB*cur,P2=A+AB*p.first;
                    ans+=(P1.x*P2.y-P1.y*P2.x);
                }
                cur=max(cur,p.second);
            }
            if(cur<1.0-eps){
                pit P1=A+AB*cur;
                ans+=(P1.x*B.y-P1.y*B.x);
            }
        }
    }
    return ans/2.0;
}
signed main()
{
    return 0;
}