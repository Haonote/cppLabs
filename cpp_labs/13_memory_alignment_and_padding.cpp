#include <iostream>

#include <cstddef> // 必须包含 offsetof 所在的头文件

struct badLayout {
  char a;
  double b;
  char c;
};

struct goodLayout {
  double b;
  char a;
  char c;
};

template <typename T, size_t N> struct MockSmallVector {
  T *begin_{nullptr};
  size_t size_{0};
  size_t capacity_{0};
  alignas(alignof(T)) std::byte inline_storage_[N * sizeof(T)];
};

struct alignas(32) AVXData {
  double v[4];
};

int main() {
  std::cout << "sizeof(badLayout): " << sizeof(badLayout) << std::endl;
  std::cout << "sizeof(goodLayout): " << sizeof(goodLayout) << std::endl;

  std::cout << "-----------------------------" << std::endl;

  MockSmallVector<int, 4> vec;
  // 使用 decltype(vec)，避开宏逗号展开问题
  std::cout << "sizeof(MockSmallVector<int, 4>): " << sizeof(vec) << std::endl;
  std::cout << "offset of begin_: " << offsetof(decltype(vec), begin_)
            << std::endl;
  std::cout << "offset of size_: " << offsetof(decltype(vec), size_)
            << std::endl;
  std::cout << "offset of capacity_: " << offsetof(decltype(vec), capacity_)
            << std::endl;
  std::cout << "offset of inline_storage_: "
            << offsetof(decltype(vec), inline_storage_) << std::endl;

  std::cout << "-----------------------------" << std::endl;

  MockSmallVector<char, 5> vec_char;
  std::cout << "sizeof(MockSmallVector<char, 5>): " << sizeof(vec_char)
            << std::endl;
  std::cout << "offset of begin_: " << offsetof(decltype(vec_char), begin_)
            << std::endl;
  std::cout << "offset of size_: " << offsetof(decltype(vec_char), size_)
            << std::endl;
  std::cout << "offset of capacity_: "
            << offsetof(decltype(vec_char), capacity_) << std::endl;
  std::cout << "offset of inline_storage_: "
            << offsetof(decltype(vec_char), inline_storage_) << std::endl;

  std::cout << "-----------------------------" << std::endl;

  MockSmallVector<AVXData, 3> vec_avx;
  std::cout << "sizeof(MockSmallVector<AVXData, 3>): " << sizeof(vec_avx)
            << std::endl;
  std::cout << "offset of begin_: " << offsetof(decltype(vec_avx), begin_)
            << std::endl;
  std::cout << "offset of size_: " << offsetof(decltype(vec_avx), size_)
            << std::endl;
  std::cout << "offset of capacity_: " << offsetof(decltype(vec_avx), capacity_)
            << std::endl;
  std::cout << "offset of inline_storage_: "
            << offsetof(decltype(vec_avx), inline_storage_) << std::endl;

  return 0;
}