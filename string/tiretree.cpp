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
struct TiretreeNode{
	vector<TiretreeNode*> child;
	bool isEnd;

	TiretreeNode(): child(26,nullptr),isEnd(false) {}
};
void insert(string s,TiretreeNode* root)
{
	for(auto ch:s)
	{
		int idx=ch-'a';
		if(root->child[idx]==nullptr)
		{
			root->child[idx]=new TiretreeNode;
		}
		root=root->child[idx];
	}
	root->isEnd=true;
}
bool iscontain(string s,TiretreeNode* root)
{
	for(auto ch:s)
	{
		int idx=ch-'a';
		if(root->child[idx]==nullptr)
		{
			return false;
		}
		root=root->child[idx];
	}
	return root!=nullptr&&root->isEnd==true;
}
int main()
{
	int T_start=clock();
	vector<string> allsub={"hello","world","ld"};
	TiretreeNode* root=new TiretreeNode();
	for(auto i:allsub)
	{
		insert(i,root);
	}
	vector<string> test={"hll","l","d","po","world"};
	for(auto i:test)
	{
		cout<<i<<' '<<(iscontain(i,root)?"is in the tree":"is not in the tree")<<endl;
	}
	return 0;
}