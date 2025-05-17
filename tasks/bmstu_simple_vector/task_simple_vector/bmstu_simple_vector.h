#include <ostream>
#include <stdexcept>
#include <utility>
#include "array_ptr.h"
#include <algorithm>
#include <compare>

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

		iterator(iterator&& other) noexcept { std::exchange(ptr_, other.ptr_); }

		explicit iterator(pointer ptr) : ptr_(ptr) {}

		reference operator*() const { return *ptr_; }

		pointer operator->() const { return ptr_; }

		friend pointer to_address(const iterator& it) noexcept
		{
			return it.ptr_;
		}

		iterator& operator=(const iterator& other) = default;

		iterator& operator=(iterator&& other) noexcept { std::exchange(*ptr_, other.ptr_); return *this; }

#pragma region Operators
		iterator& operator++() { ++ptr_; return *this; }

		iterator& operator--() { --ptr_; return *this; }

		iterator operator++(int) { ++ptr_; return this; }

		iterator operator--(int) { --ptr_; return this; }

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
			return !(lhs == rhs);
		}

		iterator operator+=(const difference_type& n) noexcept
		{
			return  (this->ptr_ + n);
		}

		iterator operator+(const difference_type& n) const noexcept
		{
			iterator copy(*this);
			copy.ptr_ += n;
			return copy;
		}

		iterator operator-=(const difference_type& n) noexcept
		{
			return (this->ptr_ - n);
		}

		iterator operator-(const difference_type& n) const noexcept
		{
			iterator copy(*this);
			copy.ptr_ -= n;
			return copy;
		}

		friend difference_type operator-(const iterator& end,
										 const iterator& begin) noexcept
		{
			return end.ptr_ - begin.ptr_;
		}

#pragma endregion
	   private:
		pointer ptr_ = nullptr;
	};

	simple_vector() noexcept = default;

	~simple_vector() = default;

	simple_vector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()), data_(init.size()) {
		iterator it = begin();
		for (auto el = init.begin(); el != init.end(); ++el, ++it) {
			*it = *el;
		}
	}

	simple_vector(const simple_vector& other) : size_(other.size_), capacity_(other.capacity_), data_(other.size_){
		for (auto it1 = begin(), it2 = other.begin(); it2 != other.end(); ++it1, ++it2) {
			*it1 = *it2;
		}
	}

	simple_vector(simple_vector&& other) noexcept { this->swap(other); }

	simple_vector& operator=(const simple_vector& other) { 
		if (other.empty()) {
			clear();
			return *this;
		}
		size_ = other.size_;
		capacity_ = other.capacity_;
		for (auto it1 = begin(), it2 = other.begin(); it2 != other.end(); ++it1, ++it2) {
			*it1 = *it2;
		}
		return *this;
	}

	simple_vector(size_t size, const T& value = T{}) : size_(size), capacity_(size), data_(size) {
		for (auto it = begin(); it != end(); ++it) {
			*it = value;
		}
	}

	iterator begin() noexcept { return iterator(data_.get()); }

	iterator end() noexcept { return iterator((data_.get() + size_)); }

	using const_iterator = iterator;

	const_iterator begin() const noexcept { return iterator(data_.get()); }

	const_iterator end() const noexcept { return iterator((data_.get() + size_)); }

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

	size_t size() const noexcept { return size_; }

	size_t capacity() const noexcept { return capacity_; }

	void swap(simple_vector& other) noexcept {
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
		data_.swap(other.data_);
	}

	friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept {
		lhs.swap(rhs);
	}

	void reserve(size_t new_cap) {
		if (new_cap > capacity_) {
			capacity_ = std::max(new_cap, capacity_ * 2);
			array_ptr<T> new_data(new_cap);
			for (auto it1 = begin(), it2 = iterator(new_data.get()); it1 != end(); ++it1, ++it2) {
				*it2 = std::move(*it1);
			}
			data_.swap(new_data);
			capacity_ = new_cap;
		}
	}

	void resize(size_t new_size) { 
		if (new_size > capacity_) {
			size_t new_cap = std::max(new_size, capacity_ * 2);
			reserve(new_cap);
		}
		if (size_ > new_size) {
			size_ = new_size;
		}
		for (auto it = end(); it != begin() + new_size; ++it) {
			*it = T{};
		}
		size_ = new_size;
	}

	iterator insert(const_iterator where, T&& value) { 
		auto index = where - begin();
		if (capacity_ == 0) {
			reserve(1);
		}
		if (size_ == capacity_) {
			capacity_ *= 2;
		}
		array_ptr<T> temp_data(capacity_);
		T* ptr_td = temp_data.get();
		for (auto it = begin(); it != begin() + index; ++it, ++ptr_td) {
			*ptr_td = std::move(*it);
		}
		temp_data[index] = value;
		ptr_td = temp_data.get() + index + 1;
		if (index + 1 < size_) {
			for (auto it = begin() + index; it != end(); ++it, ++ptr_td) {
				*ptr_td = std::move(*it);
			}
		}
		data_.swap(temp_data);
		++size_;
		return begin() + index;
	}

	iterator insert(const_iterator where, const T& value) { 
		auto index = where - begin();
		if (capacity_ == 0) {
			reserve(1);
		}
		if (size_ == capacity_) {
			capacity_ *= 2;
		}
		array_ptr<T> temp_data(capacity_);
		T* ptr_td = temp_data.get();
		for (auto it = begin(); it != begin() + index; ++it, ++ptr_td) {
			*ptr_td = std::move(*it);
		}
		temp_data[index] = value;
		ptr_td = temp_data.get() + index + 1;
		if (index + 1 < size_) {
			for (auto it = begin() + index; it != end(); ++it, ++ptr_td) {
				*ptr_td = std::move(*it);
			}
		}
		data_.swap(temp_data);
		++size_;
		return begin() + index;
	}

	void push_back(T&& value) {
		insert(end(), std::move(value));
	}

	void clear() noexcept { size_ = 0; }

	void push_back(const T& value) {
		insert(end(), value);
	}

	bool empty() const noexcept { return size_ == 0; }

	void pop_back() { 
		if (size_ != 0) { (*this)[--size_] = {}; }
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs) {
		if (lhs.size() != rhs.size()) return false;
		for (auto it1 = lhs.begin(), it2 = rhs.begin(); it1 != lhs.end(); ++it1, ++it2) {
			if (*it1 != *it2) return false;
		}
		return true;
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs) {
		return !(lhs == rhs);
	}

	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs) {
		if (lhs < rhs) return std::strong_ordering::less;
		if (lhs > rhs) return std::strong_ordering::greater;
		if (lhs == rhs) return std::strong_ordering::equal;
	}

	friend bool operator<(const simple_vector& lhs, const simple_vector& rhs) {
		return alphabet_compare(lhs, rhs);
	}

	friend bool operator>(const simple_vector& lhs, const simple_vector& rhs) {
		return !(lhs <= rhs);
	}

	friend bool operator<=(const simple_vector& lhs, const simple_vector& rhs) {
		return (lhs < rhs || lhs == rhs);
	}

	friend bool operator>=(const simple_vector& lhs, const simple_vector& rhs) {
		return !(lhs < rhs);
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		os << "[";
		for (size_t i = 0; i < vec.size(); ++i) {
			os << vec[i];
			if (i != vec.size() - 1) {
				os << ", ";
			}
		}
		os << " ]";
		return os;
	}
	iterator erase(iterator where) { 
		auto index = where - begin();
		if (!empty()) {
			std::move(begin() + index + 1, end(), begin() + index);
			--size_;
		}
		return begin() + index;
	}

	simple_vector& concat(const simple_vector& vec) {
		if (vec.size_ == 0) {
			return *this;
		}
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			push_back(*it);
		}
		return *this;
	}

   private:
	static bool alphabet_compare(const simple_vector<T>& lhs,
								 const simple_vector<T>& rhs)
	{
		auto it1 = lhs.begin(); 
		auto it2 = rhs.begin();
		for (; it1 != lhs.end(), it2 != rhs.end(); ++it1, ++it2) {
			if (*it1 < *it2) { return true; }
			if (*it1 > *it2) { return false; }
		}
		return (it1 == lhs.end()) && (it2 != rhs.end());
	}
	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};
}  // namespace bmstu
