#include <ostream>
#include <stdexcept>
#include <utility>
#include "array_ptr.h"

namespace bmstu
{

template <typename Type>
class simple_vector
{
   public:
	class iterator
	{
	   public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Type;
		using pointer = Type*;
		using reference = Type&;
		using difference_type = std::ptrdiff_t;

		iterator() = default;
		explicit iterator(pointer ptr) : current_ptr_(ptr) {}

		reference operator*() const noexcept { return *current_ptr_; }
		pointer operator->() const noexcept { return current_ptr_; }

		iterator& operator++() noexcept
		{
			++current_ptr_;
			return *this;
		}
		iterator operator++(int) noexcept
		{
			iterator temp = *this;
			++current_ptr_;
			return temp;
		}
		iterator& operator--() noexcept
		{
			--current_ptr_;
			return *this;
		}
		iterator operator--(int) noexcept
		{
			iterator temp = *this;
			--current_ptr_;
			return temp;
		}

		iterator operator+(difference_type n) const noexcept
		{
			return iterator(current_ptr_ + n);
		}
		iterator operator-(difference_type n) const noexcept
		{
			return iterator(current_ptr_ - n);
		}
		difference_type operator-(const iterator& other) const noexcept
		{
			return current_ptr_ - other.current_ptr_;
		}

		iterator& operator+=(difference_type n) noexcept
		{
			current_ptr_ += n;
			return *this;
		}
		iterator& operator-=(difference_type n) noexcept
		{
			current_ptr_ -= n;
			return *this;
		}

		bool operator==(const iterator& other) const noexcept
		{
			return current_ptr_ == other.current_ptr_;
		}
		bool operator!=(const iterator& other) const noexcept
		{
			return current_ptr_ != other.current_ptr_;
		}

		bool operator==(std::nullptr_t) const noexcept
		{
			return current_ptr_ == nullptr;
		}
		bool operator!=(std::nullptr_t) const noexcept
		{
			return current_ptr_ != nullptr;
		}
		friend bool operator==(std::nullptr_t, const iterator& it) noexcept
		{
			return it.current_ptr_ == nullptr;
		}
		friend bool operator!=(std::nullptr_t, const iterator& it) noexcept
		{
			return it.current_ptr_ != nullptr;
		}

		iterator& operator=(std::nullptr_t) noexcept
		{
			current_ptr_ = nullptr;
			return *this;
		}

		iterator& operator=(const iterator& other) noexcept
		{
			current_ptr_ = other.current_ptr_;
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return current_ptr_ != nullptr;
		}

	   private:
		pointer current_ptr_ = nullptr;
	};

	using const_iterator = const iterator;

	simple_vector() noexcept = default;

	explicit simple_vector(size_t count)
		: elements_(count), element_count_(count), capacity_(count)
	{
		for (size_t i = 0; i < element_count_; ++i)
		{
			elements_[i] = Type();
		}
	}

	simple_vector(size_t count, const Type& value)
		: elements_(count), element_count_(count), capacity_(count)
	{
		for (auto it = begin(); it != end(); ++it)
		{
			*it = value;
		}
	}

	simple_vector(std::initializer_list<Type> init)
		: elements_(init.size()),
		  element_count_(init.size()),
		  capacity_(init.size())
	{
		auto it = begin();
		for (const Type& item : init)
		{
			*it++ = item;
		}
	}

	simple_vector(const simple_vector& other)
		: elements_(other.element_count_),
		  element_count_(other.element_count_),
		  capacity_(other.element_count_)
	{
		auto it = begin();
		auto other_it = other.begin();
		for (; other_it != other.end(); ++it, ++other_it)
		{
			*it = *other_it;
		}
	}

	simple_vector(simple_vector&& other) noexcept { swap(other); }

	~simple_vector() = default;

	simple_vector& operator=(const simple_vector& right)
	{
		if (this != &right)
		{
			if (right.empty())
			{
				clear();
				return *this;
			}

			simple_vector temp(right);
			swap(temp);
		}
		return *this;
	}

	simple_vector& operator=(simple_vector&& right) noexcept
	{
		swap(right);
		return *this;
	}

	iterator begin() noexcept { return iterator(elements_.get()); }
	iterator end() noexcept
	{
		return iterator(elements_.get() + element_count_);
	}
	const_iterator begin() const noexcept { return iterator(elements_.get()); }
	const_iterator end() const noexcept
	{
		return iterator(elements_.get() + element_count_);
	}
	const_iterator cbegin() const noexcept { return iterator(elements_.get()); }
	const_iterator cend() const noexcept
	{
		return iterator(elements_.get() + element_count_);
	}

	Type& operator[](size_t index) noexcept { return elements_[index]; }
	const Type& operator[](size_t index) const noexcept
	{
		return elements_[index];
	}
	Type& at(size_t index)
	{
		if (index >= element_count_)
			throw std::out_of_range("Index out of range");
		return elements_[index];
	}

	const Type& at(size_t index) const
	{
		if (index >= element_count_)
			throw std::out_of_range("Index out of range");
		return elements_[index];
	}

	size_t size() const noexcept { return element_count_; }
	size_t capacity() const noexcept { return capacity_; }
	bool empty() const noexcept { return element_count_ == 0; }

	void swap(simple_vector& other) noexcept
	{
		elements_.swap(other.elements_);
		std::swap(element_count_, other.element_count_);
		std::swap(capacity_, other.capacity_);
	}

	friend void swap(simple_vector<Type>& left, simple_vector<Type>& right)
	{
		left.swap(right);
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity_)
		{
			array_ptr<Type> new_elements(new_capacity);
			for (size_t i = 0; i < element_count_; ++i)
			{
				new_elements[i] = std::move(elements_[i]);
			}
			elements_.swap(new_elements);
			capacity_ = new_capacity;
		}
	}

	void resize(size_t new_size)
	{
		if (new_size > capacity_)
		{
			size_t new_capacity = std::max(new_size, capacity_ * 2);
			reserve(new_capacity);
		}
		if (element_count_ > new_size)
		{
			element_count_ = new_size;
			return;
		}
		for (auto it = end(); it != begin() + new_size; ++it)
		{
			*it = Type{};
		}
		element_count_ = new_size;
	}

	void push_back(const Type& value)
	{
		Type temp_value = value;

		if (element_count_ >= capacity_)
		{
			size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
			array_ptr<Type> new_elements(new_capacity);

			for (size_t i = 0; i < element_count_; ++i)
			{
				new_elements[i] = std::move(elements_[i]);
			}
			new_elements[element_count_] = std::move(temp_value);

			elements_.swap(new_elements);
			capacity_ = new_capacity;
		}
		else
		{
			elements_[element_count_] = std::move(temp_value);
		}
		++element_count_;
	}

	void push_back(Type&& value)
	{
		if (element_count_ >= capacity_)
		{
			size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
			array_ptr<Type> new_elements(new_capacity);
			for (size_t i = 0; i < element_count_; ++i)
			{
				new_elements[i] = std::move(elements_[i]);
			}
			new_elements[element_count_] = std::move(value);
			elements_.swap(new_elements);
			capacity_ = new_capacity;
			++element_count_;
		}
		else
		{
			elements_[element_count_++] = std::move(value);
		}
	}

	void pop_back()
	{
		if (element_count_ > 0)
		{
			--element_count_;
		}
	}

	iterator insert(const_iterator pos, const Type& value)
	{
		size_t offset = pos - begin();
		if (element_count_ >= capacity_)
		{
			size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
			array_ptr<Type> new_elements(new_capacity);
			for (size_t i = 0; i < offset; ++i)
			{
				new_elements[i] = std::move(elements_[i]);
			}
			new_elements[offset] = value;
			for (size_t i = offset; i < element_count_; ++i)
			{
				new_elements[i + 1] = std::move(elements_[i]);
			}
			elements_.swap(new_elements);
			capacity_ = new_capacity;
		}
		else
		{
			for (size_t i = element_count_; i > offset; --i)
			{
				elements_[i] = std::move(elements_[i - 1]);
			}
			elements_[offset] = value;
		}
		++element_count_;
		return iterator(elements_.get() + offset);
	}

	iterator erase(iterator pos)
	{
		size_t offset = pos - begin();
		for (size_t i = offset; i < element_count_ - 1; ++i)
		{
			elements_[i] = std::move(elements_[i + 1]);
		}
		--element_count_;
		return iterator(elements_.get() + offset);
	}

	void clear() noexcept { element_count_ = 0; }

	friend bool operator==(const simple_vector& left,
						   const simple_vector& right)
	{
		if (left.element_count_ != right.element_count_)
			return false;
		for (size_t i = 0; i < left.element_count_; ++i)
		{
			if (left.elements_[i] != right.elements_[i])
				return false;
		}
		return true;
	}

	friend bool operator!=(const simple_vector& left,
						   const simple_vector& right)
	{
		return !(left == right);
	}

	friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
	{
		os << "[ ";
		for (size_t i = 0; i < vec.element_count_; ++i)
		{
			os << vec.elements_[i];
			if (i != vec.element_count_ - 1)
				os << ", ";
		}
		os << " ]";
		return os;
	}

	friend bool operator<(const simple_vector& left, const simple_vector& right)
	{
		auto left_begin = left.begin();
		auto right_begin = right.begin();
		auto left_end = left.end();
		auto right_end = right.end();
		for (; (left_begin != left_end) && (right_begin != right_end);
			 ++left_begin, ++right_begin)
		{
			if (*left_begin < *right_begin)
			{
				return true;
			}
			if (*left_begin > *right_begin)
			{
				return false;
			}
		}
		return (left_begin == left_end) && (right_begin != right_end);
	}
	friend bool operator>(const simple_vector& left, const simple_vector& right)
	{
		return !(left <= right);
	}

	friend bool operator<=(const simple_vector& left,
						   const simple_vector& right)
	{
		return (left < right || left == right);
	}

	friend bool operator>=(const simple_vector& left,
						   const simple_vector& right)
	{
		return !(left < right);
	}

   private:
	array_ptr<Type> elements_;
	size_t element_count_ = 0;
	size_t capacity_ = 0;
};

}  // namespace bmstu
