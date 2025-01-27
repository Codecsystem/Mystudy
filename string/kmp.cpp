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
vector<int> prefix_init_f(string s) //前缀函数初始化
// 前缀函数就是，子串s[0..i]最长的相等的真前缀与真后缀的长度。
// 在kmp算法中，前缀函数是核心，它决定了模式串（key）在匹配过程若不匹配应该跳转的位置。
// e.g. abcabc的前缀函数为[0,0,0,1,2,3]
{
	int len=s.length();
	vector<int> dp(len,0);
	dp[0]=0;
	for(int i=1;i<len;i++)
	{
		int j=dp[i-1];
		while(j>0&&s[i]!=s[j]) j=dp[j-1];//如果s[i]和s[j]不相同，j跳到前一个符合的位置
		if(s[i]==s[j]) j++; //如果s[i]和s[j]相同，j+1
		dp[i]=j;
	}
	return dp;
} 
void kmp(string s,string key)
{
	if(key.length()==0) return;
	vector<int> dp=prefix_init_f(key);
	int i=0,j=0;
	while(i<s.length())
	{
		if(s[i]==key[j]) {i++;j++;} //如果匹配，接着匹配下一个字符
		else if(j>0) j=dp[j-1]; //如果不匹配，j跳到前一个符合的位置
		else i++;
		if(j==key.length())
		{
			/*some operation*/
			j=dp[j-1];//匹配成功后，j跳到前一个符合的位置
		}
	}
}
int main()
{
	int T_start=clock();
	
	return 0;
}