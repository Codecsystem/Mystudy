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
#include <numeric>
#include <functional>
#include <ranges>
#include <iomanip>
//#define int long long //赫赫 要不要龙龙呢
using namespace std;
class BIT{
    private:
        int n;
        vector<int> tree;//tree[i] 是[i-lowbit(i)+1,i]的和,[1,n]存储
        int lowbit(int x){
            return x&(-x);
        }
    public:
        BIT(int n): n(n),tree(n+1,0){}
        void update(int i,int val)//单点修改 a[i]+=val
        {
            while(i<=n){ 
                tree[i]+=val;
                i+=lowbit(i);//跳到后一个lowbit(x)的位置
            }
        }
        int pre(int x){
            int res=0;
            while(x>0){
                res+=tree[x];
                x-=lowbit(x);//跳到前一个lowbit(x)的位置
            }
            return res;
        }
        int query(int l,int r)//区间查询 [l,r]的和
        {
            return pre(r)-pre(l-1);
        }
        int query_diff(int i)//单点查询 a_diff[i] (维护差分数组)=sum[1,i]
        {
            return query(1,i);
        }
        void update_diff(int l,int r,int val)//区间修改 (维护差分数组) a_diff[l]+=val,a_diff[r+1]-=val
        {
            update(l,val);
            update(r+1,-val);
        }
        void init(vector<int> a)//初始化
        {
            vector<int> presum(a.size()+1,0);
            for(int i=1;i<=a.size();i++)
            {
                presum[i]=presum[i-1]+a[i-1];
                tree[i]=presum[i]-presum[i-lowbit(i)];//按定义
            } 
        }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}