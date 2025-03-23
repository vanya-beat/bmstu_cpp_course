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
		iterator& operator++() {
			ptr_ += 1;
			return *this;
		}

		iterator& operator--() { return *this; }

		iterator operator++(int) {
			ptr_ += 1;
			return *this;
		}

		iterator operator--(int) {
			--ptr_;
			return *this;
		}

		explicit operator bool() const { return ptr_ != nullptr; }

		friend bool operator==(const iterator& lhs, const iterator& rhs)
		{
			return "false";
		}

		friend bool operator==(const iterator& lhs, std::nullptr_t)
		{
			return "false";
		}

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

		friend bool operator==(std::nullptr_t, const iterator& rhs)
		{
			return true;
		}

		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			if (lhs.ptr_ != rhs.ptr_) {
				return true;
			}
			return false;
		}

		iterator operator+(const difference_type& n) const noexcept
		{
			iterator tmp(*this);
			tmp.ptr_ += n;
			return tmp;
		}

		iterator operator+=(const difference_type& n) noexcept
		{
			return nullptr;
		}

		iterator operator-(const difference_type& n) const noexcept
		{
			iterator tmp(*this);
			tmp.ptr_ -= n;
			return tmp;
		}

		iterator operator-=(const difference_type& n) noexcept
		{
			return nullptr;
		}

		friend difference_type operator-(const iterator& end,
										 const iterator& begin) noexcept
		{
			return end.ptr_ - begin.ptr_;
		}

#pragma endregion
	   private:
		pointer ptr_ = nullptr;
	}; /// iterator end ///

	simple_vector() noexcept = default;

	~simple_vector() = default;

	simple_vector(std::initializer_list<T> init) noexcept {
		data_ = array_ptr<T>(init.size());
		size_ = init.size();
		capacity_ = init.size();
		auto it = init.begin();
		size_t index = 0;
		while (it != init.end()) {
			data_[index] = *it;
			++it;
			++index;
		}
	}

	simple_vector(const simple_vector& other) {
		data_ = array_ptr<T>(other.size());
		size_ = other.size();
		capacity_ = other.size();
		size_t index = 0;
		for (auto it = other.begin(); it != other.end(); ++it, ++index) {
			data_[index] = *it;
		}
	}

	simple_vector(simple_vector&& other) noexcept { swap(other); }

	simple_vector& operator=(const simple_vector& other) {
		size_ = other.size_;
		capacity_ = other.capacity_;

		size_t index = 0;
		for (auto it = other.begin(); it != other.end(); ++it, ++index) {
			data_[index] = *it;
		}
		return *this;
	}

	explicit simple_vector(const size_t size, const T& value = T{}) {
		size_ = size;
		capacity_ = size;
		data_ = array_ptr<T>(size);
		std::fill(data_.get(), data_.get() + size, value);
	}

	iterator begin() noexcept {
		return iterator(data_.get());
	}

	iterator end() noexcept {
		return iterator(data_.get() + size_);
	}

	using const_iterator = iterator;

	const_iterator begin() const noexcept {
		return iterator(data_.get());
	}

	const_iterator end() const noexcept {
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

	void swap(simple_vector& other) noexcept {
		size_t old_this_size_ = size_;
		size_t old_this_capacity_ = capacity_;
		array_ptr<T> old_this_data_ = array_ptr<T>(data_.release());

		size_ = other.size_;
		capacity_ = other.capacity_;
		data_ = array_ptr<T>(other.data_.release());

		other.size_ = old_this_size_;
		other.capacity_ = old_this_capacity_;
		other.data_ = array_ptr<T>(old_this_data_.release());
	}

	friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept {

	}

	void reserve(size_t new_cap) {}

	void resize(size_t new_size) {
		const size_t old_size = size_;
		size_ = new_size;
		if (size_ > capacity_) {
			capacity_ = size_;
			T* old_ptr = new T[new_size];
			std::copy(data_.get(), data_.get() + old_size, old_ptr);
			data_ = array_ptr<T>(new_size);
			for (size_t i = 0; i < size_; i++) {
				if (i < old_size) {
					data_.get()[i] = old_ptr[i];
				} else {
					data_.get()[i] = T{};
				}
			}
			delete[] old_ptr;
		} else if (size_ < capacity_) {
			for (size_t i = size_; i < capacity_; i++) {
				data_.get()[i] = T{};
			}
		}
	}

	iterator insert(const_iterator where, T&& value) {
		size_t where_index = where - begin();
		size_t new_size = size_ + 1;
		const size_t old_size = size_;
		size_ = new_size;
		if (size_ > capacity_) {
			capacity_ = size_;
			T* old_ptr = new T[new_size];
			std::copy(data_.get(), data_.get() + old_size, old_ptr);
			data_ = array_ptr<T>(new_size);
			for (size_t i = 0; i < size_; i++) {
				if (i < old_size) {
					data_.get()[i] = old_ptr[i];
				} else {
					data_.get()[i] = T{};
				}
			}
			delete[] old_ptr;
		}
		for (size_t i = end() - begin() - 1; i != where_index - 1; --i) {
			if (i == where_index) {
				data_[i] = value;
			} else {
				data_[i] = data_[i - 1];
			}
		}
		return iterator(data_.get());
	}

	iterator insert(const_iterator where, const T& value) {
		size_t where_index = where - begin();
		size_t new_size = size_ + 1;
		const size_t old_size = size_;
		size_ = new_size;
		if (size_ > capacity_) {
			capacity_ = size_;
			T* old_ptr = new T[new_size];
			std::copy(data_.get(), data_.get() + old_size, old_ptr);
			data_ = array_ptr<T>(new_size);
			for (size_t i = 0; i < size_; i++) {
				if (i < old_size) {
					data_.get()[i] = old_ptr[i];
				} else {
					data_.get()[i] = T{};
				}
			}
			delete[] old_ptr;
		}
		for (size_t i = end() - begin() - 1; i != where_index - 1; --i) {
			if (i == where_index) {
				data_[i] = value;
			} else {
				data_[i] = data_[i - 1];
			}
		}
		return iterator(data_.get());
	}

	void push_back(T&& value) {
		size_ += 1;
		if (capacity_ == 0) {
			capacity_ = size_;
			data_ = array_ptr<T>(size_);
		}
		if (size_ <= capacity_) {
			data_.get()[size_ - 1] = value;
		} else if (size_ > capacity_) {
			capacity_ *= 2;
			T* old_ptr = new T[capacity_];
			std::copy(data_.get(), data_.get() + size_ - 1, old_ptr);
			data_ = array_ptr<T>(capacity_);
			for (size_t i = 0; i < size_; i++) {
				if (i < size_ - 1) {
					data_.get()[i] = old_ptr[i];
				} else if (i == size_ - 1) {
					data_.get()[i] = value;
				} else {
					data_.get()[i] = T{};
				}
			}
		}
	}

	void clear() noexcept {
		size_ = 0;
	}

	void push_back(const T& value) {
		data_[size_] = value;
		size_++;
		if (size_ == 1) {
			capacity_ = 1;
		} else if (size_ > capacity_) {
			capacity_ *= 2;
		}
	}

	[[nodiscard]] bool empty() const noexcept { return size_ == 0; }

	void pop_back() {
		if (size_ > 0) {
			size_ -= 1;
			data_.get()[size_] = T{};
		}
	}

	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		// if (lhs.size_ == rhs.size_) {
		// 	bool result = true;
		// 	for (size_t i = 0; i < lhs.size_; i++) {
		// 		if (lhs.data_.get()[i] != rhs.data_.get()[i]) {
		// 			result = false;
		// 		}
		// 	}
		// 	return result;
		// }
		// return false;
		return alphabet_compare(lhs, rhs) == 0;
	}

	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return !(lhs == rhs);
	}

	// friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs)
	// {
	// 	return alphabet_compare(lhs, rhs) ? std::strong_ordering::less
	// 			: (lhs == rhs)			  ? std::strong_ordering::equal
	// 	                                  : std::strong_ordering::greater;
	// }

	friend auto operator>(const simple_vector& lhs, const simple_vector& rhs)
	{
		return alphabet_compare(lhs, rhs) == 1;
	}

	friend auto operator<(const simple_vector& lhs, const simple_vector& rhs)
	{
		return alphabet_compare(lhs, rhs) == -1;
	}

	friend auto operator>=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return lhs > rhs || lhs == rhs;
	}

	friend auto operator<=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return lhs < rhs || lhs == rhs;
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		return os;
	}

	iterator erase(iterator where) {
		return nullptr;
		// if (!empty()) {
		// 	size_t where_index = where - begin();
		// 	array_ptr<T> arr2 = array_ptr<T>(data_.get());
		// 	for (size_t i = where_index; i != size_ - 2; ++i) {
		// 		data_[i] = arr2[i + 1];
		// 	}
		// }
		// return iterator(data_.get());
	}
	// 1, 2, 3, 4, 5
	// 1, 2, 3, 4, 5

	// 1, 2, 3, 4, 5
	// 1, 2, 4, 5


   private:
	static size_t alphabet_compare(const simple_vector<T>& lhs,
								 const simple_vector<T>& rhs)
	{
		size_t compare = 0;  // -1 : < //  0 : == // 1 : > //
		int i = (lhs.size() < rhs.size()) ? lhs.size() - 1 : rhs.size() - 1;
		while (i >= 0) {
			if (lhs.data_[i] < rhs.data_[i]) {
				compare = -1;
			} else if (lhs.data_[i] > rhs.data_[i]) {
				compare = 1;
			}
			--i;
		}
		if (compare == 0) {
			if (lhs.size() > rhs.size()) {
				compare = 1;
			} else if (lhs.size() < rhs.size()) {
				compare = -1;
			}
		}
		return compare;

		return compare;
	}
	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};
}  // namespace bmstu
