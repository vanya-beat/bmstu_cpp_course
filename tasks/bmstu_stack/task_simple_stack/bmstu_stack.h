#pragma once
#include <exception>
#include <iostream>
#include <utility>
#include <stdexcept>

namespace bmstu {
template <typename T>
class stack {
public:
    stack() : data_(nullptr), size_(0), capacity_(0) {}

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_t size() const noexcept {
        return size_;
    }

    ~stack() {
        clear();
        ::operator delete(data_);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
    }

    void push(const T& value) {
        emplace(value);
    }

    void push(T&& value) {
        emplace(std::move(value));
    }

    void clear() noexcept {
        while (size_ > 0) {
            pop_quiet();
        }
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty!");
        }
        pop_quiet();
    }

    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty!");
        }
        return data_[size_ - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty!");
        }
        return data_[size_ - 1];
    }

private:
    void reserve(size_t new_cap) {
        T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            (data_ + i)->~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    void pop_quiet() noexcept {
        --size_;
        (data_ + size_)->~T();
    }

    T* data_;
    size_t size_;
    size_t capacity_;
};
}