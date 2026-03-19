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
class Heap{
    struct node{
        int val;
    };
    int n,size=0,inscnt=0;
    vector<node> heap;
    vector<int>ph,hp;//pos->heap, heap->pos
    public:
        Heap(int n):n(n){
            heap.resize(n+1);
            ph.resize(n+1);
            hp.resize(n+1);
        }
        bool cmp(node a,node b){
            return a.val>b.val;
        }
        void _swap(int a,int b){
            swap(ph[hp[a]],ph[hp[b]]);
            swap(hp[a],hp[b]);
            swap(heap[a],heap[b]);
        }
        void push(int val){
            heap[++size].val=val;
            ph[++inscnt]=size;hp[size]=inscnt;;
            int i=size;
            while(i>1&&cmp(heap[i],heap[i>>1])){
                _swap(i,i>>1);
                i>>=1;
            }//向上调整
        }
        void pop(){
            _swap(1,size);
            ph[size]=0;hp[size]=0;
            size--; int i=1;
            while(i<<1<=size){
                int j=i<<1;
                if(j+1<=size&&cmp(heap[j+1],heap[j]))j++;
                if(cmp(heap[i],heap[j]))break;
                _swap(i,j); i=j;
            }//向下调整
        }
        int top(){
            return heap[1].val;
        }
        void remove(int pos){//删除第pos个元素(第pos个插入的元素)
            int i=ph[pos];
            _swap(i,size);
            ph[size]=0;hp[size]=0;
            size--; int j=i;
            while(j>1&&cmp(heap[j],heap[j>>1])){
                _swap(j,j>>1);
                j>>=1;
            }
            while(j<<1<=size){
                int k=j<<1;
                if(k+1<=size&&cmp(heap[k+1],heap[k]))k++;
                if(cmp(heap[j],heap[k]))break;
                _swap(j,k); j=k;
            }
        }
        void modify(int pos,int val){
            int i=ph[pos];
            heap[i].val=val;
            int j=i;
            while(j>1&&cmp(heap[j],heap[j>>1])){
                _swap(j,j>>1);
                j>>=1;
            }
            while(j<<1<=size){
                int k=j<<1;
                if(k+1<=size&&cmp(heap[k+1],heap[k]))k++;
                if(cmp(heap[j],heap[k]))break;
                _swap(j,k); j=k;
            }
        }
    
};
int main()
{
    int T_start=clock();
    vector<int> a={1,2,3,4,5,6,7,8,9,10};
    Heap h(a.size());
    for(int i=0;i<a.size();i++){
        h.push(a[i]);
    }
    h.remove(1);
    h.modify(2,11);
    cout<<h.top()<<endl;
    return 0;
}