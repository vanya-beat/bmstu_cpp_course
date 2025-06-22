#pragma once

#include <exception>
#include <iostream>
#include <cstddef>
#include <string>
#include <sstream>
#include <fstream>

namespace bmstu {
template <typename T>
class basic_string;
typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<wchar_t> u8string;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
class basic_string {
 private:
  /// приватные хелперы
  static size_t strlen_(const T* str) {
    size_t counter = 0;
    while (str[counter] != 0) counter++;
    return counter;
  }

  static size_t strlen_(const basic_string &input) {
    return input.size_;
  }

  void clean_() {
    if (!ptr_) return;
    size_ = 0;
    delete[] ptr_;
  }

  void copy_data_(const T* str) {
    for (size_t i = 0; i < strlen_(str); ++i) {
      ptr_[i] = str[i];
    }
    ptr_[size_] = 0;
  }

  /// Поля класса
  T* ptr_ = nullptr;
  size_t size_ = 0;

 public:
  /// Конструктор по умолчанию
  basic_string() {
    ptr_ = new T[1];
    ptr_[0] = 0;
    size_ = 0;
  }

  explicit basic_string(size_t size) {
    size_ = size;
    ptr_ = new T[size_ + 1];
    for (size_t i = 0; i < size_; ++i) {
      ptr_[i] = ' ';
    }
    ptr_[size_] = 0;
  }

  basic_string(std::initializer_list<T> list) {
    size_ = list.size();
    ptr_ = new T[size_ + 1];
    size_t i = 0;
    for (auto &item : list) {
      ptr_[i] = item;
      ++i;
    }
    ptr_[size_] = 0;
  }

  /// Конструктор с параметром "cи строкой"
  basic_string(const T* c_str) { // NOLINT
    size_ = strlen_(c_str);
    ptr_ = new T[size_ + 1];
    copy_data_(c_str);
  }

  /// Копирующий конструктор
  basic_string(const basic_string& other) {
   /* size_ = other.size_;
    ptr_ = new T[size_ + 1];
    copy_data_(other.c_str());*/

    if (this != &other) {
      clean_();
      size_ = other.size_;
      ptr_ = new T[size_ + 1];
      ptr_[size_] = 0;
      for (size_t i = 0; i < size_; i++) {
          *(ptr_ + i) = other.c_str()[i];
      }
    }
  }

  /// Конструктор перемещения
  basic_string(basic_string&& dying) noexcept {
    size_ = dying.size_;
    ptr_ = dying.ptr_;
    dying.ptr_ = nullptr;
    dying.size_ = 0;
  }

  /// Деструктор
  ~basic_string() { clean_(); }

  // Output basic_string data
  void print() { std::cout << ptr_ << std::endl; }

  /// Геттер на си-строку
  [[nodiscard]] const T* c_str() const {
    return (ptr_ != nullptr) ? ptr_ : reinterpret_cast<const T*>("");
  }

  /// Геттер на размер
  [[nodiscard]] size_t size() const { return size_; }

  /// Оператор копирующего присваивания
  basic_string& operator=(const basic_string& other) {
    /* clean_();
    size_ = other.size_;
    ptr_ = new T[size_ + 1];
    copy_data_(other.c_str());
    ptr_[size_] = 0;
    return *this;*/
    if (this == &other) {
      return *this;
  }
  basic_string s = other;
  std::swap(*this, s);
  return *this; 

  }

  /// Оператор перемещающего присваивания
  basic_string& operator=(basic_string&& other) noexcept {
    delete[] ptr_;
    size_ = other.size_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  /// Оператор присваивания си строки
  basic_string& operator=(const T* c_str) {
    clean_();
    size_ = strlen_(c_str);
    ptr_ = new T[size_ + 1];
    for (size_t i = 0; i < size_; i++) ptr_[i] = c_str[i];
    ptr_[size_] = 0;
    return *this;
  }

  /* Все все понимают */
  friend bmstu::basic_string<T> operator+(const basic_string& left,
                                          const basic_string& right) {
    basic_string result;
    result.size_ = left.size_ + right.size_;
    result.ptr_ = new T[result.size_ + 1];
    result.copy_data_(left.c_str());
    for (size_t i = 0; i < right.size_; ++i) {
      result.ptr_[i + left.size_] = right.ptr_[i];
    }
    return result;
  }

  friend std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os,
                                           const basic_string& obj) {
    os << obj.c_str();
    return os;
  }

  friend std::basic_istream<T>& operator>>(std::basic_istream<T> &is,
                                           basic_string& obj) {
    is >> std::noskipws;
    T symbol = 1;
    for (size_t i = 0; symbol != 0; ++i) {
      symbol = 0;
      is >> symbol;
      if (symbol != 0) {
        obj += symbol;
      }
    }
    return is;
  }

  basic_string& operator+=(const basic_string& other) {
    *this = (*this + other);
    return *this;
  }

  bool operator==(const basic_string& input) const {
    if (input.size_ != size_) return false;
    if (input.ptr_ == ptr_) return true;
    for (size_t i = 0; i < input.size_; ++i) {
      if (ptr_[i] != input[i]) {
        return false;
      }
    }
    return true;
  }

  basic_string& operator+=(const T symbol) {
    T *prev_data = ptr_;
    ++size_;
    ptr_ = new T[size_ + 1];
    copy_data_(prev_data);
    delete[] prev_data;
    ptr_[size_ - 1] = symbol;
    ptr_[size_] = 0;
    return *this;
  }

  T& operator[](size_t index) {
    return index > size_ ? ptr_[0] : ptr_[index];
  }
};
}  // namespace bmstu