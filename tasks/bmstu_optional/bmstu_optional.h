#ifndef BMSTU_OPTIONAL_H
#define BMSTU_OPTIONAL_H

#include <cstdint>
#include <cstring>
#include <iterator>
#include <ostream>
#include <utility>
#include <exception>
#include <type_traits>

namespace bmstu
{
struct nullopt_t
{
    explicit constexpr nullopt_t(int) noexcept {}
};

class bad_optional_access : public std::exception {
public:
    const char* what() const noexcept override { return "Bad optional access"; }
};

template <typename T>
class optional 
{
public:
    optional() = default;
    
    optional(T&& value) {
        is_initialized_ = true;
        new (&data_[0]) T(std::move(value));
    }
    
    optional(const T& value) {
        is_initialized_ = true;
        new (&data_[0]) T(value);
    }
    
    optional(const optional& other) {
        if (other.is_initialized_) {
            new (&data_[0]) T(other.value());
            is_initialized_ = true;
        }
    }
    
    optional(optional&& other) noexcept {
        if (other.is_initialized_) {
            new (&data_[0]) T(std::move(other.value()));
            is_initialized_ = true;
            other.reset();
        }
    }
    
    optional& operator=(const optional& other) {
        if (this != &other) {
            if (is_initialized_) {
                reset();
            }
            if (other.is_initialized_) {
                new (&data_[0]) T(other.value());
                is_initialized_ = true;
            }
        }
        return *this;
    }
    
    bool has_value() const { return is_initialized_; }
    
    void reset() {
        if (is_initialized_) {
            value().~T();
            is_initialized_ = false;
        }  
    }

    T& value() & {
        if (!is_initialized_) {
            throw bad_optional_access();
        }
        return *reinterpret_cast<T*>(&data_[0]);
    }
    
    const T& value() const& {
        if (!is_initialized_) {
            throw bad_optional_access();
        }
        return *reinterpret_cast<const T*>(&data_[0]);
    }
    
    T&& value() && {
        if (!is_initialized_) {
            throw bad_optional_access();
        }
        return std::move(*reinterpret_cast<T*>(&data_[0]));
    }

    ~optional() { reset(); }

    T& operator*() & { 
        return value();
    }

    const T& operator*() const& {
        return value();
    }

    T&& operator*() && {
        return std::move(value());
    }

    T* operator->() { 
        return &value();
    }
    
    const T* operator->() const {
        return &value();
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (is_initialized_) {
            reset();
        }
        new (&data_[0]) T(std::forward<Args>(args)...);
        is_initialized_ = true;
    }

private:
    alignas(T) uint8_t data_[sizeof(T)];
    bool is_initialized_ = false;
};

} // namespace bmstu

#endif // BMSTU_OPTIONAL_H