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
vector<vector<int>> tarjan(vector<vector<int>> &mp,int n)
{
	//点双连通分量：无割点，且任意两点间至少有两条路径
	vector<int> low(n+1,-1),dfn(n+1,-1);
	//low:从当前点出发能到达的最早时间戳
	//dfn:当前点的时间戳
	vector<vector<int>> bccs;
	stack<int> st;
	int cnt=0;
	auto dfs=[&](auto dfs,int u,int fa)->void{
		int ch=0; //儿子数
		dfn[u]=low[u]=++cnt;
		st.push(u);
		for(auto v:mp[u])
		{
			if(dfn[v]==-1)//case1:未访问
			{
				ch++;
				dfs(dfs,v,u);
				low[u]=min(low[u],low[v]);//更新low[u]
				if((fa==-1&&ch>1)||(fa!=-1&&low[v]>=dfn[u]))//是割点
				{
					vector<int> bcc;
					while(1)
					{
						int x=st.top();st.pop();
						bcc.push_back(x);
						if(x==v)break;//处理到v
					}
					bcc.push_back(u);//把割点也加入bcc:割点有可能在多个bcc中
					bccs.push_back(bcc);
				}
			}
			else if(v!=fa)//case2:已访问且不是父节点
			{
				low[u]=min(low[u],dfn[v]);//更新low[u]
			}
		}
		// if(fa==-1&&ch==0) {
		// 	bccs.push_back({u});
		// }
	};
	for(int i=1;i<=n;i++)
	{
		if(dfn[i]==-1)
		{
			dfs(dfs,i,-1);
			//处理剩下的bcc
			vector<int> bcc;
			while(!st.empty())
			{
				int x=st.top();st.pop();
				bcc.push_back(x);
			}
			if(!bcc.empty()) bccs.push_back(bcc);
 		}
	}
	return bccs;
}
//无向图中割点：删除该点后，图的bcc数增加
//一个图中割点的判断
//1.对于某个顶点 u，如果存在至少一个顶点 v（u 的儿子），使得low[v]>=dfn[u] ，即只能回到祖先（到不了dfn更早的点），那么 u 点为割点。
//2.对于搜索的起始点，如果它的儿子数大于等于 2，那么它就是割点。
int main()
{
	int T_start=clock();
	int n,m;cin>>n>>m;
    vector<vector<int>> mp(n+1);
	vector<int> val(n+1);
    for(int i=0;i<m;i++)
    {
        int u,v;cin>>u>>v;
        mp[u].push_back(v);
		mp[v].push_back(u);
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