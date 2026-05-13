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
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using i128=__int128;
using namespace std;
namespace _EX_CRT{
    ll exgcd(ll a,ll b,ll &x,ll &y){
        if(!b) return x=1,y=0,a;
        ll d=exgcd(b,a%b,y,x);
        return y-=a/b*x,d;
    }
    //r,m 1base
    ll crt(const vector<ll> &r,const vector<ll> &m,int n){
        ll M=1,ans=0;
        for(int i=1;i<=n;i++) M*=m[i];
        for(int i=1;i<=n;i++){
            ll mi=M/m[i],x,y;
            exgcd(mi,m[i],x,y);
            x=(x%m[i]+m[i])%m[i];
            ll tp=(ll)((i128)r[i]*mi%M);
            ans+=(ll)((i128)tp*x%M)%M,ans%=M;
        }
        return (ans%M+M)%M;
    }
    array<ll,2> merge(ll r1,ll m1,ll r2,ll m2){
        ll x,y;
        ll g=exgcd(m1,m2,x,y);
        ll c=(r2-r1)%m2;
        if(c<0) c+=m2;
        if(c%g) return {-1,-1};
        ll bg=m2/g;
        x=(x%bg+bg)%bg;
        ll tp=(ll)((i128)x*(c/g)%bg);
        // i128 _nM=(i128)m1/g*m2;
        // if(_nM>(i128)1e18){
        //     ll nR=r1+(ll)((i128)tp*m1);
        //     return {nR,-1};
        // } //这里是防爆模数
        ll nM=m1/g*m2;
        ll nR=(r1+(ll)((i128)tp*m1%nM))%nM;
        return {nR,nM};
    }
    ll excrt(const vector<ll> &r,const vector<ll> &m,int n){
        ll R=r[1],M=m[1];
        for(int i=2;i<=n;i++){
            auto [nR,nM]=merge(R,M,r[i],m[i]);
            if(nR==-1) return -1;
            R=nR,M=nM;
        }
        return R;
    }
}
//使用例：
//vector<ll> r={0,1,2,3},m={0,3,5,7};
//cout<<_EX_CRT::crt(r,m,3)<<endl;
//cout<<_EX_CRT::excrt(r,m,3)<<endl;
//cout<<_EX_CRT::merge(1,3,2,5)[0]<<endl;
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    while(t--){
        int n,q;cin>>n>>q;
        vector<vector<array<int,2>>> tr(n+1);
        vector<int> fa(n+1,0),l(n+1,0),qry(q),ans(q);
        for(int i=2;i<=n;i++) cin>>fa[i];
        for(int i=2;i<=n;i++) cin>>l[i];
        for(int i=2;i<=n;i++) tr[fa[i]].push_back({i,l[i]});
        for(int i=1;i<=n;i++) sort(tr[i].begin(),tr[i].end());
        vector<ll> s(n+1,0),m(q);
        for(int i=0;i<q;i++) cin>>m[i];
        iota(qry.begin(),qry.end(),0);
        auto dfs=[&](this auto&& dfs,int u,vector<int> qr,ll nr,ll nm)->void{
            if(qr.empty()) return;
            if(tr[u].empty()){
                for(int i:qr) ans[i]=u;
                return;
            }
            int d=tr[u].size();
            if(nm==-1||nm%d==0){
                ll k=(nr+s[u])%d;
                int v=tr[u][k][0];
                s[v]=s[u]+tr[u][k][1];
                dfs(v,move(qr),nr,nm);
            }
            else{
                vector<vector<int>> nxt(d);
                for(auto id:qr){
                    nxt[(m[id]+s[u])%d].push_back(id);
                }
                for(int k=0;k<d;k++){
                    if(nxt[k].empty()) continue;
                    int v=tr[u][k][0];
                    s[v]=s[u]+tr[u][k][1];
                    auto [nR,nM]=_EX_CRT::merge(nr,nm,(k-s[u]%d+d)%d,d);
                    if(nM>1e18l) nM=-1;
                    dfs(v,move(nxt[k]),nR,nM);
                }
            }
        };
        dfs(1,move(qry),0,1);
        for(int i:ans) cout<<i<<' ';
        cout<<'\n';
    }
    return 0;
}