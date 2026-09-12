# cppLabs：现代 C++ 进阶与高性能底层基础设施实战

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-064F8C.svg?style=flat&logo=cmake)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Apache%202.0-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Linux-lightgrey.svg?logo=linux)]()

> **从现代 C++ 底层内存模型、类型系统与泛型元编程，到手写工业级 LLVM 风格高阶数据结构（SmallVector）的硬核演练场。**

---

## 📌 项目定位

`cppLabs` 是一个系统性、阶梯式的现代 C++（C++17 / C++20）底层原理与工程实战代码仓库。

项目摆脱浮于表面的 API 调用，直击 C++ 在 **CPU 缓存、内存布局、对象生命周期、零拷贝抽象与编译期计算** 上的底层实现，通过 **14 个渐进式 Labs** 筑牢内功，并最终落地于 **`small-vector` 工程实践**——从零手写支持小对象优化（SSO, Small Size Optimization）的高性能向量容器。

---

## 🗺️ 实验学习路线图 (Roadmap)

整个仓库由浅入深，分为五大演进模块：

```
[模块 1: 内存与生命周期基石] (Lab 01-04)
        │
        ▼
[模块 2: 所有权哲学与零拷贝] (Lab 05-07)
        │
        ▼
[模块 3: 泛型、闭包与元编程] (Lab 08-12)
        │
        ▼
[模块 4: 对齐与未初始化内存] (Lab 13-14)
        │
        ▼
[终局实践: 工业级 SmallVector] (small-vector/)
```

### 模块 1：内存与对象生命周期基石 (Labs 01 - 04)
- **Lab 01 · 引用与值类别 (`01_references_and_value_categories.cpp`)**：剖析左值 (lvalue)、将亡值 (xvalue)、纯右值 (prvalue)，理解临时对象生命周期与引用折叠。
- **Lab 02 · RAII 与资源管理 (`02_class_raii_and_explicit.cpp`)**：资源获取即初始化哲学，`explicit` 防止隐式转换陷阱。
- **Lab 03 · 三法则与深拷贝 (`03_rule_of_three_deep_copy.cpp`)**：析构、拷贝构造与拷贝赋值三位一体，指针资源的安全深拷贝实现。
- **Lab 04 · 五法则与移动语义 (`04_rule_of_five_and_move.cpp`)**：右值引用、`std::move` 资源偷取、移动构造与移动赋值运算符的自赋值安全处理。

### 模块 2：所有权哲学与零拷贝抽象 (Labs 05 - 07)
- **Lab 05 · 多态设计与强类型枚举 (`05_polymorphism_and_enums.cpp`)**：虚函数表 (vtable) 运行时多态机制与 `enum class` 类型安全。
- **Lab 06 · 智能指针与所有权语义 (`06_smart_pointers_and_ownership.cpp`)**：`std::unique_ptr` 独占所有权、`std::shared_ptr` 控制块引用计数机制与 `std::weak_ptr` 循环引用破局。
- **Lab 07 · 零拷贝字符串视图 (`07_string_view_and_zero_copy.cpp`)**：基于 `std::string_view` 的非拥有型内存切片与零分配高性能字符串解析。

### 模块 3：闭包与泛型元编程 (Labs 08 - 12)
- **Lab 08 · Lambda 闭包与零拷贝分词 (`08_lambda_and_closures.cpp`)**：解构编译器为 Lambda 生成的匿名仿函数结构体、值/引用/移动捕获的内存本质与 `mutable`。
- **Lab 09 · 模板全特化与偏特化 (`09_templates_and_specialization.cpp`)**：函数/类模板特化规则与编译期模式匹配。
- **Lab 10 · 万能引用与完美转发 (`10_universal_reference_and_perfect_forwarding.cpp`)**：`T&&` 引用折叠推导规则与 `std::forward<T>` 的类型保真传递。
- **Lab 11 · 可变参数模板与折叠表达式 (`11_variadic_templates_and_fold.cpp`)**：形参包展开模式、一元/二元折叠表达式的高性能打印与累加。
- **Lab 12 · Type Traits 与 if constexpr (`12_type_traits_and_if_constexpr.cpp`)**：SFINAE、`<type_traits>` 类型反射与 C++17 `if constexpr` 编译期静态分支剪枝。

### 模块 4：底层内存对齐与生命周期高级技巧 (Labs 13 - 14)
- **Lab 13 · 内存对齐与 Padding 优化 (`13_memory_alignment_and_padding.cpp`)**：CPU Cache Line 亲和性、`alignas` / `alignof` 强制对齐、结构体字段重排防内存膨胀。
- **Lab 14 · 手写纯栈上 MyOptional (`14_optional_and_monad.cpp`)**：基于 `std::byte` 未初始化栈内存的 `MyOptional<T>`，运用 placement new 手动管理对象生命周期与 Monad 式链式调用 (`and_then`)。

---

## 🚀 终局实战：LLVM 风格 SmallVector (`small-vector/`)

工业级编译器（如 LLVM / Clang）和图形渲染引擎普遍青睐 **SmallVector** 容器。它在容量 $\le N$ 时完全内联在栈上，超出容量时无缝转为堆内存分配，从而最大化利用 CPU 局部性并消除堆分配开销。

### 核心技术亮点

| 技术维度 | 实现细节 |
| :--- | :--- |
| **内存布局 (SSO)** | `alignas(alignof(T)) std::byte inline_storage_[N * sizeof(T)]` 严格栈对齐，指针 `begin_` 灵活在内联缓冲区与堆空间之间切换。 |
| **生命周期安全** | 逆序手动析构元素，**严密规避 `size_t` 无符号下溢导致的死循环与越界崩溃陷阱**。 |
| **堆/栈差异化移动** | 堆模式 $O(1)$ **零开销偷取堆指针**（零构造、零析构）；栈模式逐个 placement new 移动构造。 |
| **平凡类型快路优化** | 借助 `if constexpr (std::is_trivially_copyable_v<T>)`，对基本/平凡类型直接调用 `std::memcpy` 进行极速内存快拷。 |
| **标准容器 API** | 完美转发原位构造 `emplace_back(Args&&...)`、2 倍几何扩容机制 `grow`、`pop_back`、`clear`、`operator[]` 及范围 for 循环迭代器接口。 |
| **生命周期严密追踪** | 提供 `Tracer` 测试类，实时追踪并验证非平凡对象在搬迁、析构和移动时的生命周期状态。 |

---

## 📂 仓库目录结构

```text
.
├── CMakeLists.txt                              # 根构建配置
├── LICENSE                                     # Apache-2.0 开源协议
├── README.md                                   # 仓库全景介绍
├── cpp_labs/                                   # 14 个进阶核心实验源码
│   ├── 01_references_and_value_categories.cpp
│   ├── 02_class_raii_and_explicit.cpp
│   ├── 03_rule_of_three_deep_copy.cpp
│   ├── 04_rule_of_five_and_move.cpp
│   ├── 05_polymorphism_and_enums.cpp
│   ├── 06_smart_pointers_and_ownership.cpp
│   ├── 07_string_view_and_zero_copy.cpp
│   ├── 08_lambda_and_closures.cpp
│   ├── 09_templates_and_specialization.cpp
│   ├── 10_universal_reference_and_perfect_forwarding.cpp
│   ├── 11_variadic_templates_and_fold.cpp
│   ├── 12_type_traits_and_if_constexpr.cpp
│   ├── 13_memory_alignment_and_padding.cpp
│   ├── 14_optional_and_monad.cpp
│   └── CMakeLists.txt                          # 实验子工程构建配置
├── small-vector/                               # 工业级 SmallVector 终局项目
│   ├── CMakeLists.txt                          # C++20 构建配置
│   ├── smallvector.h                           # 完整的 SmallVector 模板类实现
│   └── test_smallvector.cpp                    # 单元测试与 Tracer 生命周期验证
└── documents/                                  # 核心技术文档与学习记录沉淀
    ├── cpp_learning_conversation_history.md    # 完整实验历程与深度答疑记录
    ├── cpp_reference_and_forwarding_qa.md      # 引用折叠与完美转发专项讲义
    └── cpp_variadic_pack_expansion_guide.md    # 形参包展开模式与折叠表达式手册
```

---

## 🛠️ 构建与运行指南

### 环境依赖
- 编译器：支持 **C++20** 的编译器（GCC 10+ 或 Clang 11+）
- 构建工具：CMake 3.15+

### 1. 运行 `small-vector`
```bash
cd small-vector
mkdir -p build && cd build
cmake ..
make
./testsmallvector
```

### 2. 批量构建与运行基础 Labs (`cpp_labs`)
```bash
cd cpp_labs
mkdir -p build && cd build
cmake ..
make

# 运行指定实验（例如 Lab 14）
./14lab
```

### 3. 单文件直接快速编译（以 Lab 13 为例）
```bash
g++ -std=c++20 -Wall -Wextra cpp_labs/13_memory_alignment_and_padding.cpp -o lab13 && ./lab13
```

---

## 📄 开源协议

本项目基于 [Apache License 2.0](LICENSE) 协议开源。
