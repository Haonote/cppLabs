#pragma once
#include <cstddef>
#include <new>          // 提供 placement new: new (ptr) T(...)
#include <cassert>
#include <type_traits>

template <typename T, size_t N>
class SmallVector {
public:
    // =========================================================================
    // 1. 构造函数
    // =========================================================================
    SmallVector() {
        // TODO 1: 初始化成员变量
        size_=0;
        capacity_=N;
        begin_=inline_buffer();
        // - size_ 应该初始化为多少？
        // - capacity_ 初始容量是多少？
        // - begin_ 应该指向哪里？（提示：指向内部的 inline_storage_）
    }

    // =========================================================================
    // 2. 析构函数 (重要：手动管理对象生命周期)
    // =========================================================================
    ~SmallVector() {
        
        // TODO 2: 思考并实现
        // 1. 当前容器里有 size_ 个有效对象，如何显式调用它们的析构函数？
        // 2. 提示：对于每个元素，使用 ptr->~T() 语法

        // =========================================================================.    
        //经典大坑：size_t 无符号数倒序遍历陷阱.定义的变量是 size_t i，size_t 是无符号整数（Unsigned Integer）：
       //下溢暗坑 1：如果当前容器是空的（size_ == 0），size_ - 1 会发生无符号数下溢，变成极其庞大的数字（例如 64 位下的 1844674407370955161518446744073709551615），直接越界崩溃！
       //死循环暗坑 2：因为无符号数永远不可能小于 0，所以 i >= 0 条件永远为真（永不退出循环）！当 i = 0 执行 --i 后，i 再次下溢变成极大值，导致无限死循环并引发段错误（Segmentation Fault）。
        //for(size_t i=size_-1;i>=0;--i){
         //   begin_[i].~T();
        //}
        // =========================================================================.    
        for (size_t i = size_; i > 0; --i) {
            begin_[i - 1].~T();
        }   

    }

    // =========================================================================
    // 3. 基础只读与访问接口
    // =========================================================================
    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] size_t capacity() const { return capacity_; }
    [[nodiscard]] bool empty() const { return size_ == 0; }

    // 下标访问操作符
    T& operator[](size_t index) {
        assert(index < size_ && "Index out of bounds");
        return begin_[index];
    }
    const T& operator[](size_t index) const {
        assert(index < size_ && "Index out of bounds");
        return begin_[index];
    }

    // =========================================================================
    // 4. 简易元素添加 (MVP 1.0 只处理 <= N 的情况)
    // =========================================================================
    void push_back(const T& value) {
        assert(size_ < capacity_ && "MVP 1.0: Out of inline storage!");
        
        new (begin_ + size_) T(value); // 使用 placement new 在内存中构造对象
        ++size_; // 更新 size_，表示新元素已添加
        // TODO 4: 如何利用 placement new 在 begin_ + size_ 处构造新对象？
        // 构造完成后，size_ 应该怎么变？
    }

private:
    // =========================================================================
    // 内部辅助函数
    // =========================================================================
    // 将 raw byte 数组的起始地址转换为 T* 类型指针
    T* inline_buffer() {
        // TODO 5: 将 inline_storage_ 转换为 T* 并返回
        return reinterpret_cast<T*>(inline_storage_);
    }

    const T* inline_buffer() const {
        return reinterpret_cast<const T*>(inline_storage_);
    }
    bool is_small() const{
        return begin_ == inline_buffer();
    }

private:
    // =========================================================================
    // 成员变量布局
    // =========================================================================
    T* begin_ = nullptr;        // 指向数据起始位置的指针
    size_t size_ = 0;           // 当前实际元素个数
    size_t capacity_ = 0;       // 当前最大容量

    // 关键点：按 T 的对齐要求开辟 N 个 T 大小的未初始化原始内存
    alignas(alignof(T)) char inline_storage_[N * sizeof(T)];
};