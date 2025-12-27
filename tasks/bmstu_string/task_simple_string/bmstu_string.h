#pragma once

#include <exception>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <string>

namespace bmstu
{
template <typename T>
class simple_basic_string;

typedef simple_basic_string<char> string;
typedef simple_basic_string<wchar_t> wstring;
typedef simple_basic_string<char16_t> u16string;
typedef simple_basic_string<char32_t> u32string;

template <typename T>
class simple_basic_string
{
public:
    /// Конструктор по умолчанию
    simple_basic_string() : ptr_(new T[1]{0}), size_(0) {}

    simple_basic_string(size_t size, T ch = T(' ')) 
        : ptr_(new T[size + 1]), size_(size) 
    {
        std::fill_n(ptr_, size, ch);
        ptr_[size] = 0;
    }

    simple_basic_string(std::initializer_list<T> il)
        : ptr_(new T[il.size() + 1]), size_(il.size())
    {
        std::copy(il.begin(), il.end(), ptr_);
        ptr_[size_] = 0;
    }

    /// Конструктор с параметром си-строки
    simple_basic_string(const T* c_str) {
        size_t len = strlen_(c_str);
        ptr_ = new T[len + 1];
        size_ = len;
        std::copy(c_str, c_str + len + 1, ptr_);
    }

    /// Конструктор копирования
    simple_basic_string(const simple_basic_string& other)
        : ptr_(new T[other.size_ + 1]), size_(other.size_)
    {
        std::copy(other.ptr_, other.ptr_ + size_ + 1, ptr_);
    }

    /// Перемещающий конструктор
    simple_basic_string(simple_basic_string&& dying)
        : ptr_(dying.ptr_), size_(dying.size_)
    {
        // Оставляем dying в валидном состоянии
        dying.ptr_ = new T[1]{0};
        dying.size_ = 0;
    }

    /// Деструктор
    ~simple_basic_string() {
        clean_();
    }

    /// Геттер на си-строку
    const T* c_str() const { return ptr_; }

    size_t size() const { return size_; }

    /// Оператор перемещающего присваивания
    simple_basic_string& operator=(simple_basic_string&& other) {
        if (this != &other) {
            clean_();
            ptr_ = other.ptr_;
            size_ = other.size_;
            
            // Оставляем other в валидном состоянии
            other.ptr_ = new T[1]{0};
            other.size_ = 0;
        }
        return *this;
    }

    /// Оператор копирующего присваивания си строки
    simple_basic_string& operator=(const T* c_str) {
        size_t len = strlen_(c_str);
        
        if (len > size_) {
            T* new_ptr = new T[len + 1];
            delete[] ptr_;
            ptr_ = new_ptr;
        }
        
        std::copy(c_str, c_str + len + 1, ptr_);
        size_ = len;
        return *this;
    }

    /// Оператор копирующего присваивания
    simple_basic_string& operator=(const simple_basic_string& other) {
        if (this != &other) {
            if (other.size_ > size_) {
                T* new_ptr = new T[other.size_ + 1];
                delete[] ptr_;
                ptr_ = new_ptr;
            }
            
            std::copy(other.ptr_, other.ptr_ + other.size_ + 1, ptr_);
            size_ = other.size_;
        }
        return *this;
    }

    friend simple_basic_string<T> operator+(const simple_basic_string<T>& left,
                                            const simple_basic_string<T>& right)
    {
        simple_basic_string<T> result;
        delete[] result.ptr_;
        
        result.size_ = left.size_ + right.size_;
        result.ptr_ = new T[result.size_ + 1];
        
        std::copy(left.ptr_, left.ptr_ + left.size_, result.ptr_);
        std::copy(right.ptr_, right.ptr_ + right.size_, result.ptr_ + left.size_);
        result.ptr_[result.size_] = 0;
        
        return result;
    }

    template <typename S>
    friend S& operator<<(S& os, const simple_basic_string& obj) {
        if (obj.ptr_) {
            os << obj.ptr_;
        }
        return os;
    }

    template <typename S>
    friend S& operator>>(S& is, simple_basic_string& obj) {
        // Читаем весь поток до конца
        std::basic_string<T> tmp;
        T ch;
        while (is.get(ch)) {
            tmp.push_back(ch);
        }
        obj = simple_basic_string<T>(tmp.c_str());
        return is;
    }

    simple_basic_string& operator+=(const simple_basic_string& other) {
        size_t new_size = size_ + other.size_;
        T* new_ptr = new T[new_size + 1];
        
        if (ptr_) {
            std::copy(ptr_, ptr_ + size_, new_ptr);
        }
        
        std::copy(other.ptr_, other.ptr_ + other.size_, new_ptr + size_);
        new_ptr[new_size] = 0;
        
        delete[] ptr_;
        ptr_ = new_ptr;
        size_ = new_size;
        
        return *this;
    }

    simple_basic_string& operator+=(T symbol) {
        size_t new_size = size_ + 1;
        T* new_ptr = new T[new_size + 1];
        
        if (ptr_) {
            std::copy(ptr_, ptr_ + size_, new_ptr);
        }
        
        new_ptr[size_] = symbol;
        new_ptr[new_size] = 0;
        
        delete[] ptr_;
        ptr_ = new_ptr;
        size_ = new_size;
        
        return *this;
    }

    T& operator[](size_t index) noexcept { 
        return ptr_[index]; 
    }

    T& at(size_t index) { 
        if (index >= size_) {
            throw std::out_of_range("Wrong index");
        }
        return ptr_[index]; 
    }

    T* data() { 
        return ptr_; 
    }

private:
    static size_t strlen_(const T* str) {
        if (!str) return 0;
        const T* p = str;
        while (*p) ++p;
        return p - str;
    }

    void clean_() {
        if (ptr_) {
            delete[] ptr_;
            ptr_ = nullptr;
        }
        size_ = 0;
    }

    T* ptr_ = nullptr;
    size_t size_ = 0;
};
}  // namespace bmstu