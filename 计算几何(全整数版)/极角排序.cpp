#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using i128=__int128_t;

//整数点是排序对象，坐标本身不做浮点化。
struct pit{
    ll x,y;
    pit(ll x=0,ll y=0):x(x),y(y){}
};

//按atan2的[-pi,pi]顺序划分方向，避免实际计算反三角函数。
int half(i128 x,i128 y){return y<0||(y==0&&x>=0)?0:1;}
//比较两个方向的极角；同向时短向量优先，零向量排在最前。
bool polarLess(i128 ax,i128 ay,i128 bx,i128 by){
    bool az=ax==0&&ay==0,bz=bx==0&&by==0;
    if(az||bz)return az&&!bz;
    int ha=half(ax,ay),hb=half(bx,by);
    if(ha!=hb)return ha<hb;
    i128 cr=ax*by-ay*bx;
    if(cr!=0)return cr>0;
    return ax*ax+ay*ay<bx*bx+by*by;
}
//以整数点o为极点逆时针排序，角度相同的点按离o由近到远排列。
void psort(vector<pit>& a,const pit& o){
    sort(a.begin(),a.end(),[&](const pit& A,const pit& B){
        i128 ax=(i128)A.x-o.x,ay=(i128)A.y-o.y;
        i128 bx=(i128)B.x-o.x,by=(i128)B.y-o.y;
        if(polarLess(ax,ay,bx,by))return true;
        if(polarLess(bx,by,ax,ay))return false;
        return A.x!=B.x?A.x<B.x:A.y<B.y;
    });
}
//以点集重心为极点排序，用n*p-sum精确表示方向而不做除法。
void psort(vector<pit>& a){
    if(a.empty())return;
    i128 sx=0,sy=0,n=a.size();
    for(const auto& p:a)sx+=p.x,sy+=p.y;
    sort(a.begin(),a.end(),[&](const pit& A,const pit& B){
        i128 ax=n*A.x-sx,ay=n*A.y-sy;
        i128 bx=n*B.x-sx,by=n*B.y-sy;
        if(polarLess(ax,ay,bx,by))return true;
        if(polarLess(bx,by,ax,ay))return false;
        return A.x!=B.x?A.x<B.x:A.y<B.y;
    });
}

//模板文件入口，不执行额外逻辑。
signed main(){return 0;}
