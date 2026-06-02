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
#include <cassert>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
template<class T>
//0.普通矩阵乘
struct StdOp{
    static T add(const T& a,const T& b) { return a+b; }
    static T mul(const T& a,const T& b) { return a*b; }
    static T zero() { return T(0); }
    static T one() { return T(1); }
};

//1.1 Min-Plus 半环 (最短路)
template<class T>
struct MinPlusOp {
    //除以 2 是为了防止发生 INF + INF 的溢出
    static const T INF=numeric_limits<T>::max()/2;
    static T add(const T&a,const T&b) { return min(a,b); }
    static T mul(const T&a,const T&b) { return a+b; }
    static T zero() { return INF; }
    static T one() { return T(0); }
};

//1.2 Max-Plus 半环 (最长路)
template<class T>
struct MaxPlusOp {
    static const T INF=numeric_limits<T>::lowest()/2;
    static T add(const T&a,const T&b) { return max(a,b); }
    static T mul(const T&a,const T&b) { return a+b; }
    static T zero() { return INF; }
    static T one() { return T(0); }
};

//2.1 Max-Min 半环 (最大瓶颈路)
template<class T>
struct MaxMinOp{
    static T add(const T&a,const T&b) { return max(a,b); }
    static T mul(const T&a,const T&b) { return min(a,b); }
    static T zero() { return numeric_limits<T>::lowest(); }
    static T one() { return numeric_limits<T>::max(); }
};

//2.2 Min-Max 半环 (最小瓶颈路)
template<class T>
struct MinMaxOp {
    static T add(const T&a,const T&b) { return min(a,b); }
    static T mul(const T&a,const T&b) { return max(a,b); }
    static T zero() { return numeric_limits<T>::max(); }
    static T one() { return numeric_limits<T>::lowest(); }
};

//3. 最短路-计数半环
template<class T,class C>
struct MinPlusCountOp {
    using P=pair<T,C>;
    static const T INF=numeric_limits<T>::max()/2;
    static P add(const P&a, const P&b) {
        if (a.first<b.first) return a;
        if (b.first<a.first) return b;
        return {a.first,a.second+b.second}; 
    }
    static P mul(const P&a, const P&b) {
        return {a.first+b.first,a.second*b.second}; 
    }
    static P zero() { return {INF,C(0)}; }
    static P one() { return {0,C(1)}; }
};

template<class T,class Op>
struct Mat
{
    int n,m;
    vector<T> a;
    Mat(int n=0,int m=0):n(n),m(m),a(n*m,Op::zero()) {}
    Mat(int n,int m,const T& val):n(n),m(m),a(n*m,val) {}
    inline T* operator[](int i) { return a.data()+i*m; }
    inline const T* operator[](int i) const { return a.data()+i*m; }
    Mat operator*(const Mat& r) const{
        assert(m==r.n);
        Mat res(n,r.m);
        for(int i=0;i<n;i++){
            for(int k=0;k<m;k++){
                T tmp=(*this)[i][k];
                if(tmp==Op::zero()) continue;
                for(int j=0;j<r.m;j++){
                    //跳过右侧零元，避免 INF/-INF 参与 mul 出现溢出风险
                    if(r[k][j]==Op::zero()) continue;
                    res[i][j]=Op::add(res[i][j],Op::mul(tmp,r[k][j]));
                }
            }
        }
        return res;
    };
    static Mat I(int _n){
        Mat res(_n,_n);
        for(int i=0;i<_n;i++) res[i][i]=Op::one();
        return res;
    }
    Mat pow(ll k) const{
        assert(n==m);
        Mat res=I(n),x=*this;
        for(;k;k>>=1,x=x*x) if(k&1) res=res*x;
        return res;
    }
};

signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n,m;cin>>n>>m;
    Mat<int,MinMaxOp<int>> a(n,n);
    for(int i=1;i<=m;i++){
        int u,v;cin>>u>>v;
        u--,v--;
        a[u][v]=0;
    }
    int k;cin>>k;
    for(int i=1;i<=k;i++){
        int u,v;cin>>u>>v;
        u--,v--;
        a[u][v]=min(a[u][v],i);
    }
    int q,w;cin>>q>>w;
    auto res=a.pow(w);
    while(q--){
        int u,v;cin>>u>>v;
        u--,v--;
        if(res[u][v]==numeric_limits<int>::max()) cout<<"-1\n";
        else cout<<res[u][v]<<"\n";
    }
    return 0;
}
