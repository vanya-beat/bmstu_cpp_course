#pragma once

#include <exception>
#include <iostream>

namespace bmstu {
template <typename T> class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T> class basic_string {
public:
  /// Конструктор по умолчанию
  basic_string()
      : ptr_(nullptr),
        size_(0) {
  }

  basic_string(size_t size)
      : ptr_(nullptr),
        size_(0) {
  }

  basic_string(std::initializer_list<T> il)
      : ptr_(nullptr),
        size_(0) {
  }

  basic_string(const T* c_str) {
  }

  basic_string(const basic_string& other) {
  }

  basic_string(basic_string&& dying) {
  }

  ~basic_string() {
  }

  const T* c_str() const {
    return nullptr;
  }

  size_t size() const {
    return 0;
  }

  basic_string& operator=(basic_string&& other) {
    return *this;
  }

  basic_string& operator=(const T* c_str) {
    return *this;
  }

  basic_string& operator=(const basic_string& other) {
    return *this;
  }

  friend basic_string<T> operator+(const basic_string<T>& left,
                                   const basic_string<T>& right) {
    return {};
  }

  template <typename S> friend S& operator<<(S& os, const basic_string& obj) {
    return os;
  }

  template <typename S> friend S& operator>>(S& is, basic_string& obj) {
    return is;
  }

  basic_string& operator+=(const basic_string& other) {
    return *this;
  }

  basic_string& operator+=(T symbol) {
    return *this;
  }

  T& operator[](size_t index) noexcept {
    return data()[0];
  }

  T& at(size_t index) {
    return data()[0];
  }

private:
  static size_t strlen_(const T* str) {
    return 0;
  }

  void clean_() {
  }

  T* data() {
    return nullptr;
  }

  T* ptr_ = nullptr;
  size_t size_;
};
} // namespace bmstu
