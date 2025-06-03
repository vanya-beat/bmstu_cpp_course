#include <cstddef>
#include <initializer_list>
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
		node* current;

		iterator() : current(nullptr) {}
		explicit iterator(node* n) : current(n) {}
		explicit iterator(const node* n) : current(const_cast<node*>(n)) {}

		iterator& operator++() override
		{
			if (current)
			{
				current = current->next_node_;
			}
			return *this;
		}

		iterator& operator--() override
		{
			if (current)
			{
				current = current->prev_node_;
			}
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
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			if (n > 0)
			{
				for (typename abstract_iterator<
						 iterator, T,
						 std::bidirectional_iterator_tag>::difference_type i =
						 0;
					 i < n && current; ++i)
				{
					++(*this);
				}
			}
			else if (n < 0)
			{
				for (typename abstract_iterator<
						 iterator, T,
						 std::bidirectional_iterator_tag>::difference_type i =
						 0;
					 i < -n && current; ++i)
				{
					--(*this);
				}
			}
			return *this;
		}

		iterator& operator-=(
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			return (*this) += -n;
		}

		iterator operator+(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const override
		{
			iterator tmp = *this;
			return tmp += n;
		}

		iterator operator-(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const override
		{
			iterator tmp = *this;
			return tmp -= n;
		}

		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::reference
		operator*() const override
		{
			return current->value_;
		}

		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::pointer
		operator->() const override
		{
			return &(current->value_);
		}

		bool operator==(const iterator& other) const override
		{
			return current == other.current;
		}

		bool operator!=(const iterator& other) const override
		{
			return !(*this == other);
		}

		explicit operator bool() const override { return current != nullptr; }

		typename abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::difference_type
		operator-(const iterator& other) const override
		{
			if (*this == other)
			{
				return 0;
			}

			typename abstract_iterator<
				iterator, T, std::bidirectional_iterator_tag>::difference_type
				dist = 0;
			iterator temp = other;
			while (temp.current != nullptr && temp != *this)
			{
				++temp;
				++dist;
			}

			if (temp == *this)
			{
				return dist;
			}

			dist = 0;
			temp = *this;
			while (temp.current != nullptr && temp != other)
			{
				++temp;
				--dist;
			}

			if (temp == other)
			{
				return dist;
			}

			return 0;
		}
	};

	// Добавляем объявление const_iterator
	using const_iterator = iterator;

	list()
	{
		head_.next_node_ = &tail_;
		tail_.prev_node_ = &head_;
	}

	template <typename InputIt>
	list(InputIt first, InputIt last) : list()
	{
		for (; first != last; ++first)
		{
			push_back(*first);
		}
	}

	list(std::initializer_list<T> init) : list(init.begin(), init.end()) {}

	list(const list& other) : list()
	{
		for (const auto& item : other)
		{
			push_back(item);
		}
	}

	list(list&& other) noexcept : size_(other.size_)
	{
		head_.next_node_ = other.head_.next_node_;
		tail_.prev_node_ = other.tail_.prev_node_;

		if (head_.next_node_ != &other.tail_)
		{
			head_.next_node_->prev_node_ = &head_;
			tail_.prev_node_->next_node_ = &tail_;
		}
		else
		{
			head_.next_node_ = &tail_;
			tail_.prev_node_ = &head_;
		}

		other.head_.next_node_ = &other.tail_;
		other.tail_.prev_node_ = &other.head_;
		other.size_ = 0;
	}

	~list() { clear(); }

	list& operator=(const list& other)
	{
		if (this != &other)
		{
			list tmp(other);
			swap(tmp);
		}
		return *this;
	}

	list& operator=(list&& other) noexcept
	{
		if (this != &other)
		{
			clear();
			head_.next_node_ = other.head_.next_node_;
			tail_.prev_node_ = other.tail_.prev_node_;
			size_ = other.size_;

			if (head_.next_node_ != &other.tail_)
			{
				head_.next_node_->prev_node_ = &head_;
				tail_.prev_node_->next_node_ = &tail_;
			}
			else
			{
				head_.next_node_ = &tail_;
				tail_.prev_node_ = &head_;
			}

			other.head_.next_node_ = &other.tail_;
			other.tail_.prev_node_ = &other.head_;
			other.size_ = 0;
		}
		return *this;
	}

	void push_back(const T& value) { insert(end(), value); }

	void push_front(const T& value) { insert(begin(), value); }

	void pop_back()
	{
		if (!empty())
		{
			erase(--end());
		}
	}

	void pop_front()
	{
		if (!empty())
		{
			erase(begin());
		}
	}

	iterator insert(iterator pos, const T& value)
	{
		node* new_node = new node(pos.current->prev_node_, value, pos.current);
		pos.current->prev_node_->next_node_ = new_node;
		pos.current->prev_node_ = new_node;
		++size_;
		return iterator(new_node);
	}

	iterator erase(iterator pos)
	{
		node* to_erase = pos.current;
		iterator ret(to_erase->next_node_);

		to_erase->prev_node_->next_node_ = to_erase->next_node_;
		to_erase->next_node_->prev_node_ = to_erase->prev_node_;

		delete to_erase;
		--size_;
		return ret;
	}

	void concat(list& other)
	{
		if (other.empty())
			return;

		node* last = tail_.prev_node_;
		node* first_other = other.head_.next_node_;
		node* last_other = other.tail_.prev_node_;

		last->next_node_ = first_other;
		first_other->prev_node_ = last;

		last_other->next_node_ = &tail_;
		tail_.prev_node_ = last_other;

		size_ += other.size_;

		other.head_.next_node_ = &other.tail_;
		other.tail_.prev_node_ = &other.head_;
		other.size_ = 0;
	}

	void clear()
	{
		while (!empty())
		{
			pop_front();
		}
	}

	void swap(list& other) noexcept
	{
		std::swap(head_.next_node_, other.head_.next_node_);
		std::swap(tail_.prev_node_, other.tail_.prev_node_);
		std::swap(size_, other.size_);

		if (head_.next_node_)
		{
			head_.next_node_->prev_node_ = &head_;
		}
		if (tail_.prev_node_)
		{
			tail_.prev_node_->next_node_ = &tail_;
		}
		if (other.head_.next_node_)
		{
			other.head_.next_node_->prev_node_ = &other.head_;
		}
		if (other.tail_.prev_node_)
		{
			other.tail_.prev_node_->next_node_ = &other.tail_;
		}
	}

	size_t size() const { return size_; }
	bool empty() const { return size_ == 0; }

	iterator begin() { return iterator(head_.next_node_); }
	iterator end() noexcept { return iterator(&tail_); }
	const_iterator begin() const { return const_iterator(head_.next_node_); }
	const_iterator end() const noexcept { return const_iterator(&tail_); }
	const_iterator cbegin() const { return const_iterator(head_.next_node_); }
	const_iterator cend() const noexcept { return const_iterator(&tail_); }

	T operator[](size_t pos) const
	{
		iterator it = begin();
		while (pos--)
		{
			++it;
		}
		return *it;
	}

	T& operator[](size_t pos)
	{
		iterator it = begin();
		while (pos--)
		{
			++it;
		}
		return *it;
	}

	friend bool operator==(const list& l, const list& r)
	{
		if (l.size() != r.size())
		{
			return false;
		}

		auto l_it = l.begin();
		auto r_it = r.begin();

		while (l_it != l.end())
		{
			if (*l_it != *r_it)
			{
				return false;
			}
			++l_it;
			++r_it;
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
			{
				return std::strong_ordering::less;
			}
			if (*it2 < *it1)
			{
				return std::strong_ordering::greater;
			}
			++it1;
			++it2;
		}
		return (it1 == lhs.end())
				   ? ((it2 == rhs.end()) ? std::strong_ordering::equal
										 : std::strong_ordering::less)
				   : std::strong_ordering::greater;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
		bool first = true;
		for (const auto& item : other)
		{
			if (!first)
			{
				os << ", ";
			}
			os << item;
			first = false;
		}
		os << "}";
		return os;
	}

   private:
	size_t size_ = 0;
	node head_;
	node tail_;

	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto it1 = l.begin();
		auto it2 = r.begin();
		while (it1 != l.end() && it2 != r.end())
		{
			if (*it1 < *it2)
			{
				return true;
			}
			if (*it2 < *it1)
			{
				return false;
			}
			++it1;
			++it2;
		}
		return (it1 == l.end()) && (it2 != r.end());
	}
};

template <typename T>
void swap(list<T>& lhs, list<T>& rhs) noexcept
{
	lhs.swap(rhs);
}

}  // namespace bmstu
