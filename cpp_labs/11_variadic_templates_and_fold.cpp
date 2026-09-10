#include<iostream>
#include<new>
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

template<typename... Args>
void printTupleLike(const Args&... args){
    std::cout << "(";
    ((std::cout<<args<<" "),...);
    std::cout << ")\n";
}


template<typename T,typename...Args>
T * constructInplace(void* storage, Args&&... args) {
    std::cout<<"sizeof(args):"<<sizeof...(Args)<<'\n';
    return new(storage)T(std::forward<Args>(args)...);
}

int main() {
    Person alice("Alice", 20, 95.5);
    printTupleLike(alice.name, alice.age, alice.score);
    alignas(alignof(Person))char buffer[2*sizeof(Person)];
    Person *p = constructInplace<Person>(buffer, "Bob", 21, 98.0);
    Person*c=constructInplace<Person>(buffer+sizeof(Person),"Charlie",22,99.0);
    
    return 0;
}
