#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace bmstu
{
template <typename CharT>
class basic_string
{
   public:
	// Конструкторы
	basic_string() noexcept : size_(0), capacity_(0), data_(nullptr) {}

	explicit basic_string(size_t count, CharT ch = CharT(' '))
		: size_(count), capacity_(count)
	{
		if (count > 0)
		{
			data_ = std::make_unique<CharT[]>(capacity_ + 1);
			std::fill_n(data_.get(), size_, ch);
			data_[size_] = '\0';
		}
	}

	basic_string(const CharT* s)
	{
		size_ = std::char_traits<CharT>::length(s);
		capacity_ = size_;
		if (size_ > 0)
		{
			data_ = std::make_unique<CharT[]>(capacity_ + 1);
			std::copy(s, s + size_, data_.get());
			data_[size_] = '\0';
		}
	}

	basic_string(std::initializer_list<CharT> ilist)
		: size_(ilist.size()), capacity_(ilist.size())
	{
		if (size_ > 0)
		{
			data_ = std::make_unique<CharT[]>(capacity_ + 1);
			std::copy(ilist.begin(), ilist.end(), data_.get());
			data_[size_] = '\0';
		}
	}

	// Копирование и перемещение
	basic_string(const basic_string& other)
		: size_(other.size_), capacity_(other.size_)
	{
		if (size_ > 0)
		{
			data_ = std::make_unique<CharT[]>(capacity_ + 1);
			std::copy(other.data_.get(), other.data_.get() + size_,
					  data_.get());
			data_[size_] = '\0';
		}
	}

	basic_string(basic_string&& other) noexcept
		: size_(other.size_),
		  capacity_(other.capacity_),
		  data_(std::move(other.data_))
	{
		other.size_ = 0;
		other.capacity_ = 0;
	}

	~basic_string() = default;

	// Присваивание
	basic_string& operator=(const basic_string& rhs)
	{
		if (this != &rhs)
		{
			basic_string temp(rhs);
			swap(temp);
		}
		return *this;
	}

	basic_string& operator=(basic_string&& rhs) noexcept
	{
		if (this != &rhs)
		{
			size_ = rhs.size_;
			capacity_ = rhs.capacity_;
			data_ = std::move(rhs.data_);
			rhs.size_ = 0;
			rhs.capacity_ = 0;
		}
		return *this;
	}

	basic_string& operator=(const CharT* s)
	{
		size_t new_size = std::char_traits<CharT>::length(s);
		if (new_size > capacity_)
		{
			basic_string temp(s);
			swap(temp);
		}
		else
		{
			std::copy(s, s + new_size, data_.get());
			size_ = new_size;
			data_[size_] = '\0';
		}
		return *this;
	}

	// Доступ к элементам
	const CharT* c_str() const noexcept
	{
		return data_ ? data_.get() : empty_string();
	}

	size_t size() const noexcept { return size_; }
	size_t length() const noexcept { return size_; }

	CharT& operator[](size_t pos) noexcept { return data_[pos]; }

	const CharT& operator[](size_t pos) const noexcept { return data_[pos]; }

	CharT& at(size_t pos)
	{
		if (pos >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[pos];
	}

	const CharT& at(size_t pos) const
	{
		if (pos >= size_)
		{
			throw std::out_of_range("Index out of range");
		}
		return data_[pos];
	}

	// Операции
	basic_string& operator+=(const basic_string& str) { return append(str); }

	basic_string& operator+=(CharT ch)
	{
		push_back(ch);
		return *this;
	}

	basic_string& operator+=(const CharT* s) { return append(s); }

	void push_back(CharT ch)
	{
		if (size_ == capacity_)
		{
			reserve(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		data_[size_++] = ch;
		data_[size_] = '\0';
	}

	basic_string& append(const basic_string& str)
	{
		return append(str.data_.get(), str.size_);
	}

	basic_string& append(const CharT* s)
	{
		return append(s, std::char_traits<CharT>::length(s));
	}

	basic_string& append(const CharT* s, size_t count)
	{
		if (count == 0)
			return *this;

		if (size_ + count > capacity_)
		{
			reserve(size_ + count);
		}

		std::copy(s, s + count, data_.get() + size_);
		size_ += count;
		data_[size_] = '\0';

		return *this;
	}

	// Утилиты
	void swap(basic_string& other) noexcept
	{
		std::swap(size_, other.size_);
		std::swap(capacity_, other.capacity_);
		std::swap(data_, other.data_);
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= capacity_)
			return;

		auto new_data = std::make_unique<CharT[]>(new_capacity + 1);
		if (data_)
		{
			std::copy(data_.get(), data_.get() + size_, new_data.get());
		}
		new_data[size_] = '\0';
		data_ = std::move(new_data);
		capacity_ = new_capacity;
	}

	void resize(size_t new_size, CharT ch = CharT())
	{
		if (new_size < size_)
		{
			size_ = new_size;
			data_[size_] = '\0';
		}
		else if (new_size > size_)
		{
			reserve(new_size);
			std::fill(data_.get() + size_, data_.get() + new_size, ch);
			size_ = new_size;
			data_[size_] = '\0';
		}
	}

	void clear() noexcept
	{
		size_ = 0;
		if (data_)
		{
			data_[0] = '\0';
		}
	}

	bool empty() const noexcept { return size_ == 0; }

	// Итераторы
	using iterator = CharT*;
	using const_iterator = const CharT*;

	iterator begin() noexcept { return data_.get(); }
	iterator end() noexcept { return data_.get() + size_; }

	const_iterator begin() const noexcept { return data_.get(); }
	const_iterator end() const noexcept { return data_.get() + size_; }

	const_iterator cbegin() const noexcept { return begin(); }
	const_iterator cend() const noexcept { return end(); }

	// Операторы сравнения
	friend bool operator==(const basic_string& lhs, const basic_string& rhs)
	{
		return lhs.size_ == rhs.size_ &&
			   std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	friend bool operator!=(const basic_string& lhs, const basic_string& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator<(const basic_string& lhs, const basic_string& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
											rhs.end());
	}

	friend bool operator>(const basic_string& lhs, const basic_string& rhs)
	{
		return rhs < lhs;
	}

	friend bool operator<=(const basic_string& lhs, const basic_string& rhs)
	{
		return !(rhs < lhs);
	}

	friend bool operator>=(const basic_string& lhs, const basic_string& rhs)
	{
		return !(lhs < rhs);
	}

	// Конкатенация
	friend basic_string operator+(const basic_string& lhs,
								  const basic_string& rhs)
	{
		basic_string result(lhs);
		result.append(rhs);
		return result;
	}

	// Потоковый ввод/вывод
	friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os,
												 const basic_string& str)
	{
		return os << str.c_str();
	}

	friend std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is,
												 basic_string& str)
	{
		str.clear();
		CharT ch;
		while (is.get(ch) && !std::isspace(ch))
		{
			str.push_back(ch);
		}
		return is;
	}

   private:
	static const CharT* empty_string()
	{
		static const CharT empty = '\0';
		return &empty;
	}

	size_t size_ = 0;
	size_t capacity_ = 0;
	std::unique_ptr<CharT[]> data_;
};

// Специализации типов
using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

}  // namespace bmstu