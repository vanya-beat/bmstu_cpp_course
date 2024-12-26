#include <iostream>
#include <cstring>
#include <utility>
#include <vector>
#include <unordered_set>
#include <charconv>

struct StringHash;
class String {
 public:
/// Конструктор по умолчанию
  String() = default;
  /*String() : size_(0), c_str(new char[1]{'\0'}) {}*/
/// Конструктор с параметром "cи строкой"
  String(const char* s) : size_(strlen(s)) {
    str_ = new char[size_ + 1];
    std::copy(s, s + size_, str_);
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
// Оператор копирующего присваивания
  String& operator=(const String& other) {
    if (this == &other) {
      return *this;
    }

    String s = other;
    std::swap(*this, s);

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
String& operator=(const char* c_str) {
  return *this;
}
/*Все все понимают*/
/*friend bmstu::string operator+(const string &left, const string &right) {}*/
friend String operator+(const String& left, const String& right) {
    String ts = left;
    return ts += right;
}

friend std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.str_;
  }

friend std::istream& operator>>(std::istream& is, String& obj) {
const char *o;
  String stri;
  std::cin >> stri;
  o = stri.c_str(); 
  strcpy(obj.str_, o);
  return is;
}

String& operator+=(const char& symbol) {
    char* new_str = new char[size_ + 2];
    new_str[size_+1] = symbol;
    str_ = new_str;
    size_ += 1;
    return *this;
}

char& operator[](size_t i) {
    return str_[i];
  }

  const char& operator[](size_t i) const {
    return str_[i];
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
    String s = *this;
    return s += other;
  }

  String& operator++() {
    return *this += String(" ");
  }

  const String operator++(int) {
    String s = *this;
    *this += String(" ");
    return s;
  }

  explicit operator std::string() const {
    return {str_};
  }

 private:
 /// приватные хелперы
static size_t strnlen(const char *s, size_t max) {
    const char *p;
    for(p = s; *p && max--; ++p);
    return(p - s);
}
/*void clean_(String str) 
{ 
 str.clear();
} 
*/

/// Поля класса
  char* str_ = nullptr;
  size_t size_ = 0;

  /*friend StringHash;
  friend std::hash<String>;*/
};

int main() {
  String a, b, c;
  std::cout << "Input String1:";
  std::cin >> a;
    std::cout << "Input String2:";
  std::cin >> b;
  String newstr;
  /*newstr = a.c_str;*/

  /*c = a + b;
  std::cout << c << std::endl;*/ 
  return 0;
}