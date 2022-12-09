//#define __DEBUG__
// #include "mat.hpp"
// #include <iostream>
// using namespace std;
// template<typename _T>
// _T fast_pow(_T a, int b, _T one){
//     _T ans = one;
//     while(b){
//         if(b&1) ans = ans * a;
//         a = a * a;
//         b>>=1;
//     }
//     return ans;
// }
// int main(){
//     int a1[]={0,1,1,1,0,1,1,1};
//     int a2[]={0,1,1,1,0,1,1,1};
//     float oneArr[]={1.1f,0.1f,0.1f,1.1f,1.1f,0.1f,0.1f,1.1f};

//     Mat<int> x1(a1,2,2,2);
//     Mat<int> x2(a2,2,2,2);
//     Mat<float> onef(oneArr,2,2,2);
//     Mat<int> onei = onef.convert<int>();
    
//     cout<<x1;
//     cout<<x2;
//     cout<<onei;

//     cout<<"-------"<<endl;
//     x1 = x1.subMatrixAssign(2,2,2,0,0);
//     x2 = x2.subMatrixClone(2,2,2,0,0);
//     cout<<"-------"<<endl;

//     //cout<<x1;
//     //cout<<x2;

//     cout<<fast_pow(x1,7,onei);
//     //cout<<one * x1;
//     //cout<<one * x1 * x1;
//     //cout<<one * x1 * x1 * x1;
//     return 0;
// }

#include "mat.hpp"
#include <iostream>
using namespace std;
class st2{
public:
    int a,b;
};
class st1{
public:
    int a,b;
    st1(int a, int b):a(a),b(b){}
    st1 (const st2 b){
        this->a=b.b;
        this->b=b.a;
    }
    friend ostream& operator << (ostream& os, const st1 u){
        os<<"("<<u.a<<' '<<u.b<<")";
        return os;
    }
};

struct ww{
    st1 operator() (const st2 &a){
        return st1(a.b,a.a);
    }
};
int main(){
    st2 u1[1];
    u1[0]=(st2){2,1};
    Mat<st1> u1 = Mat<st2>(u1,1,1,1).convert<st1>();
    Mat<st1> u2 = Mat<st2>(u1,1,1,1).convert<st1, ww>();
    cout<<u1;
    return 0;
}