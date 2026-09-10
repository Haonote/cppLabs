#include <iostream>
#include <cstring>
#include <type_traits>
#include <utility>

struct Person {
    std::string name;
    int age;
    double score;

    Person(std::string n, int a, double s) 
        : name(std::move(n)), age(a), score(s) {
        std::cout << "Person constructed: " << name << ", Age=" << age << ", Score=" << score << '\n';
    }
};

template <typename T>
void relocateBuffer(T* dest, T* src, size_t count) {
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::cout << "[Fast Path]: Trivially copyable! Using memcpy.\n";
        std::memcpy(dest, src, count * sizeof(T));
    } else {
        std::cout << "[Safe Path]: Non-trivial type! Using move & explicit destruction.\n";
        for (size_t i = 0; i < count; ++i) {
            new (dest + i) T(std::move(src[i])); // Placement new 移动构造到新地址
            src[i].~T();                         // 析构原地址上的旧对象
        }
    }
}

template <typename T>
void destroyBuffer(T* buffer, size_t count) {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::cout << "Destroying " << count << " non-trivial objects.\n";
        for (size_t i = 0; i < count; ++i) {
            buffer[i].~T();
        }
    } else {
        std::cout << "Trivially destructible: Zero destruction overhead!\n";
    }
}

void testInt(){
    alignas(alignof(int))std::byte src[3*sizeof(int)];
    alignas(alignof(int))std::byte dest[3*sizeof(int)];
    int *s=reinterpret_cast<int*>(src);
    int *d=reinterpret_cast<int*>(dest);
    for(size_t i=0;i<3;i++){
        s[i]=i+1;
    }
    relocateBuffer(d,s,3);
    destroyBuffer(d,3);
}

void testPerson(){
    alignas(alignof(Person))std::byte src[3*sizeof(Person)];
    alignas(alignof(Person))std::byte dest[3*sizeof(Person)];
    Person *s=reinterpret_cast<Person*>(src);
    Person *d=reinterpret_cast<Person*>(dest);
    for(size_t i=0;i<3;i++){
        new (s + i) Person("Alice", 20, 95.5);
    }
    relocateBuffer(d,s,3);
    destroyBuffer(d,3);
}

int main(){
    testInt();
    testPerson();
   
    return 0;
}