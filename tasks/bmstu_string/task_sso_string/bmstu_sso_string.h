#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
template <typename T>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename T>
class basic_string
{
   private:
	static constexpr size_t SSO_CAPACITY =
		(sizeof(T*) + sizeof(size_t) + sizeof(size_t)) / sizeof(T) - 1;

	struct LongString
	{
		T* ptr;
		size_t size;
		size_t capacity;
	};

	struct ShortString
	{
		T buffer[SSO_CAPACITY + 1];
		unsigned char size;
	};

	union Data
	{
		LongString long_str;
		ShortString short_str;
	};

	Data data_;
	bool is_long_;

	bool is_long() const { return is_long_; }

	T* get_ptr()
	{
		if (is_long_)
			return data_.long_str.ptr;
		return data_.short_str.buffer;
	}

	const T* get_ptr() const
	{
		if (is_long_)
			return data_.long_str.ptr;
		return data_.short_str.buffer;
	}

	size_t get_size() const
	{
		if (is_long_)
			return data_.long_str.size;
		return data_.short_str.size;
	}

	size_t get_capacity() const
	{
		if (is_long_)
			return data_.long_str.capacity;
		return SSO_CAPACITY;
	}

   public:
	basic_string()
	{
		is_long_ = false;
		data_.short_str.size = 0;
		data_.short_str.buffer[0] = T();
	}

	basic_string(size_t size)
	{
		if (size <= SSO_CAPACITY)
		{
			is_long_ = false;
			data_.short_str.size = size;
			for (size_t i = 0; i < size; ++i)
			{
				data_.short_str.buffer[i] = T(' ');
			}
			data_.short_str.buffer[size] = T(0);
		}
		else
		{
			is_long_ = true;
			data_.long_str.size = size;
			data_.long_str.capacity = size;
			data_.long_str.ptr = new T[size + 1];

			for (size_t i = 0; i < size; i++)
				data_.long_str.ptr[i] = T(' ');
			data_.long_str.ptr[size] = T(0);
		}
	}

	basic_string(std::initializer_list<T> il)
	{
		size_t len = il.size();

		if (len <= SSO_CAPACITY)
		{
			is_long_ = false;
			data_.short_str.size = len;
			size_t i = 0;

			for (auto elem = il.begin(); elem != il.end(); ++elem)
			{
				data_.short_str.buffer[i] = *elem;
				++i;
			}
			data_.short_str.buffer[len] = T(0);
		}
		else
		{
			is_long_ = true;
			data_.long_str.size = len;
			data_.long_str.capacity = len;
			data_.long_str.ptr = new T[len + 1];

			size_t i = 0;
			for (auto elem = il.begin(); elem != il.end(); ++elem)
			{
				data_.long_str.ptr[i] = *elem;
				++i;
			}
			data_.long_str.ptr[len] = T(0);
		}
	}

	basic_string(const T* c_str)
	{
		size_t len = strlen_(c_str);

		if (len <= SSO_CAPACITY)
		{
			is_long_ = false;
			data_.short_str.size = len;

			for (size_t i = 0; i < len; ++i)
			{
				data_.short_str.buffer[i] = c_str[i];
			}
			data_.short_str.buffer[len] = T(0);
		}
		else
		{
			is_long_ = true;
			data_.long_str.size = len;
			data_.long_str.capacity = len;
			data_.long_str.ptr = new T[len + 1];

			for (size_t i = 0; i < len; ++i)
			{
				data_.long_str.ptr[i] = c_str[i];
			}
			data_.long_str.ptr[len] = T(0);
		}
	}

	basic_string(const basic_string& other) {}

	basic_string(basic_string&& dying) noexcept {}

	~basic_string() {}

	const T* c_str() const { return get_ptr(); }

	size_t size() const { return get_size(); }

	bool is_using_sso() const { return !is_long(); }

	size_t capacity() const { return get_capacity(); }

	basic_string& operator=(basic_string&& other) noexcept { return *this; }

	basic_string& operator=(const T* c_str) { return *this; }

	basic_string& operator=(const basic_string& other) { return *this; }

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		return {};
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		return is;
	}

	basic_string& operator+=(const basic_string& other) { return *this; }

	basic_string& operator+=(T symbol) { return *this; }

	T& operator[](size_t index) noexcept { return get_ptr()[index]; }

	T& at(size_t index) { throw std::out_of_range("Wrong index"); }

	T* data() { return get_ptr(); }

   private:
	static size_t strlen_(const T* str)
	{
		size_t len = 0;
		while (str[len] != T(0))
		{
			++len;
		}
		return len;
	}

	void clean_() {}
};
}  // namespace bmstu
