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
//#define int ll //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
class SAM{
    public:
    vector<array<int,26>> ch;
    vector<vector<int>> tree;
    //DAG转移边,Parent树
    //parent树是指向根节点(0)的内向树
    int n,tot,last; // 根节点为0, 节点总数tot
    vector<int> len,fa,sz,fpos,is_np,id;
    //sam的每个点都代表这一个诸如此类的等价类：endpos(结束位置)集合相同的子串
    //而且到这个点的所以路径就是这个等价类里面的子串
    //这个等价类是覆盖[minlen,maxlen]的，而且每个长度有且只有一个子串
    //一个点的后缀链接指向的点的最长串是这个点的最短串的真后缀
    //一个比较显然的例子是：1<-2 1:aba,ba,a 2:caba acaba
    //父节点的endpos集合是子节点的endpos集合的无交集并集
    //O(sigma*|s|)构建
    SAM(int n):n(n),ch(2*n+5,array<int,26>{}),tree(2*n+5),
               len(2*n+5,0),fa(2*n+5,0),sz(2*n+5,0),fpos(2*n+5,0),
               is_np(2*n+5,0),id(2*n+5,0),tot(0),last(0){
                fa[0]=-1;
            }
    void extend(int c){
        int p=last,np=last=++tot;
        len[np]=len[p]+1,sz[np]=1,fpos[np]=len[np],is_np[np]=1;
        for(;~p&&!ch[p][c];p=fa[p]) ch[p][c]=np;
        if(!~p) fa[np]=0;
        else{
            int q=ch[p][c];
            if(len[q]==len[p]+1) fa[np]=q;
            else{
                int nq=++tot;
                len[nq]=len[p]+1,fpos[nq]=fpos[q];
                fa[nq]=fa[q],ch[nq]=ch[q];
                fa[q]=fa[np]=nq;
                for(;~p&&ch[p][c]==q;p=fa[p]) ch[p][c]=nq;
            }
        }
    }
    void insert(string s){for(auto c:s) extend(c-'a');}
    //按len排序天然满足拓扑序（从根到叶子)
    //O(|s|)
    void build(){
        vector<int> cnt(n+1,0);
        for(int i=0;i<=tot;i++) cnt[len[i]]++;
        for(int i=1;i<=n;i++) cnt[i]+=cnt[i-1];
        for(int i=0;i<=tot;i++) id[cnt[len[i]]--]=i;
        for(int i=tot+1;i>=2;i--) sz[fa[id[i]]]+=sz[id[i]],tree[fa[id[i]]].push_back(id[i]);
        //id[1]是根节点，id是1-based的，共tot+1个点
        //父节点的sz恰好是子节点的sz之和
    }
    //1.判定子串是否出现
    //O(|t|)
    bool check(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return false;p=ch[p][c-'a'];}
        return true;
    }
    //2.本质不同子串个数（本质不同是与位置不同相对的）
    //O(|s|)
    ll dist_sub(){
        ll ans=0;
        for(int i=1;i<=tot;i++) ans+=len[i]-len[fa[i]];
        return ans;
    }
    //3.所有本质不同子串总长度
    //O(simga*|s|)
    ll dist_sub_len(){
        vector<ll> c(tot+1,0),ans(tot+1,0);
        for(int i=tot+1;i>=1;i--){
            int u=id[i];c[u]=1;
            for(int j=0;j<26;j++) 
                if(ch[u][j]) 
                    c[u]+=c[ch[u][j]],ans[u]+=ans[ch[u][j]]+c[ch[u][j]];
        }
        return ans[0];
    }
    //4.字典序第K小的子串(T=0本质不同,T=1位置不同)
    //预处理：O(sigma*|s|),查询：O(sigma*|t|)
    string kth_sub(int T,ll k){
        //w：沿一条路径走到这个点代表的子串个数
        //本质不同就是1，位置不同就是endpos大小
        vector<ll> s(tot+1,0),w(tot+1,0);
        for(int i=tot+1;i>=2;i--) w[id[i]]=T?sz[id[i]]:1;
        w[0]=0;
        for(int i=tot+1;i>=1;i--){
            int u=id[i];s[u]=w[u];
            for(int j=0;j<26;j++) if(ch[u][j]) s[u]+=s[ch[u][j]];
        }
        //跟tire一样的思路，在sam上跑
        if(k>s[0]) return "-1";
        string res="";
        for(int u=0;k>0;){
            if(k<=w[u]) break;
            k-=w[u];
            for(int j=0;j<26;j++){
                if(int v=ch[u][j]){
                    if(k>s[v]) k-=s[v];
                    else{res+=(char)(j+'a'),u=v;break;}
                }
            }
        }
        return res;
    }
    //5.最小循环移位(初始化需insert(S+S),传原串长)
    //O(sigma*m)
    string min_cyclic(int m){
        string res="";
        for(int p=0,i=0;i<m;i++){
            for(int j=0;j<26;j++) 
                if(ch[p][j])
                {
                    res+=(char)(j+'a'),p=ch[p][j];
                    break;
                }
        }
        return res;
    }
    //6.最短未出现子串(DAG逆拓扑序DP)
    //O(sigma*|s|)
    string short_unapp(){
        vector<int> d(tot+1,1e9),nxt(tot+1,-1);
        for(int i=tot+1;i>=1;i--){
            int u=id[i];
            for(int j=0;j<26;j++) if(!ch[u][j]){d[u]=1,nxt[u]=j;break;}// 优先选字典序最小的断层
            if(d[u]==1) continue;
            for(int j=0;j<26;j++) if(d[u]>d[ch[u][j]]+1) d[u]=d[ch[u][j]]+1,nxt[u]=j;
        }
        string res="";
        for(int u=0;;u=ch[u][nxt[u]]){
            res+=(char)(nxt[u]+'a');
            if(!ch[u][nxt[u]]) break;
        }
        return res;
    }
    //7.求子串出现次数
    //就是sz[p]
    //O(|t|)
    int count(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return 0;p=ch[p][c-'a'];}
        return sz[p];
    }
    //8.子串首次出现起始位置(1base)
    //O(|t|)
    int first_pos(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return -1;p=ch[p][c-'a'];}
        return fpos[p]-s.size()+1;
    }
    //9.子串所有出现起始位置(1base)
    //找到以p为节点的子树的所有叶子节点，收集pos即可
    //O(|t|+ans)
    void _dfs_pos(int u,int len,vector<int>& res){
        if(is_np[u]) res.push_back(fpos[u]-len+1);
        for(auto v:tree[u]) _dfs_pos(v,len,res);
    }
    vector<int> all_pos(string s){
        vector<int> res;int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return res;p=ch[p][c-'a'];}
        _dfs_pos(p,s.size(),res);
        return res;
    }
    // 10.两串LCS(t在s的sam上跑匹配)
    //O(|t|)
    int lcs(string t){
        int u=0,l=0,ans=0;
        for(auto c:t){
            int x=c-'a';
            for(;u>0&&!ch[u][x];u=fa[u]) l=len[fa[u]];
            //失配，砍掉一部分前缀，往fa走
            if(ch[u][x]) u=ch[u][x],l++; //长度加一
            else u=0,l=0;//清零
            ans=max(ans,l);
        }
        return ans;
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    string s;cin>>s;
    SAM sam(s.size());
    sam.insert(s);
    sam.build();
    ll mx=0;
    for(int i=1;i<=sam.tot;i++){
        if(sam.sz[i]>=2){
            mx=max(mx,1ll*sam.len[i]*sam.sz[i]);
        }
    }
    cout<<mx<<endl;
    return 0;
}