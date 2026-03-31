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
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
//有向图的情况,注意一下此处假设图的基图全联通，未考虑孤立点
//即此处只考虑处理一个联通分量的情况
class HierholzerD{
    public:
        vector<vector<int>> mp;
        vector<int> in,out,del;
        int n;
        HierholzerD(vector<vector<int>> &mp,int n):
        mp(mp),n(n),in(n+1,0),out(n+1,0),del(n+1,0){
            for(int i=1;i<=n;i++){
                for(auto &j:mp[i]){
                    out[i]++;
                    in[j]++;
                }
            }
        }
        vector<int> get(){
            int s=-1,t=-1,cnts=0,cntt=0;
            for(int i=1;i<=n;i++){
                if(out[i]==in[i]) continue;
                if(out[i]-in[i]==1) s=i,cnts++;
                else if(in[i]-out[i]==1) t=i,cntt++;
                else return {};
            }
            if(!((cnts==0&&cntt==0)||(cnts==1&&cntt==1))) return {};
            if(s==-1) s=1;
            vector<int> res;
            auto dfs=[&](this auto&& dfs,int u)->void{
                for(int &i=del[u];i<mp[u].size();){
                    int v=mp[u][i++];
                    dfs(v);
                }
                res.push_back(u);
            };
            dfs(s);
            reverse(res.begin(),res.end());
            return res;
        }
        //返回欧拉回路/欧拉路径
};
//无向图的情况,注意一下此处假设图的基图全联通，未考虑孤立点
//即此处只考虑处理一个联通分量的情况
//注意存双向边的时候存下编号，eg.u->v 编号为0，v->u编号为1
class HierholzerN{
    public:
        vector<vector<array<int,2>>> mp;
        vector<int> deg,del,vis;
        int n,m;
        HierholzerN(vector<vector<array<int,2>>> &mp,int n,int m):
        mp(mp),n(n),deg(n+1,0),del(n+1,0),vis(2*m+5){
            for(int i=1;i<=n;i++){
                deg[i]=mp[i].size();
            }
        }
        vector<int> get(){
            int s=-1,odd=0,even=0;
            for(int i=1;i<=n;i++){
                if(deg[i]&1) odd++,s=i;
                else even++;
            }
            if(!(odd==0||odd==2)) return {};
            if(s==-1) s=1;
            vector<int> res;
            auto dfs=[&](this auto&& dfs,int u)->void{
                for(int &i=del[u];i<mp[u].size();){
                    auto [v,id]=mp[u][i++];
                    if(vis[id]) continue;
                    vis[id]=vis[id^1]=1;
                    dfs(v);
                }
                res.push_back(u);
            };
            dfs(s);
            reverse(res.begin(),res.end());
            return res;
        }
        //返回欧拉回路/欧拉路径
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}