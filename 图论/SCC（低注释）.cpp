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
pair<vector<int>,int> tarjan(vector<vector<int>> &mp,int n)
{
	vector<int> bel(n+1,-1);//bel[i]:i属于哪个强连通分量
    vector<int> dfn(n+1,-1),low(n+1,-1);
    stack<int> st;int cnt=0,scc_cnt=0;
    auto dfs=[&](auto dfs,int u)->void{
        dfn[u]=low[u]=++cnt; //时间戳+1
        st.push(u); //inst[u]=1; //入栈
        for(int v:mp[u])
        {
            if(dfn[v]==-1)//case1:u的邻接点v未被访问过
            {
                dfs(dfs,v);
                low[u]=min(low[u],low[v]);
            }
			else if(bel[v]==-1)//v所属的强连通分量还未被确定（等价于case2）
			{
				low[u]=min(low[u],dfn[v]);
			}
            //case3:u的邻接点v不在栈中,且访问过
            //说明v已经确定在某个强连通分量中，所以u的low不需要更新
        }
        if(dfn[u]==low[u])
        {
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
	return {bel,scc_cnt};
}
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