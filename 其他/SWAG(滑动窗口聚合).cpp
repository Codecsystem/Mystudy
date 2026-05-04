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
#include <random>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
constexpr ll INF=1e18;
struct Matrix{
    ll m[2][2]={{INF,INF},{INF,INF}};
    Matrix():m{{0,INF},{INF,0}}{}
    Matrix(ll a,ll b,ll c,ll d):m{{a,b},{c,d}}{}
    Matrix operator*(const Matrix& o) const{
        Matrix r(INF,INF,INF,INF);
        for(int i:{0,1})
            for(int k:{0,1})
                for (int j:{0,1})
                    r.m[i][j]=min(r.m[i][j],m[i][k]+o.m[k][j]);
        return r;
    }
};
template<typename T,class Op>
struct SWAG{
    struct Node{
        T val;
        T agg;
    };
    vector<Node> front,back;
    Op op; //满足结合律的运算
    T id;  //单位元
    SWAG(Op op,T id):op(op),id(id){}
    void push(const T& x){
        if(back.empty()) back.push_back({x,x});
        else back.push_back({x,op(back.back().agg,x)});
    }
    void pop(){
        if(front.empty()){
            while(!back.empty()){
                T x=back.back().val;
                back.pop_back();
                if(front.empty()) front.push_back({x,x});
                else front.push_back({x,op(x,front.back().agg)});
            }
        }
        if(!front.empty()) front.pop_back();
    }
    T qry(){
        if(back.empty()&&front.empty()) return id;
        if(back.empty()) return front.back().agg;
        if(front.empty()) return back.back().agg;
        return op(front.back().agg,back.back().agg);
    }
};
//一个比较经典的例子是 front从顶到底是{{B,BCD},{C,CD},{D,D}} back从顶到底是{{G,EFG},{F,EF},{E,E}}
//可以发现front是左乘，back是右乘，这样就可以用SWAG维护有结合律的玩意了
//不知道单元元的时候可以传个异常元进去
//使用例：
// auto mul=[](const Matrix& a,const Matrix& b){
//     return b*a;
// };
// SWAG<Matrix,decltype(mul)> q(mul,Matrix());
// 注意矩阵乘法的顺序QAQ
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int t;cin>>t;
    while(t--){
        int n,k;cin>>n>>k;
        vector<int> a(n+1);
        for(int i=1;i<=n;i++) cin>>a[i];
        if(k==1){
            cout<<*min_element(a.begin()+1,a.end())<<'\n';
            continue;
        }
        vector<Matrix> m(n+1);
        for(int i=1;i<=n;i++){
            m[i]=Matrix(a[i],a[i],0,INF);
        }
        auto mul=[](const Matrix& a,const Matrix& b){
            return b*a;
        };
        auto slove=[&](int w){
            if(w<1) return INF;
            SWAG<Matrix,decltype(mul)> q(mul,Matrix());
            ll res=INF;
            for(int i=1;i<=n;i++){
                q.push(m[i]);
                if(i>w) q.pop();
                if(i>=w){
                    int l=i-w;
                    if(l>=1){
                        res=min(res,q.qry().m[0][0]+a[l]);
                    }
                }
            }
            return res;
        };
        cout<<min(slove(k),slove(k-1))<<'\n';
    }
    return 0;
}