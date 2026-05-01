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
class MC{
public:
    struct node{
        int to;
        int cap;
        int cost;
        int rev;
    };
    int n,s,t;
    int maxf=0,maxc=0;
    const int INF=1e9;
    vector<vector<node>> mp;
    vector<int> dis,cur,inq,vis;
    MC(int n,int s,int t,vector<array<int,4>>& eds):
    n(n),s(s),t(t),mp(n+1),dis(n+1),
    cur(n+1),inq(n+1,0),vis(n+1,0){
        for(auto [u,v,cap,w]:eds){
            int uid=mp[u].size();
            int vid=mp[v].size();
            mp[u].push_back({v,cap,w,vid});
            mp[v].push_back({u,0,-w,uid});
            //反边的费用是负的
        }
    }

    bool spfa(){
        fill(dis.begin(),dis.end(),-INF);
        fill(inq.begin(),inq.end(),0);
        deque<int> q;dis[s]=0,inq[s]=1;
        q.push_back(s);
        while(!q.empty()){
            int u=q.front();q.pop_front();
            inq[u]=0;
            for(auto [v,cap,w,rev]:mp[u]){
                if(cap>0&&dis[u]+w>dis[v]){
                    dis[v]=dis[u]+w;
                    if(!inq[v]){
                        if(!q.empty()&&dis[v]>dis[q.front()]){
                            q.push_front(v);
                        }else{
                            q.push_back(v);
                        }
                        inq[v]=1;
                    }
                }
            }
        }
        return dis[t]>0;
    }

    int dfs(int u,int f){
        if(u==t)return f;
        vis[u]=1;
        int res=0;
        for(int &i=cur[u];i<mp[u].size();i++){
            auto [v,cap,w,rev]=mp[u][i];
            if(!vis[v]&&cap>0&&dis[u]+w==dis[v]){
                int tmp=dfs(v,min(f,cap));
                f-=tmp;
                res+=tmp;
                mp[u][i].cap-=tmp;
                mp[v][rev].cap+=tmp;
                maxc+=tmp*w;
                if(!f)break;
            }
        }
        vis[u]=0;
        return res;
    }

    void dinic(){
        while(spfa()){
            fill(vis.begin(),vis.end(),0);
            fill(cur.begin(),cur.end(),0);
            maxf+=dfs(s,INF);
        }
    }

};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m,k;cin>>n>>m>>k;
    vector<vector<int>> mp(n+1,vector<int>(m+1));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            cin>>mp[i][j];
        }
    }
    auto ch=[&](int x,int y){
        return (x-1)*m+y;
    };
    auto chk=[&](int x,int y){
        return x>=1&&x<=n&&y>=1&&y<=m&&mp[x][y]!=-1;
    };
    int dx[]={0,0,1,-1};
    int dy[]={1,-1,0,0};
    vector<array<int,4>> eds;
    //拆点，入点ch(i,j) 出点ch(i,j)+n*m
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if(mp[i][j]==0) eds.push_back({ch(i,j),ch(i,j)+n*m,1,-1});
            else if(mp[i][j]==1) eds.push_back({ch(i,j),ch(i,j)+n*m,1,0});
            if(mp[i][j]!=-1){
                for(int d=0;d<4;d++){
                    int nx=i+dx[d],ny=j+dy[d];
                    if(chk(nx,ny)){
                        eds.push_back({ch(i,j)+n*m,ch(nx,ny),1,0});
                    }
                }
            }
        }
    }
    int s=2*n*m+1,t=2*n*m+2;
    for(int i=1;i<=k;i++){
        int x,y;cin>>x>>y;
        eds.push_back({s,ch(x,y),1,0});
    }
    for(int i=1;i<=k;i++){
        int x,y;cin>>x>>y;
        eds.push_back({ch(x,y)+n*m,t,1,100});
    }
    MC mc(2*n*m+2,s,t,eds);
    mc.dinic();
    cout<<mc.maxc<<endl;
    return 0;
}