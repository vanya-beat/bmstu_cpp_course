#pragma once

#include <iterator>
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
	basic_string() noexcept
		: buffer_(std::make_unique<CharType[]>(1)), length_(0)
	{
		buffer_[0] = static_cast<CharType>(0);
	}

	// Конструктор с заданным размером
	explicit basic_string(size_t count)
		: buffer_(std::make_unique<CharType[]>(count + 1)), length_(count)
	{
		std::fill_n(buffer_.get(), count, static_cast<CharType>(' '));
		buffer_[count] = static_cast<CharType>(0);
	}

	// Конструктор из списка инициализации
	basic_string(std::initializer_list<CharType> init)
		: buffer_(std::make_unique<CharType[]>(init.size() + 1)),
		  length_(init.size())
	{
		std::copy(init.begin(), init.end(), buffer_.get());
		buffer_[length_] = static_cast<CharType>(0);
	}

	// Конструктор из C-строки
	basic_string(const CharType* cstr)
	{
		if (cstr)
		{
			length_ = calculate_length(cstr);
			buffer_ = std::make_unique<CharType[]>(length_ + 1);
			std::copy(cstr, cstr + length_, buffer_.get());
			buffer_[length_] = static_cast<CharType>(0);
		}
		else
		{
			buffer_ = std::make_unique<CharType[]>(1);
			buffer_[0] = static_cast<CharType>(0);
			length_ = 0;
		}
	}

	// Копирующий конструктор
	basic_string(const basic_string& other)
		: buffer_(std::make_unique<CharType[]>(other.length_ + 1)),
		  length_(other.length_)
	{
		std::copy(other.buffer_.get(), other.buffer_.get() + length_ + 1,
				  buffer_.get());
	}

	// Перемещающий конструктор
	basic_string(basic_string&& other) noexcept
		: buffer_(std::move(other.buffer_)), length_(other.length_)
	{
		other.length_ = 0;
		other.buffer_ = std::make_unique<CharType[]>(1);
		other.buffer_[0] = static_cast<CharType>(0);
	}

	// Деструктор
	~basic_string() = default;

	// Получение C-строки
	const CharType* c_str() const noexcept { return buffer_.get(); }

	// Получение размера
	size_t size() const noexcept { return length_; }

	// Оператор присваивания перемещением
	basic_string& operator=(basic_string&& other) noexcept
	{
		if (this != &other)
		{
			buffer_ = std::move(other.buffer_);
			length_ = other.length_;
			other.length_ = 0;
			other.buffer_ = std::make_unique<CharType[]>(1);
			other.buffer_[0] = static_cast<CharType>(0);
		}
		return *this;
	}

	// Оператор присваивания C-строки
	basic_string& operator=(const CharType* cstr)
	{
		if (cstr)
		{
			size_t new_length = calculate_length(cstr);
			auto new_buffer = std::make_unique<CharType[]>(new_length + 1);
			std::copy(cstr, cstr + new_length, new_buffer.get());
			new_buffer[new_length] = static_cast<CharType>(0);
			buffer_ = std::move(new_buffer);
			length_ = new_length;
		}
		else
		{
			buffer_ = std::make_unique<CharType[]>(1);
			buffer_[0] = static_cast<CharType>(0);
			length_ = 0;
		}
		return *this;
	}

	// Копирующий оператор присваивания
	basic_string& operator=(const basic_string& other)
	{
		if (this != &other)
		{
			basic_string temp(other);
			swap(temp);
		}
		return *this;
	}

	// Конкатенация строк
	friend basic_string operator+(const basic_string& lhs,
								  const basic_string& rhs)
	{
		basic_string result;
		result.length_ = lhs.length_ + rhs.length_;
		result.buffer_ = std::make_unique<CharType[]>(result.length_ + 1);
		std::copy(lhs.buffer_.get(), lhs.buffer_.get() + lhs.length_,
				  result.buffer_.get());
		std::copy(rhs.buffer_.get(), rhs.buffer_.get() + rhs.length_,
				  result.buffer_.get() + lhs.length_);
		result.buffer_[result.length_] = static_cast<CharType>(0);
		return result;
	}

	// Оператор вывода
	template <typename Stream>
	friend Stream& operator<<(Stream& os, const basic_string& str)
	{
		return os << str.c_str();
	}

	// Оператор ввода
	template <typename Stream>
	friend Stream& operator>>(Stream& is, basic_string& str)
	{
		std::basic_string<CharType> temp;
		std::getline(is, temp);
		str = temp.c_str();
		return is;
	}

	// Оператор += для строки
	basic_string& operator+=(const basic_string& other)
	{
		*this = *this + other;
		return *this;
	}

	// Оператор += для символа
	basic_string& operator+=(CharType ch)
	{
		auto new_buffer = std::make_unique<CharType[]>(length_ + 2);
		std::copy(buffer_.get(), buffer_.get() + length_, new_buffer.get());
		new_buffer[length_] = ch;
		new_buffer[length_ + 1] = static_cast<CharType>(0);
		buffer_ = std::move(new_buffer);
		++length_;
		return *this;
	}

	// Оператор индексации
	CharType& operator[](size_t pos) noexcept { return buffer_[pos]; }
	const CharType& operator[](size_t pos) const noexcept
	{
		return buffer_[pos];
	}

	// Доступ с проверкой границ
	CharType& at(size_t pos)
	{
		if (pos >= length_)
			throw std::out_of_range("Invalid position");
		return buffer_[pos];
	}

	const CharType& at(size_t pos) const
	{
		if (pos >= length_)
			throw std::out_of_range("Invalid position");
		return buffer_[pos];
	}

	// Получение указателя на данные
	CharType* data() noexcept { return buffer_.get(); }
	const CharType* data() const noexcept { return buffer_.get(); }

   private:
	// Вычисление длины C-строки
	static size_t calculate_length(const CharType* str)
	{
		size_t len = 0;
		while (str[len] != static_cast<CharType>(0))
			++len;
		return len;
	}

	// Обмен содержимым
	void swap(basic_string& other) noexcept
	{
		using std::swap;
		swap(buffer_, other.buffer_);
		swap(length_, other.length_);
	}

	std::unique_ptr<CharType[]> buffer_;
	size_t length_;
};

}  // namespace bmstu