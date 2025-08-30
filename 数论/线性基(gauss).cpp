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
#define int long long //赫赫 要不要龙龙呢
using namespace std;
class basic{
    public:
    vector<int> num,bas;
    int bit,cnt,n;
    basic(vector<int> a,int bit):
    num(a),bit(bit),cnt(0),n(a.size()){
        gauss();
        //usually bit=32/64
    }
    void gauss(){
        for(int i=bit-1;i>=0;i--){
            //把当前第i位是1的数换上去
            for(int j=cnt;j<n;j++){
                if(num[j]>>i&1){
                    swap(num[j],num[cnt]);
                    break;
                }
            }
            //如果这一位全0，跳过
            if((num[cnt]>>i&1)==0) continue;
            //消去其他数第i位1
            for(int j=0;j<n;j++){
                if(j!=cnt&&(num[j]>>i&1)) 
                    num[j]^=num[cnt];
            }
            cnt++;
            if(cnt==n) break;
        }
        bas.assign(num.begin(),num.begin()+cnt);
    }
    //求第k小的数 k:1base
    int kth(int k){
        //k个基向量能构造出2^k-1个数
        //case1 :cnt<n 意味这能构造出 0 所以能构造2^k个数
        //case2 :cnt=n 意味这不能构造出 0 所以只能构造2^k-1个数
        if(cnt<n) k--;
        if(k>=(1ll<<cnt)) return -1;
        int ans=0;
        for(int i=0;i<cnt;i++){
            if(k>>i&1) ans^=bas[cnt-1-i];
        }
        return ans;
    }
    //求一个数用一个数列异或得到的方案数
    //约简为0的向量是不必要的 于是可以任选
    int count(int x){
        for(auto b:bas){
            if(x&b) x^=b;
        }
        return x==0?(1ll<<(n-cnt)):0;
    }
    //求一个数在数列xor和中的排名
    int rk(int x){
        int tp=x;
        for(auto b:bas){
            if(tp&b) tp^=b;
        }
        if(tp) return -1;
        int id=0;
        for(int i=0;i<cnt;i++){
            if(x&(bas[i]))
            {
                id|=(1ll<<(cnt-1-i));
                x^=bas[i];
            }
        }
        if(cnt<n) id++;
        return id;
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    int n;cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++) cin>>a[i];
    basic b(a,64);
    int ans=0;
    for(auto i:b.bas) ans^=i;
    cout<<ans<<endl;
    return 0;
}
//异或线性基 O(bit*n)
//异或线性基是原数列的一个基向量，
//意味这基向量的线性组合能构造出原数列的任意数
//意味着原数列线性组合构造出的数和线性基线性组合构造出的数是一样的
//xor=mod 2+/GF(2)域
//guass消元法给出的线性基是行最简式
//即形如
//01001
//00100
//00011
//00000
//满足三个性质：
//1.线性基中任意两个基向量的异或结果不会是0
//2.线性基每一个基向量的高位1在别的基向量中都是0
//3.基向量是从大到小存储的