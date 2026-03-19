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
#define int long long
using namespace std;
class segtree{
    public:
        struct node
        {
            int sum;
            int s;int e;
            node* lt;
            node* rt;
            int lazysum=0;
            int lazycover=0;
            node(int sum,int s,int e): 
            sum(sum),s(s),e(e),lt(nullptr),
            rt(nullptr),lazycover(0),lazysum(0){}
        };
        node* root;
        node* build(vector<int>& nums,int l,int r)
        {
            if(l>r) return nullptr;
            if(l==r) return new node(nums[l],l,l);
            int mid=(l+r)>>1;
            node* root=new node(0,l,r);
            node* lc=build(nums,l,mid);
            node* rc=build(nums,mid+1,r);
            root->lt=lc,root->rt=rc;
            root->sum=(lc?lc->sum:0)+(rc?rc->sum:0);
            return root;
        }
        void init(vector<int>& nums) {root=build(nums,0,nums.size()-1);}
        void taglazy(node* root,int val,int op){
            //1:sum,2:cover
            if(!root) return;
            if(op==1)
            {
                root->lazysum+=val;
                root->sum+=(val)*(root->e-root->s+1);
            }
            else if(op==2)
            {
                root->lazycover=val;
                root->lazysum=0;
                root->sum=val*(root->e-root->s+1);
            }
        }
        void pushdown(node* root)
        {
            if(root->lazycover){
                taglazy(root->lt,root->lazycover,2);
                taglazy(root->rt,root->lazycover,2);
                root->lazycover=0;
            }
            if(root->lazysum){
                taglazy(root->lt,root->lazysum,1);
                taglazy(root->rt,root->lazysum,1);
                root->lazysum=0;
            }
        }
        int query(node* root,int l,int r)
        {
            pushdown(root);
            if(!root) return 0;
            if(root->s>r||root->e<l) return 0;
            if(root->s>=l&&root->e<=r) return root->sum;
            return query(root->lt,l,r)+query(root->rt,l,r);
        }
        int query(int l,int r) {return query(root,l,r);}
        void update(node* root,int l,int r,int val,int op)
        {
            
            if(!root) return ;
            if(root->s>r||root->e<l) return ;
            if(root->s>=l&&root->e<=r) 
            {
                taglazy(root,val,op);
                return ;
            }
            pushdown(root);
            update(root->lt,l,r,val,op);
            update(root->rt,l,r,val,op);
            root->sum=(root->lt?root->lt->sum:0)+(root->rt?root->rt->sum:0);
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
    
    return 0;
}