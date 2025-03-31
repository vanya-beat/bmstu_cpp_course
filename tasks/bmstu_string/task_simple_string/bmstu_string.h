#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>

namespace bmstu
{

template <typename CharType>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename CharType>
class basic_string
{
   public:
	// Конструктор по умолчанию
	basic_string() noexcept : data_(new CharType[1]{0}), size_(0) {}

	// Конструктор с заданным размером
	explicit basic_string(size_t count)
		: data_(new CharType[count + 1]), size_(count)
	{
		std::fill_n(data_.get(), count, static_cast<CharType>(' '));
		data_[count] = 0;
	}

	// Конструктор из списка инициализации
	basic_string(std::initializer_list<CharType> init)
		: data_(new CharType[init.size() + 1]), size_(init.size())
	{
		std::copy(init.begin(), init.end(), data_.get());
		data_[size_] = 0;
	}

	// Конструктор из C-строки
	basic_string(const CharType* cstr)
	{
		if (cstr)
		{
			size_ = str_len(cstr);
			data_.reset(new CharType[size_ + 1]);
			std::copy(cstr, cstr + size_, data_.get());
			data_[size_] = 0;
		}
		else
		{
			size_ = 0;
			data_.reset(new CharType[1]{0});
		}
	}

	// Копирующий конструктор
	basic_string(const basic_string& other)
		: data_(new CharType[other.size_ + 1]), size_(other.size_)
	{
		std::copy(other.data_.get(), other.data_.get() + size_ + 1,
				  data_.get());
	}

	// Перемещающий конструктор
	basic_string(basic_string&& other) noexcept
		: data_(std::move(other.data_)), size_(other.size_)
	{
		other.size_ = 0;
		other.data_.reset(new CharType[1]{0});
	}

	// Деструктор
	~basic_string() = default;

	// Операторы присваивания
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
			data_ = std::move(other.data_);
			size_ = other.size_;
			other.size_ = 0;
			other.data_.reset(new CharType[1]{0});
		}
		return *this;
	}

	basic_string& operator=(const CharType* cstr)
	{
		basic_string tmp(cstr);
		swap(tmp);
		return *this;
	}

	// Доступ к данным
	const CharType* c_str() const noexcept { return data_.get(); }
	size_t size() const noexcept { return size_; }
	CharType* data() noexcept { return data_.get(); }
	const CharType* data() const noexcept { return data_.get(); }

	// Операторы доступа
	CharType& operator[](size_t pos) noexcept { return data_[pos]; }
	const CharType& operator[](size_t pos) const noexcept { return data_[pos]; }

	CharType& at(size_t pos)
	{
		if (pos >= size_)
			throw std::out_of_range("Index out of range");
		return data_[pos];
	}

	const CharType& at(size_t pos) const
	{
		if (pos >= size_)
			throw std::out_of_range("Index out of range");
		return data_[pos];
	}

	// Операторы конкатенации
	basic_string& operator+=(const basic_string& other)
	{
		size_t new_size = size_ + other.size_;
		std::unique_ptr<CharType[]> new_data(new CharType[new_size + 1]);

		if (size_)
			std::copy(data_.get(), data_.get() + size_, new_data.get());
		if (other.size_)
			std::copy(other.data_.get(), other.data_.get() + other.size_,
					  new_data.get() + size_);

		new_data[new_size] = 0;
		data_ = std::move(new_data);
		size_ = new_size;
		return *this;
	}

	basic_string& operator+=(CharType ch)
	{
		std::unique_ptr<CharType[]> new_data(new CharType[size_ + 2]);
		if (size_)
			std::copy(data_.get(), data_.get() + size_, new_data.get());
		new_data[size_] = ch;
		new_data[size_ + 1] = 0;
		data_ = std::move(new_data);
		++size_;
		return *this;
	}

	friend basic_string operator+(const basic_string& lhs,
								  const basic_string& rhs)
	{
		basic_string result(lhs);
		result += rhs;
		return result;
	}

	// Функция вывода
	template <typename Stream>
	void print_to(Stream& os) const
	{
		const CharType* ptr = data_.get();
		while (*ptr)
		{
			os.put(*ptr);
			++ptr;
		}
	}

	// Функция ввода
	template <typename Stream>
	void read_from(Stream& is)
	{
		clear();
		CharType ch;
		while (is.get(ch) && ch != '\n' && ch != static_cast<CharType>('\0'))
		{
			*this += ch;
		}
	}

	// Вспомогательные методы
	void clear()
	{
		data_.reset(new CharType[1]{0});
		size_ = 0;
	}

	void swap(basic_string& other) noexcept
	{
		std::swap(data_, other.data_);
		std::swap(size_, other.size_);
	}

   private:
	static size_t str_len(const CharType* str)
	{
		const CharType* p = str;
		while (*p)
			++p;
		return p - str;
	}

	std::unique_ptr<CharType[]> data_;
	size_t size_;
};

}  // namespace bmstu