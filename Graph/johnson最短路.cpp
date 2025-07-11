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
using namespace std;
#define int long long
int has_neg=0;
vector<int> SPFA(vector<vector<pair<int,int>>>& mp,int s,int n)
{
    vector<int> dis(n+1,1e9);
    vector<int> vis(n+1,0);
    vector<int> cnt(n+1,0);
    dis[s]=0;queue<int> q;
    q.push(s);vis[s]=1;cnt[s]=0;
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
                if(cnt[v]>n-1)//存在负权环
                {
                    //1-n的节点，最短路最多经过n-1条边，如果经过n条边，说明存在负权环
                    has_neg=1;
                    return dis;
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
    //SPFA: 形式上Bellman_Ford是一棵树，很显然，只有上一次被松弛的节点u，才有可能对v进行松弛，所以可以采用SPFA
    //为啥只有上一次被松弛的节点u，才有可能对v进行松弛？
    //手玩一下就好了（悲
    //考虑简单图，他可以是个递推的过程
}
vector<int> dijkstra(int n,vector<vector<pair<int,int>>>& mp,int s)
{
    vector<int> dis(n+1,1e9);//初始化距离为无穷大
    dis[s]=0;//起点到起点的距离为0
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    pq.push({0,s});//将起点加入优先队列
    while(!pq.empty())
    {
        int u=pq.top().second;//取出当前距离最小的点
        int d=pq.top().first;//取出当前距离最小的点的距离
        pq.pop();
        if(d>dis[u]) continue;//u已经被更新过
        if(mp[u].empty()) continue;
        for(auto it:mp[u])
        {
            int v=it.first;
            int w=it.second;
            if(dis[v]>dis[u]+w)//更新s->v的最短距离(min(s->v,s->u->v))
            {
                dis[v]=dis[u]+w;
                pq.push({dis[v],v});
            }
        }
    }
    //正确性证明：
    //假设目前更新s->t(=3),假设存在s->u->t(=2),则s->u<s->t,而s->u一定在之前被更新过，所以s->u->t一定在之前被更新过，与假设矛盾。
    //单源最短路(正边权)
    //时间复杂度O(ElogV),E为边数,V为点数(二叉堆)
    //使用斐波那契堆的 Dijkstra 算法的时间复杂度为 O(E+VlogV)。
    //不用堆优化：O(v^2+E)
    //当E<<v^2时，使用堆优化
    //当E~v^2时，不用堆优化
    return dis;
}
vector<vector<int>> johnson(vector<vector<pair<int,int>>>& mp,int n)
{
    //1.添加一个虚拟节点0，连接到所有节点，边权为0
    for(int i=1;i<=n;i++)
    {
        mp[0].push_back({i,0});
    }
    //2.使用Bellman-Ford算法计算从虚拟节点0到所有节点的最短路径
    vector<int> h=SPFA(mp,0,n+1);
    if(has_neg==1) 
    {
        cout<<-1<<endl;
        exit(0);
    }
    //3.删除虚拟节点0，并更新所有边的权重
    for(int i=1;i<=n;i++)
    {
        for(auto& it:mp[i])
        {
            it.second+=h[i]-h[it.first];
        }
    }
    //4.对每个节点i，使用Dijkstra算法计算从i到所有节点的最短路径
    vector<vector<int>> dis(n+1,vector<int>(n+1,1e9));
    for(int i=1;i<=n;i++)
    {
        dis[i]=dijkstra(n,mp,i);
        for(int j=1;j<=n;j++)
        {
            dis[i][j]-=h[i]-h[j];
            if(dis[i][j]>=1e8) dis[i][j]=1e9;
        }
    }
    //5.更新所有边的权重
    for(int i=1;i<=n;i++)
    {
        for(auto& it:mp[i])
        {
            it.second+=h[it.first]-h[i];
        }
    }
    return dis;
}
signed main()
{
    int T_start=clock();
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,int>>> mp(n+1);
    for(int i=0;i<m;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        mp[u].push_back({v,w});
    }
    vector<vector<int>> dis=johnson(mp,n);
    for(int i=1;i<=n;i++)
    {
        int ans=0;
        for(int j=1;j<=n;j++)
        {
            //cout<<dis[i][j]<<" ";
            ans+=j*dis[i][j];
        }
        //cout<<endl;
        cout<<ans<<endl;
    }
    return 0;
}
//johnson全源最短路算法：O(nmlogm)
//重新标记边权后,u-v两点的任意路径一定有hu-hv项，最短路不变
//由于三角形不等式，所以重新标记边权，边权一定非负
//所以重新标记边权后，可以使用Dijkstra算法求最短路