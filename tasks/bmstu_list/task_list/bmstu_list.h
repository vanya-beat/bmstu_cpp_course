#include <cstddef>
#include <iterator>
#include <ostream>
#include <compare>
#include "abstract_iterator.h"

namespace bmstu
{
template <typename T>
class list
{
	struct node
	{
		node() = default;

		node(node* prev, const T& value, node* next)
			: value_(value), next_node_(next), prev_node_(prev)
		{
		}

		T value_;
		node* next_node_ = nullptr;
		node* prev_node_ = nullptr;
	};

   public:
	struct iterator
		: public abstract_iterator<iterator, T, std::bidirectional_iterator_tag>
	{
		node* current;
		iterator() : current(nullptr) {}
		iterator(node* node) : current(node) {}

		iterator& operator++() override
		{
			if (current) current = current->next_node_;
			return *this;
		}

		iterator& operator--() override
		{
			if (current) current = current->prev_node_;
			return *this;
		}

		iterator operator++(int) override
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		iterator operator--(int) override
		{
			iterator temp = *this;
			--(*this);
			return temp;
		}

		iterator& operator+=(const typename iterator::difference_type& n) override
		{
			for (typename iterator::difference_type i = 0; i < n; ++i)
				++(*this);
			return *this;
		}

		iterator& operator-=(const typename iterator::difference_type& n) override
		{
			for (typename iterator::difference_type i = 0; i < n; ++i)
				--(*this);
			return *this;
		}

		iterator operator+(const typename iterator::difference_type& n) const override
		{
			iterator it = *this;
			it += n;
			return it;
		}

		iterator operator-(const typename iterator::difference_type& n) const override
		{
			iterator it = *this;
			it -= n;
			return it;
		}

		typename iterator::reference operator*() const override
		{
			return current->value_;
		}

		typename iterator::pointer operator->() const override
		{
			return &(current->value_);
		}

		bool operator==(const iterator& other) const override
		{
			return current == other.current;
		}

		bool operator!=(const iterator& other) const override
		{
			return current != other.current;
		}

		explicit operator bool() const override { return current != nullptr; }

		typename iterator::difference_type operator-(const iterator& other) const override
		{
			typename iterator::difference_type count = 0;
			node* it = other.current;
			while (it && it != current)
			{
				it = it->next_node_;
				++count;
			}
			return count;
		}
	};

	using const_iterator = iterator;

	list()
	{
		head_ = new node();
		tail_ = new node();
		head_->next_node_ = tail_;
		tail_->prev_node_ = head_;
	}

	template <typename it>
	list(it begin, it end) : list()
	{
		for (; begin != end; ++begin)
			push_back(*begin);
	}

	list(std::initializer_list<T> values) : list()
	{
		for (const T& value : values)
			push_back(value);
	}

	list(const list& other) : list()
	{
		for (const auto& value : other)
			push_back(value);
	}

	list(list&& other) : head_(other.head_), tail_(other.tail_), size_(other.size_)
	{
		other.head_ = new node();
		other.tail_ = new node();
		other.head_->next_node_ = other.tail_;
		other.tail_->prev_node_ = other.head_;
		other.size_ = 0;
	}

#pragma endregion
#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_last = new node(last, value, tail_);
		tail_->prev_node_ = new_last;
		last->next_node_ = new_last;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		node* first = head_->next_node_;
		node* new_first = new node(head_, value, first);
		head_->next_node_ = new_first;
		first->prev_node_ = new_first;
		++size_;
	}

#pragma endregion

	bool empty() const noexcept
	{
		return size_ == 0u;
	}

	~list()
	{
		clear();
		delete head_;
		delete tail_;
	}

	void clear()
	{
		node* current = head_->next_node_;
		while (current != tail_)
		{
			node* temp = current;
			current = current->next_node_;
			delete temp;
		}
		head_->next_node_ = tail_;
		tail_->prev_node_ = head_;
		size_ = 0;
	}

	size_t size() const { return size_; }

	void swap(list& other) noexcept
	{
		std::swap(head_, other.head_);
		std::swap(tail_, other.tail_);
		std::swap(size_, other.size_);
	}

	friend void swap(list& l, list& r) { l.swap(r); }

#pragma region iterators

	iterator begin() noexcept { return iterator{head_->next_node_}; }
	iterator end() noexcept { return iterator{tail_}; }
	const_iterator begin() const noexcept { return const_iterator{head_->next_node_}; }
	const_iterator end() const noexcept { return const_iterator{tail_}; }
	const_iterator cbegin() const noexcept { return const_iterator{head_->next_node_}; }
	const_iterator cend() const noexcept { return const_iterator{tail_}; }

#pragma endregion

	T operator[](size_t pos) const
	{
		auto it = begin();
		for (size_t i = 0; i < pos; ++i)
			++it;
		return *it;
	}

	T& operator[](size_t pos)
	{
		auto it = begin();
		for (size_t i = 0; i < pos; ++i)
			++it;
		return *it;
	}

	friend bool operator==(const list& l, const list& r)
	{
		if (l.size_ != r.size_) return false;
		auto it1 = l.begin(), it2 = r.begin();
		while (it1 != l.end())
		{
			if (*it1 != *it2)
				return false;
			++it1;
			++it2;
		}
		return true;
	}

	friend bool operator!=(const list& l, const list& r) { return !(l == r); }

	friend auto operator<=>(const list& lhs, const list& rhs)
	{
		auto it1 = lhs.begin(), it2 = rhs.begin();
		while (it1 != lhs.end() && it2 != rhs.end())
		{
			if (*it1 < *it2) return std::strong_ordering::less;
			if (*it2 < *it1) return std::strong_ordering::greater;
			++it1;
			++it2;
		}
		if (lhs.size_ < rhs.size_) return std::strong_ordering::less;
		if (lhs.size_ > rhs.size_) return std::strong_ordering::greater;
		return std::strong_ordering::equal;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
		for (auto it = other.begin(); it != other.end();)
		{
			os << *it;
			if (++it != other.end()) os << ", ";
		}
		os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		node* next = pos.current;
		node* prev = next->prev_node_;
		node* new_node = new node(prev, value, next);
		prev->next_node_ = new_node;
		next->prev_node_ = new_node;
		++size_;
		return iterator{new_node};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto it1 = l.begin(), it2 = r.begin();
		while (it1 != l.end() && it2 != r.end())
		{
			if (*it1 < *it2) return true;
			if (*it2 < *it1) return false;
			++it1;
			++it2;
		}
		return l.size_ < r.size_;
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
}  // namespace bmstu
