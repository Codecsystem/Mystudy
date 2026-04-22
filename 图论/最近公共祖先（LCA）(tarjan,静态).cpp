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
#include <unordered_map>
using namespace std;
const int MaxN=5e5+5;
vector<int> tree[MaxN];
vector<pair<int,int>> q[MaxN];
int vis[MaxN],ans[MaxN];
int fa[MaxN];
void prepare_tree(int n)
{
	for(register int i=1;i<=n;i++)
	{
		fa[i]=i;
	}
}
int find(int G)
{
	if(G==fa[G]) return G;
	else
	{
		fa[G]=find(fa[G]);
		return fa[G];
	}
	//return G==fa[G]? G:(fa[G]=find(fa[G]));
}
void merge(int a,int b)//合并
{
	fa[find(a)]=find(b);//有时路径压缩可能破坏rank'(rank->树深)
	/*register int x=find(a),y=find(b);
	Rank[x]<=Rank[y]?fa[x]=y:fa[y]=x;
	if(Rank[x]==Rank[y]&&x!=y) Rank[y]++;*/

}
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
void dfs(int node)
{
	vis[node]=1;
	for(auto child:tree[node])
	{
		if(!vis[child])
		{
			dfs(child);
			fa[child]=node;//调换顺序会使路径压缩到child的父节点，此时子树还没遍历完
		}
	}
	for(auto i:q[node])
	{
		if(vis[i.first])//node及其子树已经dfs完了,如果此时i已经搜到，显然，根据dfs原则，find(i)是lca(i,node)
		{
			ans[i.second]=find(i.first);
		}
	}
}
int main()
{
	int T_start=clock();
	int n=read(),m=read(),s=read();
	for(int i=0;i<n-1;i++)
	{
		int u=read(),v=read();
		tree[v].push_back(u);
		tree[u].push_back(v);
	}
	for(int i=0;i<m;i++)
	{
		int u=read(),v=read();
		q[v].push_back(make_pair(u,i));
		q[u].push_back(make_pair(v,i));
	}
	prepare_tree(n);
	for(int i=1;i<=n;i++)
	{
		vis[i]=0;
	}
	dfs(s);
	for(int i=0;i<m;i++)
	{
		write(ans[i]);
		putchar('\n');
	}
	return 0;
}