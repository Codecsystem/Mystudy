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
using namespace std;
const int MAXN=1e4;
int Graph[MAXN][MAXN];
int dp[MAXN][MAXN];
int main()
{
    int T_start=clock();
    int T=10;
    srand(time(NULL));
    for(int i=1;i<=T;i++)
    {
        for(int j=1;j<=T;j++)
        {
            int op=rand()%3;
            if(false) Graph[i][j]=0;
            else if(op==1) Graph[i][j]=0x3f3f3f3f;
            else Graph[i][j]=(rand()*10+rand())%10;
            //Graph[i][j]=(rand()*10+rand())%10;
        }
        Graph[i][i]=0;
    }

    
    for(int i=1;i<=T;i++)
    {
        for(int j=1;j<=T;j++)
        {
            dp[i][j]=Graph[i][j];
        }
    }
    for(int i=1;i<=T;i++)
    {
        for(int j=1;j<=T;j++)
        {
            cout<<dp[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    for(int i=1;i<=T;i++)
    {
        for(int j=1;j<=T;j++)
        {
            for(int k=1;k<=T;k++)
            {
                dp[j][k]=min(dp[j][k],dp[j][i]+dp[i][k]);
            }
        }
    }

    for(int i=1;i<=T;i++)
    {
        for(int j=1;j<=T;j++)
        {
            cout<<dp[i][j]<<" ";
        }
        cout<<endl;
    }
    int T_end=clock();
    return 0;
}

//Floyed 算法
//处理任意两点之间的最短路径(无负环）
//时间复杂度O(n^3)