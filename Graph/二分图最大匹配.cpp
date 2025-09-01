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
class maxflow{
    public:
        struct node
        {
            int to,cap,id;
        };
        vector<vector<node>> mp;
        vector<int> dep,cur;//dep:层次图，cur:当前弧优化
        int n,m,s,t;
        maxflow(int n,int m,int s,int t,vector<array<int,3>>& eds):
        mp(n+1),n(n),m(m),s(s),t(t),dep(n+1),cur(n+1){
            //u->v capacity
            for(auto [u,v,cap]:eds){
                int uid=mp[u].size();
                int vid=mp[v].size();
                mp[u].push_back({v,cap,vid});
                mp[v].push_back({u,0,uid});
                //建反边
            }
        }
        bool bfs(){
            fill(dep.begin(),dep.end(),-1);
            fill(cur.begin(),cur.end(),0);
            queue<int> q;
            q.push(s);
            dep[s]=0;
            while(!q.empty()){
                int u=q.front();
                q.pop();
                for(auto [v,cap,id]:mp[u]){
                    if(cap>0&&dep[v]==-1){
                        dep[v]=dep[u]+1;
                        q.push(v);
                    }
                }
            }
            return dep[t]!=-1;
        }
        int dfs(int u,int lim)//到u点的最大流量lim
        {
            if(u==t) return lim;
            int sum=0;//u点流出的流量
            for(int &i=cur[u];i<mp[u].size();i++){
                //当前弧优化,考虑u->v有重边,那么这个优化会使
                //被榨干过的v的出边不再被访问
                auto [v,cap,id]=mp[u][i];
                if(cap>0&&dep[v]==dep[u]+1){
                    int f=dfs(v,min(lim,cap));
                    mp[u][i].cap-=f;
                    mp[v][id].cap+=f;
                    sum+=f;
                    lim-=f;
                    if(lim==0) break;
                }
            }
            if(sum==0) dep[u]=-1;//无增广路
            return sum;
        }
        int dinic(){
            int res=0;
            while(bfs()){
                res+=dfs(s,INT_MAX);
            }
            return res;
        }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m,e;cin>>n>>m>>e;
    vector<array<int,3>> eds;
    int s=n+m+1,t=n+m+2;
    //s->left cap 1
    for(int i=1;i<=n;i++){
        eds.push_back({s,i,1});
    }
    //right->t cap 1
    for(int i=1;i<=m;i++){
        eds.push_back({i+n,t,1});
    }
    //u->v cap 1
    for(int i=1;i<=e;i++){
        int u,v;cin>>u>>v;
        eds.push_back({u,v+n,1});
    }
    maxflow mf(n+m+2,e,s,t,eds);
    cout<<mf.dinic()<<endl;
    return 0;
}
//二分图最大匹配 最大流O(n^1/2*m)
//二分图的划分可以用二分图染色进行
//二分图最大匹配：设有若干男生,若干女生,若干配对关系,求最大匹配,即求出最多的配对关系

//二分图最小点覆盖：在一张无向图中选择最少的顶点，满足每条边至少有一个端点被选
//->二分图中，最小点覆盖中的顶点数量等于最大匹配中的边数量。
//从网络流的角度看，最小点覆盖问题就是最小割问题：选择左部点，相当于切割它与源点的连边；选择右部点，相当于切割它与汇点的连边。
//why?因为一条边被割掉，意味着原二分图上这个点的配对点无法跑一条流->最小割
//[引] 最小割：把图分为s集和t集 s->t的边为割边，割边的最小权值和为最小割

//最大独立集问题：在一张无向图中选择最多的顶点，满足两两之间互不相邻。
//->二分图中，最大独立集中的顶点数量等于n-最小点覆盖中的顶点数量
//[引理1] 图G(V,E)中v的子集s为点覆盖<=>v/s为独立集
//证明：s为点覆盖=>v/s为独立集 假设v/s不是独立集，则存在v1,v2∈v/s，v1,v2相邻，但是v1-v2这条边的两个端点都不在s中 ->矛盾
//v/s为独立集=>s为点覆盖 要证 对所有u-v∈E，至少有一个端点在s中
//假设u-v都不在s中，则u,v都在v/s中 但此时v/s不是独立集 ->矛盾
//[推论1] 图G(V,E)中v的子集s为最大点覆盖<=>v/s为最小独立集

//有向无环图最小路径覆盖：在一张有向图中，选择最少数量的简单路径，使得所有顶点都恰好出现在一条路径中。
//->有向无环图的最小路径覆盖数等于顶点数减去最大匹配数
//通过dag构造的二分图如下：
//将每个顶点拆成两个顶点，v_in v_out
//对于原图中的每条有向边u->v，在二分图中连边v_in-u_out
//此证明为构造性的：二分图的每个匹配对应这dag中不交的各个路径
//考虑最极端的平凡图 n个点 路径是n条 增加一个匹配->路径数减少1
//Q.E.D