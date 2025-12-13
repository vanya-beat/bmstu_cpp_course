#pragma once
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>

namespace bmstu {
struct nullopt_t {
  constexpr explicit nullopt_t(int) {}
};
inline constexpr nullopt_t nullopt{0};

class bad_optional_access : public std::exception {
 public:
  const char* what() const noexcept override { return "Bad optional access"; }
};

template <typename T>
class optional {
 public:
  optional() noexcept : is_initialized_(false) {}

  optional(const T& value) : is_initialized_(true) {
    new (data_) T(value);
  }

  optional(T&& value) noexcept : is_initialized_(true) {
    new (data_) T(std::move(value));
  }

  optional(const optional& other) : is_initialized_(other.is_initialized_) {
    if (other.is_initialized_) {
      new (data_) T(*other);
    }
  }

  optional(optional&& other) noexcept : is_initialized_(other.is_initialized_) {
    if (other.is_initialized_) {
      new (data_) T(std::move(*other));
    }
  }

  optional& operator=(const T& value) {
    if (is_initialized_) {
      **this = value;
    } else {
      new (data_) T(value);
      is_initialized_ = true;
    }
    return *this;
  }

  optional& operator=(T&& value) {
    if (is_initialized_) {
      **this = std::move(value);
    } else {
      new (data_) T(std::move(value));
      is_initialized_ = true;
    }
    return *this;
  }

  optional& operator=(const optional& other) {
    if (this != &other) {
      if (other.is_initialized_) {
        if (is_initialized_) {
          **this = *other;
        } else {
          new (data_) T(*other);
          is_initialized_ = true;
        }
      } else {
        reset();
      }
    }
    return *this;
  }

  optional& operator=(optional&& other) noexcept {
    if (this != &other) {
      if (other.is_initialized_) {
        if (is_initialized_) {
          **this = std::move(*other);
        } else {
          new (data_) T(std::move(*other));
          is_initialized_ = true;
        }
      } else {
        reset();
      }
    }
    return *this;
  }

  T& operator*() & { return *reinterpret_cast<T*>(data_); }

  const T& operator*() const& { return *reinterpret_cast<const T*>(data_); }

  T* operator->() { return reinterpret_cast<T*>(data_); }

  const T* operator->() const { return reinterpret_cast<const T*>(data_); }

  T&& operator*() && { return std::move(*reinterpret_cast<T*>(data_)); }

  T& value() & {
    if (!is_initialized_) {
      throw bad_optional_access();
    }
    return **this;
  }

  const T& value() const& {
    if (!is_initialized_) {
      throw bad_optional_access();
    }
    return **this;
  }

  template <typename... Args>
  void emplace(Args&&... args) {
    reset();
    new (data_) T(std::forward<Args>(args)...);
    is_initialized_ = true;
  }

  void reset() {
    if (is_initialized_) {
      reinterpret_cast<T*>(data_)->~T();
      is_initialized_ = false;
    }
  }

  ~optional() { reset(); }

  bool has_value() const noexcept { return is_initialized_; }

 private:
  alignas(T) uint8_t data_[sizeof(T)];
  bool is_initialized_ = false;
};
}  // namespace bmstu1