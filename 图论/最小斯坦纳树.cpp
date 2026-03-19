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
const int INF=1e18;
using namespace std;

signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m,k;cin>>n>>m>>k;
    vector<vector<array<int,2>>> mp(n+1);
    for(int i=1;i<=m;i++)
    {
        int u,v,w;cin>>u>>v>>w;
        mp[u].push_back({v,w});
        mp[v].push_back({u,w});
    }
    vector<vector<int>> dp((1<<k),vector<int>(n+1,INF));
    //dp[i][j]表示以i为集合，j为根的最小贡献
    vector<int> sp(k+1);
    for(int i=1;i<=k;i++)
    {
        cin>>sp[i];
        dp[1<<(i-1)][sp[i]]=0;
    }
    for(int st=1;st<(1<<k);st++){
        for(int t=st;t;t=(t-1)&st){
            for(int i=1;i<=n;i++)
            {
                dp[st][i]=min(dp[st][i],dp[t][i]+dp[st-t][i]);
            }
        }//枚举子集，合并 只保证了v节点的状态是最优的
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
        vector<int> vis(n+1,0);
        for(int i=1;i<=n;i++){
            if(dp[st][i]!=INF) q.push({dp[st][i],i}); 
        }
        while(!q.empty()){
            int u=q.top().second;q.pop();
            if(vis[u]) continue;
            vis[u]=1;
            for(auto [v,w]:mp[u]){
                if(dp[st][v]>dp[st][u]+w){
                    dp[st][v]=dp[st][u]+w;
                    q.push({dp[st][v],v});
                }
            }
        }//状态传播
    }
    cout<<dp[(1<<k)-1][sp[1]]<<endl;
    //状态传播完了,由于树的性质,所以sp[1]一定是根节点
    return 0;
}
//最小斯坦纳树 给定一个图 和k个关键点 求一个包含所有关键点的最小生成树(可以用其他点)
//时间复杂度(n*3^k+2^k*mlogm)