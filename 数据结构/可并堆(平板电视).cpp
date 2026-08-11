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
#include <ext/pb_ds/priority_queue.hpp>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
using namespace __gnu_pbds;

signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,q;cin>>n>>q;
    vector<int> fa(n+1),dep(n+1,0),nfa(n+1),ndep(n+1,0);
    for(int i=2;i<=n;i++){
        int x;cin>>x;fa[i]=x,dep[i]=dep[x]+1;
    }
    auto cmp=[&](int x,int y){
        return dep[x]<dep[y];
    };
    __gnu_pbds::priority_queue<int,decltype(cmp),__gnu_pbds::pairing_heap_tag> pq(cmp);
    vector<__gnu_pbds::priority_queue<int,decltype(cmp),__gnu_pbds::pairing_heap_tag>> nq(n+1,pq);
    for(int i=1;i<=q;i++){
        int u,v;cin>>u>>v;
        nq[u].push(v);
    }
    for(int i=n;i>=2;i--){
        if(nq[i].empty()) nfa[i]=1;
        else{
            int d=nq[i].top();nq[i].pop();nfa[i]=d;
            while(!nq[i].empty()&&nq[i].top()==d){
                nq[i].pop();
            }
            nq[d].join(nq[i]);
        } 
    }
    ll ans=0;
    for(int i=2;i<=n;i++){
        ndep[i]=ndep[nfa[i]]+1;
        ans+=ndep[i];
    }
    cout<<ans<<endl;
    return 0;
}
//可并堆：pairing_heap_tag,配对堆 插入,合并均摊O(1),删除堆顶(logn)