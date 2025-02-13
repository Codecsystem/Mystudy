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
class Heap{//小根堆
    public:
        struct node{
            int val;
        };
        vector<node> heap;
        int n,size=0;
        Heap(int n):n(n){
            heap.resize(n+1);
        }
        bool cmp(node a,node b){//自定义比较函数
            return a.val<b.val;
        }
        //1...n的完全二叉树,i的父亲为i/2,i的左孩子为2i,右孩子为2i+1
        void push(int val){
            heap[++size].val=val;
            int i=size;
            while(i>1&&cmp(heap[i],heap[i/2])){
                swap(heap[i],heap[i/2]);
                i/=2;//向上调整
            }
        }
        void pop(){
            heap[1].val=heap[size--].val;//将最后一个元素放到第一个位置
            int i=1;
            while(i*2<=size){
                int j=i*2;
                if(j<size&&cmp(heap[j+1],heap[j])) j++;//找到左右孩子中较大的一个
                if(cmp(heap[i],heap[j])) break;//如果父亲节点比孩子节点大,则不需要调整
                swap(heap[i],heap[j]);
                i=j;
            }//向下调整
        }
        int top(){
            return heap[1].val;
        }
    
};
int main()
{
    int T_start=clock();
    int n;
    cin>>n;
    Heap h(n);
    for(int i=0;i<n;i++){
        int x;
        cin>>x;
        h.push(x);
    }
    while(h.size>1){
        cout<<h.top()<<" ";
        h.pop();
    }
    return 0;
}