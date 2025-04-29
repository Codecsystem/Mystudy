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
	//freopen("in.txt","r",stdin);
	int n,m;cin>>n>>m;
	vector<vector<int>> mp(2*n+1);
	for(int i=0;i<m;i++)
	{
	    int u,b1,v,b2;
	    cin>>u>>b1>>v>>b2;
		//u=b1 or v=b2
		//=>u!=b1->v=b2 and v!=b2->u=b1
		mp[u+(!b1)*n].push_back(v+b2*n);
		mp[v+(!b2)*n].push_back(u+b1*n);
		//u=b1-> u+b1*n x
	}
	auto [bel,scc_cnt]=tarjan(mp,2*n);
	vector<int> ans(n+1,-1);
	int flag=1;
	for(int i=1;i<=n;i++)
	{
		if(bel[i]==bel[i+n]) {flag=0;break;}
		else ans[i]=bel[i]>bel[i+n];
	}
	//此处处理的是i的正确性
	//当bel[u==0]>bel[u==1]时,u==0的拓扑序小,i应当被赋值为false
	//因为i->!i为永真式的前提为i=0
	//此处i的含义是命题变元i的取值=0
	//所以ans[i]=1
	if(flag)
	{
		cout<<"POSSIBLE"<<endl;
		for(int i=1;i<=n;i++)
		{
			cout<<ans[i]<<" ";
		}
		cout<<endl;
	}
	else cout<<"IMPOSSIBLE"<<endl;
	return 0;
}
//2-sat
//处理n个命题变元的赋值问题，形式上判断形如(p->q) and (!p->q)是否可永真赋值
//即判断是否存在一种赋值使得p->q和!p->q同时为真
//很显然若p->q,q->p均成立,则p,q在一个scc里