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
	stack() : data_(nullptr), size_(0) {}
	//

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack()
	{
		for (size_t i = 0; i < size_; i++)
			data_[i].~T();
		::operator delete(data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		make_room();
		new (data_ + size_) T(std::forward<Args>(args)...);
		size_++;
	}

	void push(T&& value)
	{
		make_room();
		new (data_ + size_) T(std::move(value));
		size_++;
	}

	void clear() noexcept { size_ = 0; }

	void push(const T& value)
	{
		make_room();
		new (data_ + size_) T(value);
		size_++;
	}

	void pop()
	{
		if (empty())
			throw std::underflow_error("в стеке ничо нет");
		size_--;
	}

	T& top()
	{
		if (empty())
			throw std::underflow_error("в стеке ничо нет");
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
			throw std::underflow_error("в стеке ничо нет");
		return data_[size_ - 1];
	}

   private:
	void make_room()
	{
		T* new_data_ = (T*)::operator new(sizeof(T) * (size_ + 1));
		for (size_t i = 0; i < size_; i++)
		{
			new (new_data_ + i) T(std::move(data_[i]));
			(data_ + i)->~T();
		}
		::operator delete(data_);
		data_ = new_data_;
	}
	T* data_;
	size_t size_;
};
}  // namespace bmstu
