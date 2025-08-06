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
#define int long long //赫赫 要不要龙龙呢
using namespace std;
class Pre{
public:
    int n,tag;
    const int mod=1e9+7;
    vector<int> inv,fac,invfac;
    Pre(int n):n(n){}
    Pre(int n,int tag):n(n),tag(tag){
        inv.resize(n+1);
        fac.resize(n+1);
        invfac.resize(n+1);
        preC();
    }
    int ModQpow(int a,int b,int m)//快速幂
    {
        int ans=1;
        while(b)
        {
            if(b&1)ans=ans*a%m;
            a=a*a%m,b>>=1;
        }
        return ans;
    }
    //O(nlnn)求1-n所有数的约数
    vector<vector<int>> divs()
    {
        vector<vector<int>> ans(n+1);
        for(int i=1;i<=n;i++)
        {
            for(int j=i;j<=n;j+=i)
            {
                ans[j].push_back(i);
            }
        }
        return ans;
    }
    //O(n)求1-n所有的质数
    vector<int> primes()
    {
        vector<int> primes;
        vector<bool>v(n+1,0);
        for(int i=2;i<=n;i++)
        {
            if(!v[i])primes.push_back(i);
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                v[primes[j]*i]=1;
                if(i%primes[j]==0)break;
            }
        }
        return primes;
    }
    //O(n)求0-n的阶乘和阶乘逆元
    void preC()
    {
        inv[1]=1;
        for(int i=2;i<=n;i++)
        {
            inv[i]=(mod-mod/i)*inv[mod%i]%mod;
        }
        fac[0]=invfac[0]=1;
        for(int i=1;i<=n;i++)
        {
            fac[i]=fac[i-1]*i%mod;
            invfac[i]=invfac[i-1]*inv[i]%mod;
        }
    }
    //组合数C(n,m) n个数中选m个
    int C(int n,int m)
    {
        if(n<0||m<0||n<m)return 0;
        return fac[n]*invfac[m]%mod*invfac[n-m]%mod;
    }
    //O(n)求1-n的欧拉函数
    vector<int> euler()
    {
        vector<int> phi(n+1);
        phi[1]=1;
        vector<int> primes;
        vector<bool>v(n+1,0);
        for(int i=2;i<=n;i++)
        {
            if(!v[i])primes.push_back(i),phi[i]=i-1;
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                int m=primes[j]*i;
                v[m]=1;
                if(i%primes[j]==0)
                {
                    phi[m]=phi[i]*primes[j];
                    break;
                }
                else phi[m]=phi[i]*(primes[j]-1);
            }
        }
        return phi;
    }
    //O(n)求1-n的约数个数
    vector<int> d()
    {
        vector<int> a(n+1),d(n+1);
        vector<int> primes;
        vector<bool>v(n+1,0);
        for(int i=2;i<=n;i++)
        {
            if(!v[i])
            {
                primes.push_back(i);
                a[i]=1,d[i]=2;
            }
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                int m=primes[j]*i;
                v[m]=1;
                if(i%primes[j]==0)
                {
                    a[m]=a[i]+1;
                    d[m]=d[i]/(a[i]+1)*(a[m]+1);
                    break;
                }
                else
                {
                    a[m]=1;
                    d[m]=d[i]*2;
                }
            }
        }
        return d;
    }
    //O(n)求1-n的约数和
    vector<int> sumd()
    {
        vector<int> g(n+1),f(n+1);
        vector<int> primes;
        vector<bool>v(n+1,0);
        g[1]=f[1]=1;
        for(int i=2;i<=n;i++)
        {
            if(!v[i])
            {
                primes.push_back(i);
                f[i]=g[i]=i+1;
            }
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                int m=primes[j]*i;
                v[m]=1;
                if(i%primes[j]==0)
                {
                    g[m]=g[i]*primes[j]+1;
                    f[m]=f[i]*g[m]/g[i];
                    break;
                }
                else
                {
                    g[m]=primes[j]+1;
                    f[m]=f[i]*g[m];
                }
            }
        }
        return f;
    }
    //O(n)求1-n的莫比乌斯函数
    vector<int> mu()
    {
        vector<int> mu(n+1);
        mu[1]=1;
        vector<int> primes;
        vector<bool>v(n+1,0);
        for(int i=2;i<=n;i++)
        {
            if(!v[i])primes.push_back(i),mu[i]=-1;
            for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
            {
                int m=primes[j]*i;
                v[m]=1;
                if(i%primes[j]==0)
                {
                    mu[m]=0;
                    break;
                }
                else mu[m]=-mu[i];
            }
        }
        return mu;
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}
