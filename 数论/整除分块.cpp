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
#define int long long 
using namespace std;
struct blocknode{
	int l;
	int r;
	int val;
};
//对[l,r]的i,floor(n/i)相等
//n%i=n-i*floor(n/i)
//首项n-l*val 公差-val 项数r-l+1
class divb{
public:
    struct node{
        int l,r;
        int val1,val2;
    };
    int s,e;
    vector<node> a;
    divb(int s,int e):s(s),e(e){}
    void b1(int n,int m){
        for(int l=s,r;l<=e;l=r+1)
        {
            r=min(n/(n/l),m/(m/l));
            a.push_back({l,r,n/l,m/l});
        }
    }
    void b2(int n)
    {
        for(int l=s,r;l<=e;l=r+1)
        {
            r=n/(n/l);
            a.push_back({l,r,n/l,0});
        }
    }
};
signed main()
{
	int T_start=clock();
	int n,k;cin>>n>>k;
	vector<blocknode>a;
	for(int l=1,r;l<=n;l=r+1)
	{
		blocknode tp;
		r=min(n/(n/l),n);
		tp.l=l;tp.r=r;
		tp.val=n/l;
		a.push_back(tp);
	}
	//for(auto i:a)
	//{
		//cout<<i.l<<' '<<i.r<<' '<<i.val<<endl;
	//}
	return 0;
}