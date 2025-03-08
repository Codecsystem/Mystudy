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
int flag=0;
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
vector<int> SPFA(vector<vector<pair<int,int>>>& mp,int s,int n)
{
    vector<int> dis(n+1,0x7fffffff);
    vector<int> vis(n+1,0);
    vector<int> cnt(n+1,0);
    dis[s]=0;queue<int> q;
    q.push(s);vis[s]=1;cnt[s]=1;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();vis[u]=0;
        for(auto [v,w]:mp[u])
        {
            if(dis[v]>dis[u]+w)//松弛
            {
                dis[v]=dis[u]+w;
                cnt[v]=cnt[u]+1;
                if(cnt[v]>=n+1)
                {
                    flag=1;
                    return vector<int>(n+1,-1);
                }
                if(!vis[v])
                {
                    q.push(v);
                    vis[v]=1;
                }
            }
        }
    }
    return dis;
}
int main()
{
    int T_start=clock();
    int n=read(),m=read();
    vector<vector<pair<int,int>>> mp(n+1);
    for(int i=1;i<=m;i++)
    {
        int v=read(),u=read(),w=read();
        mp[u].push_back(make_pair(v,w));
    }
    for(int i=1;i<=n;i++)
    {
        mp[0].push_back(make_pair(i,0));
    }
    vector<int>ans=SPFA(mp,0,n);
    if(flag==1)
    {
        printf("NO\n");
    }
    else
    {
        //printf("YES\n");
        for(int i=1;i<=n;i++)
        {
            printf("%d ",ans[i]);
        }
        printf("\n");
    }
    return 0;
}
//对一个差分约束系统，判断是否存在一组解，使得所有约束条件都成立。
//ex. x1-x2<=3
//    x2-x3<=-2
//    x1-x3<=1
//将xn看作超级源点（到所有点的权值为w=0）到n的最短路
//那么第一个式子的意义就是x1<=x2+3,0到1的最短路<=3+0到2的最短路
//在图上的意义就是建2->1的边权为3的边，0->1,0->2的边权为0的边
//那么整个系统就转化为了一张图
//求xn即求0到n的最短路，如果存在负环，则无解，否则有解
//负环还原的形式为
//x1-x2<=-1...1
//x2-x3<=-4...2
//x3-x1<=-5...3
//1+2+3->0<=-10,不成立
//还有结论，设定w即求x1,x2..xn<=w的最大解
//如果差分约束系统换换不等号，求最长路，spfa改一下即可