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
using namespace std;
long long ModQpow(long long a,long long b,long long m)//快速幂
{
    long long ans=1;
    while(b)
    {
        if(b&1) ans=ans*a%m;
        a=a*a%m;b>>=1;
    }
    return ans;
}
long long invMod1(int a,int m)//a在模m意义下的逆元（费马小定理，m为质数），即a^(m-2)
{
    return ModQpow(a,m-2,m);
}
long long exgcd(long long a,long long b,long long &x,long long &y)//扩展欧几里得
{
    if(b==0)
    {
        x=1;y=0;
        return a;
    }
    long long d=exgcd(b,a%b,x,y),t=x;
    x=y;y=t-a/b*y;
    return d;
}
long long invMod2(int a,int m)//a在模m意义下的逆元（扩展欧几里得）
{
    long long x,y;
    exgcd(a,m,x,y);
    return (x%m+m)%m;
}
int main()
{
    int T_start=clock();
    
    return 0;
}