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
using namespace std;
class st{
    public:
        vector<vector<int>> dp;
        int inf(int a,int b){return max(a,b);}
        void init(vector<int>& a,int n)
        {
            if(!n) return;
            int len=__lg(n)+1;
            dp.assign(len,vector<int>(n+1));
            for(int i=1;i<=n;i++) dp[0][i]=a[i];
            for(int j=1;j<len;j++)
                for(int i=1;i+(1<<j)-1<=n;i++)
                    dp[j][i]=inf(dp[j-1][i],dp[j-1][i+(1<<(j-1))]);
        }
        int query(int l,int r)
        {
            int k=__lg(r-l+1);
            return inf(dp[k][l],dp[k][r-(1<<k)+1]);
        }
        st(vector<int>& a,int n){init(a,n);}
};
int read()
{
    int s=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9')
    {
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9')
    {
        s=(s<<3)+(s<<1)+ch-'0';
        ch=getchar();
    }
    return s*f;
}
inline void write(int x) 
{
    static int sta[35]; 
    int top=0;
    if(x<0&&x!=-2147483648) {putchar('-');x=-x;}
    if(x==-2147483648) {printf("-2147483648");return;}
    do{
      sta[top++]=x%10, x/=10;
      }while(x);
    while(top) putchar(sta[--top]+48);
}
int main()
{
    return 0;
}