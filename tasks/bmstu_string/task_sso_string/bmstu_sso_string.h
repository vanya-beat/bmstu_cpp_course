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

	bool is_long() const { return false; }

	T* get_ptr() { return nullptr; }

	const T* get_ptr() const { return nullptr; }

	size_t get_size() const { return 0; }

	size_t get_capacity() const { return 0; }

   public:
	basic_string() {}

	basic_string(size_t size) {}

	basic_string(std::initializer_list<T> il) {}

	basic_string(const T* c_str) {}

	basic_string(const basic_string& other) {}

	basic_string(basic_string&& dying) noexcept {}

	~basic_string() {}

	const T* c_str() const { return nullptr; }

	size_t size() const { return 0; }

	bool is_using_sso() const { return false; }

	size_t capacity() const { return 0; }

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

	T& operator[](size_t index) noexcept
	{
		static T dummy;
		return dummy;
	}

	T& at(size_t index) { throw std::out_of_range("Wrong index"); }

	T* data() { return nullptr; }

   private:
	static size_t strlen_(const T* str) { return 0; }

	void clean_() {}
};
}  // namespace bmstu
