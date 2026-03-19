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
class FHQTreap{
    //无旋Treap：1.满足二叉搜索树性质(val) 2.满足堆性质（优先级）
    //树堆：BST+Heap
    public:
        struct Node{
            int l,r;
            int size=0;
            int priority;//随机数
            Node *left, *right;
            Node(int l,int r):l(l),r(r),priority(rand()),left(NULL),right(NULL),size(1){}
        };
        bool cmp(Node *a,pair<int,int> val){
            if(a->l==val.first) return a->r<=val.second;
            return a->l<val.first;
        }
        Node *root;
        FHQTreap():root(NULL){}
        void merge(Node *&root,Node *a,Node *b){
            //val a<=val b(内部满足Treap)
            if(!a)root=b;
            else if(!b)root=a;
            else{
                if(a->priority>b->priority){//a的优先级大
                    root=a;//a作为根(为了满足Heap(大))
                    merge(a->right,a->right,b);//b合并到a的右子树（为了满足BST：a的右子树的所有节点都大于a)
                }
                else{
                    root=b;
                    merge(b->left,a,b->left);
                }
            }
            if(root)
            {
                root->size=1;
                if(root->left)root->size+=root->left->size;
                if(root->right)root->size+=root->right->size;
                //cout<<root->val<<' '<<root->size<<endl;
            }
        }
        void split(Node *root,Node *&a,Node *&b,pair<int,int> val){
            //将root按照val分割为a,b两部分
            //a的val都小于等于val，b的val都大于val
            if(!root){
                a=b=NULL;
                return;
            }
            if(cmp(root,val)){
                a=root;
                split(root->right,a->right,b,val);
            }
            else{
                b=root;
                split(root->left,a,b->left,val);
            }
            if(root)
            {
                root->size=1;
                if(root->left)root->size+=root->left->size;
                if(root->right)root->size+=root->right->size;
                //cout<<root->val<<' '<<root->size<<endl;
            }
        }
        void insert(pair<int,int> val){
            Node *a,*b;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            merge(a,a,new Node(val.first,val.second));//将val插入到a中
            merge(root,a,b);//将a,b合并为root
            //偶还能这样
        }
        void erase(pair<int,int> val){
            Node *a,*b,*c;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            split(a,a,c,{val.first,val.second-1});//将a按照val-1分割为a,c两部分
            if(c)
            {
                merge(a,a,c->right);//将c的右子树合并到a中(删除一个节点)
                merge(a,a,c->left);//将c的左子树合并到a中(删除一个节点)
            }
            merge(root,a,b);//将a,b合并为root
        }
        pair<int,int> findMax(Node *root){
            if(!root)return {-1,-1};
            while(root->right)root=root->right;
            return {root->l,root->r};
        }
        pair<int,int> findMin(Node *root){
            if(!root)return {-1,-1};
            while(root->left)root=root->left;
            return {root->l,root->r};
        }
        pair<int,int> pre(pair<int,int> val){
            Node *a,*b;
            split(root,a,b,{val.first,val.second-1});//将root按照val-1分割为a,b两部分
            pair<int,int> res=findMax(a);
            merge(root,a,b);
            return res;
        }
        pair<int,int> next(pair<int,int> val){
            Node *a,*b;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            pair<int,int> res=findMin(b);
            merge(root,a,b);
            return res;
        }
		bool find(pair<int,int> val)
		{
			Node *a,*b;
			split(root,a,b,val);
			bool res=a&&findMax(a).second==val.second&&findMax(a).first==val.first;
			merge(root,a,b);
			return res;
		}
		int size()
		{
			return root?root->size:0;
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
int main()
{
	//freopen("in.txt","r",stdin);
	//freopen("out2.txt","w",stdout);
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
    int t;cin>>t;
	FHQTreap tree;
    while(t--)
    {
        char op;
		cin>>op;
		if(op=='B')
		{
			cout<<tree.size()<<'\n';
		}
		else if(op=='A')
		{
			int x,y;
			cin>>x>>y;
			auto check=[](pair<int,int> a,pair<int,int> b)->bool{
				vector<int> v1={a.first,a.second,b.first,b.second};
				sort(v1.begin(),v1.end());
				return ((v1[0]==a.first&&v1[1]==a.second&&v1[2]==b.first&&v1[3]==b.second)||(v1[0]==b.first&&v1[1]==b.second&&v1[2]==a.first&&v1[3]==a.second))&&v1[1]!=v1[2];
			};
			int ans=0;
			if(tree.find({x,y}))
			{
				tree.erase({x,y});
				ans++;
			}
			while(tree.size()&&tree.pre({x,y}).first!=-1&&!check(tree.pre({x,y}),{x,y})) 
			{
				ans++;
				tree.erase(tree.pre({x,y}));
			}
			while(tree.size()&&tree.next({x,y}).first!=-1&&!check(tree.next({x,y}),{x,y})) 
			{
				ans++;
				tree.erase(tree.next({x,y}));
			}
			cout<<ans<<'\n';
			tree.insert({x,y});
		}
    }
    return 0;
}