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
int st[100005][20]; //st[i][j]表示从i开始，长度为2^j的区间最大值
int st2[100005][20];
int ata[100005];
//F(i,j)=max([i...i+2^j-1])
//F(i,0)=a[i]
//F(i,j)=max([i...i+2^(j-1)-1], [i+2^(j-1)...i+2^j-1])=max(F(i,j-1), F(i+2^(j-1),j-1))
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
long long _pow(int b,int p)//b^p
{
	long long ans=1;
	while(p>0)
	{
		if(p&1) ans=ans*b;  
		b*=b;
		p>>=1;//p>>1==p/2
	}
	return ans;
}
void buildST(int a[],int len)
{
	int logN=ceil(log2(len));
	for(int i=0;i<len;i++)
	{
		st[i][0]=a[i];
		st2[i][0]=a[i];
	}
	for(int j=1;j<=logN;j++) //到2^j
	{
		for(int i=0;i+(1<<j)-1<len;i++) //二分（注意不是完全二分）
		{
			st[i][j]=max(st[i][j-1],st[i+(1<<(j-1))][j-1]);
			st2[i][j]=min(st2[i][j-1],st2[i+(1<<(j-1))][j-1]);
		}
	} 
}
pair<int,int> queryST(int l,int r)
{
	int s=floor(log2(r-l+1));
	return make_pair(max(st[l][s],st[r-(1<<s)+1][s]),min(st2[l][s],st2[r-(1<<s)+1][s]));
}
int main()
{
	//freopen("in.txt","r",stdin);
	int T_start=clock();
	int n=read(),q=read();
	for(int i=0;i<n;i++)
		ata[i]=read();
	buildST(ata,n);
	while(q--)
	{
		int l=read(),r=read();
		pair<int,int> ans=queryST(l-1,r-1);
		//cout<<ans.first<<" "<<ans.second<<endl;
		write(ans.first-ans.second);
		putchar('\n');
	}
	return 0;
}