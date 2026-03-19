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
using namespace std;
#define int long long
class segtree{
    public:
        const int H=-0xfffffffffffffff; 
        const int L=0xfffffffffffffff;
        pair<int,int> error={H,L};
        struct node{
            int s;int e;
            int lazyadd;
            int lazycover;
            int lazycoveradd;
            int high,low;
            node* lt;
            node* rt;
            node(int s,int e,int high,int low):
            s(s),e(e),lazyadd(0),lazycover(0),
            high(high),low(low),lazycoveradd(-1),
            lt(nullptr),rt(nullptr){}
        };
        node* root;
        void inf(node* a,node* l,node* r)
        {
            if(!a) return ;
            if(!l&&r) a->low=r->low,a->high=r->high;
            if(l&&!r) a->low=l->low,a->high=l->high;
            if(l&&r) a->low=min(l->low,r->low),a->high=max(l->high,r->high);
        }
        node* build(vector<int> &nums,int l,int r)
        {
            if(l>r) return nullptr;
            if(l==r) return new node(l,r,nums[l],nums[l]);
            int mid=(l+r)>>1;
            node* root=new node(l,r,H,L);
            node* lc=build(nums,l,mid);
            node* rc=build(nums,mid+1,r);
            root->lt=lc,root->rt=rc;
            inf(root,lc,rc);
            return root;
        }
        void init(vector<int> &nums){root=build(nums,0,nums.size()-1);}
        void taglazy(node* root,int val,int op)
        {
            //op 1 add,2 cover
            if(!root) return ;
            if(op==1)
            {
                root->lazyadd+=val;
                root->high+=val;
                root->low+=val;
            }
            if(op==2)
            {
                root->lazycover=val;
                root->lazycoveradd=1;
                root->high=val;
                root->low=val;
                root->lazyadd=0;
            }
        }
        void pushdown(node* root)
        {
            if(!root) return ;
            if(root->lazycoveradd==1)
            {
                taglazy(root->lt,root->lazycover,2);
                taglazy(root->rt,root->lazycover,2);
                root->lazycover=0;
                root->lazycoveradd=-1;
            }
            if(root->lazyadd)
            {
                taglazy(root->lt,root->lazyadd,1);
                taglazy(root->rt,root->lazyadd,1);
                root->lazyadd=0;
            }
        }
        pair<int,int> query(node* root,int l,int r)
        {
            pushdown(root);
            if(!root) return error;
            if(root->s>r||root->e<l) return error;
            if(l<=root->s&&root->e<=r) 
            {
                //cout<<"query:"<<root->s<<' '<<root->e<<' '<<root->high<<' '<<root->low<<endl;
                return {root->high,root->low};
            }
            pair<int,int> tplt=query(root->lt,l,r);
            pair<int,int> tprt=query(root->rt,l,r);
            int anshigh=max(tplt.first,tprt.first);
            int anslow=min(tplt.second,tprt.second);
            return {anshigh,anslow};
        }
        pair<int,int> query(int l,int r) {return query(root,l,r);}
        void update(node* root,int l,int r,int val,int op)
        {
            if(!root) return ;
            if(root->s>r||root->e<l) return ;
            if(l<=root->s&&root->e<=r)
            {
                //cout<<"taglazy:"<<root->s<<' '<<root->e<<' '<<val<<' '<<op<<endl;
                taglazy(root,val,op);
                return;
            }
            pushdown(root);
            update(root->lt,l,r,val,op);
            update(root->rt,l,r,val,op);
            inf(root,root->lt,root->rt);
            return ;
        }
        void update(int l,int r,int val,int op){
            update(root,l,r,val,op);
        }

};
int read()
{
    int s=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9')
    {
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9')
    {
        s=(s<<3)+(s<<1)+ch-'0';
        ch=getchar();
    }
    return s*f;
}
inline void write(int x) 
{
    static int sta[35]; 
    int top=0;
    if(x<0&&x!=-2147483648) {putchar('-');x=-x;}
    if(x==-2147483648) {printf("-2147483648");return;}
    do{
      sta[top++]=x%10, x/=10;
      }while(x);
    while(top) putchar(sta[--top]+48);
}
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int n=read(),m=read();
    vector<int>a(n);
    for(auto&i:a) i=read();
    segtree st;st.init(a);
    while(m--)
    {
        int op=read(),l=read(),r=read();
        if(op==1)
        {
            int x=read();
            st.update(l-1,r-1,x,2);
        }
        else if(op==2)
        {
            int x=read();
            st.update(l-1,r-1,x,1);
        }
        else printf("%lld\n",st.query(l-1,r-1).first);
        // for(int i=0;i<n;i++)
        // {
        //     cout<<st.query(i,i).first<<' ';
        // }
        // cout<<endl;
    } 
    return 0;
}