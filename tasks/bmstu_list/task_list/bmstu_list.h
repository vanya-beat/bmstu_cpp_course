#include <cstddef>
#include <iterator>
#include <ostream>
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
		using difference_type = ::bmstu::abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::difference_type;
		using reference = ::bmstu::abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::reference;
		using pointer = ::bmstu::abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::pointer;
		node* current;

		iterator() : current(nullptr) {}
		explicit iterator(node* n) : current(n) {}

		iterator& operator++() override
		{
			current = current->next_node_;
			return *this;
		}

		iterator& operator--() override
		{
			current = current->prev_node_;
			return *this;
		}

		iterator operator++(int) override
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		iterator operator--(int) override
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}

		iterator& operator+=(
			const ::bmstu::abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			if (n > 0)
			{
				for (difference_type i = 0; i < n; ++i)
					++(*this);
			}
			else
			{
				for (difference_type i = 0; i < -n; ++i)
					--(*this);
			}
			return *this;
		}

		iterator& operator-=(const difference_type& n) override
		{
			return *this += (-n);
		}

		iterator operator+(const difference_type& n) const override
		{
			iterator tmp = *this;
			tmp += n;
			return tmp;
		}

		iterator operator-(const difference_type& n) const override
		{
			iterator tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator*() const override { return current->value_; }

		pointer operator->() const override { return &current->value_; }

		bool operator==(const iterator& other) const override
		{
			return current == other.current;
		}

		bool operator!=(const iterator& other) const override
		{
			return current != other.current;
		}

		explicit operator bool() const override { return current != nullptr; }

		difference_type operator-(const iterator& other) const override
		{
			difference_type count = 0;
			iterator tmp = other;
			while (tmp != *this && tmp.current)
			{
				++tmp;
				++count;
			}
			return (tmp == *this) ? count : 0;
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
		{
			push_back(*begin);
		}
	}

	list(std::initializer_list<T> values) : list(values.begin(), values.end())
	{
	}

	list(const list& other) : list()
	{
		for (const auto& item : other)
		{
			push_back(item);
		}
	}

	list(list&& other) noexcept : list() { swap(other); }

#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_node = new node(last, value, tail_);
		tail_->prev_node_ = new_node;
		last->next_node_ = new_node;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		node* first = head_->next_node_;
		node* new_node = new node(head_, value, first);
		head_->next_node_ = new_node;
		first->prev_node_ = new_node;
		++size_;
	}

#pragma endregion

	bool empty() const noexcept { return size_ == 0; }

	~list()
	{
		clear();
		delete head_;
		delete tail_;
	}

	void clear()
	{
		while (!empty())
		{
			pop_front();
		}
	}

	void pop_front()
	{
		if (!empty())
		{
			node* first = head_->next_node_;
			head_->next_node_ = first->next_node_;
			first->next_node_->prev_node_ = head_;
			delete first;
			--size_;
		}
	}

	void pop_back()
	{
		if (!empty())
		{
			node* last = tail_->prev_node_;
			tail_->prev_node_ = last->prev_node_;
			last->prev_node_->next_node_ = tail_;
			delete last;
			--size_;
		}
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

	iterator begin() noexcept { return iterator(head_->next_node_); }

	iterator end() noexcept { return iterator(tail_); }

	const_iterator begin() const noexcept
	{
		return const_iterator(head_->next_node_);
	}

	const_iterator end() const noexcept { return const_iterator(tail_); }

	const_iterator cbegin() const noexcept
	{
		return const_iterator(head_->next_node_);
	}

	const_iterator cend() const noexcept { return const_iterator(tail_); }

#pragma endregion

	T operator[](size_t pos) const
	{
		iterator it = begin();
		while (pos--)
			++it;
		return *it;
	}

	T& operator[](size_t pos)
	{
		iterator it = begin();
		while (pos--)
			++it;
		return *it;
	}

	friend bool operator==(const list& l, const list& r)
	{
		if (l.size_ != r.size_)
			return false;
		auto it1 = l.begin();
		auto it2 = r.begin();
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
		auto it1 = lhs.begin();
		auto it2 = rhs.begin();
		while (it1 != lhs.end() && it2 != rhs.end())
		{
			if (*it1 < *it2)
				return -1;
			if (*it2 < *it1)
				return 1;
			++it1;
			++it2;
		}
		return (it1 == lhs.end()) ? ((it2 == rhs.end()) ? 0 : -1) : 1;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
		bool first = true;
		for (const auto& item : other)
		{
			if (!first)
				os << ", ";
			os << item;
			first = false;
		}
		os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		node* new_node = new node(pos.current->prev_node_, value, pos.current);
		pos.current->prev_node_->next_node_ = new_node;
		pos.current->prev_node_ = new_node;
		++size_;
		return iterator(new_node);
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto it1 = l.begin();
		auto it2 = r.begin();
		while (it1 != l.end() && it2 != r.end())
		{
			if (*it1 < *it2)
				return true;
			if (*it2 < *it1)
				return false;
			++it1;
			++it2;
		}
		return (it1 == l.end()) && (it2 != r.end());
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};

}  // namespace bmstu