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
struct blocknode{
	int l;
	int r;
	int val;
};
//对[l,r]的i,floor(n/i)相等
//n%i=n-i*floor(n/i)
//首项n-l*val 公差-val 项数r-l+1
signed main()
{
	int T_start=clock();
	int n=read(),k=read();
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