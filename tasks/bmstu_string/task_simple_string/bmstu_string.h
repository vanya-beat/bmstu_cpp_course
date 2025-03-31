#pragma once

#include <algorithm>
#include <exception>
#include <iostream>

namespace bmstu
{
template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
class basic_string
{
   public:
	basic_string() : ptr_(new T[1]{0}), size_(0) {}

	explicit basic_string(size_t size) : ptr_(new T[size + 1]), size_(size)
	{
		std::fill(ptr_, ptr_ + size_, static_cast<T>(' '));
		ptr_[size_] = 0;
	}

	basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		std::copy(il.begin(), il.end(), ptr_);
		ptr_[size_] = 0;
	}

	basic_string(const T* c_str)
	{
		if (c_str)
		{
			size_ = strlen_(c_str);
			ptr_ = new T[size_ + 1];
			std::copy(c_str, c_str + size_ + 1, ptr_);
		}
		else
		{
			size_ = 0;
			ptr_ = new T[1]{0};
		}
	}

	basic_string(const basic_string& other)
		: ptr_(new T[other.size_ + 1]), size_(other.size_)
	{
		std::copy(other.ptr_, other.ptr_ + other.size_ + 1, ptr_);
	}

	basic_string(basic_string&& other) noexcept
		: ptr_(other.ptr_), size_(other.size_)
	{
		other.ptr_ = new T[1]{0};
		other.size_ = 0;
	}

	~basic_string() { delete[] ptr_; }

	basic_string& operator=(const basic_string& other)
	{
		if (this != &other)
		{
			basic_string tmp(other);
			swap(tmp);
		}
		return *this;
	}

	basic_string& operator=(basic_string&& other) noexcept
	{
		if (this != &other)
		{
			delete[] ptr_;
			ptr_ = other.ptr_;
			size_ = other.size_;
			other.ptr_ = new T[1]{0};
			other.size_ = 0;
		}
		return *this;
	}

	basic_string& operator=(const T* c_str)
	{
		basic_string tmp(c_str);
		swap(tmp);
		return *this;
	}

	const T* c_str() const
	{
		return ptr_ ? ptr_ : reinterpret_cast<const T*>("");
	}
	size_t size() const { return size_; }

	T& operator[](size_t index) noexcept { return ptr_[index]; }
	const T& operator[](size_t index) const noexcept { return ptr_[index]; }

	T& at(size_t index)
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
		return ptr_[index];
	}

	const T& at(size_t index) const
	{
		if (index >= size_)
			throw std::out_of_range("Index out of range");
		return ptr_[index];
	}

	T* data() { return ptr_; }
	const T* data() const { return ptr_; }

	basic_string& operator+=(const basic_string& other)
	{
		*this = *this + other;
		return *this;
	}

	basic_string& operator+=(T symbol)
	{
		T* new_ptr = new T[size_ + 2];
		if (ptr_)
			std::copy(ptr_, ptr_ + size_, new_ptr);
		new_ptr[size_] = symbol;
		new_ptr[size_ + 1] = 0;
		delete[] ptr_;
		ptr_ = new_ptr;
		++size_;
		return *this;
	}

	friend basic_string operator+(const basic_string& left,
								  const basic_string& right)
	{
		basic_string result;
		result.size_ = left.size_ + right.size_;
		result.ptr_ = new T[result.size_ + 1];
		std::copy(left.ptr_, left.ptr_ + left.size_, result.ptr_);
		std::copy(right.ptr_, right.ptr_ + right.size_,
				  result.ptr_ + left.size_);
		result.ptr_[result.size_] = 0;
		return result;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		const T* p = obj.c_str();
		while (*p)
			os.put(*p++);
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		obj.clean_();
		T ch;
		while (is.get(ch) && ch != '\n' && ch != static_cast<T>('\0'))
			obj += ch;
		return is;
	}

   private:
	static size_t strlen_(const T* str)
	{
		const T* p = str;
		while (*p)
			++p;
		return p - str;
	}

	void clean_()
	{
		delete[] ptr_;
		ptr_ = new T[1]{0};
		size_ = 0;
	}

	void swap(basic_string& other) noexcept
	{
		std::swap(ptr_, other.ptr_);
		std::swap(size_, other.size_);
	}

	T* ptr_;
	size_t size_;
};
}  // namespace bmstu