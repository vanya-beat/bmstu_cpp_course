#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <stdexcept>
#include <memory>

namespace bmstu
{
template <typename T>
class stack
{
public:
    stack() : data_(nullptr), size_(0), capacity_(0) {}

    stack(const stack& other) : size_(other.size_), capacity_(other.size_) {
        if (capacity_ > 0) {
            data_ = static_cast<T*>(operator new[](capacity_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                new (&data_[i]) T(other.data_[i]);
            }
        } else {
            data_ = nullptr;
        }
    }

    stack(stack&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    stack& operator=(const stack& other) {
        if (this != &other) {
            clear_memory();
            
            size_ = other.size_;
            capacity_ = other.size_;
            
            if (capacity_ > 0) {
                data_ = static_cast<T*>(operator new[](capacity_ * sizeof(T)));
                for (size_t i = 0; i < size_; ++i) {
                    new (&data_[i]) T(other.data_[i]);
                }
            }
        }
        return *this;
    }

    stack& operator=(stack&& other) noexcept {
        if (this != &other) {
            clear_memory();
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~stack() {
        clear_memory();
    }

    bool empty() const noexcept { 
        return size_ == 0; 
    }

    size_t size() const noexcept { 
        return size_; 
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }

    void push(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (&data_[size_]) T(value);
        ++size_;
    }

    void push(T&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    void pop() {
        if (size_ == 0) {
            throw std::underflow_error("Stack is empty");
        }
        --size_;
        data_[size_].~T();
    }

    T& top() {
        if (size_ == 0) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    const T& top() const {
        if (size_ == 0) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

private:
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
        
        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        if (data_) {
            operator delete[](data_);
        }
        
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void clear_memory() {
        clear();
        if (data_) {
            operator delete[](data_);
        }
        data_ = nullptr;
        capacity_ = 0;
    }

    T* data_;
    size_t size_;
    size_t capacity_;
};
}  // namespace bmstu