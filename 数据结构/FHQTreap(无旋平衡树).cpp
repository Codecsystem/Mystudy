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
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
class FHQTreap{
    //无旋Treap：1.满足二叉搜索树性质(val) 2.满足堆性质（优先级）
    //树堆：BST+Heap
    public:
        struct Node{
            ll val,sum,tag;      
            int sz,rnd,l,r,id;
        };
        vector<Node> tr;
        int rt,tot;

        bool cmp(ll a,ll b){
            return a<=b;
        }

        FHQTreap(int n){
            srand(time(0));
            tr.resize(n+5); //预分配空间
            tr[0]={0,0,0,0,0,0,0};
            rt=tot=0;
        }

        int nw(ll val,int id=0){
            tot++;
            tr[tot]={val,val,0,1,(int)rng(),0,0,id};
            return tot;
        }

        void pushup(int u){
            tr[u].sz=1;
            tr[u].sum=tr[u].val;
            if(tr[u].l){
                tr[u].sz+=tr[tr[u].l].sz;
                tr[u].sum+=tr[tr[u].l].sum;
            }
            if(tr[u].r){
                tr[u].sz+=tr[tr[u].r].sz;
                tr[u].sum+=tr[tr[u].r].sum;
            }
        }

        void pushdown(int u){
            if(tr[u].tag){
                ll t=tr[u].tag;
                if(tr[u].l){
                    tr[tr[u].l].val+=t;
                    tr[tr[u].l].sum+=t*tr[tr[u].l].sz;
                    tr[tr[u].l].tag+=t;
                }
                if(tr[u].r){
                    tr[tr[u].r].val+=t;
                    tr[tr[u].r].sum+=t*tr[tr[u].r].sz;
                    tr[tr[u].r].tag+=t;
                }
                tr[u].tag=0;
            }
        }

        void merge(int &u,int a,int b){
            //val a<=val b(内部满足Treap)
            if(!a||!b){
                u=a|b;
                return;
            }
            pushdown(a); pushdown(b); //下传标记
            if(tr[a].rnd>tr[b].rnd){//a的优先级大
                u=a;//a作为根(为了满足Heap(大))
                merge(tr[a].r,tr[a].r,b);//b合并到a的右子树（为了满足BST：a的右子树的所有节点都大于a)
            }
            else{
                u=b;
                merge(tr[b].l,a,tr[b].l);
            }
            pushup(u);
        }

        void split(int u,int &a,int &b,ll val){
            //将root按照val分割为a,b两部分
            //a的val都小于等于val，b的val都大于val
            if(!u){
                a=b=0;
                return;
            }
            pushdown(u); //下传标记
            if(cmp(tr[u].val,val)){
                a=u;
                split(tr[u].r,tr[a].r,b,val);
            }
            else{
                b=u;
                split(tr[u].l,a,tr[b].l,val);
            }
            pushup(u);
        }

        void insert(ll val,int id=0){
            int a,b;
            split(rt,a,b,val);//将root按照val分割为a,b两部分
            int node=nw(val,id);
            merge(a,a,node);//将val插入到a中
            merge(rt,a,b);//将a,b合并为root
            //偶还能这样
        }

        void erase(ll val){
            int a,b,c;
            split(rt,a,b,val);//将root按照val分割为a,b两部分
            split(a,a,c,val-1);//将a按照val-1分割为a,c两部分
            if(c)
            {
                merge(a,a,tr[c].r);//将c的右子树合并到a中(删除一个节点)
                merge(a,a,tr[c].l);//将c的左子树合并到a中(删除一个节点)
            }
            merge(rt,a,b);//将a,b合并为root
        }

        //对>=lim的节点进行加值操作
        void range_add(ll lim,ll add){
            int x,y;
            split(rt,x,y,lim-1); 
            if(y){
                tr[y].val+=add;
                tr[y].tag+=add;
            }
            merge(rt,x,y);
        }

        void print(int u){
            if(!u)return;
            pushdown(u);
            print(tr[u].l);
            cout<<tr[u].val<<" ";
            print(tr[u].r);
        }
        ll findMax(int u){
            if(!u)return -1;
            pushdown(u);
            while(tr[u].r){
                u=tr[u].r;
                pushdown(u);
            }
            return tr[u].val;
        }
        ll findMin(int u){
            if(!u)return -1;
            pushdown(u);
            while(tr[u].l){
                u=tr[u].l;
                pushdown(u);
            }
            return tr[u].val;
        }
        ll pre(ll val){
            int a,b;
            split(rt,a,b,val-1);//将root按照val-1分割为a,b两部分
            ll res=findMax(a);
            merge(rt,a,b);
            return res;
        }
        ll next(ll val){
            int a,b;
            split(rt,a,b,val);//将root按照val分割为a,b两部分
            ll res=findMin(b);
            merge(rt,a,b);
            return res;
        }
        int rank(ll val){
            int a,b;
            split(rt,a,b,val-1);//将root按照val-1分割为a,b两部分
            int res=(a?tr[a].sz:0)+1;
            merge(rt,a,b);
            return res;
        }
        ll QueryKth(int k){
            return KthQuery(rt,k);
        }
        ll KthQuery(int u,int k){
            if(u==0) return -1;
            pushdown(u);
            int lsz=tr[u].l?tr[tr[u].l].sz:0;
            if(k<=lsz) return KthQuery(tr[u].l,k);
            else if(k==lsz+1) return tr[u].val;
            else return KthQuery(tr[u].r,k-lsz-1);
        }
        bool find(ll val){
            int a,b;
            split(rt,a,b,val);//将root按照val分割为a,b两部分
            bool res=a&&findMax(a)==val;
            merge(rt,a,b);
            return res;
        }
        
        //中序遍历获取答案
        void get_ans(int u,vector<ll>& ans){
            if(!u) return;
            pushdown(u);
            get_ans(tr[u].l,ans);
            if(tr[u].id) ans[tr[u].id]=tr[u].val;
            get_ans(tr[u].r,ans);
        }

        vector<ll> get_ans(){
            vector<ll> ans(tot+1);
            get_ans(rt,ans);
            return ans;
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
        int n;cin>>n;
        vector<int> a(n+1);
        FHQTreap fhq(n+1);
        for(int i=1;i<=n;i++) cin>>a[i];
        for(int i=1;i<=n;i++){
            fhq.range_add(a[i],a[i]);
            fhq.insert(a[i],i);
        }
        auto ans=fhq.get_ans();
        for(int i=1;i<=n;i++) cout<<ans[i]<<" ";
        cout<<endl;
    }
    return 0;
}