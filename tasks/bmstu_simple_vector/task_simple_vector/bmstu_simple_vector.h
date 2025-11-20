#ifndef VECTOR_H
#define VECTOR_H
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <ostream>
#include <compare>
#include "array_ptr.h"
namespace bmstu {
    template <typename T>
    class simple_vector {
    private:
        T* _data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    public:
        void clear() noexcept {
            for (size_t i = 0; i < _size; i++) {
                _data[i].~T();
            }
            _size = 0;
        }
        ~simple_vector() noexcept{
            clear();
            delete[] reinterpret_cast<char*>(_data);
        }
        simple_vector() =default;
        simple_vector(size_t size,const T& value = T()) {
            resize(size,value);
        }
        simple_vector(std::initializer_list<T> init) {
            reserve(init.size());
            for (auto& el : init) {
                push_back(el);
            }
        }
        simple_vector(const simple_vector& other) {
            reserve(other._size);
            for (int i = 0; i < other._size; i++) {
                push_back(other[i]);
            }
        }
        simple_vector(simple_vector&& other) noexcept {
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            other._data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        void swap(simple_vector& other) noexcept {
            std::swap(_data, other._data);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
        }

        simple_vector& operator=(const simple_vector& other) {
            if (this != &other) {
                simple_vector temp = other;
                swap(temp);
            }
            return *this;
        }

        simple_vector& operator=(simple_vector&& other)  noexcept {
            if (this != &other) {
                simple_vector temp = std::move(other);
                swap(temp);
            }
            return *this;
        }
        void reserve(size_t capacity) {
            if (_capacity < capacity) {
                T* tmp = reinterpret_cast<T*>(new char[capacity * sizeof(T)]);
                size_t copied = 0;
                try {
                    if (_capacity != 0) {
                        for (size_t i = 0; i < _size; i++) {
                            new(tmp + i) T(std::move_if_noexcept(_data[i]));
                            copied++;
                            _data[i].~T();
                        }
                        delete[] reinterpret_cast<char*>(_data);
                    }
                    _data = tmp;
                    _capacity = capacity;
                } catch (...) {
                    for (size_t i = 0; i < copied; i++) {
                        tmp[i].~T();
                    }
                    delete[] reinterpret_cast<char*>(tmp);
                    throw;
                }
            }
        }
        void resize(size_t size, T value = T()) {
            if (size > _capacity) {
                reserve(size);
            }
            if (size > _size) {
                for (size_t i = _size; i < size; i++) {
                    new(_data + i) T(value);
                }
            } else {
                for (size_t i = size; i < _size; i++) {
                    _data[i].~T();
                }
            }
            _size = size;
        }
        
       void push_back(T value) {
            if (_size == _capacity) {
                if (_capacity == 0) {
                    reserve(1);
                }else{
                    reserve(_capacity * 2);
                }
            }
                T temp2 = value;
            new(_data+_size) T(std::move(value));
            _size++;
       }
       
        template <typename... Args>
        void emplace_back(Args... args) {
            if (_size == _capacity) {
                if (_capacity == 0) {
                    reserve(2);
                }else{
                    reserve(_capacity * 2);
                }
            }
            new(_data+_size) T(std::move(args...));
            _size++;
        }
        void pop_back() {
            erase(end()-1);
        }
        T& operator[](size_t index) noexcept{
            return _data[index];
        }
        T operator[](size_t index) const noexcept{
            return _data[index];
        }
        T& at(size_t index)  {
            if (index >= _size) {
                throw std::out_of_range("Index out of range");
            }
            return _data[index];
        }
        T at(size_t index) const{
            if (index >= _size) {
                throw std::out_of_range("Index out of range");
            }
            return _data[index];
        }
        size_t size() const noexcept{
            return _size;
        }
        size_t capacity() const noexcept{
            return _capacity;
        }
        bool empty() const noexcept {
            return _size == 0;
        }
        class iterator {
            T* el;
        public:
            using iterator_category = std::contiguous_iterator_tag;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using difference_type = std::ptrdiff_t;
            iterator(T* ptr) noexcept : el(ptr) {
            }
            iterator operator++() {
                return ++el;
            }
            iterator operator--() {
                return --el;
            }
            iterator operator+(int index)const {
                return iterator(el + index);
            }
            difference_type operator-(iterator other)const {
                return el - other.el;
            }
            iterator operator-(int index) const {
                return iterator(el - index);
            }
            reference operator->() {
                return el;
            }
            bool operator==(const iterator& other) const {
                return el == other.el;
            }
            bool operator!=(const iterator& other) const {
                return el != other.el;
            }
            reference operator*() noexcept {
                return *el;
            }
            bool operator<(const iterator& other) {
                return el < other.el;
            }
            bool operator>(const iterator& other) {
                return el > other.el;
            }
        };
        iterator begin() noexcept {
            return iterator(_data);
        }
        iterator end() noexcept {
            return iterator(_data+_size);
        }
        void erase(iterator it) noexcept {
            auto pos = it - begin();
            for (size_t i = pos; i < size()-1; i++) {
                _data[i] = std::move(_data[i+1]);
            }
            _size--;
        }
        void insert(iterator it, T value) {
            auto pos = it - begin();
            if (_size+1 > _capacity) {
                reserve((_size+1)*2);
            }

            push_back(std::move(_data[size()-1]));
            for (size_t i = size()-1; i > pos; i--) {
                _data[i] = std::move(_data[i-1]);
            }
            _data[pos] = std::move(value);
        }
        bool operator==(const simple_vector& other) const {
            if (_size != other._size) {
                return false;
            }
            return this->operator<=>(other) == 0;
        }
        bool operator!=(const simple_vector& other) const {
            return !(*this == other);
        }
        auto operator<=>(const simple_vector& other) const {
            for (size_t i = 0; i < std::min(this->size(),other.size()); i++) {
                if (_data[i] < other._data[i]) {
                    return -1;
                }
                if (_data[i] > other._data[i]) {
                    return 1;
                }
            }

            if (this->size() > other.size()) {
                return 1;
            }
            if (this->size() < other.size()) {
                return -1;
            }

            return 0;
        }
        friend std::ostream& operator<<(std::ostream& os, const simple_vector& vec)
        {
            os << "{ ";
            for (size_t i = 0; i < vec.size()-1; i++) {
                os << vec[i] << ", ";
            }
            os << vec[vec.size()-1] << " }";
            return os;
        }
    };
};

#endif //VECTOR_H
