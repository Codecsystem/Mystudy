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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n;
    vector<int> sz(n+5,0),hson(n+5,0),fa(n+5,0);
    vector<int> dfn(n+5,0),id(n+5,0),out(n+5,0);
    //子树大小,重儿子,父节点,dfs序,dfs序对应的节点,出栈序
    vector<int> c(n+5,0),s(n+5,0),ans(n+5,0);
    vector<vector<int>> tr(n+5);
    int tot=0,cnt=0;
    for(int i=1;i<n;i++)
    {
        int u,v;cin>>u>>v;
        tr[u].push_back(v);
        tr[v].push_back(u);
    }
    for(int i=1;i<=n;i++) cin>>c[i];
    auto dfs1=[&](this auto&& dfs1,int u,int f)->void{
        dfn[u]=++tot;
        id[tot]=u,sz[u]=1;
        for(auto v:tr[u])
        {
            if(v==f) continue;
            dfs1(v,u);
            sz[u]+=sz[v];
            if(sz[v]>sz[hson[u]]) hson[u]=v;
        }
        out[u]=tot;
    };//预处理一些东西
    dfs1(1,0);
    auto dfs2=[&](this auto& dfs2,int u,int f,bool keep)->void{
        for(auto v:tr[u]) //先遍历轻儿子，不保留其对集合的影响
        {
            if(v==f||v==hson[u]) continue;
            dfs2(v,u,0);
        }
        if(hson[u]) dfs2(hson[u],u,1);// 然后遍历重儿子，保留其对集合的影响
        if(!s[c[u]]) ++cnt,s[c[u]]=1; // 加入根结点对集合的贡献
        for(auto v:tr[u])
        {
            if(v==f||v==hson[u]) continue;
            for(int i=dfn[v];i<=out[v];i++) //遍历轻儿子的子树
            {
                int x=id[i];
                if(!s[c[x]]) ++cnt,s[c[x]]=1; //加入轻儿子的贡献
            }
        }
        ans[u]=cnt;
        if(!keep) //如果当前节点不是重儿子，则撤销当前节点的贡献
        {
            for(int i=dfn[u];i<=out[u];i++)
            {
                int x=id[i];
                s[c[x]]=0;
            }
            cnt=0;
        }
    };
    dfs2(1,0,1);
    cin>>m;
    for(int i=1;i<=m;i++)
    {
        int x;cin>>x;
        cout<<ans[x]<<'\n';
    }
    return 0;
}
//树上启发式合并(dsu on tree)
//时间复杂度O(nlogn)
//考虑将树上的问题转化为集合合并信息的问题
//想到子树，就想到dsu on tree
//考虑把小集合合并到大集合里，这样小集合的大小至少变成原来的两倍，这样合并的次数就变少了
//当然可以不用dsu on tree 来暴力合并，不过要一些数据结构支持/时空复杂度会多一个log
//这边加上-撤销贡献的操作是为了保证空间复杂度
//同时保证了此时s数组是空的，所以不会影响后续的合并操作
//注意：这里撤销贡献的操作是必须的，如果不撤销贡献，空间复杂度会退化到O(n^2)
//考虑时间复杂度证明
//考虑一个节点被作为轻儿子做出贡献的次数
//实际上就转化为从该节点到根节点路径上的轻边数量
//why?路径上有轻边意味着该节点作为轻儿子的子树被合并到该节点上
//撤销操作和合并操作是互反的
//所以轻边数量就是该节点被作为轻儿子做出贡献的次数
//根据HLD的结论：该节点到根节点路径上的轻边数量<logn
//所以时间复杂度是O(nlogn),Q.E.D