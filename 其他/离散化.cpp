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
int main()
{
    int T_start=clock();
    vector<int> a;
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(),a.end()),a.end());
    auto getid=[&](int x){return lower_bound(a.begin(),a.end(),x)-a.begin()+1;};
    return 0;
}