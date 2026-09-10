#include<iostream>
void sink(int& x)        { std::cout << "sink(int&): non-const lvalue\n"; }
void sink(const int& x)  { std::cout << "sink(const int&): const lvalue\n"; }
void sink(int&& x)       { std::cout << "sink(int&&): rvalue\n"; }
void sink(const int&& x) { std::cout << "sink(const int&&): const rvalue\n"; }

template <typename T>
void badRelay(T&& arg) {
    std::cout << "[badRelay]  ";
    sink(arg); // 直接传递，观察由于 arg 有名字导致的“右值退化为左值”陷阱
}

template <typename T>
void goodRelay(T&& arg) {
    std::cout << "[goodRelay] ";
    sink(std::forward<T>(arg)); // 使用 std::forward 实现完美转发
}

void  wrapper(int&& val){
    badRelay(val);
    goodRelay(val);
}

int main(){
    std::cout<<"-----1-----\n";
    badRelay(1);
    std::cout<<"-----2-----\n";
    goodRelay(1);
    std::cout<<"-----3-----\n";
    wrapper(5);
    std::cout<<"-----4-----\n";
    return 0;
}