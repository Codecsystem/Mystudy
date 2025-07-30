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
class Trie{
public:
    struct node
    {
        vector<node*> ch;
        int cnt;
        bool isend;
        node():ch(26,nullptr),cnt(0),isend(false) {}
    };
    node* root;
    Trie():root(new node()) {}
    void ins(string s)
    {
        node* p=root;
        for(auto i:s)
        {
            int idx=i-'a';
            if(p->ch[idx]==nullptr)
            {
                p->ch[idx]=new node();
            }
            p=p->ch[idx];
        }
        p->isend=true;
    }
    bool con(string s){
        node* p=root;
        for(auto i:s)
        {
            int idx=i-'a';
            if(p->ch[idx]==nullptr)
            {
                return false;
            }
            p=p->ch[idx];
        }
        return p!=nullptr&&p->isend;
    }
};
signed main()
{
    int T_start=clock();
    //freopen("in.txt","r",stdin);
    //freopen("out2.txt","w",stdout);
    //ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    return 0;
}