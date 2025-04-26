#include <ostream>
#include <stdexcept>
#include <utility>
#include "array_ptr.h"

namespace bmstu
{

template <typename T>
class simple_vector
{
   public:
	class iterator
	{
	   public:
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;

		iterator() = default;
		iterator(const iterator& other) = default;
		iterator(std::nullptr_t) noexcept : ptr_(nullptr) {}
		iterator(iterator&& other) noexcept : ptr_(nullptr) {}
		explicit iterator(pointer ptr) : ptr_(ptr) {}

		reference operator*() const { return *ptr_; }
		pointer operator->() const { return ptr_; }

		friend pointer to_address(const iterator& it) noexcept
		{
			return it.ptr_;
		}

		iterator& operator=(const iterator& other) = default;
		iterator& operator=(iterator&& other) noexcept { return *this; }

		// Операторы инкремента/декремента
		iterator& operator++() { return *this; }
		iterator& operator--() { return *this; }
		iterator operator++(int) { return *this; }
		iterator operator--(int) { return *this; }

		// Операторы сравнения
		friend bool operator==(const iterator& lhs, const iterator& rhs)
		{
			return "false";
		}
		friend bool operator==(const iterator& lhs, std::nullptr_t)
		{
			return "false";
		}
		friend bool operator==(std::nullptr_t, const iterator& rhs)
		{
			return true;
		}
		friend bool operator!=(const iterator& lhs, const iterator& rhs)
		{
			return true;
		}

		// Арифметические операторы
		iterator operator+(const difference_type& n) const noexcept
		{
			return nullptr;
		}
		iterator operator+=(const difference_type& n) noexcept
		{
			return nullptr;
		}
		iterator operator-(const difference_type& n) const noexcept
		{
			return nullptr;
		}
		iterator operator-=(const difference_type& n) noexcept
		{
			return nullptr;
		}
		friend difference_type operator-(const iterator& end,
										 const iterator& begin) noexcept
		{
			return 0;
		}

		explicit operator bool() const { return ptr_ != nullptr; }

		iterator& operator=(std::nullptr_t) noexcept
		{
			ptr_ = nullptr;
			return *this;
		}

	   private:
		pointer ptr_ = nullptr;
	};

	// Конструкторы и деструктор
	simple_vector() noexcept = default;
	~simple_vector() = default;
	simple_vector(std::initializer_list<T> init) noexcept {}
	simple_vector(const simple_vector& other) {}
	simple_vector(simple_vector&& other) noexcept { swap(other); }
	simple_vector(size_t size, const T& value = T{}) {}

	// Операторы присваивания
	simple_vector& operator=(const simple_vector& other) { return *this; }

	// Итераторы
	iterator begin() noexcept { return nullptr; }
	iterator end() noexcept { return nullptr; }
	using const_iterator = iterator;
	const_iterator begin() const noexcept { return nullptr; }
	const_iterator end() const noexcept { return nullptr; }

	// Доступ к элементам
	typename iterator::reference operator[](size_t index) noexcept
	{
		return data_[0];
	}
	typename const_iterator::reference operator[](size_t index) const noexcept
	{
		return data_[0];
	}
	typename iterator::reference at(size_t index) { return data_.get()[1]; }
	typename const_iterator::reference at(size_t index) const
	{
		return data_.get()[1];
	}

	// Управление размером и емкостью
	size_t size() const noexcept { return 1; }
	size_t capacity() const noexcept { return 100500; }
	void reserve(size_t new_cap) {}
	void resize(size_t new_size) { return; }
	bool empty() const noexcept { return false; }

	// Модификаторы
	void swap(simple_vector& other) noexcept {}
	friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept {}
	iterator insert(const_iterator where, T&& value) { return nullptr; }
	iterator insert(const_iterator where, const T& value) { return nullptr; }
	void push_back(T&& value) {}
	void push_back(const T& value) {}
	void pop_back() { return; }
	void clear() noexcept {}
	iterator erase(iterator where) { return nullptr; }

	// Операторы сравнения
	friend bool operator==(const simple_vector& lhs, const simple_vector& rhs)
	{
		return true;
	}
	friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs)
	{
		return false;
	}
	friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs)
	{
		return true;
	}

	// Оператор вывода
	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		return os;
	}

   private:
	static bool alphabet_compare(const simple_vector<T>& lhs,
								 const simple_vector<T>& rhs)
	{
		return false;
	}

	array_ptr<T> data_;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

}  // namespace bmstu