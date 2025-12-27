#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
template <typename T>
class simple_basic_string;

typedef simple_basic_string<char> string;
typedef simple_basic_string<wchar_t> wstring;
typedef simple_basic_string<char16_t> u16string;
typedef simple_basic_string<char32_t> u32string;

template <typename T>
class simple_basic_string
{
   public:
	/// Конструктор по умолчанию
	simple_basic_string() : ptr_(new T[1]{0}), size_(0) {}

	simple_basic_string(size_t size) : ptr_(new T[size + 1]), size_(size)
	{
		T space = T(' ');
		for (auto i = 0; i < size_; ++i)
		{
			ptr_[i] = space;
		}
		ptr_[size_] = T(0);
	}

	simple_basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		for (auto i = 0u; i < il.size(); ++i)
		{
			*(ptr_ + i) = *(il.begin() + i);
		}
		size_ = il.size();
		*(ptr_ + size_) = T(0);
	}

	/// Конструктор с параметром си-с
	simple_basic_string(const T* c_str)
	{
		auto len = strlen_(c_str);
		ptr_ = new T[len + 1];
		size_ = len;
		ptr_[size_] = T(0);
		for (auto i = 0; i < size_; ++i)
		{
			*(ptr_ + i) = *(c_str + i);
		}
	}

	/// Конструктор копирования
	simple_basic_string(const simple_basic_string& other)
	{
		ptr_ = new T[other.size() + 1];
		size_ = other.size_;
		ptr_[size_] = T(0);
		for (auto i = 0; i < size_; ++i)
		{
			*(ptr_ + i) = *(other.ptr_ + i);
		}
	}

	/// Перемещающий конструктор
	simple_basic_string(simple_basic_string&& other)
	{
		delete[] ptr_;

		ptr_ = other.ptr_;
		size_ = other.size_;

		other.size_ = 0;
		other.ptr_ = new T[1];
		other.ptr_[0] = T(0);
	}

	/// Деструктор
	~simple_basic_string() { delete[] ptr_; }

	/// Геттер на си-строку
	const T* c_str() const { return ptr_; }

	size_t size() const { return size_; }

	/// Оператор копирующего присваивания
	simple_basic_string& operator=(simple_basic_string&& other)
	{
		delete[] ptr_;

		ptr_ = other.ptr_;
		size_ = other.size_;
		other.size_ = 0;
		other.ptr_ = new T[1];
		other.ptr_[0] = T(0);
		return *this;
	}

	/// Оператор копирующего присваивания си строки
	simple_basic_string& operator=(const T* c_str)
	{
		simple_basic_string copy(c_str);
		std::swap(copy, *this);

		return *this;
	}

	/// Оператор копирующего присваивания
	simple_basic_string& operator=(const simple_basic_string& other)
	{
		if (this != &other)
		{
			ptr_ = new T[other.size() + 1];
			size_ = other.size_;
			ptr_[size_] = T(0);
			for (auto i = 0; i < size_; ++i)
			{
				*(ptr_ + i) = *(other.ptr_ + i);
			}
		}
		return *this;
	}

	friend simple_basic_string<T> operator+(const simple_basic_string<T>& left,
											const simple_basic_string<T>& right)
	{
		simple_basic_string<T> result(left.size() + right.size());
		size_t pos = 0;
		for (auto i = 0; i < left.size(); ++i, ++pos)
		{
			*(result.data() + i) = *(left.c_str() + i);
		}
		for (auto i = 0; i < right.size(); ++i)
		{
			*(result.data() + i + pos) = *(right.c_str() + i);
		}
		return result;
	}

	template <typename S>
	friend S& operator<<(S& os, const simple_basic_string& obj)
	{
		os << obj.c_str();
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, simple_basic_string& obj)
	{
		T ch;
		size_t i = 0;
		T* new_ptr = new T[100];

		while (is.get(ch))
		{
			new_ptr[i++] = ch;
		}
		new_ptr[i] = T(0);

		delete[] obj.ptr_;
		obj.ptr_ = new_ptr;
		obj.size_ = i;

		is.putback(ch);

		return is;
	}

	simple_basic_string& operator+=(const simple_basic_string& other)
	{
		*this = *this + other;
		return *this;
	}

	simple_basic_string& operator+=(T symbol)
	{
		T* new_ptr = new T[size_ + 2];

		for (auto i = 0; i < size_; ++i)
		{
			new_ptr[i] = ptr_[i];
		}
		new_ptr[size_] = symbol;
		new_ptr[size_ + 1] = T(0);
		delete[] ptr_;
		ptr_ = new_ptr;
		size_ = size_ + 1;

		return *this;
	}

	T& operator[](size_t index) noexcept { return *(ptr_ + index); }

	T& at(size_t index)
	{
		if (index >= size_)
		{
			throw std::out_of_range("Wrong index");
		}
		return *(ptr_ + index);
	}

	T* data() { return ptr_; }

   private:
	static size_t strlen_(const T* str)
	{
		size_t result = 0;
		while (*str != T(0))
		{
			++result;
			++str;
		}
		return result;
	}

	void clean_()
	{
		delete[] ptr_;
		ptr_ = nullptr;
		size_ = 0;
	}

	T* ptr_ = nullptr;
	size_t size_;
};
}  // namespace bmstu