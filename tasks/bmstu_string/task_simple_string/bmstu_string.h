#pragma once

#include <cstring>
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
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
// typedef basic_string<char8_t> u8string;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
#ifdef _MSC_VER
class basic_string
#else
class basic_string
#endif
{
   public:
	/// Конструктор по умолчанию
	basic_string() : ptr_(nullptr), size_(0)
	{
		ptr_ = new T[1];
		ptr_[0] = 0;
	}

	basic_string(size_t size) : ptr_(nullptr), size_(0)
	{
		size_ = size;
		ptr_ = new T[size + 1];
		for (size_t i = 0; i < size; i++)
		{
			ptr_[i] = ' ';
		}
		ptr_[size] = '\0';
	}

	basic_string(std::initializer_list<T> il) : ptr_(nullptr), size_(0)
	{
		size_ = il.size();
		ptr_ = new T[size_ + 1];
		size_t i = 0;
		for (auto& item : il)
		{
			ptr_[i] = item;
			i++;
		}
		ptr_[size_] = '\0';
	}

	basic_string(const T* c_str)
	{
		size_ = strlen_(c_str);
		ptr_ = new T[size_ + 1];
		std::memcpy(ptr_, c_str, sizeof(T) * strlen_(c_str));
		ptr_[size_] = '\0';
	}

	basic_string(const basic_string& other)
	{
		size_ = other.size_;
		ptr_ = new T[size_ + 1];
		std::memcpy(ptr_, other.ptr_, sizeof(T) * strlen_(other.ptr_));
		ptr_[size_] = '\0';
	}

	basic_string(basic_string&& dying)
	{
		ptr_ = dying.ptr_;
		size_ = dying.size_;
		dying.ptr_ = nullptr;
		dying.size_ = 0;
	}

	~basic_string() { clean_(); }

	const T* c_str() const
	{
		if (ptr_ == nullptr)
		{
			return reinterpret_cast<const T*>("");
		}
		return ptr_;
	}

	size_t size() const { return size_; }

	basic_string& operator=(basic_string&& other)
	{
		delete[] ptr_;
		size_ = other.size_;
		ptr_ = other.ptr_;
		other.ptr_ = nullptr;
		other.size_ = 0;
		return *this;
	}

	basic_string& operator=(const T* c_str)
	{
		clean_();
		size_ = strlen_(c_str);
		ptr_ = new T[size_ + 1];
		std::memcpy(ptr_, c_str, sizeof(T) * strlen_(c_str));
		ptr_[size_] = '\0';
		return *this;
	}

	basic_string& operator=(const basic_string& other)
	{
		clean_();
		size_ = other.size_;
		ptr_ = new T[size_ + 1];
		std::memcpy(ptr_, other.c_str(), sizeof(T) * strlen_(other.c_str()));
		ptr_[size_] = '\0';
		return *this;
	}

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		basic_string result;
		result.size_ = left.size_ + right.size_;
		result.ptr_ = new T[result.size_ + 1];
		std::memcpy(result.ptr_, left.c_str(),
					sizeof(T) * strlen_(left.c_str()));
		result.ptr_[result.size_] = '\0';
		for (size_t i = 0; i < right.size_; ++i)
		{
			result.ptr_[i + left.size_] = right.ptr_[i];
		}
		return result;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		os << obj.ptr_;
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		is >> std::noskipws;
		T symbol = 1;
		for (size_t i = 0; symbol != 0; ++i)
		{
			symbol = 0;
			is >> symbol;
			if (symbol != 0)
			{
				obj += symbol;
			}
		}
		return is;
	}

	basic_string& operator+=(const basic_string& other)
	{
		*this = (*this + other);
		return *this;
	}

	basic_string& operator+=(T symbol)
	{
		size_++;
		T* left = ptr_;
		ptr_ = new T[size_ + 1];
		std::memcpy(ptr_, left, sizeof(T) * strlen_(left));
		delete[] left;
		ptr_[size_ - 1] = symbol;
		ptr_[size_] = 0;
		return *this;
	}

	T& operator[](size_t index) noexcept { return data()[index]; }

	T& at(size_t index) { return data()[0]; }

   private:
	static size_t strlen_(const T* str)
	{
		size_t len = 0;
		while (*str != '\0')
		{
			len++;
			++str;
		}
		return len;
	}

	void clean_()
	{
		size_ = 0;
		delete[] ptr_;
	}

	T* data() { return ptr_; }

	T* ptr_ = nullptr;
	size_t size_;
};
}  // namespace bmstu