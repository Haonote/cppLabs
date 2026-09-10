#include<iostream>
template <typename T, size_t N> 
class FixedBuffer{
    private:
        T data_[N];
        size_t size_{0};
    
    public:
        void push(const T& val){
            if(this->size()<N){
                data_[size_++]=val;
            }else{
                std::cout<<"data is full!\n";
            }
        }
        size_t size() const { return size_; }
        size_t capacity() const { return N; }
        const T& operator[](size_t idx) const { return data_[idx]; }

};

template <typename T>
struct TypeDescriptor {
    static void describe() { std::cout << "General Value Type\n"; }
};

template <>
struct TypeDescriptor<int> {
    static void describe() { std::cout << "int Type\n"; }
};

template <typename T>
struct TypeDescriptor<T*> {
    static void describe() { std::cout << "Pointer to T Type\n"; }
};


template <typename T,size_t N>
struct TypeDescriptor<T[N]>{
    static void describe() { std::cout << "Type: Fixed-size Array of length " << N<<"\n"; }
};

template <typename T>
void printValue(const T& val) {
    std::cout << "Value: " << val << '\n';
};

template <typename T>
void printValue(T* ptr) {
    if (ptr) {
        std::cout << "Pointer dereferenced: " << *ptr << '\n';
    } else {
        std::cout << "Null pointer\n";
    }
};


int main(){
    FixedBuffer<int,5> buffer;
    buffer.push(1);
    buffer.push(7);
    buffer.push(121);
    buffer.push(1);
    buffer.push(7);
    TypeDescriptor<decltype(buffer)>::describe();
    TypeDescriptor<int*>::describe();
    TypeDescriptor<double*>::describe();
    TypeDescriptor<int[5]>::describe();
    TypeDescriptor<double[]>::describe();
    return 0;
}