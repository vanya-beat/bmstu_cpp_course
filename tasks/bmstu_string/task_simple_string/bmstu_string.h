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
	basic_string() : ptr_(new T[1]{0}), size_(0) {
		// std::cout << "default constructor_1" <<  "\n" ;
		ptr_ = new T[1];
		size_ = 0;
		ptr_[0] = 0;
	}

	basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {  
		//std::cout << "default constructor_2" <<  "\n" ;
		size_=size;
		ptr_ = new T[size + 1];
		for (size_t i = 0; i < size_; ++i) {
			ptr_[i] = ' ';
		  }
		ptr_[size_] = 0;
	}

	basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		//std::cout << "initializer_list" <<  "\n" ;
		size_=il.size();
		ptr_ = new T[size_ + 1];
		size_t i = 0;
    	for (auto &item : il) {
      		ptr_[i] = item;
      		++i;
    	}
    	ptr_[size_] = 0;		
	}

	/// Конструктор с параметром си-с
	basic_string(const T* c_str) {
		//std::cout << "c_str constructor" <<  "\n" ;
        size_ = strlen_(c_str);
		if (size_ != 0) {
			ptr_ = new T[size_ + 1];
			ptr_[size_] = static_cast<T>('\0');
        	std::copy(c_str, c_str + size_, ptr_);
		}
		else {
            ptr_ = new T[1];
            *ptr_ = static_cast<T>('\0');
            size_ = 0;
        }
    }

	/// Конструктор копирования
	basic_string(const basic_string& other) {
		//std::cout << "copy constructor" <<  "\n" ;
		if (this != &other)
		{
			clean_();
      		size_ = other.size_;
      		ptr_ = new T[size_ + 1];
      		ptr_[size_] = 0;
      		for (size_t i = 0; i < size_; i++) {
          		*(ptr_ + i) = other.c_str()[i];
      	} 
		}
    }

	/// Перемещающий конструктор
	basic_string(basic_string&& dying) noexcept {
		//std::cout << "move constructor" <<  "\n" ;
		clean_();
		ptr_ = dying.ptr_;
		size_ = dying.size_;
        *this = std::move(dying);
    }

	/// Деструктор
	~basic_string() {
		//std::cout << "destructor" <<  "\n" ;
		clean_();
    }

	/// Геттер на си-строку
	const T* c_str() const { 
		//std::cout << "getter c_str" <<  "\n" ;
		return static_cast<const T *>(ptr_); 
	}

    /// Геттер на размер
	
	size_t size() const { 
		//std::cout << "getter size" <<  "\n" ;
		return size_; 
	}

	/// Оператор копирующего присваивания
	basic_string& operator=(basic_string&& other) {
		//std::cout << "copy = operator 1" <<  "\n" ;
        if (this == &other) {
            return *this;
        }
		else
		{
			basic_string s = other;
  			std::swap(*this, s);
  			return *this; 
		} 
    }

	/// Оператор копирующего присваивания си строки
	basic_string& operator=(const T* c_str) { 
		//std::cout << "copy = operatior c_str" <<  "\n" ;
		if (c_str != nullptr) {
            clean_();
            size_ = strlen_(c_str);
            ptr_ = new T[size_ + 1];
			for (size_t i = 0; i < size_; i++) ptr_[i] = c_str[i];
            ptr_[size_] = 0;
		} 
		else {
            clean_();
        }
        return *this;
    }

	/// Оператор копирующего присваивания
	basic_string& operator=(const basic_string& other) {
		//std::cout << "copy = operator 2" <<  "\n" ;
        if (this == &other) {
            return *this;
        }
        basic_string s = other;
        std::swap(*this, s);
        return *this; 
    }

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		//std::cout << "friend operator plus" <<  "\n" ;
        basic_string ts = left;
        return ts += right;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		//std::cout << "friend operator vyvod" <<  "\n" ;
		os << obj.ptr_;
        return os;
		//return os << obj.ptr_();
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{		
		//std::cout << "friend operator vvod" <<  "\n" ;
		obj.clean_();  // Используем функцию clean_ для очистки содержимого
        T input_char;
        while (is.get(input_char)) {
            if (input_char == ' ' || input_char == '\n' || input_char == '\t') {
                break;  // Прекращаем считывание при пробеле, новой строке или табуляции
            }
            obj += input_char;  // Добавляем символ к объекту
        }
        return is;

		//return is >> obj.ptr_();
	}

	basic_string& operator+=(const basic_string& other) { 
		//std::cout << " operator plus ravno 1" <<  "\n" ;
		size_t new_size = size_ + other.size_;
        T *new_ptr = new T[new_size + 1];
        std::copy(ptr_, ptr_ + size_, new_ptr);
        std::copy(other.ptr_, other.ptr_ + other.size_ + 1, new_ptr + size_);
        delete[] ptr_;
        ptr_ = new_ptr;
        size_ = new_size;
        return *this;
    }

	basic_string& operator+=(T symbol) { 
		//std::cout << " operator plus ravno 2" <<  "\n" ;
		T *prev_data = ptr_;
		++size_;
		ptr_ = new T[size_ + 1];
		std::copy(ptr_, ptr_ + size_, ptr_);
		delete[] prev_data;
		ptr_[size_ - 1] = symbol;
		ptr_[size_] = 0;
		return *this;
    }

	T& operator[](size_t index) noexcept { 
		//std::cout << " operator index" <<  "\n" ;
		return *(ptr_ + index); 
	}

	T& at(size_t index) { 
		//std::cout << " Wrong index" <<  "\n" ;
		throw std::out_of_range("Wrong index"); 
	}

	T* data() { return ptr_; }

   private:
	static size_t strlen_(const T* str) { 
		size_t counter = 0;
    	while (str[counter] != 0) counter++;
    	return counter; 
	}

	void clean_() {
		if (!ptr_) return;
		size_ = 0;
		delete[] ptr_;
	  }

	T* ptr_ = nullptr;
	size_t size_;
};
}  // namespace bmstu