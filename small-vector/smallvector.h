#pragma once
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <new> // 提供 placement new: new (ptr) T(...)
#include <type_traits>
#include <utility>

template <typename T, size_t N> class SmallVector {
public:
  // =========================================================================
  // 1. 构造函数
  // =========================================================================
  SmallVector() {
    // TODO 1: 初始化成员变量
    size_ = 0;
    capacity_ = N;
    begin_ = inline_buffer();
    // - size_ 应该初始化为多少？
    // - capacity_ 初始容量是多少？
    // - begin_ 应该指向哪里？（提示：指向内部的 inline_storage_）
  }

  // =========================================================================
  // 移动构造函数
  // =========================================================================
  SmallVector(SmallVector &&other) noexcept {
    if (!other.is_small()) {
      // 1. 如果 other 已经在堆上：直接把它的堆指针“偷”过来（零开销）
      begin_ = other.begin_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      // 把 other 复位回小栈模式，防止其析构时释放堆内存
      other.begin_ = other.inline_buffer();
      other.size_ = 0;
      other.capacity_ = N;
    } else {
      // 2. 如果 other 在栈上：栈内存无法偷指针，必须逐个 move 元素
      begin_ = inline_buffer();
      capacity_ = N;
      size_ = other.size_;
      for (size_t i = 0; i < other.size_; ++i) {
        new (begin_ + i) T(std::move(other.begin_[i]));
      }
      other.clear(); // 析构 other 的栈元素并置 size_ 为 0
    }
  }
  // =========================================================================
  // 拷贝构造函数
  // =========================================================================
  SmallVector(const SmallVector &other) {
    if (other.size_ <= N) {
      begin_ = inline_buffer();
      capacity_ = N;
    } else {
      begin_ = static_cast<T *>(::operator new(other.size_ * sizeof(T)));
      capacity_ = other.size_;
    }
    size_ = other.size_;
    // 拷贝元素
    if constexpr (std::is_trivially_copyable_v<T>) {
      std::memcpy(begin_, other.begin_, size_ * sizeof(T));
    } else {
      for (size_t i = 0; i < size_; ++i) {
        new (begin_ + i) T(other.begin_[i]);
      }
    }
  }
  // =========================================================================
  // 移动赋值运算符
  // =========================================================================
  SmallVector &operator=(SmallVector &&other) noexcept {
    if (this == &other)
      return *this; // 自赋值保护
    // 先清理当前对象的旧元素和旧堆内存
    clear();
    if (!is_small()) {
      ::operator delete(begin_);
    }
    if (!other.is_small()) {
      // 偷取 other 的堆指针
      begin_ = other.begin_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.begin_ = other.inline_buffer();
      other.size_ = 0;
      other.capacity_ = N;
    } else {
      // 在本地栈缓冲区搬迁元素
      begin_ = inline_buffer();
      capacity_ = N;
      size_ = other.size_;
      for (size_t i = 0; i < other.size_; ++i) {
        new (begin_ + i) T(std::move(other.begin_[i]));
      }
      other.clear();
    }
    return *this; // 必须返回自身引用
  }

  SmallVector &operator=(const SmallVector &) = delete;

  // =========================================================================
  // 2. 析构函数 (重要：手动管理对象生命周期)
  // =========================================================================
  ~SmallVector() {

    // TODO 2: 思考并实现
    // 1. 当前容器里有 size_ 个有效对象，如何显式调用它们的析构函数？
    // 2. 提示：对于每个元素，使用 ptr->~T() 语法

    // =========================================================================.
    // 经典大坑：size_t 无符号数倒序遍历陷阱.定义的变量是 size_t i，size_t
    // 是无符号整数（Unsigned Integer）：
    // 下溢暗坑 1：如果当前容器是空的（size_ == 0），size_ - 1
    // 会发生无符号数下溢，变成极其庞大的数字（例如 64 位下的
    // 1844674407370955161518446744073709551615），直接越界崩溃！ 死循环暗坑
    // 2：因为无符号数永远不可能小于 0，所以 i >= 0
    // 条件永远为真（永不退出循环）！当 i = 0 执行 --i 后，i
    // 再次下溢变成极大值，导致无限死循环并引发段错误（Segmentation Fault）。
    // for(size_t i=size_-1;i>=0;--i){
    //    begin_[i].~T();
    //}
    // =========================================================================.
    std::cout << "析构了" << size_ << "\n";
    for (size_t i = size_; i > 0; --i) {
      begin_[i - 1].~T();
    }

    if (!is_small()) {
      ::operator delete(begin_); // 堆模式才需要归还系统堆内存
    }
  }

  // =========================================================================
  // 3. 基础只读与访问接口
  // =========================================================================
  [[nodiscard]] size_t size() const { return size_; }
  [[nodiscard]] size_t capacity() const { return capacity_; }
  [[nodiscard]] bool empty() const { return size_ == 0; }
  T *begin() noexcept { return begin_; }
  const T *begin() const noexcept { return begin_; }
  T *end() noexcept { return begin_ + size_; }
  const T *end() const noexcept { return begin_ + size_; }

  // 下标访问操作符
  T &operator[](size_t index) {
    assert(index < size_ && "Index out of bounds");
    return begin_[index];
  }
  const T &operator[](size_t index) const {
    assert(index < size_ && "Index out of bounds");
    return begin_[index];
  }

  // =========================================================================
  // 4. 元素添加与扩容
  // =========================================================================
  void grow(size_t min_capacity) {
    T *new_begin = static_cast<T *>(::operator new(min_capacity * sizeof(T)));

    if constexpr (std::is_trivially_copyable_v<T>) {
      std::cout << "快拷了\n";
      std::memcpy(new_begin, begin_, size_ * sizeof(T));

    } else {
      std::cout << "搬运元素\n";
      for (size_t i = 0; i < size_; ++i) {
        new (new_begin + i) T(std::move(begin_[i]));
        begin_[i].~T();
      }
    }
    if (!is_small()) {
      std::cout << "释放旧内存\n";
      ::operator delete(begin_);
    }
    begin_ = new_begin;
    capacity_ = min_capacity;
  }

  template <typename... Args> void emplace_back(Args &&...args) {
    if (size_ >= capacity_) {
      std::cout << "扩容\n";
      grow(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (begin_ + size_)
        T(std::forward<Args>(args)...); // 使用 placement new 在内存中构造对象
    ++size_; // 更新 size_，表示新元素已添加
    // TODO 4: 如何利用 placement new 在 begin_ + size_ 处构造新对象？
    // 构造完成后，size_ 应该怎么变？
  }

  void pop_back() {
    assert(size_ > 0 && "Cannot pop from empty vector");
    --size_;
    begin_[size_].~T(); // 显式析构被弹出的元素
  }

  void clear() {
    for (size_t i = size_; i > 0; --i) {
      begin_[i - 1].~T();
    }
    size_ = 0;
  }

  T &front() {
    assert(!empty());
    return begin_[0];
  }
  const T &front() const {
    assert(!empty());
    return begin_[0];
  }
  T &back() {
    assert(!empty());
    return begin_[size_ - 1];
  }
  const T &back() const {
    assert(!empty());
    return begin_[size_ - 1];
  }

private:
  // =========================================================================
  // 内部辅助函数
  // =========================================================================
  // 将 raw byte 数组的起始地址转换为 T* 类型指针
  T *inline_buffer() {
    // TODO 5: 将 inline_storage_ 转换为 T* 并返回
    return reinterpret_cast<T *>(inline_storage_);
  }

  const T *inline_buffer() const {
    return reinterpret_cast<const T *>(inline_storage_);
  }
  bool is_small() const { return begin_ == inline_buffer(); }

private:
  // =========================================================================
  // 成员变量布局
  // =========================================================================
  T *begin_ = nullptr;  // 指向数据起始位置的指针
  size_t size_ = 0;     // 当前实际元素个数
  size_t capacity_ = 0; // 当前最大容量

  // 关键点：按 T 的对齐要求开辟 N 个 T 大小的未初始化原始内存
  alignas(alignof(T)) std::byte inline_storage_[N * sizeof(T)];
};