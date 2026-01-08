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
using namespace std;
unordered_map<int,int> dis(vector<int> a)
{
    sort(a.begin(),a.end());
    unordered_map<int,int> mp;
    for(int i=0;i<a.size();i++)
    {
        mp[a[i]]=i+1;
    }
    return mp;
}
int main()
{
    int T_start=clock();
    vector<int> a;
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(),a.end()),a.end());
    auto getid=[&](int x){return lower_bound(a.begin(),a.end(),x)-a.begin()+1;};
    return 0;
}