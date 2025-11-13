#pragma once

#include <exception>
#include <utility>
#include <stdexcept>
#include <new>
#include <cstddef>
#include <iostream>  

namespace bmstu
{
template <typename T>
class stack
{
public:
    stack() noexcept : data_(nullptr), size_(0u), capacity_(0u) {}

    stack(const stack&) = delete;
    stack& operator=(const stack&) = delete;

    stack(stack&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    stack& operator=(stack&& other) noexcept
    {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~stack()
    {
        clear();
        ::operator delete(data_);
    }

    bool empty() const noexcept { return size_ == 0u; }

    size_t size() const noexcept { return size_; }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        if (size_ >= capacity_) {
            resize();
        }
        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }

    void push(T&& value)
    {
        if (size_ >= capacity_) {
            resize();
        }
        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    void push(const T& value)
    {
        if (size_ >= capacity_) {
            resize();
        }
        new (&data_[size_]) T(value);
        ++size_;
    }

    void clear() noexcept
    {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0u;
    }

    void pop()
    {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --size_;
        data_[size_].~T();
    }

    T& top()
    {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    const T& top() const
    {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize()
    {
        size_t new_capacity = (capacity_ == 0u) ? 1u : capacity_ * 2u;
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }

        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};

} // namespace bmstu