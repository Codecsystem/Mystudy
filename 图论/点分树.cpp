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
#include <chrono>
#include <random>
#include <cstdint>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
class DivTree{
public:
    vector<vector<int>> tr,ntr;
    vector<int> sz,vis,fa;
    int n,frt;
    DivTree(int n,const vector<vector<int>>& tr):
    n(n),sz(n+1,0),vis(n+1,0),fa(n+1,0),tr(tr),ntr(n+1){
        build();
    }
    int getsz(int u,int p){
        sz[u]=1;
        for(auto v:tr[u]){
            if(v==p||vis[v]) continue;
            sz[u]+=getsz(v,u);
        }
        return sz[u];
    }
    int getrt(int u,int p,int szrt){
        for(auto v:tr[u]){
            if(v==p||vis[v]) continue;
            if(sz[v]>szrt/2) return getrt(v,u,szrt);
        }
        return u;
    }
    void div(int u){
        vis[u]=1;
        for(auto v:tr[u]){
            if(vis[v]) continue;
            getsz(v,0);
            int subrt=getrt(v,0,sz[v]);
            fa[subrt]=u;
            ntr[u].push_back(subrt);
            ntr[subrt].push_back(u);
            div(subrt);
        }
    }
    void build(){
        getsz(1,0);
        frt=getrt(1,0,sz[1]);
        div(frt);
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}
//点分树，处理一类树上带修路径问题
//把点分治的分治重心连成一条树，这样就可以用点分治的思路处理树上带修路径问题了
//如果在点分树上，$x$ 是 $u$ 和 $v$ 的最近公共祖先（LCA），那么 $x$ 一定在原树 $u$ 到 $v$ 的路径上。
//换句话说 在点分树上以x子树的不同分支的点对 一一对应 以x为最早分治中心的那些路径
//换句话说 点分树 把所有路径做了如下划分：按路径上作为分治中心最早的那个点进行分类
//换句话说 点分树 从u向上跳到根 等价于 访问了所有以u为端点的路径
//注意：在点分树上，当你从 $u$ 跳到祖先 $x$，并利用 $x$ 的信息去计算 $u$ 到其他分支的路径时，
//你会多算“和 $u$ 在 $x$ 的同一个子树分支里的点”，可以容斥掉或者用答案不劣规避掉
