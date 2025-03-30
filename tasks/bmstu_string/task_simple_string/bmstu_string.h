#pragma once

#include <exception>
#include <iostream>
#include <initializer_list>
#include <algorithm>

namespace bmstu {

template <typename T>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename T>
#ifdef _MSC_VER
class __declspec(dllexport) basic_string
#else
class basic_string
#endif
{
public:
    basic_string() noexcept : data_(new T[1]{0}), length_(0) {}
    
    explicit basic_string(size_t count) : data_(new T[count+1]), length_(count) {
        std::fill_n(data_, count, static_cast<T>(' '));
        data_[count] = 0;
    }
    
    basic_string(const T* str) : length_(strlen_(str)) {
        data_ = new T[length_ + 1];
        std::copy(str, str + length_, data_);
        data_[length_] = 0;
    }
    
    basic_string(std::initializer_list<T> init) : length_(init.size()) {
        data_ = new T[length_ + 1];
        std::copy(init.begin(), init.end(), data_);
        data_[length_] = 0;
    }
    
    basic_string(const basic_string& other) : length_(other.length_) {
        data_ = new T[length_ + 1];
        std::copy(other.data_, other.data_ + length_ + 1, data_);
    }
    
    basic_string(basic_string&& other) noexcept 
        : data_(other.data_), length_(other.length_) {
        other.data_ = nullptr;
        other.length_ = 0;
    }
    
    ~basic_string() {
        delete[] data_;
    }
    
    basic_string& operator=(const basic_string& other) {
        if (this != &other) {
            basic_string tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    basic_string& operator=(basic_string&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            length_ = other.length_;
            other.data_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }
    
    basic_string& operator=(const T* str) {
        basic_string tmp(str);
        swap(tmp);
        return *this;
    }
    
    const T* c_str() const noexcept { 
        return data_ ? data_ : empty_str(); 
    }
    
    size_t size() const noexcept { return length_; }
    bool empty() const noexcept { return length_ == 0; }
    
    T& operator[](size_t pos) noexcept { return data_[pos]; }
    const T& operator[](size_t pos) const noexcept { return data_[pos]; }
    
    T& at(size_t pos) {
        if (pos >= length_) throw std::out_of_range("Position out of range");
        return data_[pos];
    }
    
    const T& at(size_t pos) const {
        if (pos >= length_) throw std::out_of_range("Position out of range");
        return data_[pos];
    }
    
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    
    basic_string& operator+=(const basic_string& other) {
        *this = *this + other;
        return *this;
    }
    
    basic_string& operator+=(T ch) {
        T* new_data = new T[length_ + 2];
        std::copy(data_, data_ + length_, new_data);
        new_data[length_] = ch;
        new_data[length_ + 1] = 0;
        delete[] data_;
        data_ = new_data;
        ++length_;
        return *this;
    }
    
    void clear() noexcept {
        delete[] data_;
        data_ = new T[1]{0};
        length_ = 0;
    }
    
    void swap(basic_string& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(length_, other.length_);
    }
    
    friend basic_string operator+(const basic_string& lhs, const basic_string& rhs) {
        basic_string result;
        result.length_ = lhs.length_ + rhs.length_;
        result.data_ = new T[result.length_ + 1];
        std::copy(lhs.data_, lhs.data_ + lhs.length_, result.data_);
        std::copy(rhs.data_, rhs.data_ + rhs.length_, result.data_ + lhs.length_);
        result.data_[result.length_] = 0;
        return result;
    }
    
    template <typename S>
    friend S& operator<<(S& os, const basic_string& str) {
        return os << str.c_str();
    }
    
    template <typename S>
    friend S& operator>>(S& is, basic_string& str) {
        str.clear();
        T ch;
        while (is.get(ch) && ch != '\n') {
            str += ch;
        }
        return is;
    }

private:
    T* data_;
    size_t length_;
    
    static const T* empty_str() noexcept {
        static const T empty = 0;
        return &empty;
    }
    
    static size_t strlen_(const T* str) noexcept {
        const T* p = str;
        while (*p) ++p;
        return p - str;
    }
};

} // namespace bmstu