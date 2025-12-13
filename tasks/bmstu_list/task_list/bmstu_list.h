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
			: next_node_(nullptr), prev_node_(nullptr)
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
		iterator& operator++() override { return *this; }
		iterator& operator--() override { return *this; }
		iterator operator++(int) override { return nullptr; }
		iterator operator--(int) override { return nullptr; }
		iterator& operator+=(
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			return *this;
		}
		iterator& operator-=(
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			return *this;
		}
		iterator operator+(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const override
		{
			return nullptr;
		}
		iterator operator-(const typename abstract_iterator<
						   iterator,
						   T,
						   std::bidirectional_iterator_tag>::difference_type& n)
			const override
		{
			return nullptr;
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
			return current != other.current;
		}
		explicit operator bool() const override { return current != nullptr; }
		typename abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::difference_type
		operator-(const iterator& other) const override
		{
			return 0;
		}
	};
	using const_iterator = iterator;

	list() {}

	template <typename it>
	list(it begin, it end)
	{
	}

	list(std::initializer_list<T> values) {}

	list(const list& other) {}

	list(list&& other) {}

#pragma endregion
#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_last = new node(tail_->prev_node_, value, tail_);
		tail_->prev_node_ = new_last;
		last->next_node_ = new_last;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		// адрес реального последнего элемента
		node* first = head_->next_node_;
		node* new_first = new node(head_, value, first);
		head_->next_node_ = new_first;
		first->prev_node_ = new_first;
		++size_;
	}

#pragma endregion

	bool empty() const

		noexcept
	{
		return (size_ == 0u);
	}

	~list() {}

	void clear() {}

	size_t size() const { return 0; }

	void swap(list& other)

		noexcept
	{
	}

	friend void swap(list& l, list& r) { l.swap(r); }

#pragma region iterators

	iterator begin()

		noexcept
	{
		return iterator{head_->next_node_};
	}

	iterator end()

		noexcept
	{
		return iterator{tail_};
	}

	const_iterator begin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator end() const

		noexcept
	{
		return const_iterator{tail_};
	}

	const_iterator cbegin() const

		noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator cend() const

		noexcept
	{
		return const_iterator{tail_};
	}

#pragma endregion

	T operator[](size_t pos) const {}

	T& operator[](size_t pos) { return *(static_cast<T*>((void*)&pos)); }

	friend bool operator==(const list& l, const list& r) { return true; }

	friend bool operator!=(const list& l, const list& r) { return false; }

	friend auto operator<=>(const list& lhs, const list& rhs) { return true; }

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		return iterator{nullptr};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		return "123";
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
}  // namespace bmstu