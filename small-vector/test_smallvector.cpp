#include "smallvector.h"
#include <cstddef>
#include <iostream>

using namespace std;

struct Tracer {
  std::string name;
  Tracer(std::string n) : name(std::move(n)) {
    std::cout << "  [Tracer Ctor]: " << name << '\n';
  }
  Tracer(const Tracer &o) : name(o.name) {
    std::cout << "  [Tracer Copy Ctor]: " << name << '\n';
  }
  Tracer(Tracer &&o) noexcept : name(std::move(o.name)) {
    std::cout << "  [Tracer Move Ctor]: " << name << '\n';
  }
  ~Tracer() {
    if (name.empty()) {
      std::cout << "  [Tracer Dtor]: <moved-out empty>\n";
    } else {
      std::cout << "  [Tracer Dtor]: " << name << '\n';
    }
  }
};
void testFunc() {
  SmallVector<Tracer, 2> vec;
  vec.emplace_back("A");
  vec.emplace_back("B");
  // 插入第 3 个，观察是否触发了非平凡类型的移动搬迁（搬运元素 + 旧地址析构）
  vec.emplace_back("C");

  // 测试范围 for 循环
  for (const auto &item : vec) {
    std::cout << "Item: " << item.name << '\n';
  }

  vec.pop_back(); // 观察 C 是否被独立析构
}

void testFunc2() {
  // 测试 1：栈模式移动（元素逐个被 move）
  SmallVector<Tracer, 4> v1;
  v1.emplace_back("X");
  SmallVector<Tracer, 4> v2 = std::move(v1);

  // 测试 2：堆模式移动（1 纳秒偷指针，不触发任何 Tracer 的构造/析构！）
  SmallVector<Tracer, 2> v3;
  v3.emplace_back("Y1");
  v3.emplace_back("Y2");
  v3.emplace_back("Y3"); // 扩容到堆
  std::cout << "--- 开始堆移动 ---\n";
  SmallVector<Tracer, 2> v4 =
      std::move(v3); // 观察是否 0 次 Tracer 构造与析构！
}
int main() {
  testFunc2();
  return 0;
}