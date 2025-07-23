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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
vector<int> primes(int n)
{
    //积性函数gcd(a,b)=1,f(ab)=f(a)f(b)
    //当f(p^k),p为质数时时的函数值可以快速求出，
    //即可以通过递推求出所有积性函数
    vector<int> primes;
    vector<bool>v(n+1,0);
    for(int i=2;i<=n;i++)
    {
        if(!v[i])
        {
            primes.push_back(i);
            //考虑f(p)=...
            //单个质数的情况
        }
        for(int j=0;j<primes.size()&&primes[j]*i<=n;j++)
        {
            v[primes[j]*i]=1;
            int m=primes[j]*i;
            if(i%primes[j]==0)
            {
                //此时p[j]是m的最小质因子,运用反证法p[j]也是m的最小质因子
                //考虑f(m)=...
                //多个质因子的情况
                break;
            }
            else{
                //此时gcd(i,pj)=1,f(m)=f(i)f(pj)
                //新增质因子的情况
            }
        }
    }
    return primes;
}
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}