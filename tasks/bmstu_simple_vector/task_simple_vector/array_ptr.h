#pragma once
#include <algorithm>
#include <type_traits>

namespace
{
template <typename T>
void my_fill(T* ptr, size_t size, const T& value = {})
{
	for (size_t i = 0; i < size; ++i)
	{
		ptr[i] = value;
	}
}

template <typename T>
void my_swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
}  // namespace

namespace bmstu
{
template <typename T>
class array_ptr
{
   public:
	array_ptr() = default;
	explicit array_ptr(size_t size)
	{
		if (size > 0)
		{
			raw_ptr_ = new T[size];
			my_fill(raw_ptr_, size);
		}
		else
		{
			raw_ptr_ = nullptr;
		}
	}
	explicit array_ptr(T* raw_ptr) : raw_ptr_(raw_ptr) {}
	array_ptr(const array_ptr& other) = delete;
	array_ptr& operator=(const array_ptr& other) = delete;
	array_ptr(array_ptr&& other) noexcept : raw_ptr_(other.raw_ptr_)
	{
		other.raw_ptr_ = nullptr;
	}
	array_ptr& operator=(array_ptr&& other) noexcept
	{
		if (this != &other)
		{
			delete[] raw_ptr_;
			raw_ptr_ = other.raw_ptr_;
			other.raw_ptr_ = nullptr;
		}
		return *this;
	}

	T* get() const noexcept { return raw_ptr_; }

	explicit operator bool() const noexcept { return raw_ptr_ != nullptr; }

	~array_ptr() { delete[] raw_ptr_; }
	void swap(array_ptr& other) noexcept { my_swap(raw_ptr_, other.raw_ptr_); }

	const T& operator[](size_t index) const
	{
		return const_cast<const T&>(raw_ptr_[index]);
	}

	T& operator[](size_t index) { return raw_ptr_[index]; }

	[[nodiscard]] T* release() noexcept
	{
		T* tmp = raw_ptr_;
		raw_ptr_ = nullptr;
		return tmp;
	}

   private:
	T* raw_ptr_ = nullptr;
};
}  // namespace bmstu