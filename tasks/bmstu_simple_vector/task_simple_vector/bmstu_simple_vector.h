#include <algorithm>
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

		iterator() noexcept : ptr_(nullptr) {}
		iterator(const iterator& other) noexcept = default;
		explicit iterator(pointer ptr) noexcept : ptr_(ptr) {}

		reference operator*() const { return *ptr_; }
		pointer operator->() const { return ptr_; }

		iterator& operator++()
		{
			++ptr_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr_;
			return tmp;
		}

		iterator& operator--()
		{
			--ptr_;
			return *this;
		}

		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr_;
			return tmp;
		}

		iterator& operator+=(difference_type n)
		{
			ptr_ += n;
			return *this;
		}
		iterator operator+(difference_type n) const
		{
			return iterator(ptr_ + n);
		}
		iterator& operator-=(difference_type n)
		{
			ptr_ -= n;
			return *this;
		}
		iterator operator-(difference_type n) const
		{
			return iterator(ptr_ - n);
		}
		friend difference_type operator-(const iterator& lhs,
										 const iterator& rhs)
		{
			return lhs.ptr_ - rhs.ptr_;
		}

		friend bool operator==(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ == rhs.ptr_;
		}
		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ != rhs.ptr_;
		}
		friend bool operator<(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ < rhs.ptr_;
		}
		friend bool operator>(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ > rhs.ptr_;
		}
		friend bool operator<=(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ <= rhs.ptr_;
		}
		friend bool operator>=(const iterator& lhs, const iterator& rhs)
		{
			return lhs.ptr_ >= rhs.ptr_;
		}

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

		friend bool operator==(const iterator& it, std::nullptr_t) noexcept
		{
			return it.ptr_ == nullptr;
		}
		friend bool operator==(std::nullptr_t, const iterator& it) noexcept
		{
			return it.ptr_ == nullptr;
		}
		friend bool operator!=(const iterator& it, std::nullptr_t) noexcept
		{
			return it.ptr_ != nullptr;
		}
		friend bool operator!=(std::nullptr_t, const iterator& it) noexcept
		{
			return it.ptr_ != nullptr;
		}

	   private:
		pointer ptr_;
	};

	simple_vector() noexcept = default;

	simple_vector(size_t size, const T& value = T{})
		: data_(size), size_(size), capacity_(size)
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i] = value;
		}
	}

	simple_vector(std::initializer_list<T> init) : simple_vector(init.size())
	{
		size_t i = 0;
		for (const T& elem : init)
		{
			data_[i++] = elem;
		}
	}

	simple_vector(const simple_vector& other)
		: data_(other.capacity_), size_(other.size_), capacity_(other.capacity_)
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i] = other.data_[i];
		}
	}

	simple_vector(simple_vector&& other) noexcept
		: data_(std::move(other.data_)),
		  size_(other.size_),
		  capacity_(other.capacity_)
	{
		other.size_ = 0;
		other.capacity_ = 0;
	}

	simple_vector& operator=(const simple_vector& other)
	{
		if (this != &other)
		{
			simple_vector tmp(other);
			swap(tmp);
		}
		return *this;
	}

	simple_vector& operator=(simple_vector&& other) noexcept
	{
		if (this != &other)
		{
			data_ = std::move(other.data_);
			size_ = other.size_;
			capacity_ = other.capacity_;
			other.size_ = 0;
			other.capacity_ = 0;
		}
		return *this;
	}

	~simple_vector() = default;

	iterator begin() noexcept { return iterator(data_.get()); }
	iterator end() noexcept { return iterator(data_.get() + size_); }
	const iterator begin() const noexcept { return iterator(data_.get()); }
	const iterator end() const noexcept
	{
		return iterator(data_.get() + size_);
	}

	T& operator[](size_t index) noexcept { return data_.get()[index]; }
	const T& operator[](size_t index) const noexcept
	{
		return data_.get()[index];
	}

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_.get()[index];
	}

	const T& at(size_t index) const
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_.get()[index];
	}

	size_t size() const noexcept { return size_; }
	size_t capacity() const noexcept { return capacity_; }
	bool empty() const noexcept { return size_ == 0; }

	void clear() noexcept { size_ = 0; }

	void reserve(size_t new_cap)
	{
		if (new_cap > capacity_)
		{
			array_ptr<T> new_data;
			if (size_ == 0)
			{
				new_data = array_ptr<T>(new T[new_cap]);
			}
			else
			{
				new_data = array_ptr<T>(new_cap);
			}
			for (size_t i = 0; i < size_; ++i)
			{
				new_data[i] = std::move(data_[i]);
			}
			capacity_ = new_cap;
			data_.swap(new_data);
		}
	}

	void resize(size_t new_size, const T& value = T{})
	{
		if (new_size > capacity_)
		{
			reserve(new_size);
		}
		if (new_size > size_)
		{
			for (size_t i = size_; i < new_size; ++i)
			{
				data_[i] = value;
			}
		}
		size_ = new_size;
	}

	void push_back(const T& value)
	{
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		T temp(value);
		T copy(temp);
		data_[size_++] = std::move(copy);
	}

	void push_back(T&& value)
	{
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		T temp(std::move(value));
		T copy(temp);
		data_[size_++] = std::move(copy);
	}

	void pop_back()
	{
		if (empty())
		{
			throw std::out_of_range("simple_vector::pop_back: vector is empty");
		}
		--size_;
	}

	iterator insert(const iterator where, const T& value)
	{
		auto index = where - begin();
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		for (size_t i = size_; i > static_cast<size_t>(index); --i)
		{
			data_[i] = std::move(data_[i - 1]);
		}
		data_[index] = value;
		++size_;
		return iterator(data_.get() + index);
	}

	iterator insert(const iterator where, T&& value)
	{
		auto index = where - begin();
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		for (size_t i = size_; i > static_cast<size_t>(index); --i)
		{
			data_[i] = std::move(data_[i - 1]);
		}
		data_[index] = std::move(value);
		++size_;
		return iterator(data_.get() + index);
	}

	iterator erase(iterator where)
	{
		if (where == end())
		{
			return end();
		}
		auto index = where - begin();
		for (size_t i = index; i < size_ - 1; ++i)
		{
			data_[i] = std::move(data_[i + 1]);
		}
		--size_;
		return iterator(data_.get() + index);
	}

	void swap(simple_vector& other) noexcept
	{
		using std::swap;
		swap(data_, other.data_);
		swap(size_, other.size_);
		swap(capacity_, other.capacity_);
	}

	friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		if (lhs.size_ != rhs.size_)
		{
			return false;
		}
		for (size_t i = 0; i < lhs.size_; ++i)
		{
			if (lhs.data_.get()[i] != rhs.data_.get()[i])
			{
				return false;
			}
		}
		return true;
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs)
	{
		size_t min_size = std::min(lhs.size_, rhs.size_);
		for (size_t i = 0; i < min_size; ++i)
		{
			if (auto cmp = lhs.data_.get()[i] <=> rhs.data_.get()[i]; cmp != 0)
			{
				return cmp;
			}
		}
		if (lhs.size_ < rhs.size_)
			return std::strong_ordering::less;
		else if (lhs.size_ > rhs.size_)
			return std::strong_ordering::greater;
		else
			return std::strong_ordering::equal;
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		os << "[ ";
		for (size_t i = 0; i < vec.size_; ++i)
		{
			os << vec.data_.get()[i];
			if (i < vec.size_ - 1)
			{
				os << ", ";
			}
		}
		os << " ]";
		return os;
	}

   private:
	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};
}  // namespace bmstu
