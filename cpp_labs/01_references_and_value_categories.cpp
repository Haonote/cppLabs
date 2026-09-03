#include<iostream>
#include<utility>
using namespace std;


void printType(int& x){
    cout<<"Non-const Lvalue Reference"<<endl;
    return;
}

void printType(const int& x){
    cout<<"Const Lvalue Reference"<<endl;
    return;
}

void printType(int&& x){
    cout<<"Rvalue Reference"<<endl;
     // printType(x); // 猜猜看？ x进入函数作为形参可以作为左值
    return; 
}



int main(){
    int val=10;
    printType(val);
    printType(20);
    const int cval=30;
    printType(cval);
    printType(std::move(val));
    return 0;
}