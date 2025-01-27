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
using namespace std;
const int base1=27,base2=27;
const int hash_mod1=100663319,hash_mod2=402653189;
/*hash mod number 53 97 193 389 769 1543 3079 6151 12289 24593 49157 98317 196613 393241 786433 1572869 3145739 6291469 12582917 25165843 50331653 100663319 402653189 805306457 1610612741 (1e9+7,1e9+9)*/ 
//char ss[10][10010];
struct Data
{
    long long hash1;
    long long hash2;
}a[10005];
long long make_string_hash1(char s[])
{
    register long long ans=0;
    register int len=strlen(s);
    for(register int i=0;i<len;i++)
    {
        ans=(ans*base1+(long long)(s[i]))%hash_mod1;
    }
    return ans;
}
long long make_string_hash2(char s[])
{
    register long long ans=0;
    register int len=strlen(s);
    for(register int i=0;i<len;i++)
    {
        ans=(ans*base2+(long long)(s[i]))%hash_mod2;
    }
    return ans;
}//求子串哈希值hash=((hash[r]−hash[l−1]∗mod^(r−l+1))%mod+mod)%mod
int main()
{
    int T_start=clock();
    
    return 0;
}