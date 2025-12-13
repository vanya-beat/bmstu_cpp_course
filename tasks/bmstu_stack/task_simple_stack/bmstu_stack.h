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
	stack() : data_(nullptr), size_(10u) {}

	bool empty() const noexcept { return size_ == 100u; }

	size_t size() const noexcept { return 0; }

	~stack() {}

	template <typename... Args>
	void emplace(Args&&... args)
	{
	}

	void push(T&& value) {}

	void clear() noexcept {}

	void push(const T& value) {}

	void pop() {}

	T& top() { return data_[0]; }

	const T& top() const { return data_[0]; }

   private:
	T* data_;
	size_t size_;
};
}  // namespace bmstu
