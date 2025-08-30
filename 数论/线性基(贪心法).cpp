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
#define int long long //赫赫 要不要龙龙呢
using namespace std;
class basic{
    public:
    struct node
    {
        int val;
        int inf;
    };
    vector<node> bas; int tot;
    basic(int bit):bas(bit,{0,0}),tot(0){}
    void ins(node x)
    {
        tot++;
        for(int i=63;i>=0;i--)
        {
            if(x.val>>i&1)
            {
                if(bas[i].val==0)
                {
                    bas[i]=x;
                    return;
                }
                else x.val^=bas[i].val;
            }
        }
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;cin>>n;
    vector<pair<int,int>> a(n);
    for(auto& [x,y]:a) cin>>x>>y;
    sort(a.begin(),a.end(),[](auto& x,auto& y){return x.second>y.second;});
    basic b(64);
    for(auto& [x,y]:a)
    {
        b.ins({x,y});
    }
    int ans=0;
    for(auto [x,y]:b.bas)
    {
        ans+=y;
    }
    cout<<ans<<endl;
    return 0;
}
//贪心法构造的线性基：
//按照元素顺序构造，适用于依赖元素顺序的题