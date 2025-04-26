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
#include <unordered_set>
#include <numeric>
using namespace std;

int main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int n,m;cin>>n>>m;
    vector<vector<pair<int,int>>> tr(n+1);
    for(int i=1;i<n;i++)
    {
        int u,v,w;
        cin>>u>>v>>w;
        tr[u].push_back({v,w});
        tr[v].push_back({u,w});
    }
    vector<int> siz(n+1,0),q(m+1),vis(n+1,0),ans(m+1,0);
    for(int i=1;i<=m;i++) cin>>q[i];
    auto getsz=[&](auto getsz,int u,int p=-1)->int
    {
        siz[u]=1;
        for(auto [v,w]:tr[u])
        {
            if(v==p||vis[v])continue;
            siz[u]+=getsz(getsz,v,u);
        }
        return siz[u];
    };//统计以u为根的子树大小
    auto getrt=[&](auto getrt,int u,int p=-1,int sizrt)->int
    {
        for(auto [v,w]:tr[u])
        {
            if(v==p||vis[v])continue;
            if(siz[v]>sizrt/2)return getrt(getrt,v,u,sizrt);
        }
        return u;
    };//寻找重心
    //重心：对于一棵树，如果存在一个顶点，其子树中最大的子树大小不超过整棵树大小的一半，则称该顶点为这棵树的重心。
    auto clac=[&](auto clac,int uu,int dis,int p=-1,vector<int>& tpd)->void
    {
        tpd.push_back(dis);
        for(auto [vv,ww]:tr[uu])
        {
            if(vv==p||vis[vv])continue;
            clac(clac,vv,dis+ww,uu,tpd);
        }
    };
    auto div=[&](auto div,int u)->void{
        vis[u]=1;
        unordered_set<int> s{0};
        for(auto [v,w]:tr[u])
        {
            if(vis[v])continue;
            vector<int> tpd;
            clac(clac,v,w,u,tpd);
            for(auto d:tpd)
            {
                for(int i=1;i<=m;i++)
                {
                    if(!ans[i]&&d<=q[i]&&s.find(q[i]-d)!=s.end())
                    {
                        ans[i]=1;
                    }
                }
            }
            for(auto d:tpd)s.insert(d);
        }
        for(auto [v,w]:tr[u])
        {
            //用重心划分u的子树
            if(vis[v])continue;
            getsz(getsz,v);
            int subrt=getrt(getrt,v,-1,siz[v]);
            div(div,subrt);
        }
    };//处理以u为根的子树
    getsz(getsz,1);
    int rt=getrt(getrt,1,-1,siz[1]);
    div(div,rt);
    for(int i=1;i<=m;i++) 
    {
        if(ans[i])cout<<"AYE\n";
        else cout<<"NAY\n";
    }
    return 0;
}
//淀粉质：把树上路径问题转化为子树分治问题
//把树按重心划分，那么树高（或树的大小）不超过n/2，递归深度不超过logn(最坏：退化为链），于是可以暴力处理子树
//根据实现方式的不同，时间复杂度可以做到O(nlogn)或O(nlog^2n)