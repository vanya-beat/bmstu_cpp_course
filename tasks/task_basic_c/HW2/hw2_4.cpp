#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <unordered_set>
#include <charconv>

class String {
 public:
/// Конструктор по умолчанию
  String() = default;
/// Конструктор с параметром "cи строкой"
  String(const char* c_str) : size_(strlen(c_str)) {
    str_ = new char[size_ + 1];
    std::copy(c_str, c_str + size_, str_);
    str_[size_] = '\0';
  }

/// Копирующий конструктор
  String(const String& other) : str_(new char[other.size_ + 1]), size_(other.size_) {
    std::copy(other.str_, other.str_ + other.size_, str_);
    str_[size_] = '\0';
  }
/// Конструктор перемещения
  String(String&& other) noexcept {
    *this = std::move(other);
  }

/// Деструктор
  ~String() {
    delete[] str_;
  }
/// Геттер на си-строку
const char *c_str() const { 
    return str_;
}

/// Геттер на размер
size_t size() const {
    return size_;
}

/// Оператор копирующего присваивания
    String& operator=(const String& other) {
    if (this == &other) {
      return *this;
    }
    String c_str = other;
    std::swap(*this, c_str);
    return *this;
  }

/// Оператор перемещающего присваивания
    String& operator=(String&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    std::swap(str_, other.str_);
    std::swap(size_, other.size_);

    return *this;
  }

  /// Оператор присваивания си строки
   

/*Все все понимают*/


  friend std::ostream& operator<<(std::ostream& os, const String& obj) {
    return os << obj.str_;
  }

 friend std::istream& operator>>(std::istream &is, String &obj) {
        const char *o;
     //   unsigned long int i;
        String stri;
        std::cin >> stri;
        o = stri.c_str();                                 
        strcpy(obj.str_, o);
        return is;
    }

  String& operator+=(const String& other) {
    char* new_str = new char[size_ + other.size_ + 1];
    std::copy(str_, str_ + size_, new_str);
    std::copy(other.str_, other.str_ + other.size_ + 1, new_str + size_);

    delete[] str_;
    str_ = new_str;
    size_ += other.size_;

    return *this;
  }

  const String operator+(const String& other) const {
    String c_str = *this;
    return c_str += other;
  }

  char& operator[](size_t i) {
    return str_[i];
  }
  
  const String operator-() const {
    // Or some more complex logic
    return {};
  }

  const String operator-(const String& other) const {
    return *this + (-other);
  }

  String& operator++() {
    return *this += String(" ");
  }

  const String operator++(int) {
    String c_str = *this;
    *this += String(" ");
    return c_str;
  }


  bool operator<(const String& other) const {
    for (size_t i = 0; i < std::min(size_, other.size_) + 1; ++i) {
      if (str_[i] >= other.str_[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator>(const String& other) const {
    return other < *this;
  }

  bool operator>=(const String& other) const {
    return !(other < *this);
  }

  bool operator<=(const String& other) const {
    return !(other > *this);
  }

  bool operator!=(const String& other) const {
    return *this < other || *this > other;
  }

  bool operator==(const String& other) const {
    return !(*this != other);
  }

  const char& operator[](size_t i) const {
    return str_[i];
  }

  explicit operator std::string() const {
    return {str_};
  }

 private:
  char* str_ = nullptr;
  size_t size_ = 0;

  friend std::hash<String>;
};


class Functor {
 public:
  size_t my_i;

  void operator()() const {
    std::cout << "I call operator(). " << my_i << std::endl;
  }

  void operator()(size_t i, const std::string& c_str, ...) {
    my_i = i;
    std::cout << "I call operator() with many parameters. " << i << ' ' << c_str << std::endl;
  }
};



template<> struct std::hash<String> {
  size_t operator()(const String& s) const {
    return std::hash<char*>{}(s.str_);
  }
};

template<typename T>
class AutoPtr {
 public:
  template<typename... Args>
  explicit AutoPtr(Args... args) : ptr_(new T(std::forward<Args...>(args)...)) {
  }

  explicit AutoPtr(T* arg) : ptr_(arg) {
  }

  AutoPtr(const AutoPtr&) = delete;
  AutoPtr(AutoPtr&&) = delete;
  AutoPtr& operator=(const AutoPtr&) = delete;
  AutoPtr& operator=(AutoPtr&&) = delete;

  ~AutoPtr() {
    delete ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

 private:
  T* ptr_;
};

void func(String a) {
  std::cout << a << std::endl;
}

struct Test {
  struct Inner {
    std::string ptr_{"abob"};

    std::string* operator->() {
      return &ptr_;
    }
  } in;

  Inner& operator->() {
    return in;
  }
};


int main() {
  String s1;
  String s2{"Aboba"};

  Functor a;
  a(1, "2", 3.);
  a();
  std::unordered_set<String> some_set2;


  AutoPtr<String> p("boba");
  std::cout << *p << std::endl;


  Test comp;
  std::cout << comp->data() << std::endl;

  return 0;
}