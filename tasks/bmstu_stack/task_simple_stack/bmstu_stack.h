#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0), capacity_(0) {}

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack() {
        for (size_t i = 0; i < size_; i++)
            data_[i].~T();
        ::operator delete(data_);
    }

	template <typename... Args>
	void emplace(Args&&... args)
	{
        if (size_ == capacity_)

	}

	void push(T&& value) {}

	void clear() noexcept {}

	void push(const T& value) {}

	void pop() {}

	T& top() { return data_[0]; }

	const T& top() const { return data_[0]; }

   private:
    void make_room() {
        capacity_ ++;
        T* new_data_ = (T*)::operator new(sizeof(T) * capacity_);
        for (size_t i = 0; i < size_; i++) {
            new(new_data_ + i) T(std::move(data_[i]));
            (data_ + i)->~T();
        }
        ::operator delete(data_);
        data_ = new_data_;
    }
	T* data_;
	size_t size_;
    size_t capacity_;
};
}  // namespace bmstu
