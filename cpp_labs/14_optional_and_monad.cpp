#include <iostream>
#include <utility>

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
  ~Tracer() { std::cout << "  [Tracer Dtor]: " << name << '\n'; }
};

template <class T> class MyOptional {
private:
  bool has_value_ = false;
  alignas(alignof(T)) std::byte storage_[sizeof(T)];

  T *ptr() { return reinterpret_cast<T *>(storage_); }
  const T *ptr() const { return reinterpret_cast<const T *>(storage_); }

public:
  bool has_value() const { return has_value_; }
  explicit operator bool() const { return has_value_; }
  T &value() { return *ptr(); }
  T &operator*() { return *ptr(); }
  T *operator->() { return ptr(); }

  void reset() {
    if (has_value()) {
      ptr()->~T();
      has_value_ = false;
    }
  }

  template <typename... Args> void emplace(Args &&...args) {
    reset();
    new (ptr()) T(std::forward<Args>(args)...);
    has_value_ = true;
  }

  MyOptional() = default;
  MyOptional(MyOptional &&other) noexcept {
    if (other.has_value()) {

      new (ptr()) T(std::move(*other.ptr()));

      has_value_ = true;
      other.reset();
    }
  }
  ~MyOptional() { reset(); }
};

int main() {
  MyOptional<Tracer> ot1;
  ot1.emplace("ObjectA");
  MyOptional<Tracer> ot2 = std::move(ot1);
  //   if (ot2) {
  //     std::cout << "yes";
  //     ot2 + 10;
  //   }test explicit
  ot2.reset();

  return 0;
}