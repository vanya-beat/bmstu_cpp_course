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

	basic_string(size_t size) : ptr_(new T[size + 1]), size_(size)
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
			T* new_ptr = new T[other.size_ + 1];
			std::copy(other.ptr_, other.ptr_ + other.size_ + 1, new_ptr);
			delete[] ptr_;
			ptr_ = new_ptr;
			size_ = other.size_;
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
		if (c_str)
		{
			size_t new_size = strlen_(c_str);
			T* new_ptr = new T[new_size + 1];
			std::copy(c_str, c_str + new_size + 1, new_ptr);
			delete[] ptr_;
			ptr_ = new_ptr;
			size_ = new_size;
		}
		else
		{
			delete[] ptr_;
			ptr_ = new T[1]{0};
			size_ = 0;
		}
		return *this;
	}

	const T* c_str() const { return ptr_; }

	size_t size() const { return size_; }

	T& operator[](size_t index) noexcept { return ptr_[index]; }
	const T& operator[](size_t index) const noexcept { return ptr_[index]; }

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return ptr_[index];
	}
	const T& at(size_t index) const
	{
		if (index >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return ptr_[index];
	}

	T* data() { return ptr_; }
	const T* data() const { return ptr_; }

	basic_string& operator+=(const basic_string& other)
	{
		size_t new_size = size_ + other.size_;
		T* new_ptr = new T[new_size + 1];
		std::copy(ptr_, ptr_ + size_, new_ptr);
		std::copy(other.ptr_, other.ptr_ + other.size_, new_ptr + size_);
		new_ptr[new_size] = 0;
		delete[] ptr_;
		ptr_ = new_ptr;
		size_ = new_size;
		return *this;
	}

	basic_string& operator+=(T symbol)
	{
		size_t new_size = size_ + 1;
		T* new_ptr = new T[new_size + 1];
		std::copy(ptr_, ptr_ + size_, new_ptr);
		new_ptr[size_] = symbol;
		new_ptr[new_size] = 0;
		delete[] ptr_;
		ptr_ = new_ptr;
		size_ = new_size;
		return *this;
	}

	friend basic_string operator+(const basic_string& left,
								  const basic_string& right)
	{
		basic_string result(left);
		result += right;
		return result;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		os << obj.c_str();
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		typedef typename S::char_type CharT;
		std::basic_string<CharT> tmp((std::istreambuf_iterator<CharT>(is)),
									 std::istreambuf_iterator<CharT>());
		obj = tmp.c_str();
		return is;
	}

   private:
	static size_t strlen_(const T* str)
	{
		size_t len = 0;
		while (str[len] != 0)
		{
			++len;
		}
		return len;
	}

	T* ptr_;
	size_t size_;
};
}  // namespace bmstu
