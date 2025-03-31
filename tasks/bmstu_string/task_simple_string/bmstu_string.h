#pragma once

#include <exception>
#include <iostream>
#include <cstring>
#include <initializer_list>
#include <sstream>
#include <algorithm>

namespace bmstu {

template <typename T>
class basic_string {
public:
    
    basic_string() : ptr_(new T[1]{0}), size_(0) {}

    
    explicit basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {
        for (size_t i = 0; i < size; ++i) {
            ptr_[i] = ' ';
        }
        ptr_[size] = 0;
    }

    
    basic_string(std::initializer_list<T> il) : ptr_(new T[il.size() + 1]), size_(il.size()) {
        std::copy(il.begin(), il.end(), ptr_);
        ptr_[size_] = 0;
    }

    
    basic_string(const T* c_str) : ptr_(new T[strlen_(c_str) + 1]), size_(strlen_(c_str)) {
        std::copy(c_str, c_str + size_, ptr_);
        ptr_[size_] = 0;
    }

    
    basic_string(const basic_string& other) : ptr_(new T[other.size_ + 1]), size_(other.size_) {
        std::copy(other.ptr_, other.ptr_ + size_ + 1, ptr_);
    }

    
    basic_string(basic_string&& dying) noexcept : ptr_(dying.ptr_), size_(dying.size_) {
        dying.ptr_ = new T[1]{0};
        dying.size_ = 0;
    }

    
    ~basic_string() {
        delete[] ptr_;
    }

    
    const T* c_str() const {
        return ptr_;
    }

    
    size_t size() const {
        return size_;
    }

    
    basic_string& operator=(const basic_string& other) {
        if (this != &other) {
            delete[] ptr_;
            size_ = other.size_;
            ptr_ = new T[size_ + 1];
            for (size_t i = 0; i <= size_; ++i) {
                ptr_[i] = other.ptr_[i];
            }
        }
        return *this;
    }

    
    basic_string& operator=(basic_string&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = new T[1]{0};
            other.size_ = 0;
        }
        return *this;
    }

    
    basic_string& operator=(const T* c_str) {
        delete[] ptr_;
        size_ = strlen_(c_str);
        ptr_ = new T[size_ + 1];
        for (size_t i = 0; i <= size_; ++i) {
            ptr_[i] = c_str[i];
        }
        return *this;
    }

    
    friend basic_string operator+(const basic_string& left, const basic_string& right) {
        basic_string result(left.size_ + right.size_);
        std::copy(left.ptr_, left.ptr_ + left.size_, result.ptr_);
        std::copy(right.ptr_, right.ptr_ + right.size_ + 1, result.ptr_ + left.size_);
        return result;
    }

    
    template <typename S>
    friend S& operator<<(S& os, const basic_string& obj) {
        for (size_t i = 0; i < obj.size_; ++i) {
            os << obj.ptr_[i];
        }
        return os;
    }

    
    template <typename S>
    friend S& operator>>(S& is, basic_string& obj) {
        T buffer[4096]; 
        size_t i = 0;
        T ch;
        while (is.get(ch) && i < 4095) { 
            buffer[i++] = ch;
        }
        buffer[i] = 0;
        obj = buffer;
        return is;
    }

    
    basic_string& operator+=(const basic_string& other) {
        basic_string temp(*this + other);
        swap(temp);
        return *this;
    }

    
    basic_string& operator+=(T symbol) {
        basic_string temp(size_ + 1);
        std::copy(ptr_, ptr_ + size_, temp.ptr_);
        temp.ptr_[size_] = symbol;
        temp.ptr_[size_ + 1] = 0;
        swap(temp);
        return *this;
    }

    
    T& operator[](size_t index) noexcept {
        return ptr_[index];
    }

    
    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return ptr_[index];
    }

    T* data() {
        return ptr_;
    }

private:
    
    static size_t strlen_(const T* str) {
        return std::char_traits<T>::length(str);
    }

    
    void clean_() {
        delete[] ptr_;
        ptr_ = new T[1]{0};
        size_ = 0;
    }

 
    void swap(basic_string& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(size_, other.size_);
    }

    T* ptr_ = nullptr;
    size_t size_ = 0;
};


typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

}  // namespace bmstu
