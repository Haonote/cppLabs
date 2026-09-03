#include<iostream>
#include<cstdint>
#include <utility>

enum class NodeKind : uint8_t { Number, BinaryOp };
class ASTNode{
    private:
        NodeKind kind_;
    public:
        explicit ASTNode(NodeKind kind) : kind_(kind) {}
        virtual ~ASTNode(){
            std::cout<<"Destroying ASTNode of kind: "<<static_cast<int>(kind_)<<"\n";
        }
        virtual void dump() const = 0; // 纯虚函数，要求派生类必须实现
        NodeKind getKind() const { return kind_; }
};

class NumberNode final : public ASTNode{
    public:
        explicit NumberNode(int value): ASTNode(NodeKind::Number), value(value) {}
        void dump() const override{
            std::cout<<"NumberNode with value: "<<value<<"\n"; 
        }
        ~NumberNode() override{
            std::cout<<"Destroying NumberNode with value: "<<value<<"\n";
        }
        int get(){return value;}
    private:
        int value;
};


template<class T>
class MyUniquePtr{
    private:
        T* ptr_{nullptr};
    public:
        explicit MyUniquePtr(T* ptr = nullptr):ptr_(ptr){}
        ~MyUniquePtr(){
            if(ptr_!=nullptr){
                delete ptr_;
                std::cout<<"delete ptr_\n";
            }
        }

        MyUniquePtr(const MyUniquePtr&)=delete;
        MyUniquePtr& operator=(const MyUniquePtr&)=delete;

        MyUniquePtr(MyUniquePtr&& otherPtr) noexcept:ptr_(otherPtr.ptr_){
            otherPtr.ptr_=nullptr;
        }
        MyUniquePtr& operator=(MyUniquePtr&&otherPtr)noexcept{
            if(this!=&otherPtr){
                delete ptr_;
                ptr_=otherPtr.ptr_;
                otherPtr.ptr_=nullptr;
            }
            return *this;
        }

        T* operator->()const {return ptr_;}
        T* get() const { return ptr_; }
        T& operator*() const { return *ptr_; }
};

int main(){
    {
        MyUniquePtr<NumberNode>p1(new NumberNode(10));
        p1->dump();
        std::cout<<"p1 "<<p1->get()<<"\n";
        MyUniquePtr<NumberNode>p2 = std::move(p1);
        std::cout<<"p2 "<<p2->get();
        
    }
    return 0;
}