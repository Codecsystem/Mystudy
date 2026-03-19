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
using namespace std;
const int MAXN=5e5+5;
const int LOG=25;//MAXN<=2^LOG
vector<int> tree[MAXN];
int dep[MAXN],st[MAXN][LOG];//节点深度，st表，st[i][j]=i的2^j级祖先
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
void init(int node,int parent)//用dfs预处理dep和st
{
	dep[node]=(parent==-1)?0:dep[parent]+1;
	st[node][0]=parent;//一级祖先为自身
	for(int i=1;i<LOG;i++)//更新node的祖先表
	{
		if(st[node][i-1]!=-1)
		{
			st[node][i]=st[st[node][i-1]][i-1];
			//node的2^j级祖先为node的2^j-1祖先的2^j-1祖先
		}
		else st[node][i]=-1;//你的码的码没了，你还有码？（可删吗？）
	}
	for(auto child:tree[node])
	{
		if(child!=parent)
		{
			init(child,node);//从父节点向下dfs
		}
	}
}
int lca(int u,int v)
{
	if(dep[u]<dep[v]) swap(u,v);//确保u比v深
	int diff=dep[u]-dep[v];
	for(int i=0;i<LOG;i++)
	{
		if((diff>>i)&1)
		{
			u=st[u][i];//u向上跳转2^i,其中i为diff的二进制表示中第i位为一
		}
	}
	if(u==v) return u;//深度相等，可能找到
	//不相等，假设他们与lca(u,v)的距离为diff
	//注意到5=4+1，5-4=1
	//7=4+2+1,7-4-2=1
	//6=4+2,6-4-1=1
	//12=8+4,12-8-2-1
	//做以下操作总能使diff=1
	// for(int i=LOG-1;i>=0;i--)
	// {
	// 	if(st[u][i]!=st[v][i])
	// 	{
	// 		u=st[u][i];
	// 		v=st[v][i];
	// 	}
	// }
	// return st[u][0];
	//优化版
	for(int i=LOG-1;i>=0;i--)
	{
		if(st[u][i]!=st[v][i])
		{
			u=st[u][i];
			v=st[v][i];
		}
	}
	return st[u][0];
}
int main()
{
	int T_start=clock();
	int n=read(),m=read(),s=read();//n个点，n-1条边,m个询问，s为根
	for(int i=0;i<n-1;i++)
	{
		int u=read(),v=read();
		tree[u].push_back(v);
		tree[v].push_back(u);//存树
	}
	for(int i=1;i<=n;i++)
	{
		dep[i]=-1;
		for(int j=0;j<LOG;j++)
		{
			st[i][j]=-1;
		}
	}
	init(s,-1);
	while(m--)
	{
		write(lca(read(),read()));
		putchar('\n');
	}
	return 0;
}