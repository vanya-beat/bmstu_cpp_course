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
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;

		iterator() = default;
		explicit iterator(pointer ptr) : ptr_(ptr) {}

		reference operator*() const noexcept { return *ptr_; }
		pointer operator->() const noexcept { return ptr_; }

		iterator& operator++() noexcept
		{
			++ptr_;
			return *this;
		}
		iterator operator++(int) noexcept
		{
			iterator i = *this;
			++ptr_;
			return i;
		}
		iterator& operator--() noexcept
		{
			--ptr_;
			return *this;
		}
		iterator operator--(int) noexcept
		{
			iterator i = *this;
			--ptr_;
			return i;
		}

		iterator operator+(difference_type n) const noexcept
		{
			return iterator(ptr_ + n);
		}
		iterator operator-(difference_type n) const noexcept
		{
			return iterator(ptr_ - n);
		}
		difference_type operator-(const iterator& other) const noexcept
		{
			return ptr_ - other.ptr_;
		}

		iterator& operator+=(difference_type n) noexcept
		{
			ptr_ += n;
			return *this;
		}
		iterator& operator-=(difference_type n) noexcept
		{
			ptr_ -= n;
			return *this;
		}

		bool operator==(const iterator& other) const noexcept
		{
			return ptr_ == other.ptr_;
		}
		bool operator!=(const iterator& other) const noexcept
		{
			return ptr_ != other.ptr_;
		}

		bool operator==(std::nullptr_t) const noexcept
		{
			return ptr_ == nullptr;
		}
		bool operator!=(std::nullptr_t) const noexcept
		{
			return ptr_ != nullptr;
		}
		friend bool operator==(std::nullptr_t, const iterator& it) noexcept
		{
			return it.ptr_ == nullptr;
		}
		friend bool operator!=(std::nullptr_t, const iterator& it) noexcept
		{
			return it.ptr_ != nullptr;
		}

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

		iterator& operator=(const iterator& other) noexcept
		{
			ptr_ = other.ptr_;
			return *this;
		}

		explicit operator bool() const noexcept { return ptr_ != nullptr; }

	   private:
		pointer ptr_ = nullptr;
	};

	using const_iterator = const iterator;

	simple_vector() noexcept = default;

	explicit simple_vector(size_t size)
		: data_(size), size_(size), capacity_(size)
	{
		std::fill(begin(), end(), T());
	}

	simple_vector(size_t size, const T& value)
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

	simple_vector(simple_vector&& other) noexcept { swap(other); }

	~simple_vector() = default;
	simple_vector& operator=(const simple_vector& rhs)
	{
		if (this != &rhs)
		{
			if (rhs.empty())
			{
				clear();
				return *this;
			}

			simple_vector copy(rhs);
			swap(copy);
		}
		return *this;
	}

	simple_vector& operator=(simple_vector&& rhs) noexcept
	{
		swap(rhs);
		return *this;
	}

	iterator begin() noexcept { return iterator(data_.get()); }
	iterator end() noexcept { return iterator(data_.get() + size_); }
	const_iterator begin() const noexcept { return iterator(data_.get()); }
	const_iterator end() const noexcept
	{
		return iterator(data_.get() + size_);
	}

	T& operator[](size_t index) noexcept { return data_[index]; }
	const T& operator[](size_t index) const noexcept { return data_[index]; }

	T& at(size_t index)
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
		return data_[index];
	}

	const T& at(size_t index) const
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
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

	friend void swap(simple_vector<T>& lhs, simple_vector<T>& rhs)
	{
		lhs.swap(rhs);
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity_)
		{
			array_ptr<T> new_data(new_capacity);
			for (size_t i = 0; i < size_; ++i)
			{
				new_data[i] = std::move(data_[i]);
			}
			data_.swap(new_data);
			capacity_ = new_capacity;
		}
	}

	void resize(size_t new_size)
	{
		if (new_size > capacity_)
		{
			size_t new_capacity = std::max(new_size, capacity_ * 2);
			reserve(new_capacity);
		}
		if (size_ > new_size)
		{
			size_ = new_size;
			return;
		}
		for (auto it = end(); it != begin() + new_size; ++it)
		{
			*it = T{};
		}
		size_ = new_size;
	}

    template <typename U>
    void push_back_impl(U&& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }
        data_[size_++] = std::forward<U>(value);
    }

    void push_back(const T& value) {
        push_back_impl(value);  // Вызов с lvalue - произойдет копирование
    }

    void push_back(T&& value) {
        push_back_impl(std::move(value));  // Вызов с rvalue - произойдет перемещение
    }
	

	// void push_back(const T& value)
	// {
	// 	T tmp_value = value;

	// 	if (size_ >= capacity_)
	// 	{
	// 		size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
	// 		array_ptr<T> new_data(new_capacity);

	// 		for (size_t i = 0; i < size_; ++i)
	// 		{
	// 			new_data[i] = std::move(data_[i]);
	// 		}
	// 		new_data[size_] = std::move(tmp_value);

	// 		data_.swap(new_data);
	// 		capacity_ = new_capacity;
	// 	}
	// 	else
	// 	{
	// 		data_[size_] = std::move(tmp_value);
	// 	}
	// 	++size_;
	// }

	// void push_back(T&& value)
	// {
	// 	if (size_ >= capacity_) // DRY
	// 	{
	// 		size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
	// 		array_ptr<T> new_data(new_capacity);
	// 		for (size_t i = 0; i < size_; ++i)
	// 		{
	// 			new_data[i] = std::move(data_[i]);
	// 		}
	// 		new_data[size_] = std::move(value);
	// 		data_.swap(new_data);
	// 		capacity_ = new_capacity;
	// 		++size_;
	// 	}
	// 	else
	// 	{
	// 		data_[size_++] = std::move(value);
	// 	}
	// }

	void pop_back()
	{
		if (size_ > 0)
		{
			--size_;
		}
	}

	iterator insert(const_iterator pos, const T& value)
	{
		size_t offset = pos - begin();
		if (size_ >= capacity_)
			reserve(capacity_ ? capacity_ * 2 : 1);
		std::move_backward(begin() + offset, end(), end() + 1);
		data_[offset] = value;
		++size_;
		return begin() + offset;
	}

	iterator erase(iterator pos)
	{
		std::move(pos + 1, end(), pos);
		--size_;
		return pos;
	}

	void clear() noexcept { size_ = 0; }

	simple_vector& concat(const simple_vector& other)
	{
		size_t new_size = size_ + other.size_;
		if (new_size > capacity_)
		{
			reserve(new_size);
		}

		std::copy(other.begin(), other.end(), end());
		size_ = new_size;

		return *this;
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

	friend bool operator<(const simple_vector& lhs, const simple_vector& rhs)
	{
		auto lb = lhs.begin();
		auto rb = rhs.begin();
		auto le = lhs.end();
		auto re = rhs.end();
		for (; (lb != le) && (rb != re); ++lb, ++rb)
		{
			if (*lb < *rb)
			{
				return true;
			}
			if (*lb > *rb)
			{
				return false;
			}
		}
		return (lb == le) && (rb != re);
	}

	friend bool operator>(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs <= rhs);
	}

	friend bool operator<=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return (lhs < rhs || lhs == rhs);
	}

	friend bool operator>=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs < rhs);
	}

   private:
	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

}  //  namespace bmstu
