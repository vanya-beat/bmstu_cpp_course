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

	~stack()
	{
		clear();
		::operator delete[](data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		ensure_capacity(size_ + 1);
		new (data_ + size_) T(std::forward<Args>(args)...);
		size_++;
	}

	void push(const T& value)
	{
		ensure_capacity(size_ + 1);
		new (data_ + size_) T(value);
		size_++;
	}

	void push(T&& value)
	{
		ensure_capacity(size_ + 1);
		new (data_ + size_) T(std::move(value));
		size_++;
	}

	void pop()
	{
		if (empty())
			throw std::underflow_error("empty_error");
		data_[size_ - 1].~T();
		size_--;
	}

	void clear() noexcept
	{
		for (size_t i = 0; i < size_; ++i)
			data_[i].~T();
		size_ = 0;
	}

	T& top()
	{
		if (empty())
			throw std::underflow_error("empty_error");
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
			throw std::underflow_error("empty_error");
		return data_[size_ - 1];
	}

   private:
	void ensure_capacity(size_t new_size)
	{
		if (new_size <= capacity_)
			return;

		size_t new_capacity = new_size;

		T* new_data =
			static_cast<T*>(::operator new[](new_capacity * sizeof(T)));

		for (size_t i = 0; i < size_; i++)
		{
			new (new_data + i) T(std::move(data_[i]));
			data_[i].~T();
		}

		::operator delete[](data_);
		data_ = new_data;
		capacity_ = new_capacity;
	}

	T* data_;
	size_t size_;
	size_t capacity_;
};
}  // namespace bmstu