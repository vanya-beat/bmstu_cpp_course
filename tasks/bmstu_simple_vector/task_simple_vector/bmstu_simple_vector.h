#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "array_ptr.h"

namespace bmstu
{

template <typename T>
class simple_vector
{
   public:
	using Iterator = T*;
	using ConstIterator = const T*;

	simple_vector() noexcept = default;

	explicit simple_vector(size_t size, const T& value = T{})
		: data_(size), size_(size), capacity_(size)
	{
		std::fill(begin(), end(), value);
	}

	simple_vector(std::initializer_list<T> init)
		: data_(init.size()), size_(init.size()), capacity_(init.size())
	{
		std::copy(init.begin(), init.end(), begin());
	}

	simple_vector(const simple_vector& other)
		: data_(other.size_), size_(other.size_), capacity_(other.size_)
	{
		std::copy(other.begin(), other.end(), begin());
	}

	simple_vector(simple_vector&& other) noexcept
		: data_(std::move(other.data_)),
		  size_(other.size_),
		  capacity_(other.capacity_)
	{
		other.size_ = 0;
		other.capacity_ = 0;
	}

	simple_vector& operator=(const simple_vector& rhs)
	{
		if (this != &rhs)
		{
			simple_vector temp(rhs);
			swap(temp);
		}
		return *this;
	}

	simple_vector& operator=(simple_vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			data_ = std::move(rhs.data_);
			size_ = rhs.size_;
			capacity_ = rhs.capacity_;
			rhs.size_ = 0;
			rhs.capacity_ = 0;
		}
		return *this;
	}

	Iterator begin() noexcept { return data_.get(); }
	Iterator end() noexcept { return data_.get() + size_; }

	ConstIterator begin() const noexcept { return data_.get(); }
	ConstIterator end() const noexcept { return data_.get() + size_; }

	T& operator[](size_t index) noexcept
	{
		assert(index < size_);
		return data_[index];
	}

	const T& operator[](size_t index) const noexcept
	{
		assert(index < size_);
		return data_[index];
	}

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[index];
	}

	const T& at(size_t index) const
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[index];
	}

	size_t size() const noexcept { return size_; }
	size_t capacity() const noexcept { return capacity_; }
	bool empty() const noexcept { return size_ == 0; }

	void swap(simple_vector& other) noexcept
	{
		data_.swap(other.data_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= capacity_)
			return;

		array_ptr<T> new_data(new_capacity);
		std::move(begin(), end(), new_data.get());
		data_.swap(new_data);
		capacity_ = new_capacity;
	}

	void resize(size_t new_size)
	{
		if (new_size < size_)
		{
			size_ = new_size;
		}
		else if (new_size > size_)
		{
			reserve(new_size);
			for (size_t i = size_; i < new_size; ++i)
			{
				data_[i] = T{};
			}
			size_ = new_size;
		}
	}

	void push_back(const T& value)
	{
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		data_[size_++] = value;
	}

	void push_back(T&& value)
	{
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		data_[size_++] = std::move(value);
	}

	void pop_back()
	{
		if (size_ > 0)
		{
			--size_;
		}
	}

	Iterator insert(ConstIterator pos, const T& value)
	{
		return insert_impl(pos, value);
	}

	Iterator insert(ConstIterator pos, T&& value)
	{
		return insert_impl(pos, std::move(value));
	}

	Iterator erase(ConstIterator pos) { return erase(pos, pos + 1); }

	Iterator erase(ConstIterator first, ConstIterator last)
	{
		if (first == last)
			return begin() + (first - begin());

		size_t offset = first - begin();
		size_t count = last - first;
		std::move(begin() + offset + count, end(), begin() + offset);
		size_ -= count;
		return begin() + offset;
	}

	void clear() noexcept { size_ = 0; }

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		return lhs.size_ == rhs.size_ &&
			   std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const simple_vector& lhs, const simple_vector& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
											rhs.end());
	}

	friend bool operator<=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>(const simple_vector& lhs, const simple_vector& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator>=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs < rhs);
	}

   private:
	template <typename U>
	Iterator insert_impl(ConstIterator pos, U&& value)
	{
		size_t offset = pos - begin();
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		Iterator insert_pos = begin() + offset;
		std::move_backward(insert_pos, end(), end() + 1);
		*insert_pos = std::forward<U>(value);
		++size_;
		return insert_pos;
	}

	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

}  // namespace bmstu