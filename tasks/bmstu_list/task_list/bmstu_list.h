#include <cstddef>
#include <iterator>
#include <ostream>
#include "abstract_iterator.h"

namespace bmstu {

template <typename T>
class list {
    struct node {
        node() = default;
        
        node(node* prev, const T& value, node* next)
            : value_(value), next_node_(next), prev_node_(prev) {}
        
        T value_;
        node* next_node_ = nullptr;
        node* prev_node_ = nullptr;
    };

public:
    struct iterator : public abstract_iterator<iterator, T, std::bidirectional_iterator_tag> {
        using typename abstract_iterator<iterator, T, std::bidirectional_iterator_tag>::difference_type;
        using typename abstract_iterator<iterator, T, std::bidirectional_iterator_tag>::reference;
        using typename abstract_iterator<iterator, T, std::bidirectional_iterator_tag>::pointer;
        
        node* current;
        
        iterator() noexcept : current(nullptr) {}
        explicit iterator(node* node_ptr) noexcept : current(node_ptr) {}
        
        iterator& operator++() override {
            if (current) {
                current = current->next_node_;
            }
            return *this;
        }
        
        iterator operator++(int) override {
            iterator old_value = *this;
            ++(*this);
            return old_value;
        }
        
        iterator& operator--() override {
            if (current) {
                current = current->prev_node_;
            }
            return *this;
        }
        
        iterator operator--(int) override {
            iterator old_value = *this;
            --(*this);
            return old_value;
        }
        
        reference operator*() const override {
            return current->value_;
        }
        
        pointer operator->() const override {
            return &(current->value_);
        }
        
        bool operator==(const iterator& other) const override {
            return current == other.current;
        }
        
        bool operator!=(const iterator& other) const override {
            return !(*this == other);
        }
        
        explicit operator bool() const override {
            return current != nullptr;
        }
        
        iterator& operator+=(const difference_type& n) override {
            if (n > 0) {
                difference_type count = n;
                while (count-- > 0 && current) {
                    current = current->next_node_;
                }
            } else if (n < 0) {
                difference_type count = -n;
                while (count-- > 0 && current) {
                    current = current->prev_node_;
                }
            }
            return *this;
        }
        
        iterator& operator-=(const difference_type& n) override {
            return *this += -n;
        }
        
        iterator operator+(const difference_type& n) const override {
            iterator result = *this;
            result += n;
            return result;
        }
        
        iterator operator-(const difference_type& n) const override {
            iterator result = *this;
            result -= n;
            return result;
        }
        
        difference_type operator-(const iterator& other) const override {
            difference_type distance = 0;
            iterator it = *this;
            
            while (it.current && it != other) {
                --it;
                ++distance;
            }
            
            if (it == other) {
                return distance;
            }
            
            it = *this;
            distance = 0;
            while (it.current && it != other) {
                ++it;
                --distance;
            }
            
            return distance;
        }
    };
    
    using const_iterator = iterator;

    list() noexcept {
        head_.next_node_ = &tail_;
        tail_.prev_node_ = &head_;
    }

    list(std::initializer_list<T> init) : list() {
        for (const auto& value : init) {
            push_back(value);
        }
    }

    template <typename InputIt>
    list(InputIt first, InputIt last) : list() {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    list(const list& other) : list() {
        for (const auto& value : other) {
            push_back(value);
        }
    }

    list(list&& other) noexcept : list() {
        swap(other);
    }

    ~list() {
        clear();
    }

    list& operator=(const list& other) {
        if (this != &other) {
            list temp(other);
            swap(temp);
        }
        return *this;
    }

    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    iterator begin() noexcept { 
        return iterator(head_.next_node_); 
    }
    
    iterator end() noexcept { 
        return iterator(&tail_); 
    }
    
    const_iterator begin() const noexcept { 
        return const_iterator(head_.next_node_); 
    }
    
    const_iterator end() const noexcept { 
        return const_iterator(const_cast<node*>(&tail_)); 
    }
    
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    T& front() {
        return head_.next_node_->value_;
    }

    const T& front() const {
        return head_.next_node_->value_;
    }

    T& back() {
        return tail_.prev_node_->value_;
    }

    const T& back() const {
        return tail_.prev_node_->value_;
    }

    void push_back(const T& value) { 
        insert(end(), value); 
    }
    
    void push_front(const T& value) { 
        insert(begin(), value); 
    }
    
    void pop_back() { 
        erase(--end()); 
    }
    
    void pop_front() { 
        erase(begin()); 
    }

    iterator insert(const_iterator pos, const T& value) {
        node* new_node = new node(pos.current->prev_node_, value, pos.current);
        pos.current->prev_node_->next_node_ = new_node;
        pos.current->prev_node_ = new_node;
        ++size_;
        return iterator(new_node);
    }

    iterator erase(const_iterator pos) {
        node* next_node = pos.current->next_node_;
        pos.current->prev_node_->next_node_ = next_node;
        next_node->prev_node_ = pos.current->prev_node_;
        delete pos.current;
        --size_;
        return iterator(next_node);
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    void swap(list& other) noexcept {
        std::swap(head_.next_node_, other.head_.next_node_);
        std::swap(tail_.prev_node_, other.tail_.prev_node_);
        std::swap(size_, other.size_);
        
        if (head_.next_node_) head_.next_node_->prev_node_ = &head_;
        if (tail_.prev_node_) tail_.prev_node_->next_node_ = &tail_;
        if (other.head_.next_node_) other.head_.next_node_->prev_node_ = &other.head_;
        if (other.tail_.prev_node_) other.tail_.prev_node_->next_node_ = &other.tail_;
    }

    T& operator[](size_t pos) {
        iterator it = begin();
        for (size_t i = 0; i < pos; ++i) {
            ++it;
        }
        return *it;
    }

    const T& operator[](size_t pos) const {
        const_iterator it = begin();
        for (size_t i = 0; i < pos; ++i) {
            ++it;
        }
        return *it;
    }

    friend void swap(list<T>& lhs, list<T>& rhs) noexcept {
        lhs.swap(rhs);
    }

    friend bool operator==(const list<T>& lhs, const list<T>& rhs) {
        return lhs.size() == rhs.size() && 
               std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const list<T>& lhs, const list<T>& rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const list<T>& lst) {
        os << "{";
        bool first = true;
        for (const auto& item : lst) {
            if (!first) {
                os << ", ";
            }
            os << item;
            first = false;
        }
        os << "}";
        return os;
    }

private:
    node head_;
    node tail_;
    size_t size_ = 0;
};

} // namespace bmstu