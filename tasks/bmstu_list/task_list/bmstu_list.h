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
   		node* current;

		iterator() : current(nullptr) {}
		iterator(node* node) : current(node) {}

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


        using difference_type = abstract_iterator<iterator,
			T, std::bidirectional_iterator_tag>::difference_type;
		iterator& operator+=(const difference_type& n) override
		{
			for (difference_type i = 0; i < n; ++i)
				++(*this);
			return *this;
		}

		iterator& operator-=(const difference_type& n) override
		{
			for (difference_type i = 0; i < n; ++i)
					--(*this);
			return *this;
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

        using reference = abstract_iterator<iterator,
			T, std::bidirectional_iterator_tag>::reference;
		reference operator*() const override 
        { 
            return current->value_; 
        }

        using pointer = abstract_iterator<iterator,
			T, std::bidirectional_iterator_tag>::pointer; 
		pointer operator->() const override 
        { 
            return &current->value_; 
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

		difference_type operator-(
            const iterator& other) const override
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
	list(it begin, it end)  
    : size_(0), tail_(new node()), head_(new node()) 
    {
        tail_->prev_node_ = head_;
        head_->next_node_ = tail_;
        for (auto i = begin; i != end; ++i) 
            push_back(*i);
    }

	list(std::initializer_list<T> values) 
    : size_(0), tail_(new node()), head_(new node()) 
    {
        tail_->prev_node_ = head_;
        head_->next_node_ = tail_;
        for (auto &i : values)
            push_back(i);
    }    

	list(const list& other) 
    : size_(0), tail_(new node()), head_(new node())
	{
		for (auto& i : other)
		{
			push_back(i);
		}
	}

	list(list&& other) noexcept 
    { 
        swap(other); 
    }

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
        return size_ == 0; 
    }

	~list()
	{
		clear();
		delete head_;
		delete tail_;
	}

	void clear()
    {
	    if (!empty()) 
        {
            while (head_->next_node_ != tail_) {
                node *tmp = head_->next_node_;
                head_->next_node_ = tmp->next_node_;
                delete tmp;
            }
            size_ = 0;
        }
    }

	size_t size() const 
    { 
        return size_; 
    }
	void pop_back() {  
    	if (!empty()) {
        	node* to_delete = head_->next_node_;  
        	node* new_first = to_delete->next_node_;  
        	head_->next_node_ = new_first;
        	new_first->prev_node_ = head_;
        	delete to_delete;
        	--size_;
    	}
	}
	
	void swap(list& other) noexcept
	{
		std::swap(head_, other.head_);
		std::swap(tail_, other.tail_);
		std::swap(size_, other.size_);
	}

	friend void swap(list& l, list& r) 
    { 
        l.swap(r); 
    }

#pragma region iterators

	iterator begin() noexcept 
    { 
        return iterator(head_->next_node_); 
    }

	iterator end() noexcept 
    { 
        return iterator(tail_); 
    }

	const_iterator begin() const noexcept
	{
		return const_iterator(head_->next_node_);
	}

	const_iterator end() const noexcept 
    { 
        return const_iterator(tail_); 
    }

	const_iterator cbegin() const noexcept
	{
		return const_iterator(head_->next_node_);
	}

	const_iterator cend() const noexcept 
    { 
        return const_iterator(tail_); 
    }

#pragma endregion

	T operator[](size_t pos) const
	{
		iterator rr = begin();
		while (pos--)
			++rr;
		return *rr;
	}

	T& operator[](size_t pos)
	{
		iterator rr = begin();
		while (pos--)
			++rr;
		return *rr;
	}

	friend bool operator==(const list& l, const list& r)
	{
		if (l.size_ != r.size_)
			return false;
		for (auto l_it = l.begin(), r_it = r.begin(); 
            l_it != l.end(); ++l_it, ++r_it) 
            {
                if (*l_it != *r_it) 
                    return false;
            }
		return true;
	}

	friend bool operator!=(const list& l, const list& r) 
    { 
        return !(l == r); 
    }

	friend auto operator<=>(const list& lhs, const list& rhs)
	{
		return auto (lhs <=> rhs);
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
        for (size_t i = 0; i < other.size(); ++i) 
        {
            os << other[i];
            if (i < other.size() - 1) 
                    os << ", ";
        }
        os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		node *current = head_->next_node_;
        for (size_t i = 0; i < (pos - begin()); ++i) 
            current = current->next_node_;
        node *new_node = new node(current->prev_node_, value, current);
        current->prev_node_->next_node_ = new_node;
        current->prev_node_ = new_node;
        ++size_;
        return pos;
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto lb = l.begin();
        auto rb = r.begin();
        auto le = l.end();
        auto re = r.end();
        for (; (lb != le) && (rb != re); ++lb, ++rb) 
        {
            if (*lb < *rb) 
                return true;
            if (*lb > *rb) 
                return false;
        }
        return (lb == le) && (rb != re);
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
}  // namespace bmstu