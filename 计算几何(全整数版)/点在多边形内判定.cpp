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
#include <chrono>
#include <random>
#include <cstdint>
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
using ll=long long;
using ld=long double;
using dpit=array<ld,2>;
const ld eps=1e-9;
struct vec{
    ll x,y;
    vec(ll x=0,ll y=0):x(x),y(y){}
    vec operator+(const vec& o)const{return vec(x+o.x,y+o.y);}
    vec operator-(const vec& o)const{return vec(x-o.x,y-o.y);}
    ll operator*(const vec& o)const{return x*o.y-y*o.x;} //叉积
    ll operator&(const vec& o)const{return x*o.x+y*o.y;} //点积
};
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
    vec operator-(const pit& o)const{return vec(x-o.x,y-o.y);}
    pit operator+(const vec& o)const{return pit(x+o.x,y+o.y);}
    pit operator+(const pit& o)const{return pit(x+o.x,y+o.y);}
};
int cross(pit a,pit b,pit c){
    ll val=(b-a)*(c-a);
    if(val>0)return 1;
    if(val<0)return -1;
    return 0;
}
bool onSeg(pit a,pit b,pit p){
    return cross(a,b,p)==0&&((a-p)&(b-p))<=0;
}
bool scross(pit a,pit b,pit c,pit d){
    int c1=cross(a,b,c),c2=cross(a,b,d),c3=cross(c,d,a),c4=cross(c,d,b);
    if(c1==0&&onSeg(a,b,c))return 1;
    if(c2==0&&onSeg(a,b,d))return 1;
    if(c3==0&&onSeg(c,d,a))return 1;
    if(c4==0&&onSeg(c,d,b))return 1;
    return c1*c2<0&&c3*c4<0;
}
dpit getnode(pit a,pit b,pit c,pit d){
    vec u=b-a,v=d-c;
    ld t=1.0*((c-a)*v)/(u*v);
    return {a.x+u.x*t,a.y+u.y*t};
}
bool inPoly(pit p,const vector<pit>& poly){
    int n=poly.size();
    if(n==1) return p.x==poly[0].x&&p.y==poly[0].y;
    if(n==2) return onSeg(poly[0],poly[1],p);
    int cnt=0;
    for(int i=0;i<n;++i){
        pit a=poly[i],b=poly[(i+1)%n];
        if(onSeg(a,b,p))return 1;
        int k=cross(a,b,p);
        ll d1=a.y-p.y,d2=b.y-p.y;
        if(k>0&&d1<=0&&d2>0)cnt++;
        if(k<0&&d2<=0&&d1>0)cnt--;
    }
    return cnt!=0;
}
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    while(t--){
        int n,m,q;cin>>n>>m>>q;
        vector<pit> a(n),b(m);
        for(int i=0;i<n;i++) cin>>a[i].x>>a[i].y;
        for(int i=0;i<m;i++) cin>>b[i].x>>b[i].y;
        while(q--){
            pit c;cin>>c.x>>c.y;
            vector<pit> nb(m);
            for(int i=0;i<m;i++) nb[i]=pit(c.x-b[i].x,c.y-b[i].y);
            bool flag=0; 
            dpit ans1,ans2;
            for(int i=0;i<n;i++){
                if(flag) break;
                for(int j=0;j<m;j++){
                    if(flag) break;
                    pit a1=a[i],a2=a[(i+1)%n],b1=nb[j],b2=nb[(j+1)%m];
                    if(scross(a1,a2,b1,b2)){
                        if(cross(a1,a2,b1)==0&&cross(a1,a2,b2)==0){
                            if(onSeg(a1,a2,b1)) ans1={1.0*b1.x,1.0*b1.y};
                            else if(onSeg(a1,a2,b2)) ans1={1.0*b2.x,1.0*b2.y};
                            else if(onSeg(b1,b2,a1)) ans1={1.0*a1.x,1.0*a1.y};
                            else ans1={1.0*a2.x,1.0*a2.y};
                        }
                        else ans1=getnode(a1,a2,b1,b2);
                        ans2={c.x-ans1[0],c.y-ans1[1]};
                        flag=1;
                    }
                }
            }
            for(int i=0;i<n;i++){
                if(inPoly(a[i],nb)){
                    ans1={(ld)a[i].x,(ld)a[i].y};
                    ans2={c.x-ans1[0],c.y-ans1[1]};
                    flag=1;
                    break;
                }
            }
            for(int i=0;i<m;i++){
                if(inPoly(nb[i],a)){
                    ans1={(ld)nb[i].x,(ld)nb[i].y};
                    ans2={c.x-ans1[0],c.y-ans1[1]};
                    flag=1;
                    break;
                }
            }
            if(flag) cout<<fixed<<setprecision(10)<<ans1[0]<<" "<<ans1[1]<<" "<<ans2[0]<<" "<<ans2[1]<<'\n';
            else cout<<"-1 -1 -1 -1\n";
        }
    }
    return 0;
}