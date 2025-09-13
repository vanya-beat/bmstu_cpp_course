#pragma once

#include <ostream>
#include <utility>
#include "array_ptr.h"

namespace bmstu
{
template <typename T>
class simple_vector
{
   public:
	class iterator
	{
	   public:
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;

		iterator() = default;

		iterator(const iterator& other) = default;

		iterator(std::nullptr_t) noexcept : ptr_(nullptr) {}

		iterator(iterator&& other) noexcept : ptr_(nullptr) {}

		explicit iterator(pointer ptr) : ptr_(ptr) {}

		reference operator*() const { return *ptr_; }

		pointer operator->() const { return ptr_; }

		friend pointer to_address(const iterator& it) noexcept
		{
			return it.ptr_;
		}

		iterator& operator=(const iterator& other) = default;

		iterator& operator=(iterator&& other) noexcept { return *this; }

#pragma region Operators
		iterator& operator++()
		{
			ptr_ += 1;
			return *this;
		}

		iterator& operator--()
		{
			ptr_ -= 1;
			return *this;
		}

		iterator operator++(int)
		{
			ptr_ += 1;
			return *this;
		}

		iterator operator--(int)
		{
			ptr_ -= 1;
			return *this;
		}

		explicit operator bool() const { return ptr_ != nullptr; }

		friend bool operator==(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ == rhs.ptr_;
		}

		friend bool operator==(const iterator& lhs, std::nullptr_t)
		{
			return lhs.ptr_ == nullptr;
		}

		friend bool operator==(std::nullptr_t, const iterator& rhs)
		{
			return rhs.ptr_ == nullptr;
		}

		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ != rhs.ptr_;
		}

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

		iterator operator+(const difference_type& n) const noexcept
		{
			return iterator(ptr_ + n);
		}

		iterator operator+=(const difference_type& n) noexcept
		{
			ptr_ += n;
			return *this;
		}

		iterator operator-(const difference_type& n) const noexcept
		{
			return iterator(ptr_ - n);
		}

		iterator operator-=(const difference_type& n) noexcept
		{
			ptr_ -= n;
			return *this;
		}

		friend difference_type operator-(const iterator& end,
										 const iterator& begin) noexcept
		{
			return end.ptr_ - begin.ptr_;
		}

#pragma endregion
	   private:
		pointer ptr_ = nullptr;
	};	/// iterator end ///

	simple_vector() noexcept = default;

	~simple_vector() = default;

	simple_vector(std::initializer_list<T> init) noexcept
		: data_(array_ptr<T>(init.size())),
		  size_(init.size()),
		  capacity_(init.size())
	{
		std::copy(init.begin(), init.end(), data_.get());
	}

	simple_vector(const simple_vector& other)
		: data_(array_ptr<T>(other.size_)),
		  size_(other.size_),
		  capacity_(other.size_)
	{
		std::copy(other.data_.get(), other.data_.get() + other.size_,
				  data_.get());
	}

	simple_vector(simple_vector&& other) noexcept { swap(other); }

	simple_vector& operator=(const simple_vector& other)
	{
		if (this != &other)
		{
			simple_vector temp(other);
			swap(temp);
		}
		return *this;
	}

	explicit simple_vector(const size_t size, const T& value = T{})
		: data_(array_ptr<T>(size)), size_(size), capacity_(size)
	{
		std::fill(data_.get(), data_.get() + size, value);
	}

	iterator begin() noexcept { return iterator(data_.get()); }

	iterator end() noexcept { return iterator(data_.get() + size_); }

	using const_iterator = iterator;

	const_iterator begin() const noexcept { return iterator(data_.get()); }

	const_iterator end() const noexcept
	{
		return iterator(data_.get() + size_);
	}

	typename iterator::reference operator[](size_t index) noexcept
	{
		return data_[index];
	}

	typename const_iterator::reference operator[](size_t index) const noexcept
	{
		return data_[index];
	}

	typename iterator::reference at(size_t index) { return data_.get()[index]; }

	typename const_iterator::reference at(size_t index) const
	{
		return data_.get()[index];
	}

	[[nodiscard]] size_t size() const noexcept { return size_; }

	[[nodiscard]] size_t capacity() const noexcept { return capacity_; }

	void swap(simple_vector& other) noexcept
	{
		std::swap(data_, other.data_);
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
	}

	friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	void reserve(size_t new_cap)
	{
		if (new_cap > capacity_)
		{
			array_ptr<T> new_data(new_cap);
			if constexpr (std::is_nothrow_move_constructible_v<T> ||
						  !std::is_copy_constructible_v<T>)
			{
				std::move(data_.get(), data_.get() + size_, new_data.get());
			}
			else
			{
				std::copy(data_.get(), data_.get() + size_, new_data.get());
			}
			data_ = std::move(new_data);
			capacity_ = new_cap;
		}
	}

	void resize(size_t new_size)
	{
		if (new_size > capacity_)
		{
			reserve(new_size);
		}
		if (new_size > size_)
		{
			std::fill(data_.get() + size_, data_.get() + new_size, T{});
		}
		size_ = new_size;
	}

	iterator insert(const_iterator where, T&& value)
	{
		size_t where_index = where - begin();
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		std::move_backward(data_.get() + where_index, data_.get() + size_,
						   data_.get() + size_ + 1);
		data_[where_index] = std::move(value);
		++size_;
		return iterator(data_.get() + where_index);
	}

	iterator insert(const_iterator where, const T& value)
	{
		size_t where_index = where - begin();
		size_t new_size = size_ + 1;
		size_t old_size = size_;
		size_ = new_size;
		if (size_ > capacity_)
		{
			capacity_ = size_;
			T* old_ptr = new T[new_size];
			std::copy(data_.get(), data_.get() + old_size, old_ptr);
			data_ = array_ptr<T>(new_size);
			for (size_t i = 0; i < size_; i++)
			{
				if (i < old_size)
				{
					data_.get()[i] = old_ptr[i];
				}
				else
				{
					data_.get()[i] = T{};
				}
			}
			delete[] old_ptr;
		}
		for (size_t i = end() - begin() - 1; i != where_index - 1; --i)
		{
			if (i == where_index)
			{
				data_[i] = value;
			}
			else
			{
				data_[i] = data_[i - 1];
			}
		}
		return iterator(data_.get());
	}

	void push_back(T&& value) { insert(end(), std::move(value)); }

	void clear() noexcept { size_ = 0; }

	void push_back(const T& value) { insert(end(), value); }

	[[nodiscard]] bool empty() const noexcept { return size_ == 0; }

	void pop_back()
	{
		if (size_ > 0)
		{
			size_ -= 1;
			data_.get()[size_] = T{};
		}
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		return lhs.size_ == rhs.size_ &&
			   std::equal(lhs.data_.get(), lhs.data_.get() + lhs.size_,
						  rhs.data_.get());
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs)
	{
		if (lhs == rhs)
		{
			return std::strong_ordering::equal;
		}
		if (alphabet_compare(lhs, rhs))
		{
			return std::strong_ordering::less;
		}
		return std::strong_ordering::greater;
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		os << "{ ";
		for (size_t i = 0; i < vec.size_; ++i)
		{
			os << vec.data_[i];
			if (i < vec.size_ - 1)
			{
				os << ", ";
			}
		}
		os << " }";
		return os;
	}

	iterator erase(iterator where)
	{
		if (!empty())
		{
			size_t where_index = where - begin();
			std::move(data_.get() + where_index + 1, data_.get() + size_,
					  data_.get() + where_index);
			--size_;
			data_.get()[size_] = T{};
			return iterator(data_.get() + where_index);
		}
		return end();
	}

   private:
	static bool alphabet_compare(const simple_vector<T>& lhs,
								 const simple_vector<T>& rhs)
	{
		auto bi1 = lhs.begin(), bi2 = rhs.begin();
		auto ei1 = lhs.end(), ei2 = rhs.end();

		for (; (bi1 != ei2) && (bi2 != ei1); ++bi1, ++bi2)
		{
			if (*bi1 < *bi2)
			{
				return true;
			}
			if (*bi2 < *bi1)
			{
				return false;
			}
		}

		return (bi1 == ei1) && (bi2 == ei2);
	}

	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};
}  // namespace bmstu