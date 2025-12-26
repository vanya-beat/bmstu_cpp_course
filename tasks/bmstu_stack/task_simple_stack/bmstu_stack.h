#pragma once

#include <new>
#include <stdexcept>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
public:
	stack() noexcept : data_(nullptr), size_(0u) {}

	~stack() { clear(); }

	bool empty() const noexcept { return size_ == 0u; }

	size_t size() const noexcept { return size_; }

	template <typename... Args>
	void emplace(Args&&... args)
	{
		const size_t new_size = size_ + 1u;
		T* new_data = allocate_raw(new_size);

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (new_data + (new_size - 1u)) T(std::forward<Args>(args)...);

		destroy_all();
		deallocate_raw(data_);

		data_ = new_data;
		size_ = new_size;
	}

	void push(const T& value)
	{
		const size_t new_size = size_ + 1u;
		T* new_data = allocate_raw(new_size);

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (new_data + (new_size - 1u)) T(value);

		destroy_all();
		deallocate_raw(data_);

		data_ = new_data;
		size_ = new_size;
	}

	void push(T&& value)
	{
		const size_t new_size = size_ + 1u;
		T* new_data = allocate_raw(new_size);

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (new_data + (new_size - 1u)) T(std::move(value));

		destroy_all();
		deallocate_raw(data_);

		data_ = new_data;
		size_ = new_size;
	}

	void pop()
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}

		data_[size_ - 1u].~T();
		--size_;
	}

	void clear() noexcept
	{
		destroy_all();
		deallocate_raw(data_);
		data_ = nullptr;
		size_ = 0u;
	}

	T& top()
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}
		return data_[size_ - 1u];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("stack is empty");
		}
		return data_[size_ - 1u];
	}

private:
	static T* allocate_raw(size_t n)
	{
		return static_cast<T*>(::operator new(sizeof(T) * n));
	}

	static void deallocate_raw(T* ptr) noexcept
	{
		::operator delete(ptr);
	}

	void destroy_all() noexcept
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}
	}

	T* data_;
	size_t size_;
};
}  // namespace bmstu
