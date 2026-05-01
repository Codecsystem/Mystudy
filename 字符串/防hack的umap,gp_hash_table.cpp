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
#include <ext/pb_ds/assoc_container.hpp>
//#define int long long //赫赫 要不要龙龙呢
using ull=unsigned long long;
using ll=long long;
using namespace std;
using namespace __gnu_pbds;
struct Xhash64{
    static ull splitmix64(ull x){
        x+=0x9e3779b97f4a7c15;
        x=(x^(x>>30))*0xbf58476d1ce4e5b9;
        x=(x^(x>>27))*0x94d049bb133111eb;
        return x^(x>>31);
    }
    size_t operator()(ull x) const{
        static const ull SALT=chrono::steady_clock::now().time_since_epoch().count()^(ull)(new char);
        return splitmix64(x+SALT);
    }
};
struct Xhash32{
    static uint32_t splitmix32(uint32_t x){
        x+=0x9e3779b9;
        x=(x^(x>>16))*0x85ebca6b;
        x=(x^(x>>13))*0xc2b2ae35;
        return x^(x>>16);
    }
    size_t operator()(uint32_t x) const{
        static const uint32_t SALT=std::chrono::steady_clock::now().time_since_epoch().count()^(ull)(new char);
        return splitmix32(x+SALT);
    }
};
signed main()
{
    auto T_start=chrono::steady_clock::now();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    unordered_map<ull,int,Xhash64> mp; //拉链法
    gp_hash_table<ull,int,Xhash64> gp; //二次寻址，缓存更友好，速度更快。
    return 0;
}