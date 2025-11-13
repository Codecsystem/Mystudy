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
        vector<vector<int>> dp;//dp[i][j]是[i,i+2^j-1]的min/max
        int inf(int a,int b)
        {
            return max(a,b);
        }
        void init(vector<int>& nums,int siz)
        {
            int len=log2(siz)+1;
            dp.resize(siz+1);
            for(auto &i:dp) i.resize(len);
            for(int i=1;i<=siz;i++)
            {
                dp[i][0]=nums[i];
            }
            for(int j=1;j<=len;j++)
            {
                for(int i=1;i+(1<<j)-1<=siz;i++)
                {
                    dp[i][j]=inf(dp[i][j-1],dp[i+(1<<(j-1))][j-1]);
                }
            }
        }
        int query(int l,int r)
        {
            int k=log2(r-l+1);
            return inf(dp[l][k],dp[r-(1<<k)+1][k]);
        }
        st(vector<int>& nums,int n){
            init(nums,n);
        }
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
    int T_start=clock();
    int n=read(),m=read();
    vector<int> nums(n+1);
    for(int i=1;i<=n;i++)
    {
        nums[i]=read();
    }
    st s(nums,n);
    for(int i=0;i<m;i++)
    {
        int l=read(),r=read();
        write(s.query(l,r));
        putchar('\n');
    }
    return 0;
}