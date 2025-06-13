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
using namespace std;
#define int long long
const int mod=1e9+7;
class MaTQpow{
	public:
		vector<vector<int>> mat;
		int _mod;
		MaTQpow(vector<vector<int>> _mat,int mod):mat(_mat),_mod(mod){}
		MaTQpow(int n,int mod):_mod(mod)
		{
			mat.resize(n,vector<int>(n,0));
			for(int i=0;i<n;i++) mat[i][i]=1;
			_mod=mod;
		}
		MaTQpow operator*(const MaTQpow& other)const{
			vector<vector<int>> res(mat.size(),vector<int>(other.mat[0].size(),0));
			for(int i=0;i<mat.size();i++)
			{
				for(int j=0;j<other.mat[0].size();j++)
				{
					for(int k=0;k<other.mat.size();k++)
					{
						res[i][j]=(res[i][j]+mat[i][k]*other.mat[k][j])%_mod;
					}
				}
			}
			return MaTQpow(res,_mod);
		}
		MaTQpow Qpow(int n){
			MaTQpow res(mat.size(),_mod);
			MaTQpow base(mat,_mod);
			while(n)
			{
				if(n&1) res=res*base;
				base=base*base;
				n>>=1;
			}
			return res;
		}
		vector<vector<int>> get(int n){
			return Qpow(n).mat;
		}
};
signed main()
{
	int T_start=clock();
	//freopen("in.txt","r",stdin);
	int n;cin>>n;
	vector<int> a(n+1);
	for(int i=1;i<=n;i++) cin>>a[i];
	int c,m,k,t;cin>>c>>m>>k>>t;c%=m;
	vector<int> dp(m,0);
	for(int i=1;i<=n;i++)
	{
	    vector<int> ndp(m,0);
		ndp[a[i]%m]=1;
		for(int j=0;j<m;j++)
		{
		    if(dp[j])
		    {
		        ndp[(j+a[i])%m]=(ndp[(j+a[i])%m]+dp[j])%mod;
		    }
		}
		for(int j=0;j<m;j++) dp[j]=(dp[j]+ndp[j])%mod;
	}
	vector<vector<int>> p(m,vector<int>(m,0));
	for(int i=0;i<m;i++)
	{
	    for(int j=0;j<m;j++)
	    {
	        p[i][(i*j)%m]=(p[i][(i*j)%m]+dp[j])%mod;
	    }
	}
	MaTQpow mat(p,mod);
	auto res=mat.Qpow(t);
	cout<<res.mat[c][k]<<endl;
	return 0;
}
//矩阵快速幂：处理快速形式变换
//时间复杂度：O(n^3logk)