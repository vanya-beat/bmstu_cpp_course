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
	stack() : data_(nullptr), size_(0u) {}

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack()
	{
		if (data_ != nullptr)
		{
			operator delete(data_);
		}
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}

		new (&new_data[size_]) T(std::forward<Args>(args)...);
		operator delete(data_);

		data_ = new_data;
		++size_;
	}

	void push(T&& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}

		new (&new_data[size_]) T(std::forward<T>(value));
		operator delete(data_);

		data_ = new_data;
		++size_;
	}

	void push(const T& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (&new_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}

		new (&new_data[size_]) T(value);
		operator delete(data_);

		data_ = new_data;
		++size_;
	}

	void clear() noexcept
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}
		operator delete(data_);
		data_ = nullptr;
		size_ = 0;
	}

	void pop()
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		--size_;
		data_[size_].~T();
	}

	T& top()
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

   private:
	T* data_;
	size_t size_;
};
}  // namespace bmstu
