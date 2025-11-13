#include <algorithm>
#include <compare>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
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

		iterator(iterator&& other) noexcept : ptr_(other.ptr_)
		{
			other.ptr_ = nullptr;
		}

		explicit iterator(pointer ptr) : ptr_(ptr) {}

		reference operator*() const { return *ptr_; }

		pointer operator->() { return ptr_; }

		iterator& operator=(const iterator& other) = default;

		iterator& operator=(iterator&& other) noexcept
		{
			ptr_ = other.ptr_;
			other.ptr_ = nullptr;
			return *this;
		}

		iterator& operator++()
		{
			++ptr_;
			return *this;
		}

		iterator& operator--()
		{
			--ptr_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++ptr_;
			return temp;
		}

		iterator operator--(int)
		{
			iterator temp = *this;
			--ptr_;
			return temp;
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

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

		friend bool operator==(std::nullptr_t, const iterator& rhs)
		{
			return rhs.ptr_ == nullptr;
		}

		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ != rhs.ptr_;
		}

		iterator operator+(difference_type n) const noexcept
		{
			return iterator(ptr_ + n);
		}

		iterator operator+=(difference_type n) noexcept
		{
			ptr_ += n;
			return *this;
		}

		iterator operator-(difference_type n) const noexcept
		{
			return iterator(ptr_ - n);
		}

		iterator operator-=(difference_type n) noexcept
		{
			ptr_ -= n;
			return *this;
		}

		friend difference_type operator-(const iterator& end,
										 const iterator& begin) noexcept
		{
			return end.ptr_ - begin.ptr_;
		}

	   private:
		pointer ptr_ = nullptr;
	};

	simple_vector() noexcept = default;

	~simple_vector() { clear(); }

	simple_vector(std::initializer_list<T> init) noexcept
	{
		reserve(init.size());
		for (const T& item : init)
		{
			push_back(item);
		}
	}

	simple_vector(const simple_vector& other)
	{
		reserve(other.size_);
		for (size_t i = 0; i < other.size_; ++i)
		{
			push_back(other.data_[i]);
		}
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

	simple_vector(size_t size, const T& value = T{}) { resize(size, value); }

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

	typename iterator::reference at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[index];
	}

	typename const_iterator::reference at(size_t index) const
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[index];
	}

	size_t size() const noexcept { return size_; }

	size_t capacity() const noexcept { return capacity_; }

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
			for (size_t i = 0; i < size_; ++i)
			{
				new_data[i] = std::move(data_[i]);
			}
			data_.swap(new_data);
			capacity_ = new_cap;
		}
	}

	void resize(size_t new_size, const T& value = T{})
	{
		if (new_size > size_)
		{
			reserve(new_size);
			for (size_t i = size_; i < new_size; ++i)
			{
				data_[i] = value;
			}
		}
		size_ = new_size;
	}

	iterator insert(const_iterator where, T&& value)
	{
		size_t pos = where - begin();
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		for (size_t i = size_; i > pos; --i)
		{
			data_[i] = std::move(data_[i - 1]);
		}
		data_[pos] = std::move(value);
		++size_;
		return iterator(data_.get() + pos);
	}

	iterator insert(const_iterator where, const T& value)
	{
		return insert(where, T(value));
	}

	void push_back(T&& value) { insert(end(), std::move(value)); }

	void push_back(const T& value) { insert(end(), value); }

	void clear() noexcept { size_ = 0; }

	bool empty() const noexcept { return size_ == 0; }

	void pop_back()
	{
		if (size_ > 0)
		{
			--size_;
		}
	}

	iterator erase(iterator where)
	{
		size_t pos = where - begin();
		for (size_t i = pos; i < size_ - 1; ++i)
		{
			data_[i] = std::move(data_[i + 1]);
		}
		--size_;
		return iterator(data_.get() + pos);
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return !LexicographicalCompare(lhs, rhs) && !LexicographicalCompare(rhs, lhs);
}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs) {
    return LexicographicalCompare(lhs, rhs) ? std::strong_ordering::less
         : (lhs == rhs)                    ? std::strong_ordering::equal
         : std::strong_ordering::greater;
}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		for (size_t i = 0; i < vec.size_; ++i)
		{
			os << vec.data_[i] << " ";
		}
		return os;
	}

   private:
   static bool LexicographicalCompare(const simple_vector& lhs, const simple_vector& rhs) {
    auto it1 = lhs.begin();
    auto it2 = rhs.begin();
    
    for (; it1 != lhs.end() && it2 != rhs.end(); ++it1, ++it2) {
        if (*it1 < *it2) {
            return true;
        }
		if (*it2 < *it1){
            return false;
        }
    }
	return (it1 == lhs.end()) && (it2 != rhs.end());	 
	}

	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

}  // namespace bmstu