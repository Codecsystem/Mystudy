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
#include <cstdint>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
//#define int long long //赫赫 要不要龙龙呢
using ll=long long;
using namespace std;
using namespace __gnu_pbds;
template<typename T>
//多重集
class multiSet{
    private:
        typedef tree<pair<T,int>,null_type,less<pair<T,int>>,
            rb_tree_tag,tree_order_statistics_node_update> pb_tree;
        pb_tree tr; int tot;
        const int INF=1e9;
        const T bad=-1;
        //此处记得贴合T的类型
        //INF 插入元素个数的最大值
        //bad 没有前驱或后继时的返回值
    public:
        multiSet():tot(0){}
        //插入元素
        void insert(T x){ tr.insert({x,++tot}); }
        //查询元素的排名
        int rank(T x){ return tr.order_of_key({x,0})+1; }
        //查询元素的前驱
        T pre(T x){ 
            auto it=tr.lower_bound({x,0});
            if(it==tr.begin()) return bad;
            --it; return it->first;
        }
        //查询元素的后继
        T next(T x){
            auto it=tr.lower_bound({x,INF});
            if(it==tr.end()) return bad;
            return it->first;
        }
        T kth(int k){
            auto it=tr.find_by_order(k-1);
            if(it==tr.end()) return bad;
            return it->first;
        }
        bool find(T x){
            auto it=tr.lower_bound({x,0});
            return it!=tr.end()&&it->first==x;
        }
        int size(){ return tr.size(); }
        bool empty(){ return tr.empty(); }
        //删除元素
        void erase(T x){
            auto it=tr.lower_bound({x,0});
            if(it!=tr.end()&&it->first==x) tr.erase(it);
        }
};

template<typename T>
//不可重集
class Set{
    private:
        typedef tree<T,null_type,less<T>,
            rb_tree_tag,tree_order_statistics_node_update> pb_tree;
        pb_tree tr; 
        const T bad=-1;
        //此处记得贴合T的类型
        //bad 没有前驱或后继时的返回值
    public:
        Set(){}
        //插入元素
        void insert(T x){ tr.insert(x); }
        //查询元素的排名
        int rank(T x){ return tr.order_of_key(x)+1; }
        //查询元素的前驱
        T pre(T x){ 
            auto it=tr.lower_bound(x);
            if(it==tr.begin()) return bad;
            --it; return *it;
        }
        //查询元素的后继
        T next(T x){
            auto it=tr.upper_bound(x);
            if(it==tr.end()) return bad;
            return *it;
        }
        T kth(int k){
            auto it=tr.find_by_order(k-1);
            if(it==tr.end()) return bad;
            return *it;
        }
        bool find(T x){
            return tr.find(x)!=tr.end();
        }
        int size(){ return tr.size(); }
        bool empty(){ return tr.empty(); }
        //删除元素
        void erase(T x){
            tr.erase(x);
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