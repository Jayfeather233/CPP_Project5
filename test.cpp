#include "mat.hpp"
#include <iostream>
using namespace std;
template<typename _T>
_T fast_pow(_T a, int b, _T one){
    _T ans = one;
    while(b){
        if(b&1) ans = ans * a;
        a = a * a;
        b>>=1;
    }
    return ans;
}
int main(){
    int a1[]={0,1,1,1,0,1,1,1};
    int a2[]={0,1,1,1,0,1,1,1};
    int onex[]={1,0,0,1,1,0,0,1};

    Mat<int> x1(a1,2,2,2);
    Mat<int> x2(a2,2,2,2);
    Mat<int> one(onex,2,2,2);
    
    cout<<x1;
    cout<<x2;
    cout<<one;

    cout<<"-------"<<endl;
    //x1 = x1.subMatrix(1,1,2,1,1);
    //x2 = x2.subMatrix(1,1,2,1,1);
    //cout<<"-------"<<endl;

    //cout<<x1;
    //cout<<x2;

    cout<<fast_pow(x1,7,one);
    //cout<<one * x1;
    //cout<<one * x1 * x1;
    //cout<<one * x1 * x1 * x1;
    return 0;
}