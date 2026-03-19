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
#define int long long
const int N=1e5+5;
int dep[N],fa[N],hson[N],top[N],siz[N],dfn[N],rk[N],val[N];
int mod;
class SegTree{
    public:
        struct Node
        {
            int sum;
            int s,e;
            int lazy=0;
            Node* lt;
            Node* rt;
            Node(int sum,int s,int e):s(s),e(e),sum(sum),lt(nullptr),rt(nullptr){}
        };
        Node* root;
        Node* buildtree(vector<int> &nums,int l,int r)
        {
            if(l>r) return nullptr;
            if(l==r) return new Node(nums[l],l,l);
            int mid=(l+r)>>1;
            Node* root=new Node(0,l,r);
            Node* lc=buildtree(nums,l,mid);
            Node* rc=buildtree(nums,mid+1,r);
            if(lc) root->lt=lc,root->sum=(root->sum+lc->sum)%mod;
            if(rc) root->rt=rc,root->sum=(root->sum+rc->sum)%mod;
            return root;
        }
        void init(vector<int>nums)
        {
            root=buildtree(nums,0,nums.size()-1);
            return;
        }
        void taglazy(Node* root,int val)
        {
            if(root==nullptr) return;
			val%=mod;
            root->lazy=(root->lazy+val)%mod;
            root->sum=(root->sum+(root->e-root->s+1)%mod*val)%mod;
        }
        void pushdown(Node* root)
        {
            if(!root) return ;
            if(root->lazy)
            {
                taglazy(root->lt,root->lazy);
                taglazy(root->rt,root->lazy);
                root->lazy=0;
            }
        }
        void update(Node* root,int l,int r,int val)
        {
            if(!root) return ;
            if(root->s>r||root->e<l) return ;
            if(root->s>=l&&root->e<=r)
            {
                taglazy(root,val);
                return;
            }
            pushdown(root);
            update(root->lt,l,r,val);
            update(root->rt,l,r,val);
            root->sum=((root->lt?root->lt->sum:0)+(root->rt?root->rt->sum:0))%mod;
            return ;
        }
        void update(int l,int r,int val)
        {
            update(root,l,r,val);
            return ;
        }
        int query(Node* root,int l,int r)
        {
            pushdown(root);
            if(!root) return 0;
            if(root->s>r||root->e<l) return 0;
            if(root->s>=l&&root->e<=r) return root->sum;
            return query(root->lt,l,r)+query(root->rt,l,r);
        }
        int query(int l,int r)
        {
            return query(root,l,r);
        }
};
class cutTree
{
	//树链剖分，把树剖分成若干条链，每条链上维护一个线段树
	//可以支持链上修改和查询，也可以支持树上修改和查询
	//还可以求lca
	//重链剖分有一个重要的性质：对于节点数为n的树，从任意节点向上走到根节点，经过的轻边数量不超过logn。这是因为，如果一个节点连向父节点的边是轻边，
	//就必然存在子树不小于它的兄弟节点，那么父节点对应子树的大小一定超过该节点的两倍(由dfs1可得)。每经过一条轻边，子树大小就翻倍，所以最多只能经过logn条。
	public:
		int n,tot,s;
		//s:根节点
		vector<vector<int>> tree;
		//dep:树深,fa:父节点,hson:i的重儿子,top:重链顶端,siz:子树大小,dfn:dfs序,rk:dfs序对应的节点
		SegTree seg;
		void dfs1(int u,int f)
		{
			//cntt++;cout<<cntt<<endl;
			dep[u]=dep[f]+1;//更新树深
			fa[u]=f;siz[u]=1;
			for(auto v:tree[u])
			{
				if(v==f)continue;
				dfs1(v,u);
				siz[u]+=siz[v];
				if(hson[u]==-1||siz[v]>siz[hson[u]]) hson[u]=v;
				//u的重儿子是所有子树大小最大的儿子
			}
		}
		void dfs2(int u)
		{
			dfn[u]=++tot;rk[tot]=u;
			//优先访问重儿子,保证重链顶端的dfn最小
			if(hson[u]!=-1)
			{
				top[hson[u]]=top[u];
				//重儿子的top是它所在重链的顶端
				dfs2(hson[u]);
			}
			for(auto v:tree[u])
			{
				if(v==fa[u]||v==hson[u])//跳过父节点和重儿子
				 	continue;
				top[v]=v;//轻儿子的top是自己
				dfs2(v);
			}
		}
		void init()
		{
			tot=0;
			dfs1(s,0);
			dfs2(s);
		}
		int lca(int u,int v)
		{
			while(top[u]!=top[v])//不在同一条重链上
			{
				if(dep[top[u]]<dep[top[v]])swap(u,v);
				u=fa[top[u]];
				//链头深度大的往上跳
			}
			return dep[u]<dep[v]?u:v;
		}
		int queryPath(int u,int v)
		{
			int ans=0;
			while(top[u]!=top[v])//遍历所有的边
			{
				if(dep[top[u]]<dep[top[v]])swap(u,v);
				ans=(ans+seg.query(dfn[top[u]],dfn[u]))%mod;
				u=fa[top[u]];
			}
			if(dep[u]>dep[v])swap(u,v);
			ans=(ans+seg.query(dfn[u],dfn[v]))%mod;
			return ans;
		}
		void updatePath(int u,int v,int val)
		{
			while(top[u]!=top[v])
			{
				if(dep[top[u]]<dep[top[v]])swap(u,v);
				seg.update(dfn[top[u]],dfn[u],val);
				u=fa[top[u]];
			}
			if(dep[u]>dep[v])swap(u,v);
			seg.update(dfn[u],dfn[v],val);
		}
		void updateSub(int u,int val)
		{
			//子树的dfn一定是连续的
			seg.update(dfn[u],dfn[u]+siz[u]-1,val);
		}
		int querySub(int u)
		{
			return seg.query(dfn[u],dfn[u]+siz[u]-1);
		}
	cutTree(int n,vector<vector<int>> tree,int s):n(n),tree(tree),s(s)
	{
		for(int i=0;i<=n;i++)
		{
			dep[i]=0;fa[i]=-1;hson[i]=-1;top[i]=-1;
			siz[i]=0;dfn[i]=-1;rk[i]=-1;
		}
		top[s]=s;init(); vector<int> inf(n+1,0);
		for(int i=1;i<=n;i++)inf[dfn[i]]=val[i]%mod;
		seg.init(inf);
	}
};
signed main()
{
	int T_start=clock();
	ios::sync_with_stdio(false);
	cin.tie(0);
	//freopen("in.txt","r",stdin);
	//freopen("out.txt","w",stdout);
	int n,m,s;
	cin>>n>>m>>s>>mod;
	vector<vector<int>> tree(n+1);
	for(int i=1;i<=n;i++)
	{
		cin>>val[i];
	}
	for(int i=1;i<n;i++)
	{
		int u,v;
		cin>>u>>v;
		tree[u].push_back(v);
		tree[v].push_back(u);
	}
	cutTree ct(n,tree,s);
	while(m--)
	{
		int op,x,y,z;
		cin>>op;
		if(op==1)
		{
			cin>>x>>y>>z;
			ct.updatePath(x,y,z);
		}
		else if(op==2)
		{
			cin>>x>>y;
			cout<<ct.queryPath(x,y)%mod<<endl;
		}
		else if(op==3)
		{
			cin>>x>>y;
			ct.updateSub(x,y);
		}
		else if(op==4)
		{
			cin>>x;
			cout<<ct.querySub(x)%mod<<endl;
		}
	}
	int T_end=clock();
	//cout<<"time: "<<(double)(T_end-T_start)/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}