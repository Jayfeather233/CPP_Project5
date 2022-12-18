//#define __DEBUG__

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
//operations example
int main(){
    cout<<"Begin of the Matrix." << endl;
    {
        int a1[]={0,1,1,1,0,1,1,1};
        int a2[]={0,1,1,1,0,1,1,1};
        float one1Arr[]={1.1f,0.1f,0.1f,0.1f,1.1f,0.1f,0.1f,0.1f};
        float one3Arr[]={1.1f,0.1f,0.1f,0.1f,1.1f,0.1f,0.1f,0.1f};
        int one2Arr[]={0,0,0,1,0,0,0,1};

        cs205::Mat<int> x1(a1,2,2,2);
        cs205::Mat<int> x2(a2,2,2,2);
        cs205::Mat<float> one1f(one1Arr,2,2,2);
        cs205::Mat<float> one3f(one3Arr,2,2,2);
        cs205::Mat<int> one2f(one2Arr,2,2,2);
        cs205::Mat<int> onei = (one1f+one2f).convert<int>();
        cout<<(one1f==one3f)<<endl;
        cout<<x1+onei;
        cout<<x1;
        cout<<x2;
        cout<<onei;

        cout<<"-------"<<endl;
        x1 = x1.subMatrixAssign(2,2,2,0,0);
        x2 = x2.subMatrixClone(2,2,2,0,0);
        cout<<"-------"<<endl;
        cout<<x1+x2;

        cout<<fast_pow(x1,7,onei);
    }
    cout<<"End of the Matrix." << endl;
    
    return 0;
}

// #include "mat.hpp"
// #include <iostream>
// using namespace std;
// class st2{
// public:
//     int a,b;
// };
// class st1{
// public:
//     int a,b;
//     st1(int a=0, int b=0):a(a),b(b){}
//     st1 (const st2 b){
//         this->a=b.b;
//         this->b=b.a;
//     }
//     friend ostream& operator << (ostream& os, const st1 u){
//         os<<"("<<u.a<<' '<<u.b<<")";
//         return os;
//     }
// };

// struct my_converter{
//     st1 operator() (const st2 &a){
//         return st1(a.b,a.a);
//     }
// };
// // Convert example
// int main(){
//     st2 u1[1];
//     u1[0]=(st2){2,1};
//     cs205::Mat<st1> x1 = cs205::Mat<st2>(u1,1,1,1).convert<st1>();// Use the default converter
//     cs205::Mat<st1> x2 = cs205::Mat<st2>(u1,1,1,1).convert<st1, my_converter>();// Use user-defined converter
//     cout<<x1;
//     return 0;
// }