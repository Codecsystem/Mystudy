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
            int val;
            int size=0;
            int priority;//随机数
            Node *left, *right;
            Node(int val):val(val),priority(rand()),left(NULL),right(NULL),size(1){}
            Node(int val,int priority):val(val),priority(priority),left(NULL),right(NULL),size(1){}
        };
        bool cmp(int a,int b){
            return a<=b;
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
        void split(Node *root,Node *&a,Node *&b,int val){
            //将root按照val分割为a,b两部分
            //a的val都小于等于val，b的val都大于val
            if(!root){
                a=b=NULL;
                return;
            }
            if(cmp(root->val,val)){
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
        void insert(int val){
            Node *a,*b;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            merge(a,a,new Node(val));//将val插入到a中
            merge(root,a,b);//将a,b合并为root
            //偶还能这样
        }
        void erase(int val){
            Node *a,*b,*c;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            split(a,a,c,val-1);//将a按照val-1分割为a,c两部分
            if(c)
            {
                merge(a,a,c->right);//将c的右子树合并到a中(删除一个节点)
                merge(a,a,c->left);//将c的左子树合并到a中(删除一个节点)
            }
            merge(root,a,b);//将a,b合并为root
        }
        void print(Node *root){
            if(!root)return;
            print(root->left);
            cout<<root->val<<" ";
            print(root->right);
        }
        int findMax(Node *root){
            if(!root)return -1;
            while(root->right)root=root->right;
            return root->val;
        }
        int findMin(Node *root){
            if(!root)return -1;
            while(root->left)root=root->left;
            return root->val;
        }
        int pre(int val){
            Node *a,*b;
            split(root,a,b,val-1);//将root按照val-1分割为a,b两部分
            int res=findMax(a);
            merge(root,a,b);
            return res;
        }
        int next(int val){
            Node *a,*b;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            int res=findMin(b);
            merge(root,a,b);
            return res;
        }
        int rank(int val){
            Node *a,*b;
            split(root,a,b,val-1);//将root按照val-1分割为a,b两部分
            int res=(a?a->size:0)+1;
            merge(root,a,b);
            return res;
        }
        int QueryKth(int k){
            return KthQuery(root,k);
        }
        int KthQuery(Node* root,int k){
            if(root==nullptr) return -1;
            int leftsize=root->left?root->left->size:0;
            if(k<=leftsize) return KthQuery(root->left,k);
            else if(k==leftsize+1) return root->val;
            else return KthQuery(root->right,k-leftsize-1);
        }
        bool find(int val){
            Node *a,*b;
            split(root,a,b,val);//将root按照val分割为a,b两部分
            bool res=a&&findMax(a)==val;
            merge(root,a,b);
            return res;
        }
        
};
int main()
{
    int T_start=clock();
    FHQTreap treap;
    vector<int> test={1,2,3,4,5,6,7,8,9,10};
    /*for(int i=0;i<test.size();i++){
        treap.insert(test[i]);
    }
    treap.print(treap.root);
    cout<<endl;
    treap.erase(5);
    treap.print(treap.root);
    cout<<endl;
    treap.insert(5);
    treap.insert(5);
    treap.insert(5);
    treap.insert(5);
    treap.print(treap.root);
    cout<<endl;
    treap.erase(5);
    treap.print(treap.root);
    cout<<endl;*/
    cout<<treap.pre(5)<<endl;
    cout<<treap.next(5)<<endl;
    cout<<treap.rank(5)<<endl;
    treap.erase(5);
    treap.print(treap.root);
    cout<<endl;
    treap.insert(5);
    treap.insert(5);
    treap.insert(5);
    treap.print(treap.root);
    cout<<endl;
    cout<<treap.rank(6)<<endl;
    treap.erase(5);
    treap.print(treap.root);
    cout<<endl;
    cout<<treap.rank(8)<<endl;
    cout<<treap.QueryKth(7)<<endl;
    return 0;
}