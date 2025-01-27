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
struct SBTreeNode{
    int key;
    SBTreeNode* left;
    SBTreeNode* right;
    int size; //子树大小
    int count; //重复个数

    SBTreeNode(int val):
        key(val),left(nullptr),right(nullptr),size(1),count(1){}
};
//二叉查找树具有以下性质：
//1. 若任意节点的左子树不空，则左子树上所有节点的值均小于它的根节点的值；
//2. 若任意节点的右子树不空，则右子树上所有节点的值均大于它的根节点的值；
//3. 任意节点的左、右子树也分别为二叉查找树；
int FindMin(SBTreeNode* root)
{
    if(root==nullptr) return -0x3f3f3f3f;
    while(root->left!=nullptr) root=root->left;
    return root->key;
}
int FindMax(SBTreeNode* root)
{
    if(root==nullptr) return 0x3f3f3f3f;
    while(root->right!=nullptr) root=root->right;
    return root->key;
}
SBTreeNode* FindMinNode(SBTreeNode* root)
{
    if(root==nullptr) return nullptr;
    while(root->left!=nullptr) root=root->left;
    return root;
}
void Inorder(SBTreeNode* root){
    if(root==nullptr) return;
    Inorder(root->left);
    cout<<root->key<<" ";
    Inorder(root->right);
}

bool search(SBTreeNode* root,int val){
    if(root==nullptr) return false;
    if(root->key==val) return true;
    if(val<root->key) return search(root->left,val);
    else return search(root->right,val);
}

SBTreeNode* insert(SBTreeNode* root,int val){
    if(root==nullptr) return new SBTreeNode(val);
    if(val<root->key) root->left=insert(root->left,val);
    else if(val>root->key) root->right=insert(root->right,val);
    else root->count++;
    root->size=root->count+(root->left?root->left->size:0)+(root->right?root->right->size:0);
    return root;
}
//先搜索到，再判断子树有无，再删除
SBTreeNode* deleteNode(SBTreeNode* root,int val){
    if(root==nullptr) return nullptr;
    if(val<root->key) root->left=deleteNode(root->left,val);
    else if(val>root->key) root->right=deleteNode(root->right,val);
    else{
        if(root->count>1) root->count--;
        else{
            if(root->left==nullptr) 
            {
                SBTreeNode* temp=root->right;
                delete root;
                return temp;
            }
            else if(root->right==nullptr) 
            {
                SBTreeNode* temp=root->left;
                delete root;
                return temp;
            }
            else if(root->left!=nullptr&&root->right!=nullptr)
            {
                SBTreeNode* temp=FindMinNode(root->right);//右子树最小值代替该节点
                root->key=temp->key;
                root->count=temp->count;
                temp->count=1;
                root->right=deleteNode(root->right,temp->key);
            }
        }
    }
    root->size=root->count+(root->left?root->left->size:0)+(root->right?root->right->size:0);
    return root;
}
int QueryRank(SBTreeNode* root,int val){
    if(root==nullptr) return 0;
    if(val==root->key) return (root->left?root->left->size:0)+1; //找到，返回左子树大小+1
    if(val<root->key) return QueryRank(root->left,val); //向左走
    else return QueryRank(root->right,val)+(root->left?root->left->size:0)+root->count; //向右走，加上左子树大小+根节点个数
}
//左子树的大小>=k,在左子树中找第k个
//左子树的大小在[k-count,k-1],为根节点
//左子树的大小<k-count,在右子树中找
int KthQuery(SBTreeNode* root,int k){
    if(root==nullptr) return -0x3f3f3f3f;
    if(root->left)
    {
        if(root->left->size>=k) return KthQuery(root->left,k);
        else if(root->left->size+root->count>=k) return root->key;
        else 
        {
            if(k==1) return root->key;
        }
    }
    return KthQuery(root->right,k-(root->left?root->left->size:0)-root->count);
}

int main()
{
    int T_start=clock();
    //生成测试代码
    vector<int> v(0,10);
    for(int i=0;i<10;i++) v.push_back(rand()%100);
    for(int i=0;i<10;i++) cout<<v[i]<<" ";
    cout<<endl;
    SBTreeNode* root=nullptr;
    for(int i=0;i<10;i++) root=insert(root,v[i]);
    Inorder(root);
    cout<<endl;
    cout<<"v[0]:"<<v[0]<<endl;
    cout<<QueryRank(root,v[0])<<endl;
    cout<<KthQuery(root,4)<<endl;
    cout<<search(root,v[0])<<endl;
    root=deleteNode(root,v[0]);
    Inorder(root);
    cout<<endl;
    int insertded=39;
    root=insert(root,insertded);
    Inorder(root);
    int T_end=clock();
    cout<<"Time cost: "<<T_end-T_start<<endl;
    //测试代码结束
    return 0;
}