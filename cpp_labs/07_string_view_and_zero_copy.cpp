#include <cstddef>
#include <cstring>
#include<iostream>

class MyStringRef{
    private:
        const char* data_{nullptr};
        size_t length_{0};
    public:
        MyStringRef()=default;
        MyStringRef(const char* str):data_(str),length_(std::strlen(str)){}
        size_t size(){return length_;}
        bool empty(){return length_==0;}
        const char* data() const { return data_; }
        char operator[](size_t idx) const { return data_[idx]; }

        const char* get()const{return data_;}//简易输出，在substr后不会真正正切断
        MyStringRef(const char*data,const size_t length):data_(data),length_(length){}
        MyStringRef substr(size_t start,size_t end)const{return MyStringRef(data_+start,end-start+1);}
        MyStringRef drop_front(size_t n) const{return substr(n, length_-n);}

        
};


int main(){
    MyStringRef view("func @toy_func() -> f32");
    for (size_t i = 0; i < view.size(); ++i) std::cout << view.drop_front(i).get()<<"\n";
    return 0;
}