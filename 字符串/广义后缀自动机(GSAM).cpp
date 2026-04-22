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
class Trie{
    public:
    vector<array<int,26>> ch;
    int tot;
    // len:预估最大节点数
    Trie(int len):ch(len+5,array<int,26>{}),tot(0){}
    void insert(string s){
        int p=0;
        for(auto c:s){
            if(!ch[p][c-'a']) ch[p][c-'a']=++tot;
            p=ch[p][c-'a'];
        }
    }
};

class GSAM{
    public:
    vector<array<int,26>> ch;
    vector<vector<int>> tree;
    int tot;
    vector<int> len,fa,sz,id;
    // n为Trie节点数，GSAM最多2n个节点
    GSAM(int n):ch(2*n+5,array<int,26>{}),tree(2*n+5),
                len(2*n+5,0),fa(2*n+5,0),sz(2*n+5,0),id(2*n+5,0),tot(0){
        fa[0]=-1;
    }
    int extend(int c,int last){
        //如果转移边存在的情况
        if(ch[last][c]){
            int q=ch[last][c];
            if(len[q]==len[last]+1) return q;
            int nq=++tot;
            len[nq]=len[last]+1,fa[nq]=fa[q],ch[nq]=ch[q];
            fa[q]=nq;
            for(int p=last;~p&&ch[p][c]==q;p=fa[p]) ch[p][c]=nq;
            return nq;
        }
        //如果转移边不存在的情况，与普通sam一致
        int np=++tot,p=last;
        len[np]=len[p]+1,sz[np]=1;
        for(;~p&&!ch[p][c];p=fa[p]) ch[p][c]=np;
        if(!~p) fa[np]=0;
        else{
            int q=ch[p][c];
            if(len[q]==len[p]+1) fa[np]=q;
            else{
                int nq=++tot;
                len[nq]=len[p]+1,fa[nq]=fa[q],ch[nq]=ch[q];
                fa[q]=fa[np]=nq;
                for(;~p&&ch[p][c]==q;p=fa[p]) ch[p][c]=nq;
            }
        }
        return np;
    }
    // 传入建好的Trie跑BFS建机，保证len拓扑序严格递增
    // 如果不想搞tire可以自己建树然后跑bfs,空间复杂度可以少个simga
    // 当然，dfs也是可行的
    void insert(const Trie& tr){
        queue<pair<int,int>> q;
        q.push({0,0});
        while(!q.empty()){
            auto [tu,su]=q.front();q.pop();
            for(int c=0;c<26;c++)
                if(tr.ch[tu][c]) q.push({tr.ch[tu][c],extend(c,su)});
        }
    }
    void build(){
        vector<int> cnt(tot+1,0);
        for(int i=0;i<=tot;i++) cnt[len[i]]++;
        for(int i=1;i<=tot;i++) cnt[i]+=cnt[i-1];
        for(int i=0;i<=tot;i++) id[cnt[len[i]]--]=i;
        for(int i=tot+1;i>=2;i--) sz[fa[id[i]]]+=sz[id[i]],tree[fa[id[i]]].push_back(id[i]);
    }
    ll dist_sub(){
        ll ans=0;
        for(int i=1;i<=tot;i++) ans+=len[i]-len[fa[i]];
        return ans;
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
//GSAM:描述tire的子串性质。