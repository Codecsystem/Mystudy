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
class O1Tire{
	public:
		struct node
		{
			node* ch[2];
			int cnt;
			node():ch{nullptr,nullptr},cnt(0){}
		};
		node* root;
		O1Tire():root(new node){}
		void set(int x,int t)//从高到低建树
		{
			node* p=root;
			for(int i=31;i>=0;i--)
			{
				int d=(x>>i)&1;
				if(!p->ch[d])
					p->ch[d]=new node();
				p->ch[d]->cnt+=t;
				p=p->ch[d];
			}
		}
		int findMax(int x)//从高到低找,贪心选择,求解x对tire中所有数的最大异或值
		{
			node* p=root;
			int res=0;
			for(int i=31;i>=0;i--)
			{
				int d=(x>>i)&1;
				if(p->ch[d^1]&&p->ch[d^1]->cnt)
					p=p->ch[d^1],res+=(1<<i);
				else
					p=p->ch[d];
				if(!p)
					return res;
			}
			return res;
		}
		
};
int main()
{
	int T_start=clock();
	int t;cin>>t;
	while(t--)
	{
	    int n,k;cin>>n>>k;
	    vector<int> a(n);
	    for(int i=0;i<n;i++)
	        cin>>a[i];
		O1Tire tire;
		int ans=0xfffffff;
		for(int i=0,j=0;i<n;i++)
		{
			tire.set(a[i],1);
			while(j<=i&&tire.findMax(a[i])>=k)
			{
				ans=min(ans,i-j+1);
				tire.set(a[j],-1);
				j++;
			}
		}
		if(ans==0xfffffff) cout<<-1<<endl;
		else cout<<ans<<endl;
	}
	return 0;
}