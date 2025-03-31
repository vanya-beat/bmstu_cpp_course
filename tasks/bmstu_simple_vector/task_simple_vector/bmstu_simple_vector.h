#pragma once

#include <ostream>
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

              iterator& operator++() 
              {
                  ptr_ += 1;
                  return *this;
              }

              iterator& operator--() { return *this; }

              iterator operator++(int) 
              {
                  ptr_ += 1;
                  return *this;
              }

              iterator operator--(int) 
              {
                  --ptr_;
                  return *this;
              }

              explicit operator bool() const { return ptr_ != nullptr; }

              friend bool operator==(const iterator& lhs, const iterator& rhs) 
              {
                  return lhs.ptr_ == rhs.ptr_;
              }

              friend bool operator==(const iterator& lhs, std::nullptr_t) 
              {
                  return lhs.ptr_ == nullptr;
              }

              iterator& operator=(std::nullptr_t) noexcept 
              {
                  ptr_ = nullptr;
                  return *this;
              }

              friend bool operator==(std::nullptr_t, const iterator& rhs) 
              {
                  return rhs.ptr_ == nullptr;
              }

              friend bool operator!=(const iterator& lhs, const iterator& rhs) 
              {
                  return lhs.ptr_ != rhs.ptr_;
              }

              iterator operator+(const difference_type& n) const noexcept 
              {
                  iterator tmp(*this);
                  tmp.ptr_ += n;
                  return tmp;
              }

              iterator operator+=(const difference_type& n) noexcept 
              {
                  ptr_ += n;
                  return *this;
              }

              iterator operator-(const difference_type& n) const noexcept 
              {
                  iterator tmp(*this);
                  tmp.ptr_ -= n;
                  return tmp;
              }

              iterator operator-=(const difference_type& n) noexcept 
              {
                  ptr_ -= n;
                  return *this;
              }

              friend difference_type operator-(const iterator& end, const iterator& begin) noexcept 
              {
                  return end.ptr_ - begin.ptr_;
              }

              private:
              pointer ptr_ = nullptr;
};

  simple_vector() noexcept = default;
  ~simple_vector() = default;

  simple_vector(std::initializer_list<T> init) noexcept {
    data_ = array_ptr<T>(init.size());
    size_ = init.size();
    capacity_ = init.size();
    auto src_it = init.begin();
    size_t index = 0;
    while (src_it != init.end()) {
      data_[index] = *src_it;
      ++src_it;
      ++index;
    }
  }

  simple_vector(const simple_vector& other) {
    data_ = array_ptr<T>(other.size());
    size_ = other.size();
    capacity_ = other.size();
    size_t index = 0;
    for (auto src_it = other.begin(); src_it != other.end(); ++src_it, ++index) {
      data_[index] = *src_it;
    }
  }

  simple_vector(simple_vector&& other) noexcept { swap(other); }

  simple_vector& operator=(const simple_vector& other) {
    if (this != &other) {
      simple_vector temp(other);
      swap(temp);
    }
    return *this;
  }

  explicit simple_vector(size_t size, const T& value = T{}) {
    size_ = size;
    capacity_ = size;
    data_ = array_ptr<T>(size);
    std::fill(data_.get(), data_.get() + size, value);
  }

  iterator begin() noexcept { return iterator(data_.get()); }
  iterator end() noexcept { return iterator(data_.get() + size_); }

  using const_iterator = iterator;
  const_iterator begin() const noexcept { return iterator(data_.get()); }
  const_iterator end() const noexcept { return iterator(data_.get() + size_); }

  typename iterator::reference operator[](size_t index) noexcept {
    return data_[index];
  }

  typename const_iterator::reference operator[](size_t index) const noexcept {
    return data_[index];
  }

  typename iterator::reference at(size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return data_[index];
  }

  typename const_iterator::reference at(size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return data_[index];
  }

  [[nodiscard]] size_t size() const noexcept { return size_; }
  [[nodiscard]] size_t capacity() const noexcept { return capacity_; }

  void swap(simple_vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  friend void swap(simple_vector& lhs, simple_vector& rhs) noexcept {
    lhs.swap(rhs);
  }

  void reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
      array_ptr<T> new_data(new_capacity);
      std::copy(begin(), end(), iterator(new_data.get()));
      data_.swap(new_data);
      capacity_ = new_capacity;
    }
  }

  void resize(size_t new_size) {
    if (new_size > capacity_) {
      reserve(new_size);
    }
    if (new_size > size_) {
      std::fill(data_.get() + size_, data_.get() + new_size, T{});
    }
    size_ = new_size;
  }

  iterator insert(const_iterator pos, T&& value) {
    size_t insert_pos = pos - begin();
    if (size_ == capacity_) {
      reserve(capacity_ ? capacity_ * 2 : 1);
    }
    std::move_backward(begin() + insert_pos, end(), end() + 1);
    data_[insert_pos] = std::move(value);
    ++size_;
    return begin() + insert_pos;
  }

  iterator insert(const_iterator pos, const T& value) {
    return insert(pos, T(value));
  }

  void push_back(T&& value) { insert(end(), std::move(value)); }
  void clear() noexcept { size_ = 0; }
  void push_back(const T& value) { insert(end(), value); }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

  void pop_back() {
    if (size_ > 0) {
      --size_;
    }
  }

  friend bool operator==(const simple_vector& lhs, const simple_vector& rhs) {
    if (lhs.size_ != rhs.size_) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  friend bool operator!=(const simple_vector& lhs, const simple_vector& rhs) {
    return !(lhs == rhs);
  }

  friend auto operator<=>(const simple_vector& lhs, const simple_vector& rhs) {
    return std::lexicographical_compare_three_way(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
  }

  friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size_; ++i) {
      if (i != 0) os << ", ";
      os << vec.data_[i];
    }
    os << "]";
    return os;
  }

  iterator erase(iterator pos) {
    if (pos == end()) return pos;
    std::move(pos + 1, end(), pos);
    --size_;
    return pos;
  }

 private:
  static bool alphabet_compare(const simple_vector<T>& lhs,
                              const simple_vector<T>& rhs) {
    auto lhs_it = lhs.begin(), rhs_it = rhs.begin();
    auto lhs_end = lhs.end(), rhs_end = rhs.end();

    for (; (lhs_it != lhs_end) && (rhs_it != rhs_end); ++lhs_it, ++rhs_it) {
      if (*lhs_it < *rhs_it) return true;
      if (*rhs_it < *lhs_it) return false;
    }

    return (lhs_it == lhs_end) && (rhs_it != rhs_end);
  }

  array_ptr<T> data_;
  size_t size_ = 0;
  size_t capacity_ = 0;
};
}  // namespace bmstu
