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
using namespace std;
struct DyBitset
{
    using ull=uint64_t;
    int n; vector<ull> a;
    struct ref
    {
        ull& block;
        const ull st;
        ref& operator=(bool x){
            if(x) block|=st;
            else block&=~st;
            return *this;
        }
        ref& operator=(const ref& rhs){ return *this=(bool)rhs; }
        operator bool() const { return block&st; }
    };
    
    DyBitset(int _n):n(_n){
        a.resize((n+64)/64,0);
    }
    void sant(){
        a[0]&=~1ull;
        if((n+1)%64){
            a.back()&=((1ull<<((n+1)%64))-1);
        }
    }
    
    void set(int i){ a[i>>6]|=(1ull<<(i&63)); } //置1
    void reset(int i){ a[i>>6]&=~(1ull<<(i&63)); } //置0
    void clear(){ fill(a.begin(),a.end(),0); } //清空
    bool test(int i) const{ return (a[i>>6]>>(i&63))&1; } //查询
    bool operator[](int i) const{ return test(i); } //下标访问
    ref operator[](int i){ return {a[i>>6],1ull<<(i&63)}; } //下标修改
    int count() const{
        int res=0;
        for(auto x:a) res+=__builtin_popcountll(x);
        return res;
    }
    DyBitset operator~() const {
        DyBitset res(n);
        for(int i=0;i<a.size();i++){
            res.a[i]=~a[i];
        }
        res.sant(); 
        return res;
    }
    DyBitset operator&(const DyBitset& rhs) const{
        DyBitset res(max(n,rhs.n));
        for(int i=0;i<min(a.size(),rhs.a.size());i++){
            res.a[i]=a[i]&rhs.a[i];
        }
        return res;
    }
    DyBitset operator|(const DyBitset& rhs) const{
        DyBitset res(max(n,rhs.n));
        for(int i=0;i<min(a.size(),rhs.a.size());i++){
            res.a[i]=a[i]|rhs.a[i];
        }
        const auto& rs=(a.size()>rhs.a.size()?a:rhs.a);
        for(int i=min(a.size(),rhs.a.size());i<rs.size();i++){
            res.a[i]=rs[i];
        }
        res.sant();
        return res;
    }
    DyBitset operator^(const DyBitset& rhs) const{
        DyBitset res(max(n,rhs.n));
        for(int i=0;i<min(a.size(),rhs.a.size());i++){
            res.a[i]=a[i]^rhs.a[i];
        }
        const auto& rs=(a.size()>rhs.a.size()?a:rhs.a);
        for(int i=min(a.size(),rhs.a.size());i<rs.size();i++){
            res.a[i]=rs[i];
        }
        res.sant();
        return res;
    }
    bool operator==(const DyBitset& rhs) const{
        if(n!=rhs.n) return false;
        return a==rhs.a;
    }
    bool operator!=(const DyBitset& rhs) const{
        return !(*this==rhs);
    }
    bool operator<(const DyBitset& rhs) const{
        if(n!=rhs.n) return n<rhs.n;
        return a<rhs.a;
    }
    int ctz() const{
        for(int i=0;i<a.size();i++){
            if(a[i]){
                return (i<<6)+__builtin_ctzll(a[i]);
            }
        }
        return n+1;
    }
};

signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    
    return 0;
}