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
class PAM{
    public:
    vector<vector<int>> ch; 
    vector<int> fail,len,cnt; 
    int sz,last,idx; string s; 
    //PAM(回文树/回文自动机)
    //是这样的东西 0:偶根,代表偶数长度的回文串 1:奇根,代表奇数长度的回文串
    //一个点代表的回文串是 从他出发到根，又回到他的路径上的所有字符(若是奇数长度的回文串,与根节点相邻的边只走一次)
    //fail:节点代表串的最长真回文后缀
    //len:节点代表串的长度
    //cnt:节点代表串的出现次数(需要bulid一次统计)
    //编号>=2的点是真实节点
    PAM(int maxn):ch(maxn+5,vector<int>(26,0)),
    fail(maxn+5,0),len(maxn+5,0),cnt(maxn+5,0){
        sz=1,last=0,idx=0;
        fail[0]=1,fail[1]=1;
        len[0]=0,len[1]=-1; 
        s="$"; 
    }
    //找到一个形如 s[i-len[x]-1]+x+s[i] 的回文串
    int get_fail(int x,int i){
        while(s[i]!=s[i-len[x]-1]) x=fail[x];
        return x;
    }
    void insert(string str){
        //str 0-based
        for(auto x:str){
            s+=x;idx++;
            int c=x-'a',p=get_fail(last,idx);
            //没有这样的转移边,新建节点
            if(!ch[p][c]){
                //此处可以算某个本质不同回文串的首次出现下标
                //或者以该下标结尾的回文串的个数(即该节点对应的fail树的深度),开个num,用fail[u]更新即可
                int u=++sz;
                fail[u]=ch[get_fail(fail[p],idx)][c];
                len[u]=len[p]+2,ch[p][c]=u;
            }
            last=ch[p][c],cnt[last]++;
            //记录当前本质不同回文串的出现次数
        }
    }
    //构建/合并fail树上的信息(如统计所有回文子串的真实出现频次)
    void build(){
        //拓扑序直接按节点编号倒序即可，因为sz越大的节点一定越晚建立，深度越大
        for(int i=sz;i>=2;i--){
            cnt[fail[i]]+=cnt[i];
        }
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
//PAM 处理回文串结构的数据结构，时间复杂度O(n),通过势能分析能做到,不表。