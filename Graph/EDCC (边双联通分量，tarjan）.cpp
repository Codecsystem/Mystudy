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
vector<vector<int>> tarjan(vector<vector<pair<int,int>>> &mp,int n)
{
	//边双联通分量：无向图中，边双联通分量是指一个极大子图，删除该子图中的任意一条边，该子图仍然连通
    //连接边双联通分量的边称为桥
	vector<int> low(n+1,-1),dfn(n+1,-1);
	//low:从当前点出发能到达的最早时间戳
	//dfn:当前点的时间戳
	vector<vector<int>> dccs;
	stack<int> st; int cnt=0;
	auto dfs=[&](auto dfs,int u,int fre)->void{
        //fre:来时的边
		dfn[u]=low[u]=++cnt;
		st.push(u);
		for(auto [v,rev]:mp[u])
		{
			if(dfn[v]==-1)//case1:未访问
			{
				dfs(dfs,v,rev);
				low[u]=min(low[u],low[v]);//更新low[u]
			}
			else if(rev!=(fre^1))//case2:已访问且不是该边的反边
			{
                //阻断向父节点更新的可能
                //多重边可能有一种特殊的组合让v!=fa失效
                //eg.1-2,1-2,2-3,2-3
				low[u]=min(low[u],dfn[v]);//更新low[u]
			}
		}
        if(dfn[u]==low[u])//case3:u的子树中不存在能到达u的祖先的边
        //u的子树全为dcc
		{
			vector<int> dcc;
			while(true){
				int t=st.top();st.pop();
				dcc.push_back(t);
				if(t==u) break;
            }
			dccs.push_back(dcc);
        }
	};
	for(int i=1;i<=n;i++)
	{
		if(dfn[i]==-1)
		{
			dfs(dfs,i,-1);
 		}
	}
	return dccs;
}
int main()
{
	int T_start=clock();
	int n,m;cin>>n>>m;
    vector<vector<pair<int,int>>> mp(n+1);
	vector<int> val(n+1);
    int tot=0;
    for(int i=0;i<m;i++)
    {
        int u,v;cin>>u>>v;
        if(u==v) continue;
        mp[u].push_back({v,tot+1});
		mp[v].push_back({u,tot});
        tot+=2;//存各自的边的编号
    }
	vector<vector<int>> bccs=tarjan(mp,n);
	cout<<bccs.size()<<endl;
	for(auto bcc: bccs)
	{
		cout<<bcc.size()<<' ';
		for(auto x: bcc)
		{
			cout<<x<<" ";
		}
		cout<<endl;
	}
	return 0;
}