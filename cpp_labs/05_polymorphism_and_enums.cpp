#include<iostream>
#include<cstdint>

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
    private:
        int value;
};

int main(){
    ASTNode* node = new NumberNode(42);
    node->dump();
    delete node; // 触发析构函数，确保资源正确释放
    return 0;
}