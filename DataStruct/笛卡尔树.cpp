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
struct DKRTreeNode{
    int val;
    int index;
    DKRTreeNode *left, *right;
    DKRTreeNode(int x,int i):val(x),index(i),left(NULL),right(NULL){}
};
//笛卡尔树具有一下性质：
//1.二叉搜索树，2.堆
//i:index,val:nums[i]
//因为BST的中序遍历一定是原序列，所以新插入的节点一定在右边
//需要调整最右边的纵向位置，使其满足堆的性质，用单调栈维护最右链
//大根堆
DKRTreeNode* buildTree(vector<int> &nums){
    stack<DKRTreeNode*> s;
    DKRTreeNode* root=nullptr;
    for(int i=0;i<nums.size();i++)
    {
        DKRTreeNode* node=new DKRTreeNode(nums[i],i);
        DKRTreeNode* last=nullptr;
        while(!s.empty()&&s.top()->val<node->val)
        {
            last=s.top();
            s.pop();
        }//单调栈维护
        if(!s.empty()) s.top()->right=node;//栈顶元素的右子树为node
        if(last) node->left=last;//栈弹出的元素为node的左子树
        s.push(node);
    }
    while(!s.empty()) root=s.top(),s.pop();//最后一个元素为根节点
    return root;
}
void printTree(DKRTreeNode* root)
{
    if(root==nullptr) return;
    printTree(root->left);
    cout<<root->val<<" ";
    printTree(root->right);
}
int main()
{
    int T_start=clock();
    vector<int> nums(10,0);
    for(int i=0;i<nums.size();i++) nums[i]=rand()%100;
    for(auto i:nums) cout<<i<<" ";
    cout<<endl;
    DKRTreeNode* root=buildTree(nums);
    printTree(root);
    cout<<root->val<<endl;
    return 0;
}
