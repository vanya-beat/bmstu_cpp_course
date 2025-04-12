#pragma once

#include <ostream>
#include <utility>
#include <algorithm>
#include "array_ptr.h"

namespace bmstu
{
template <typename T>
class simple_vector
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;

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
        iterator(pointer ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }

        iterator& operator++() { ++ptr_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator--(int) { iterator tmp = *this; --ptr_; return tmp; }

        iterator operator+(difference_type n) const noexcept { return iterator(ptr_ + n); }
        iterator& operator+=(difference_type n) noexcept { ptr_ += n; return *this; }
        iterator operator-(difference_type n) const noexcept { return iterator(ptr_ - n); }
        iterator& operator-=(difference_type n) noexcept { ptr_ -= n; return *this; }

        friend difference_type operator-(const iterator& lhs, const iterator& rhs) noexcept {
            return lhs.ptr_ - rhs.ptr_;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.ptr_ == rhs.ptr_; }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) { return lhs.ptr_ != rhs.ptr_; }

    private:
        pointer ptr_ = nullptr;
    };

    simple_vector() noexcept = default;
    ~simple_vector() = default;

    simple_vector(std::initializer_list<T> init) noexcept : simple_vector(init.size()) {
        std::copy(init.begin(), init.end(), data_.get());
    }

    simple_vector(const simple_vector& other) : simple_vector(other.size_) {
        std::copy(other.data_.get(), other.data_.get() + other.size_, data_.get());
    }

    simple_vector(simple_vector&& other) noexcept : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {
        other.size_ = 0;
        other.capacity_ = 0;
    }

    simple_vector& operator=(const simple_vector& other) {
        if (this != &other) {
            simple_vector temp(other);
            swap(temp);
        }
        return *this;
    }

    simple_vector& operator=(simple_vector&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    explicit simple_vector(size_t size, const T& value = T{}) : data_(size), size_(size), capacity_(size) {
        std::fill(data_.get(), data_.get() + size, value);
    }

    iterator begin() noexcept { return iterator(data_.get()); }
    iterator end() noexcept { return iterator(data_.get() + size_); }

    using const_iterator = iterator;
    const_iterator begin() const noexcept { return iterator(data_.get()); }
    const_iterator end() const noexcept { return iterator(data_.get() + size_); }

    reference operator[](size_t index) noexcept { return data_[index]; }
    const_reference operator[](size_t index) const noexcept { return data_[index]; }

    reference at(size_t index) { return data_.get()[index]; }
    const_reference at(size_t index) const { return data_.get()[index]; }

    [[nodiscard]] size_t size() const noexcept { return size_; }
    [[nodiscard]] size_t capacity() const noexcept { return capacity_; }

    void swap(simple_vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept {
        lhs.swap(rhs);
    }

    void reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            array_ptr<T> new_data(new_cap);
            std::move(data_.get(), data_.get() + size_, new_data.get());
            data_ = std::move(new_data);
            capacity_ = new_cap;
        }
    }

    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        if (new_size > size_) {
            std::fill(data_.get() + size_, data_.get() + new_size, T{});
        }
        size_ = new_size;
    }
	iterator insert(const_iterator where, T&& value)
	{
		size_t where_index = where - begin();
		size_t new_size = size_ + 1;
		const size_t old_size = size_;
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
					data_[i] = std::move(old_ptr[i]);
				}
				else
				{
					data_[i] = T{};
				}
			}
			delete[] old_ptr;
		}
		for (size_t i = end() - begin() - 1; i != where_index - 1; --i)
		{
			if (i == where_index)
			{
				data_[i] = std::move(value);
			}
			else
			{
				data_[i] = data_[i - 1];
			}
		}
		return iterator(data_.get());
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

    void push_back(T&& value) {insert(end(), std::move(value));}

    void push_back(const T& value) {insert(end(), value); }

    void clear() noexcept { size_ = 0; }

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_] = T{};
        }
    }

    friend bool operator==(const simple_vector& lhs, const simple_vector& rhs) {
        return lhs.size_ == rhs.size_ && std::equal(lhs.data_.get(), lhs.data_.get() + lhs.size_, rhs.data_.get());
    }

    friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs) {
        return !(lhs == rhs);
    }

    friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs) {
        return std::lexicographical_compare_three_way(lhs.data_.get(), lhs.data_.get() + lhs.size_, rhs.data_.get(), rhs.data_.get() + rhs.size_);
    }

    friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec) {
        for (size_t i = 0; i < vec.size_; ++i) {
            os << vec.data_[i] << " ";
        }
        return os;
    }

    iterator erase(iterator where) {
        if (!empty()) {
            size_t where_index = where - begin();
            std::move(data_.get() + where_index + 1, data_.get() + size_, data_.get() + where_index);
            --size_;
            data_[size_] = T{};
        }
        return where;
    }

private:
    array_ptr<T> data_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};
}  // namespace bmstu
