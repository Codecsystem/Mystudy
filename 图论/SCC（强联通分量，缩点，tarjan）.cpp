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
///vector<vector<int>> tarjan(vector<vector<int>> &mp,int n,int m)
pair<vector<int>,int> tarjan(vector<vector<int>> &mp,int n)
{
    //求强连通分量，强连通分量是有向图中的极大顶点子集，其中任意两个顶点都是互相可达的
    ///vector<vector<int>> scc;//强连通分量
	vector<int> bel(n+1,-1);//bel[i]:i属于哪个强连通分量
    vector<int> dfn(n+1,-1),low(n+1,-1);
	//vector<int> inst(n+1,0);
    //dfn:时间戳（dfs序），low:从i开始能到达的最小时间戳，inst:是否在栈中
    stack<int> st;int cnt=0,scc_cnt=0;
    //st:未放到scc的点，cnt：计时器，初始为0
    auto dfs=[&](auto dfs,int u)->void{
        dfn[u]=low[u]=++cnt; //时间戳+1
        st.push(u); //inst[u]=1; //入栈
        for(int v:mp[u])
        {
            if(dfn[v]==-1)//case1:u的邻接点v未被访问过
            {
                dfs(dfs,v);
                low[u]=min(low[u],low[v]);//用v的low更新u的low
            }
            // else if(inst[v])//case2:u的邻接点v在栈中,且访问过
            // {
            //     low[u]=min(low[u],dfn[v]);
            //     //有可能存在一个环
            // }
			else if(bel[v]==-1)//v所属的强连通分量还未被确定（等价于case2）
			{
				low[u]=min(low[u],dfn[v]);
			}
            //case3:u的邻接点v不在栈中,且访问过
            //说明v已经确定在某个强连通分量中，所以u的low不需要更新
        }
        if(dfn[u]==low[u])
        {
            //u是某个强连通分量的根（第一个被访问的结点）
            //why,low[u]==dfn[u]说明u没有指向自己的边，所以u是某个强连通分量的根
            //某个强连通分量的根的low值不会被更新
            // vector<int> s;
            // while(true)
            // {
            //     int v=st.top();
            //     st.pop();inst[v]=0;
            //     s.push_back(v);
            //     if(v==u) break;
            // }
            // scc.push_back(s);
			scc_cnt++;
            while(true)
			{
				int v=st.top();
				st.pop();
				bel[v]=scc_cnt;
				if(v==u) break;
			}
        }
    };
    //图有可能不是强联通的
    for(int i=1;i<=n;i++)
    {
        if(dfn[i]==-1)
        {
            dfs(dfs,i);
        }
    }
    //return scc;
	return {bel,scc_cnt};
}
//证明：如果结点 u 是某个强连通分量在搜索树中遇到的第一个结点，那么这个强连通分量的其余结点肯定是在搜索树中以 u 为根的子树中。
//结点 u 被称为这个强连通分量的根。
//反证法：假设有个结点 v 在该强连通分量中但是不在以 u 为根的子树中，那么 u 到 v 的路径中肯定有一条离开子树的边。
//但是这样的边只可能是横叉边或者反祖边，然而这两条边都要求指向的结点已经被访问过了，这就和 v 不在以 u 为根的子树中矛盾了。得证。
//其实手玩一下，若有个结点 v 在该强连通分量中但是不在以 u 为根的子树中，他的与u形成的那个环，其实是v一定在u的子树中，从u的dfs一定能够遍历到v
//画个图就知道了
//并且，很容易想到，对于一个连通分量图，有且只有一个根，即第一个被访问的结点
//所以算法正确性显然
//时间复杂度：O(n+m)
int main()
{
    int T_start=clock();
    int n,m;cin>>n>>m;
    vector<vector<int>> mp(n+1);
	vector<int> val(n+1);
    for(int i=1;i<=n;i++) cin>>val[i];
    for(int i=0;i<m;i++)
    {
        int u,v;cin>>u>>v;
        mp[u].push_back(v);
    }
    auto [bel,cnt]=tarjan(mp,n);
	vector<vector<int>> mp2(cnt+1);
	vector<int> val2(cnt+1,0);
	vector<int> in(cnt+1,0);
	vector<int> dp(cnt+1,0);
	for(int i=1;i<=n;i++)
	{
		val2[bel[i]]+=val[i];
	}
    for(int i=1;i<=n;i++)
    {
        for(int v:mp[i])
        {
            if(bel[i]!=bel[v])
            {
                mp2[bel[i]].push_back(bel[v]);
				in[bel[v]]++;
            }
        }
    }
	queue<int> q;
	for(int i=1;i<=cnt;i++)
	{
		if(in[i]==0) q.push(i),dp[i]=val2[i];
	}
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:mp2[u])
		{
			dp[v]=max(dp[v],dp[u]+val2[v]);
			in[v]--;
			if(in[v]==0) q.push(v);
		}
	}
	cout<<*max_element(dp.begin()+1,dp.end())<<endl;
    return 0;
}