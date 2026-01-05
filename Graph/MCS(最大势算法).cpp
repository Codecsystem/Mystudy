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
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
class MCS{
public:
    vector<vector<int>> mp;
    int n,mxlab,tim;
    int omg,alp;//最大团/色数+最大独立集
    vector<int> lab,vis,peo,mark,rk;
    vector<vector<int>> st,chklis;
    MCS(int n,vector<vector<int>>& mp):
        n(n),vis(n+1,0),lab(n+1,0),mp(mp),
        mxlab(0),peo(n+1,0),tim(0),chklis(n+1),
        mark(n+1,0),rk(n+1,0),omg(0),alp(0),st(n+1){
            mcs();
    }
    void mcs(){
        st[0].resize(n);
        iota(st[0].begin(),st[0].end(),1);
        for(int i=n;i>=1;i--){
            int u=-1;
            while(true){
                while(mxlab>0&&st[mxlab].empty()){
                    mxlab--;
                }
                int cur=st[mxlab].back();
                st[mxlab].pop_back();
                if(!vis[cur]){
                    u=cur;
                    break;
                }
            }
            peo[i]=u,vis[u]=1,rk[u]=i;
            omg=max(omg,lab[u]+1);
            for(auto v:mp[u]){
                if(!vis[v]){
                    lab[v]++;
                    st[lab[v]].push_back(v);
                    mxlab=max(mxlab,lab[v]);
                }
            }
        }
    }
    bool chk(){
        for(int u=1;u<=n;u++){
            int p=0,minrk=n+1;
            for(auto v:mp[u]){
                if(rk[v]>rk[u]&&rk[v]<minrk){
                    minrk=rk[v];
                    p=v;
                }
            }
            if(p){
                chklis[p].push_back(u);
            }
        }
        for(int p=1;p<=n;p++){
            if(chklis[p].empty()) continue;
            tim++;
            for(auto v:mp[p]){
                mark[v]=tim;
            }
            for(auto u:chklis[p]){
                for(auto v:mp[u]){
                    if(rk[v]>rk[u]){
                        if(p==v) continue;
                        if(mark[v]!=tim){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    void getalp(){
        fill(vis.begin(),vis.end(),0);
        for(int i=1;i<=n;i++){
            int u=peo[i];
            if(!vis[u]){
                alp++;
                vis[u]=1;
                for(auto v:mp[u]){
                    vis[v]=1;
                }
            }
        }
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    while(t--){
        int n,m;cin>>n>>m;
        vector<vector<int>> mp(n+1);
        for(int i=1;i<=m;i++){
            int u,v;cin>>u>>v;
            mp[u].push_back(v);
            mp[v].push_back(u);
        }
        MCS mcs(n,mp);
        if(mcs.chk()){
            cout<<"Yes"<<endl;
            mcs.getalp();
            for(int i=1;i<=n;i++){
                cout<<mcs.peo[i]<<" ";
            }
            cout<<endl;
            cout<<mcs.omg<<' '<<mcs.omg<<' '<<mcs.alp<<'\n';
        }
        else cout<<"No"<<endl;
    }
    return 0;
}