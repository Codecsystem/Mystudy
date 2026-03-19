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
class SegTree{
    public:
        pair<int,int> error={-0x3f3f3f3f,0x3f3f3f3f};
        struct Node{
            int start;int end;
            int high;int low;
            int lazy=0;
            Node* left;
            Node* right;
            Node(int start,int end,int high,int low)
                :start(start),end(end),high(high),
                low(low),left(nullptr),right(nullptr){}
        };
        Node* root;
        Node* Build(vector<int>& nums,int start,int end)
        {
            if(start>end) return nullptr;
            if(start==end) return new Node(start,end,nums[start],nums[start]);
            int mid=(start+end)>>1;
            Node* root=new Node(start,end,-0x3f3f3f3f,0x3f3f3f3f);
            Node* leftchild=Build(nums,start,mid);
            Node* rightchild=Build(nums,mid+1,end);
            if(leftchild) 
            {
                root->left=leftchild;
                root->high=max(root->high,leftchild->high);
                root->low=min(root->low,leftchild->low);
            }
            if(rightchild)
            {
                root->right=rightchild;
                root->high=max(root->high,rightchild->high);
                root->low=min(root->low,rightchild->low);
            }
            return root;
        }
        void init(vector<int>& nums)
        {
            root=Build(nums,0,nums.size()-1);
            return ;
        }
        
        void taglazy(Node* root,int val)
        {
            if(root==nullptr) return ;
            root->low+=val,root->high+=val;
            root->lazy+=val;
        }
        void pushdown(Node* root)
        {
            if(root==nullptr) return ;
            if(root->lazy!=0)
            {
                taglazy(root->left,root->lazy);
                taglazy(root->right,root->lazy);
                root->lazy=0;
            }
        }
        void update(Node* root,int l,int r,int val)
        {
            if(root==nullptr) return;
            if(root->end<l||root->start>r) return ;
            if(root->start>=l&&root->end<=r) 
            {
                taglazy(root,val);
                return;
            }
            pushdown(root);
            update(root->left,l,r,val);
            update(root->right,l,r,val);
            if(root->left) 
            {
                root->high=max(root->left->high,root->high);
                root->low=min(root->left->low,root->low);
            }
            if(root->right)
            {
                root->high=max(root->right->high,root->high);
                root->low=min(root->right->low,root->low);
            }
            return ;
        }
        void update(int l,int r,int val) {update(root,l,r,val);}
        pair<int,int> query(Node* root,int l,int r)
        {
            pushdown(root);
            if(root==nullptr) return error;
            if(root->end<l||root->start>r) return error;
            if(root->start>=l&&root->end<=r) return {root->high,root->low};
            int tpmax=-0x3f3f3f3f,tpmin=0x3f3f3f3f;
            if(root->left) 
            {
                pair<int,int> tp1=query(root->left,l,r);
                tpmax=max(tp1.first,tpmax);
                tpmin=min(tp1.second,tpmin);
            }
            if(root->right)
            {
                pair<int,int> tp2=query(root->right,l,r);
                tpmax=max(tp2.first,tpmax);
                tpmin=min(tp2.second,tpmin);
            }
            return {tpmax,tpmin};
        }
        pair<int,int> query(int l,int r) {return query(root,l,r);}
};
int main()
{
    
    return 0;
}