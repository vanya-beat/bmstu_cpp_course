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

		pointer operator->() const { return ptr_; }

		friend pointer to_address(const iterator& it) noexcept
		{
			return it.ptr_;
		}

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
			iterator tmp = *this;
			++ptr_;
			return tmp;
		}

		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr_;
			return tmp;
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

	   private:
		pointer ptr_ = nullptr;
	};

	simple_vector() noexcept = default;

	~simple_vector() = default;

	simple_vector(std::initializer_list<T> init) noexcept
		: data_(init.size()), size_(init.size()), capacity_(init.size())
	{
		std::copy(init.begin(), init.end(), data_.get());
	}

	simple_vector(const simple_vector& other)
		: data_(other.size_), size_(other.size_), capacity_(other.size_)
	{
		std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
	}

	simple_vector(simple_vector&& other) noexcept { swap(other); }

	simple_vector& operator=(const simple_vector& other)
	{
		if (this != &other)
		{
			simple_vector tmp(other);
			swap(tmp);
		}
		return *this;
	}

	simple_vector(size_t size, const T& value = T{})
		: data_(size), size_(size), capacity_(size)
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i] = value;
		}
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

	typename iterator::reference at(size_t index)
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
		return data_[index];
	}

	typename const_iterator::reference at(size_t index) const
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
		return data_[index];
	}

	size_t size() const noexcept { return size_; }

	size_t capacity() const noexcept { return capacity_; }

	void swap(simple_vector& other) noexcept
	{
		data_.swap(other.data_);
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

	void resize(size_t new_size)
	{
		if (new_size > capacity_)
		{
			reserve(new_size);
		}
		if (new_size > size_)
		{
			for (size_t i = size_; i < new_size; ++i)
			{
				data_[i] = T();
			}
		}
		size_ = new_size;
	}

	iterator insert(const_iterator where, T&& value)
	{
		size_t offset = where - begin();
		if (size_ >= capacity_)
		{
			size_t new_cap = capacity_ ? capacity_ * 2 : 1;
			array_ptr<T> new_data(new_cap);
			for (size_t i = 0; i < offset; ++i)
			{
				new_data[i] = std::move(data_[i]);
			}
			new_data[offset] = std::move(value);
			for (size_t i = offset; i < size_; ++i)
			{
				new_data[i + 1] = std::move(data_[i]);
			}
			data_.swap(new_data);
			capacity_ = new_cap;
		}
		else
		{
			for (size_t i = size_; i > offset; --i)
			{
				data_[i] = std::move(data_[i - 1]);
			}
			data_[offset] = std::move(value);
		}
		++size_;
		return begin() + offset;
	}

	iterator insert(const_iterator where, const T& value)
	{
		T tmp = value;
		return insert(where, std::move(tmp));
	}

	void push_back(T&& value)
	{
		if (size_ >= capacity_)
		{
			size_t new_cap = capacity_ ? capacity_ * 2 : 1;
			array_ptr<T> new_data(new_cap);
			for (size_t i = 0; i < size_; ++i)
			{
				new_data[i] = std::move(data_[i]);
			}
			new_data[size_] = std::move(value);
			data_.swap(new_data);
			capacity_ = new_cap;
		}
		else
		{
			data_[size_] = std::move(value);
		}
		++size_;
	}

	void clear() noexcept { size_ = 0; }

	void push_back(const T& value)
	{
		T tmp = value;
		push_back(std::move(tmp));
	}

	bool empty() const noexcept { return size_ == 0; }

	void pop_back()
	{
		if (size_ > 0)
		{
			--size_;
		}
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		if (lhs.size_ != rhs.size_)
			return false;
		for (size_t i = 0; i < lhs.size_; ++i)
		{
			if (lhs.data_[i] != rhs.data_[i])
				return false;
		}
		return true;
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs)
	{
		return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
													  rhs.begin(), rhs.end());
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		os << "[ ";
		for (size_t i = 0; i < vec.size_; ++i)
		{
			os << vec.data_[i];
			if (i != vec.size_ - 1)
				os << ", ";
		}
		os << " ]";
		return os;
	}

	iterator erase(iterator where)
	{
		size_t offset = where - begin();
		for (size_t i = offset; i < size_ - 1; ++i)
		{
			data_[i] = std::move(data_[i + 1]);
		}
		--size_;
		return begin() + offset;
	}

   private:
	static bool alphabet_compare(const simple_vector<T>& lhs,
								 const simple_vector<T>& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
											rhs.end());
	}
	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

}  // namespace bmstu