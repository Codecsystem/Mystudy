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
class SAM{
    public:
    vector<vector<int>> ch,tree;
    int n,tot,last; // 根节点为0, 节点总数tot
    vector<int> len,fa,sz,fpos,is_np,id;
    SAM(int n):n(n),ch(2*n+5,vector<int>(26,0)),tree(2*n+5),
               len(2*n+5,0),fa(2*n+5,0),sz(2*n+5,0),fpos(2*n+5,0),
               is_np(2*n+5,0),id(2*n+5,0),tot(0),last(0){fa[0]=-1;}
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
    void build(){
        vector<int> cnt(tot+1,0);
        for(int i=0;i<=tot;i++) cnt[len[i]]++;
        for(int i=1;i<=tot;i++) cnt[i]+=cnt[i-1];
        for(int i=0;i<=tot;i++) id[cnt[len[i]]--]=i;
        for(int i=tot+1;i>=2;i--) sz[fa[id[i]]]+=sz[id[i]],tree[fa[id[i]]].push_back(id[i]);
    }
    // 1.判定子串是否出现
    bool check(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return false;p=ch[p][c-'a'];}
        return true;
    }
    // 2.不同子串个数
    long long dist_sub(){
        long long ans=0;
        for(int i=1;i<=tot;i++) ans+=len[i]-len[fa[i]];
        return ans;
    }
    // 3.所有不同子串总长度
    long long dist_sub_len(){
        vector<long long> c(tot+1,0),ans(tot+1,0);
        for(int i=tot+1;i>=1;i--){
            int u=id[i];c[u]=1;
            for(int j=0;j<26;j++) if(ch[u][j]) c[u]+=c[ch[u][j]],ans[u]+=ans[ch[u][j]]+c[ch[u][j]];
        }
        return ans[0];
    }
    // 4.字典序第K小(T=0本质不同,T=1位置不同)
    string kth_sub(int T,long long k){
        vector<long long> s(tot+1,0),w(tot+1,0);
        for(int i=tot+1;i>=2;i--) w[id[i]]=T?sz[id[i]]:1;
        w[0]=0;
        for(int i=tot+1;i>=1;i--){
            int u=id[i];s[u]=w[u];
            for(int j=0;j<26;j++) if(ch[u][j]) s[u]+=s[ch[u][j]];
        }
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
    // 5.最小循环移位(初始化需insert(S+S),传原串长)
    string min_cyclic(int m){
        string res="";
        for(int p=0,i=0;i<m;i++){
            for(int j=0;j<26;j++) if(ch[p][j]){res+=(char)(j+'a'),p=ch[p][j];break;}
        }
        return res;
    }
    // 6.最短未出现子串
    string short_unapp(){
        queue<int> q;q.push(0);
        vector<int> pre(tot+1,-1),edge(tot+1,0);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int j=0;j<26;j++){
                if(!ch[u][j]){
                    string res="";res+=(char)(j+'a');
                    for(;u!=0;u=pre[u]) res+=(char)(edge[u]+'a');
                    reverse(res.begin(),res.end());return res;
                }else if(pre[ch[u][j]]==-1){
                    pre[ch[u][j]]=u,edge[ch[u][j]]=j,q.push(ch[u][j]);
                }
            }
        }
        return "";
    }
    // 7.求出现次数
    int count(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return 0;p=ch[p][c-'a'];}
        return sz[p];
    }
    // 8.首次出现起始位置(1-indexed)
    int first_pos(string s){
        int p=0;
        for(auto c:s){if(!ch[p][c-'a']) return -1;p=ch[p][c-'a'];}
        return fpos[p]-s.size()+1;
    }
    // 9.所有出现起始位置(1-indexed)
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
    // 10.两串LCS
    int lcs(string t){
        int u=0,l=0,ans=0;
        for(auto c:t){
            int x=c-'a';
            for(;u>0&&!ch[u][x];u=fa[u]) l=len[fa[u]];
            if(ch[u][x]) u=ch[u][x],l++;
            else u=0,l=0;
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
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}