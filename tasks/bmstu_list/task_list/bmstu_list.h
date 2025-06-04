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
			: next_node_(next), prev_node_(prev), value_(value)
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
		iterator& operator++() override { 
			current = current->next_node_;
			return *this;
		}
		iterator& operator--() override { 
			current = current->prev_node_;
			return *this;
		}
		iterator operator++(int) override { 
			iterator copy = *this;
			current = current->next_node_;
			return copy;
		}
		iterator operator--(int) override { 
			iterator copy = *this;
			current = current->prev_node_;
			return copy; 
		}
		iterator& operator+=(const typename iterator::difference_type& n) override
		{
			for (auto it = 0; it < n; ++it) {
				current = current->next_node_;
			}
			return *this;
		}
		iterator& operator-=(const typename iterator::difference_type& n) override
		{
			for (auto it = 0; it < n; ++it) {
				current = current->prev_node_;
			}
			return *this;
		}
		iterator operator+(const typename iterator::difference_type& n) const override
		{
			iterator copy = *this;
			copy += n;
			return copy;
		}
		iterator operator-(const typename iterator::difference_type& n) const override
		{
			iterator copy = *this;
			copy -= n;
			return copy;
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
			auto count = 0;
			iterator copy_other(other);
			while (current != copy_other.current) {
				++count;
				copy_other = copy_other.current->next_node_;
			}
			return count;
		}
	};
	using const_iterator = iterator;

	list() : head_(new node()), tail_(new node()), size_(0) {
		head_->next_node_ = tail_;
		tail_->prev_node_ = head_;
	}

	template <typename it>
	list(it begin, it end) : list()
	{
		for (it iter = begin; iter != end; ++iter) {
			push_back(*iter);
		}
	}

	list(std::initializer_list<T> values) : list()
	{
		for (auto it = values.begin(); it != values.end(); ++it) {
			push_back(*it);
		}
	}

	list(const list& other) 
	{
		for (auto it = other.begin(); it != other.end(); ++it) {
			push_back(*it);
		}
	}

	list(list&& other) 
	{
		swap(other);
	}

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

	bool empty() const noexcept
	{
		return (size_ == 0u);
	}

	~list() {
		clear();
		delete head_;
		delete tail_;
	}

	void clear() {
		if (!empty()) {
			while (head_->next_node_ != tail_) {
				node* next = head_->next_node_;
				head_->next_node_ = next->next_node_;
				delete next;
			}
			size_ = 0;
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

	iterator begin() noexcept
	{
		return iterator{head_->next_node_};
	}

	iterator end() noexcept
	{
		return iterator{tail_};
	}

	const_iterator begin() const noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator end() const noexcept
	{
		return const_iterator{tail_};
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator cend() const noexcept
	{
		return const_iterator{tail_};
	}

#pragma endregion

	T operator[](size_t pos) const {
		return *(begin() + pos);
	}

	T& operator[](size_t pos) {
		return *(begin() + pos);
	}

	friend bool operator==(const list& l, const list& r) { 
		if (l.size_ == r.size_) {
			bool equal = true;
			for (size_t it = 0; it < l.size_; ++it) {
				if (l[it] != r[it]) {
					equal = false;
				}
			}
			return equal;
		}
		return false;
	}

	friend bool operator!=(const list& l, const list& r) { return !(l == r); }

	friend auto operator<=>(const list& lhs, const list& rhs) 
	{ 
		if (lhs == rhs) {
			return std::strong_ordering::equal;
		}
		if (lexicographical_compare_(lhs, rhs)) {
			return std::strong_ordering::less;
		}
		return std::strong_ordering::greater;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
		for (size_t it = 0; it != other.size_; ++it) {
			if (it == other.size_ - 1) {
				os << other[it];
				break;
			}
			os << other[it] << ", ";
		}
		os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		node* old_last_ = pos.current->prev_node_;
		node* new_last_ = new node(pos.current->prev_node_, value, pos.current);
		pos.current->next_node_ = new_last_;
		old_last_->next_node_ = new_last_;
		++size_;
		return iterator{nullptr};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto it1 = l.begin(), it2 = r.begin();
		for (; (it1 != l.end()) && (it2 != r.end()); ++it1, ++it2) {
			if (*it1 < *it2) {
				return true;
			}
			if (*it1 > *it2) {
				return false;
			}
		}
		return (it1 == l.end()) && (it2 == r.end());
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
}  // namespace bmstu