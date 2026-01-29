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
#include <chrono>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
const int mod=1e9+7;
template <int MOD>
struct SMC {
    int val;
    SMC(ll v=0) : val(v%MOD) { if (val<0) val+=MOD; }
    SMC& operator+=(const SMC &r) { val+=r.val; if (val>=MOD) val-=MOD; return *this; }
    SMC& operator-=(const SMC &r) { val-=r.val; if (val<0) val+=MOD; return *this; }
    SMC& operator*=(const SMC &r) { val=1LL*val*r.val%MOD; return *this; }
    SMC& operator/=(const SMC &r) { return *this*=r.inv(); }
    friend SMC operator+(SMC a,const SMC &b) { return a+=b; }
    friend SMC operator-(SMC a,const SMC &b) { return a-=b; }
    friend SMC operator*(SMC a,const SMC &b) { return a*=b; }
    friend SMC operator/(SMC a,const SMC &b) { return a/=b; }
    SMC pow(ll k) const {
        SMC res=1,a=*this;
        for (;k;k>>=1,a*=a) if(k&1) res*=a;
        return res;
    }
    SMC inv() const { return pow(MOD-2); }
    friend istream& operator>>(istream &in,SMC &a) { ll v; in>>v; a=v; return in; }
    friend ostream& operator<<(ostream &out,const SMC &a) { return out<<a.val; }
};
using Z=SMC<mod>;
class MatQpow{
    public:
    int n;
    vector<vector<Z>> mat,e;
    MatQpow(int n,vector<vector<Z>> mat):n(n),mat(mat){
        e=vector<vector<Z>>(n,vector<Z>(n,0));
        for(int i=0;i<n;i++) e[i][i]=1;
    }
   static vector<vector<Z>> mul(
        const vector<vector<Z>>& A,
        const vector<vector<Z>>& B,
        int n
    ){
        vector<vector<Z>> C(n,vector<Z>(n,0));
        for(int i=0;i<n;i++)
            for(int k=0;k<n;k++)
                for(int j=0;j<n;j++)
                    C[i][j]+=A[i][k]*B[k][j];
        return C;
    }
    vector<vector<Z>> qpow(ll k) const{
        vector<vector<Z>> res=e;
        vector<vector<Z>> base=mat;
        while(k){
            if(k&1) res=mul(res,base,n);
            base=mul(base,base,n);
            k>>=1;
        }
        return res;
    }
    
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;ll k;cin>>n>>k;
    vector<vector<Z>> mat(n,vector<Z>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>mat[i][j];
        }
    }
    MatQpow m(n,mat);
    auto res=m.qpow(k);
    Z ans=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            ans+=res[i][j];
        }
    }
    cout<<ans<<endl;
    return 0;
}
//矩阵快速幂：处理快速形式变换
//时间复杂度：O(n^3logk)