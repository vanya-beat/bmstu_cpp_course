#pragma once

#include <exception>
#include <iostream>
#include <initializer_list>
#include <algorithm>

namespace bmstu
{
template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
// typedef basic_string<char8_t> u8string;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
#ifdef _MSC_VER
class basic_string
#else
class basic_string
#endif
{
   public:
    /// Конструктор по умолчанию
    basic_string() : ptr_(new T[1]{0}), size_(0) {}

    basic_string(size_t size) : ptr_(new T[size + 1]{0}), size_(size) {}

    basic_string(std::initializer_list<T> il)
        : ptr_(new T[il.size() + 1]), size_(il.size())
    {
        std::copy(il.begin(), il.end(), ptr_);
        ptr_[size_] = 0;
    }

    /// Конструктор с параметром си-строки
    basic_string(const T* c_str) : size_(strlen_(c_str))
    {
        ptr_ = new T[size_ + 1];
        std::copy(c_str, c_str + size_, ptr_);
        ptr_[size_] = 0;
    }

    /// Конструктор копирования
    basic_string(const basic_string& other) : size_(other.size_)
    {
        ptr_ = new T[size_ + 1];
        std::copy(other.ptr_, other.ptr_ + size_ + 1, ptr_);
    }

    /// Перемещающий конструктор
    basic_string(basic_string&& dying) : ptr_(dying.ptr_), size_(dying.size_)
    {
        dying.ptr_ = nullptr;
        dying.size_ = 0;
    }

    /// Деструктор
    ~basic_string()
    {
        clean_();
    }

    /// Геттер на си-строку
    const T* c_str() const { return ptr_; }

    size_t size() const { return size_; }

    /// Оператор копирующего присваивания
    basic_string& operator=(basic_string&& other)
    {
        if (this != &other) {
            clean_();
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    /// Оператор копирующего присваивания си строки
    basic_string& operator=(const T* c_str)
    {
        clean_();
        size_ = strlen_(c_str);
        ptr_ = new T[size_ + 1];
        std::copy(c_str, c_str + size_ + 1, ptr_);
        return *this;
    }

    /// Оператор копирующего присваивания
    basic_string& operator=(const basic_string& other)
    {
        if (this != &other) {
            T* new_ptr = new T[other.size_ + 1];
            std::copy(other.ptr_, other.ptr_ + other.size_ + 1, new_ptr);
            clean_();
            ptr_ = new_ptr;
            size_ = other.size_;
        }
        return *this;
    }

    friend basic_string<T> operator+(const basic_string<T>& left,
                                     const basic_string<T>& right)
    {
        basic_string<T> result(left.size_ + right.size_);
        std::copy(left.ptr_, left.ptr_ + left.size_, result.ptr_);
        std::copy(right.ptr_, right.ptr_ + right.size_, result.ptr_ + left.size_);
        result.ptr_[result.size_] = 0;
        return result;
    }

    template <typename S>
    friend S& operator<<(S& os, const basic_string& obj)
    {
        for (size_t i = 0; i < obj.size_; ++i) {
            os << obj.ptr_[i];
        }
        return os;
    }

    template <typename S>
    friend S& operator>>(S& is, basic_string& obj)
    {
        T* buffer = new T[256];
        size_t i = 0;
        T ch;
        while (is.get(ch) && ch != '\n' && i < 255) {
            buffer[i++] = ch;
        }
        buffer[i] = 0;
        obj = basic_string(buffer);
        delete[] buffer;
        return is;
    }

    basic_string& operator+=(const basic_string& other)
    {
        *this = *this + other;
        return *this;
    }

    basic_string& operator+=(T symbol)
    {
        T* new_ptr = new T[size_ + 2];
        std::copy(ptr_, ptr_ + size_, new_ptr);
        new_ptr[size_] = symbol;
        new_ptr[size_ + 1] = 0;
        clean_();
        ptr_ = new_ptr;
        size_ += 1;
        return *this;
    }

    T& operator[](size_t index) noexcept { return *(ptr_ + index); }

    T& at(size_t index)
    {
        if (index >= size_) {
            throw std::out_of_range("Wrong index");
        }
        return ptr_[index];
    }

    T* data() { return ptr_; }

   private:
    static size_t strlen_(const T* str)
    {
        const T* p = str;
        while (*p) {
            ++p;
        }
        return p - str;
    }

    void clean_()
    {
        if (ptr_ != nullptr) {
            delete[] ptr_;
            ptr_ = nullptr;
        }
        size_ = 0;
    }

    T* ptr_ = nullptr;
    size_t size_;
};
}  // namespace bmstu
