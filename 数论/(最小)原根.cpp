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
#include <assert.h>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
class Pre{
public:
    int n;
    vector<int> phi,primes,ex,spf;
    Pre(int n):n(n),phi(n+1,0),ex(n+1,0),spf(n+1){
        prep();
    }
    ll qpow(int a,int b,int m)//快速幂
    {
        ll ans=1;
        while(b)
        {
            if(b&1) ans=ans*a%m;
            a=1ll*a*a%m,b>>=1;
        }
        return ans;
    }
    void prep()
    {
        phi[1]=1;
        vector<bool> v(n+1,0);
        for(int i=2;i<=n;i++)
        {
            if(!v[i]) 
            {
                primes.push_back(i);
                phi[i]=i-1;
                spf[i]=i;
            }
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                int m=primes[j]*i;
                v[m]=1;spf[m]=primes[j];
                if(i%primes[j]==0)
                {
                    phi[m]=phi[i]*primes[j];
                    break;
                }
                else phi[m]=phi[i]*(primes[j]-1);
            }
        }
        assert(n>=4);
        ex[2]=ex[4]=1;
        for(auto p:primes){
            if(p&1){
                ll tp=p;
                while(tp<=n){
                    ex[tp]=1;
                    if(2*tp<=n) ex[2*tp]=1;
                    tp*=p;
                }
            }
        }
        return ;
    }
    //O(m^1/4*logm^2) 求最小原根
    int getr(int m){
        if(!ex[m]) return -1;
        vector<int> now;
        int tp=phi[m];
        while(tp>1){
            int p=spf[tp];
            now.push_back(phi[m]/p);
            while(tp%p==0) tp/=p;
        }
        for(int j=1;j<=m;j++){
            if(__gcd(j,m)!=1) continue;
            int flag=1;
            for(auto k:now){
                if(qpow(j,k,m)==1){
                    flag=0;
                    break;
                }
            }
            if(flag) return j;
        }
    }
    //O(phi(m)*log phi(m)) 求m的所有原根
    vector<int> getar(int m){
        vector<int> ans;
        int mi=getr(m);
        if(mi==-1) return ans;
        int tp=mi;
        for(int i=1;i<=phi[m];i++){
            if(__gcd(i,phi[m])==1){
                ans.push_back(tp);
            }
            tp=1ll*tp*mi%m;
        }
        sort(ans.begin(),ans.end());
        return ans;
    }
    //O(n^5/4logn) 求1-n的所有最小原根
    vector<int> get()
    {
        vector<int> ans(n+1,-1);
        for(int i=1;i<=n;i++)
        {
            ans[i]=getr(i);
        }
        return ans;
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    Pre p(1e6+5);
    int t;cin>>t;
    while(t--)
    {
        int n,d;cin>>n>>d;
        auto now=p.getar(n);
        cout<<now.size()<<'\n';
        for(int i=d-1;i<now.size();i+=d) cout<<now[i]<<" ";
        cout<<'\n';
    }
    return 0;
}
