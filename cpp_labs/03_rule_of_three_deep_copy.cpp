#include<iostream>
#include<utility>
#include <algorithm>
using namespace std;

class MyBuffer{
public:
    explicit MyBuffer(size_t size) : size_(size) ,data_(new int[size_]) {
        //编译器报出：field 'size_' will be initialized after field 'data_'。
       //C++ 底层铁律：类成员初始化的先后顺序，完全取决于它们在类体中的【声明顺序】，而不是你在初始化列表里写的顺序！

       //std::move 只对拥有堆资源、文件句柄等管理性对象（如 std::string, std::vector, 指针包装类）有意义。基础类型直接 size_(size) 即可。
    }

    ~MyBuffer() {
        delete[] data_;
        data_=nullptr;
        cout<<"Freed buffer of size: " << size_<<"\n";
    } 
    MyBuffer(const MyBuffer& other) : size_(other.size_), data_(new int[size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
    MyBuffer& operator=(const MyBuffer& other){
        if(this !=&other){
            delete[] data_;
            size_=other.size_;
            data_=new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

private:
    size_t size_{0};    
    int* data_{nullptr};
};

int main(){
    MyBuffer buffer(10); // 创建一个 MyBuffer 对象，大小为 10
    MyBuffer buffer2(20); // 创建另一个 MyBuffer 对象，大小为 20
    buffer2 = buffer; // 使用拷贝赋值运算符，将 buffer 的
    MyBuffer copyBuffer(buffer); // 尝试拷贝构造，将会导致编译错误
    MyBuffer copy=buffer; // 尝试拷贝赋值，将会导致编译错误
    return 0;
}