#pragma once

#include <exception>
#include <iostream>
#include <cstring>
#include <memory>
#include <algorithm>
#include <iterator>

namespace bmstu
{
template <typename T>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename T>
class basic_string
{
private:
    static constexpr size_t SSO_CAPACITY = 
        (sizeof(T*) + sizeof(size_t) + sizeof(size_t)) / sizeof(T) - 1;

    struct LongString {
        T* ptr;
        size_t size;
        size_t capacity;
    };

    struct ShortString {
        T buffer[SSO_CAPACITY + 1]; // +1 для нуль-терминатора
        unsigned char size;
    };

    union Data {
        LongString long_str;
        ShortString short_str;
        
        Data() : short_str{} {}
        ~Data() {}
    };

    Data data_;
    bool is_long_;

    bool is_long() const {
        return is_long_;
    }

    T* get_ptr() {
        return is_long_ ? data_.long_str.ptr : data_.short_str.buffer;
    }

    const T* get_ptr() const {
        return is_long_ ? data_.long_str.ptr : data_.short_str.buffer;
    }

    size_t get_size() const {
        return is_long_ ? data_.long_str.size : data_.short_str.size;
    }

    size_t get_capacity() const {
        return is_long_ ? data_.long_str.capacity : SSO_CAPACITY;
    }

    void set_size(size_t size) {
        if (is_long_) {
            data_.long_str.size = size;
        } else {
            data_.short_str.size = static_cast<unsigned char>(size);
        }
    }

    void set_capacity(size_t capacity) {
        if (is_long_) {
            data_.long_str.capacity = capacity;
        }
    }

public:
    basic_string() : is_long_(false) {
        data_.short_str.buffer[0] = T();
        data_.short_str.size = 0;
    }

    basic_string(size_t count, T ch = T(' ')) : is_long_(count > SSO_CAPACITY) {
        if (is_long_) {
            data_.long_str.ptr = new T[count + 1];
            std::fill_n(data_.long_str.ptr, count, ch);
            data_.long_str.ptr[count] = T();
            data_.long_str.size = count;
            data_.long_str.capacity = count;
        } else {
            std::fill_n(data_.short_str.buffer, count, ch);
            data_.short_str.buffer[count] = T();
            data_.short_str.size = static_cast<unsigned char>(count);
        }
    }

    basic_string(std::initializer_list<T> il) : is_long_(il.size() > SSO_CAPACITY) {
        if (is_long_) {
            data_.long_str.ptr = new T[il.size() + 1];
            std::copy(il.begin(), il.end(), data_.long_str.ptr);
            data_.long_str.ptr[il.size()] = T();
            data_.long_str.size = il.size();
            data_.long_str.capacity = il.size();
        } else {
            std::copy(il.begin(), il.end(), data_.short_str.buffer);
            data_.short_str.buffer[il.size()] = T();
            data_.short_str.size = static_cast<unsigned char>(il.size());
        }
    }

    basic_string(const T* c_str) {
        size_t len = strlen_(c_str);
        is_long_ = len > SSO_CAPACITY;
        
        if (is_long_) {
            data_.long_str.ptr = new T[len + 1];
            std::copy(c_str, c_str + len + 1, data_.long_str.ptr);
            data_.long_str.size = len;
            data_.long_str.capacity = len;
        } else {
            std::copy(c_str, c_str + len + 1, data_.short_str.buffer);
            data_.short_str.size = static_cast<unsigned char>(len);
        }
    }

    basic_string(const basic_string& other) : is_long_(other.is_long_) {
        if (is_long_) {
            data_.long_str.ptr = new T[other.data_.long_str.capacity + 1];
            std::copy(other.data_.long_str.ptr, 
                     other.data_.long_str.ptr + other.data_.long_str.size + 1, 
                     data_.long_str.ptr);
            data_.long_str.size = other.data_.long_str.size;
            data_.long_str.capacity = other.data_.long_str.capacity;
        } else {
            std::copy(other.data_.short_str.buffer, 
                     other.data_.short_str.buffer + other.data_.short_str.size + 1,
                     data_.short_str.buffer);
            data_.short_str.size = other.data_.short_str.size;
        }
    }

    basic_string(basic_string&& dying) noexcept 
        : data_(dying.data_), is_long_(dying.is_long_) {
        if (dying.is_long_) {
            dying.data_.long_str.ptr = nullptr;
            dying.data_.long_str.size = 0;
            dying.data_.long_str.capacity = 0;
            dying.is_long_ = false;
        } else {
            dying.data_.short_str.buffer[0] = T();
            dying.data_.short_str.size = 0;
        }
    }

    ~basic_string() {
        clean_();
    }

    const T* c_str() const {
        return get_ptr();
    }

    size_t size() const {
        return get_size();
    }

    bool is_using_sso() const {
        return !is_long_;
    }

    size_t capacity() const {
        return get_capacity();
    }

    basic_string& operator=(basic_string&& other) noexcept {
        if (this != &other) {
            clean_();
            
            is_long_ = other.is_long_;
            if (is_long_) {
                data_.long_str = other.data_.long_str;
                other.data_.long_str.ptr = nullptr;
                other.data_.long_str.size = 0;
                other.data_.long_str.capacity = 0;
                other.is_long_ = false;
            } else {
                std::copy(other.data_.short_str.buffer,
                         other.data_.short_str.buffer + other.data_.short_str.size + 1,
                         data_.short_str.buffer);
                data_.short_str.size = other.data_.short_str.size;
                other.data_.short_str.buffer[0] = T();
                other.data_.short_str.size = 0;
            }
        }
        return *this;
    }

    basic_string& operator=(const T* c_str) {
        size_t len = strlen_(c_str);
        
        if (len > get_capacity()) {
            clean_();
            is_long_ = true;
            data_.long_str.ptr = new T[len + 1];
            std::copy(c_str, c_str + len + 1, data_.long_str.ptr);
            data_.long_str.size = len;
            data_.long_str.capacity = len;
        } else {
            std::copy(c_str, c_str + len + 1, get_ptr());
            set_size(len);
            is_long_ = len > SSO_CAPACITY;
        }
        return *this;
    }

    basic_string& operator=(const basic_string& other) {
        if (this != &other) {
            size_t other_size = other.size();
            
            if (other_size > get_capacity()) {
                clean_();
                is_long_ = true;
                data_.long_str.ptr = new T[other_size + 1];
                std::copy(other.get_ptr(), other.get_ptr() + other_size + 1, data_.long_str.ptr);
                data_.long_str.size = other_size;
                data_.long_str.capacity = other_size;
            } else {
                std::copy(other.get_ptr(), other.get_ptr() + other_size + 1, get_ptr());
                set_size(other_size);
                is_long_ = other_size > SSO_CAPACITY;
            }
        }
        return *this;
    }

    friend basic_string<T> operator+(const basic_string<T>& left,
                                     const basic_string<T>& right) {
        size_t new_size = left.size() + right.size();
        basic_string<T> result;
        
        if (new_size > SSO_CAPACITY) {
            result.is_long_ = true;
            result.data_.long_str.ptr = new T[new_size + 1];
            result.data_.long_str.size = new_size;
            result.data_.long_str.capacity = new_size;
            
            std::copy(left.get_ptr(), left.get_ptr() + left.size(), result.data_.long_str.ptr);
            std::copy(right.get_ptr(), right.get_ptr() + right.size(), 
                     result.data_.long_str.ptr + left.size());
            result.data_.long_str.ptr[new_size] = T();
        } else {
            std::copy(left.get_ptr(), left.get_ptr() + left.size(), result.data_.short_str.buffer);
            std::copy(right.get_ptr(), right.get_ptr() + right.size(),
                     result.data_.short_str.buffer + left.size());
            result.data_.short_str.buffer[new_size] = T();
            result.data_.short_str.size = static_cast<unsigned char>(new_size);
        }
        
        return result;
    }

    template <typename S>
    friend S& operator<<(S& os, const basic_string& obj) {
        os << obj.get_ptr();
        return os;
    }

    template <typename S>
    friend S& operator>>(S& is, basic_string& obj) {
        // Читаем весь поток до конца
        std::basic_string<T> tmp;
        T ch;
        while (is.get(ch)) {
            tmp.push_back(ch);
        }
        obj = basic_string(tmp.c_str());
        return is;
    }

    basic_string& operator+=(const basic_string& other) {
        size_t old_size = size();
        size_t other_size = other.size();
        size_t new_size = old_size + other_size;
        
        if (new_size > get_capacity()) {
            T* new_ptr = new T[new_size + 1];
            std::copy(get_ptr(), get_ptr() + old_size, new_ptr);
            std::copy(other.get_ptr(), other.get_ptr() + other_size, new_ptr + old_size);
            new_ptr[new_size] = T();
            
            clean_();
            is_long_ = true;
            data_.long_str.ptr = new_ptr;
            data_.long_str.size = new_size;
            data_.long_str.capacity = new_size;
        } else {
            std::copy(other.get_ptr(), other.get_ptr() + other_size, get_ptr() + old_size);
            get_ptr()[new_size] = T();
            set_size(new_size);
        }
        
        return *this;
    }

    basic_string& operator+=(T symbol) {
        size_t old_size = size();
        size_t new_size = old_size + 1;
        
        if (new_size > get_capacity()) {
            T* new_ptr = new T[new_size + 1];
            std::copy(get_ptr(), get_ptr() + old_size, new_ptr);
            new_ptr[old_size] = symbol;
            new_ptr[new_size] = T();
            
            clean_();
            is_long_ = true;
            data_.long_str.ptr = new_ptr;
            data_.long_str.size = new_size;
            data_.long_str.capacity = new_size;
        } else {
            get_ptr()[old_size] = symbol;
            get_ptr()[new_size] = T();
            set_size(new_size);
        }
        
        return *this;
    }

    T& operator[](size_t index) noexcept {
        return get_ptr()[index];
    }

    T& at(size_t index) {
        if (index >= size()) {
            throw std::out_of_range("Wrong index");
        }
        return get_ptr()[index];
    }

    T* data() {
        return get_ptr();
    }

private:
    static size_t strlen_(const T* str) {
        const T* p = str;
        while (*p) ++p;
        return p - str;
    }

    void clean_() {
        if (is_long_ && data_.long_str.ptr) {
            delete[] data_.long_str.ptr;
            data_.long_str.ptr = nullptr;
        }
    }
};
}  // namespace bmstu