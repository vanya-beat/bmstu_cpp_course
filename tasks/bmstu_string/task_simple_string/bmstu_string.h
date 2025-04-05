#pragma once

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
		for (size_t i = 0; i < size; ++i)
		{
			ptr_[i] = static_cast<T>(' ');
		}
		ptr_[size] = 0;
	}

	basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		size_t i = 0;
		for (auto it = il.begin(); it != il.end(); ++it, ++i)
		{
			ptr_[i] = *it;
		}
		ptr_[size_] = 0;
	}

	basic_string(const T* c_str)
	{
		if (c_str)
		{
			size_ = 0;
			while (c_str[size_] != 0)
				++size_;

			ptr_ = new T[size_ + 1];
			for (size_t i = 0; i <= size_; ++i)
			{
				ptr_[i] = c_str[i];
			}
		}
		else
		{
			ptr_ = new T[1]{0};
			size_ = 0;
		}
	}

	basic_string(const basic_string& other)
		: ptr_(new T[other.size_ + 1]), size_(other.size_)
	{
		for (size_t i = 0; i <= size_; ++i)
		{
			ptr_[i] = other.ptr_[i];
		}
	}

	basic_string(basic_string&& dying) noexcept
		: ptr_(dying.ptr_), size_(dying.size_)
	{
		dying.ptr_ = new T[1]{0};
		dying.size_ = 0;
	}

	~basic_string() { delete[] ptr_; }

	const T* c_str() const { return ptr_; }

	size_t size() const { return size_; }

	basic_string& operator=(basic_string&& other)
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
		*this = std::move(tmp);
		return *this;
	}

	basic_string& operator=(const basic_string& other)
	{
		if (this != &other)
		{
			basic_string tmp(other);
			*this = std::move(tmp);
		}
		return *this;
	}

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		basic_string<T> result;
		result.size_ = left.size_ + right.size_;
		result.ptr_ = new T[result.size_ + 1];

		for (size_t i = 0; i < left.size_; ++i)
		{
			result.ptr_[i] = left.ptr_[i];
		}

		for (size_t i = 0; i <= right.size_; ++i)
		{
			result.ptr_[left.size_ + i] = right.ptr_[i];
		}

		return result;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		obj.clean_();
		T ch;
		while (is.get(ch))
		{
			if (ch == static_cast<T>('\0'))
			{
				break;
			}
			obj += ch;
			if (!is.good())
			{
				break;
			}
		}
		return is;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		for (size_t i = 0; i < obj.size_; ++i)
		{
			os.put(obj.ptr_[i]);
		}
		return os;
	}

	basic_string& operator+=(const basic_string& other)
	{
		*this = *this + other;
		return *this;
	}

	basic_string& operator+=(T symbol)
	{
		T* new_ptr = new T[size_ + 2];
		for (size_t i = 0; i < size_; ++i)
		{
			new_ptr[i] = ptr_[i];
		}
		new_ptr[size_] = symbol;
		new_ptr[size_ + 1] = 0;
		delete[] ptr_;
		ptr_ = new_ptr;
		++size_;
		return *this;
	}

	T& operator[](size_t index) noexcept { return ptr_[index]; }

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Wrong index");
		}
		return ptr_[index];
	}

	T* data() { return ptr_; }

   private:
	static size_t strlen_(const T* str)
	{
		size_t len = 0;
		while (str[len] != 0)
			++len;
		return len;
	}

	void clean_()
	{
		delete[] ptr_;
		ptr_ = new T[1]{0};
		size_ = 0;
	}

	T* ptr_;
	size_t size_;
};

}  // namespace bmstu
