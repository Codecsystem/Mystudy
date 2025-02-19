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
using namespace std;

struct SegTreeNode{
    int start;
    int end;//[start,end]
    int sum;
    int lazy=0;
    SegTreeNode *left;
    SegTreeNode *right;

    SegTreeNode(int start,int end,int sum)
        :start(start),end(end),sum(sum),left(nullptr),right(nullptr){}

};
SegTreeNode* buildTree(vector<int>& nums,int start,int end);
int query(SegTreeNode* root,int start,int end);
void applylazy(SegTreeNode* root,int val);
void pushdown(SegTreeNode* root);
void update(SegTreeNode* root,int start,int end,int val);
SegTreeNode* buildTree(vector<int>& nums,int start,int end)//建树
{
    if(start>end) return nullptr;
    if(start==end) return new SegTreeNode(start,end,nums[start]);
    int mid=(start+end)>>1;
    SegTreeNode* root=new SegTreeNode(start,end,0);
    SegTreeNode* leftChild=buildTree(nums,start,mid);
    SegTreeNode* rightChild=buildTree(nums,mid+1,end);
    if(leftChild!=nullptr) 
    {
        root->left=leftChild;
        root->sum+=leftChild->sum;
    }
    if(rightChild!=nullptr)
    {
        root->right=rightChild;
        root->sum+=rightChild->sum;
    }
    return root;
}
int query(SegTreeNode* root,int start,int end)//查询区间和
{
    if(root==nullptr) return 0;
    if(start>root->end||end<root->start) return 0;
    if(start<=root->start&&end>=root->end) return root->sum;
    return query(root->left,start,end)+query(root->right,start,end);
}
void applylazy(SegTreeNode* root,int val) //更新当前节点的值，并打上lazy标
{
    if(root==nullptr) return;
    root->sum+=val*(root->end-root->start+1);
    root->lazy+=val;
}
void pushdown(SegTreeNode* root) //将lazy值下传
{
    if(root==nullptr) return;
    if(root->lazy!=0)
    {
        applylazy(root->left,root->lazy);
        applylazy(root->right,root->lazy);
        root->lazy=0;
    }
}

void update(SegTreeNode* root,int start,int end,int val)
{
    if(root==nullptr) return;
    if(start>root->end||end<root->start) return;
    if(start<=root->start&&end>=root->end)
    {
        applylazy(root,val);
        return;
    }
    pushdown(root);
    update(root->left,start,end,val);
    update(root->right,start,end,val);
    root->sum=root->left->sum+root->right->sum;
}
int main()
{
    int T_start=clock();
    
    return 0;
}