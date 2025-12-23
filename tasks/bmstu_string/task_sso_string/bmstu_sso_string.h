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

	basic_string(const basic_string& other)
	{
		is_long_ = other.is_long_;
		if (is_long())
		{
			size_t othersize = other.data_.long_str.capacity;
			data_.long_str.ptr = new T[othersize];
			data_.long_str.size = other.data_.long_str.size;
			data_.long_str.capacity = othersize;
			for (size_t i = 0; i <= data_.long_str.size; ++i)
			{
				data_.long_str.ptr[i] = other.data_.long_str.ptr[i];
			}
		}
		else
		{
			data_.short_str = other.data_.short_str;
		}
	}

	basic_string(basic_string&& dying) noexcept
	{
		is_long_ = dying.is_long_;
		if (is_long())
		{
			data_.long_str = dying.data_.long_str;
			dying.data_.long_str.ptr = nullptr;
			dying.data_.long_str.size = 0;
			dying.data_.long_str.capacity = 0;
		}
		else
		{
			data_.short_str = dying.data_.short_str;
		}
		dying.is_long_ = false;
		dying.data_.short_str.size = 0;
	}

	~basic_string() { clean_(); }

	const T* c_str() const { return get_ptr(); }

	size_t size() const { return get_size(); }

	bool is_using_sso() const { return !is_long(); }

	size_t capacity() const { return get_capacity(); }

	basic_string& operator=(basic_string&& other) noexcept
	{
		if (this != &other)
		{
			clean_();
			is_long_ = other.is_long_;
			if (is_long())
			{
				data_.long_str = other.data_.long_str;
				other.data_.long_str.ptr = nullptr;
				other.data_.long_str.size = 0;
				other.data_.long_str.capacity = 0;
			}
			else
			{
				data_.short_str = other.data_.short_str;
			}
			other.is_long_ = false;
			other.data_.short_str.size = 0;
			other.data_.short_str.buffer[0] = '\0';
		}
		return *this;
	}

	basic_string& operator=(const T* c_str)
	{
		size_t len = strlen_(c_str);
		if (is_long() && data_.long_str.ptr)
		{
			delete[] data_.long_str.ptr;
		}

		if (len <= SSO_CAPACITY)
		{
			is_long_ = false;
			for (size_t i = 0; i < len; ++i)
			{
				data_.short_str.buffer[i] = c_str[i];
			}
			data_.short_str.buffer[len] = '\0';
			data_.short_str.size = len;
		}
		else
		{
			is_long_ = true;
			data_.long_str.capacity = len + 1;
			data_.long_str.size = len;
			data_.long_str.ptr = new T[data_.long_str.capacity];
			for (size_t i = 0; i <= len; ++i)
			{
				data_.long_str.ptr[i] = c_str[i];
			}
		}
		return *this;
	}

	basic_string& operator=(const basic_string& other)
	{
		if (this != &other)
		{
			if (is_long() && data_.long_str.ptr)
			{
				delete[] data_.long_str.ptr;
			}
			is_long_ = other.is_long_;
			if (is_long())
			{
				size_t cap = other.data_.long_str.capacity;
				data_.long_str.ptr = new T[cap];
				data_.long_str.size = other.data_.long_str.size;
				data_.long_str.capacity = cap;
				for (size_t i = 0; i <= data_.long_str.size; i++)
				{
					data_.long_str.ptr[i] = other.data_.long_str.ptr[i];
				}
			}
			else
			{
				data_.short_str = other.data_.short_str;
			}
		}
		return *this;
	}

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		basic_string<T> result = left;
		result += right;
		return result;
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

	basic_string& operator+=(const basic_string& other)
	{
		size_t this_size = get_size();
		size_t other_size = other.get_size();
		size_t new_size = this_size + other_size;
		if (new_size > get_capacity())
		{
			size_t new_cap = new_size + 1;
			T* new_ptr = new T[new_cap];
			for (size_t i = 0; i < this_size; i++)
			{
				new_ptr[i] = get_ptr()[i];
			}
			for (size_t i = 0; i < other_size; i++)
			{
				new_ptr[this_size + i] = other.get_ptr()[i];
			}
			new_ptr[new_size] = '\0';
			clean_();
			is_long_ = true;
			data_.long_str.ptr = new_ptr;
			data_.long_str.size = new_size;
			data_.long_str.capacity = new_cap;
		}
		else
		{
			for (size_t i = 0; i < other_size; i++)
			{
				get_ptr()[this_size + i] = other.get_ptr()[i];
			}
			get_ptr()[new_size] = '\0';
			if (is_long())
			{
				data_.long_str.size = new_size;
			}
			else
			{
				data_.short_str.size = new_size;
			}
		}
		return *this;
	}

	basic_string& operator+=(T symbol)
	{
		size_t this_size = get_size();
		size_t new_size = this_size + 1;
		if (new_size > get_capacity())
		{
			size_t new_cap = new_size + 1;
			T* new_ptr = new T[new_cap];
			for (size_t i = 0; i < this_size; i++)
			{
				new_ptr[i] = get_ptr()[i];
			}
			new_ptr[this_size] = symbol;
			new_ptr[new_size] = '\0';
			clean_();
			is_long_ = true;
			data_.long_str.ptr = new_ptr;
			data_.long_str.size = new_size;
			data_.long_str.capacity = new_cap;
		}
		else
		{
			get_ptr()[this_size] = symbol;
			get_ptr()[new_size] = '\0';
			if (is_long())
			{
				data_.long_str.size = new_size;
			}
			else
			{
				data_.short_str.size = new_size;
			}
		}
		return *this;
	}
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

	void clean_()
	{
		if (is_long() && data_.long_str.ptr)
		{
			delete[] data_.long_str.ptr;
			data_.long_str.ptr = nullptr;
		}
	}
};
}  // namespace bmstu
