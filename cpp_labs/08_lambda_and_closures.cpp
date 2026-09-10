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
        MyStringRef(const char*data,const size_t length):data_(data),length_(length){}

        size_t size()const{return length_;}
        bool empty(){return length_==0;}
        const char* data() const { return data_; }
        char operator[](size_t idx) const { return data_[idx]; }
        const char* get()const{return data_;}//简易输出，在substr后不会真正正切断
        
        MyStringRef substr(size_t start,size_t end)const{return MyStringRef(data_+start,end-start);}
        MyStringRef drop_front(size_t n) const{return substr(n, length_-n);}

        friend inline std::ostream& operator<<(std::ostream& os, const MyStringRef& ref) {
            os.write(ref.data(), ref.size());
            return os;
        } 
};

template <typename Callback>
void splitAndForEach(MyStringRef text, char delimiter, Callback callback) {
    // 算法逻辑：
    // 1. 用 start 标记当前 token 的起始下标；
    // 2. 遍历 i 从 0 到 text.size()；
    // 3. 当 text[i] == delimiter 时：
    //    - 如果 i > start，用 substr 截取出非空的 token 视图；
    //    - 调用 callback(token)；
    //    - 将 start 更新为 i + 1；
    // 4. 循环结束后，处理最后一个 token（如果 start < text.size()）。
    const size_t n=text.size();
    size_t start=0;
    for(size_t i=0;i<n;++i){
        if(text[i]==delimiter){
            if(i>start){
                callback(text.substr(start,i));
            }
            start=i+1;
        }
    }
    if(start<n){
        callback(text.substr(start,n));
    }
}

void test01(){
    MyStringRef ir = "toy.print %arg0 %arg1 %result";
    int count=0;
    splitAndForEach(ir,' ',[&count](MyStringRef token){
        std::cout<<token<<'\n';
        ++count;
    });
    std::cout<<count<<'\n';
}
void test02(){
    MyStringRef ir = "toy.print %arg0 %arg1 %result";
    int count=0;
    splitAndForEach(ir,' ',[&count,index=0](MyStringRef token)mutable{
        std::cout<<index<<":"<<token<<'\n';
        ++count;
        ++index;
    });
    std::cout<<count<<'\n';
}
void test03(){
    int count=0;
    int cnt=0;
    size_t lambda_size1= sizeof([&](){
        return 0;
    });
    size_t lambda_size2=sizeof([&cnt](){
        return cnt;
    });
    std::cout<<lambda_size1<<'\n';
    std::cout<<lambda_size2<<'\n';
}
int main(){
    std::cout<<"test01"<<"\n";
    test01();
    std::cout<<"test02"<<"\n";
    test02();
    std::cout<<"test03"<<"\n";
    test03();
    return 0;   
}

