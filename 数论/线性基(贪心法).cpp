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
#include <cstdint>
#include <cassert>
#define int long long //赫赫 要不要龙龙呢
using namespace std;
class basic{
    public:
    using u64=uint64_t;
    struct node
    {
        u64 val;
        int inf;
    };
    vector<node> bas; int bit,tot;
    basic(int bit=64):bas(bit,{0,0}),bit(bit),tot(0){
        assert(1<=bit&&bit<=64);
    }
    bool ins(node x)
    {
        for(int i=bit-1;i>=0;i--)
        {
            if(x.val>>i&1)
            {
                if(bas[i].val==0)
                {
                    bas[i]=x;
                    tot++;
                    return true;
                }
                else x.val^=bas[i].val;
            }
        }
        return false;
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;cin>>n;
    vector<basic::node> a(n);
    for(auto& [x,y]:a) cin>>x>>y;
    sort(a.begin(),a.end(),[](auto& x,auto& y){return x.inf>y.inf;});
    basic b(64);
    int ans=0;
    for(auto& [x,y]:a)
    {
        if(b.ins({x,y})) ans+=y;
    }
    cout<<ans<<endl;
    return 0;
}
//贪心法构造的线性基：
//按照元素顺序构造，适用于依赖元素顺序的题
