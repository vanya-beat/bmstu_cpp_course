#ifndef BMSTU_OPTIONAL_H
#define BMSTU_OPTIONAL_H

#include <cstdint>
#include <utility>
#include <exception>
#include <cstdint>


namespace bmstu {
struct nullopt_t {
    explicit constexpr nullopt_t(int) noexcept {}
};

constexpr nullopt_t nullopt{0};

class bad_optional_access : public std::exception {
   public:
    const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class optional {
   public:
    constexpr optional() noexcept = default;

    constexpr optional(nullopt_t) noexcept {}

    optional(const T& value) : is_initialized_(true) {
        new (&data_) T(value);
    }

    optional(T&& value) : is_initialized_(true) {
        new (&data_) T(std::move(value));
    }

    optional(const optional& other) : is_initialized_(other.is_initialized_) {
        if (other.is_initialized_) {
            new (&data_) T(other.get_value());
        }
    }

    optional(optional&& other) noexcept : is_initialized_(other.is_initialized_) {
        if (other.is_initialized_) {
            new (&data_) T(std::move(other.get_value()));
        }
    }

    optional& operator=(nullopt_t) noexcept {
        reset();
        return *this;
    }

    optional& operator=(const T& value) {
        if (is_initialized_) {
            get_value() = value;
        } else {
            new (&data_) T(value);
            is_initialized_ = true;
        }
        return *this;
    }

    optional& operator=(T&& value) {
        if (is_initialized_) {
            get_value() = std::move(value);
        } else {
            new (&data_) T(std::move(value));
            is_initialized_ = true;
        }
        return *this;
    }

    optional& operator=(const optional& other) {
        if (this != &other) {
            if (other.is_initialized_) {
                if (is_initialized_) {
                    get_value() = other.get_value();
                } else {
                    new (&data_) T(other.get_value());
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
                    get_value() = std::move(other.get_value());
                } else {
                    new (&data_) T(std::move(other.get_value()));
                    is_initialized_ = true;
                }
            } else {
                reset();
            }
        }
        return *this;
    }

    ~optional() {
        reset();
    }

    T& value() & {
        check_initialized();
        return get_value();
    }

    const T& value() const& {
        check_initialized();
        return get_value();
    }

    T&& value() && {
        check_initialized();
        return std::move(get_value());
    }

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

    bool has_value() const noexcept {
        return is_initialized_;
    }

    explicit operator bool() const noexcept {
        return has_value();
    }

    void reset() noexcept {
        if (is_initialized_) {
            get_value().~T();
            is_initialized_ = false;
        }
    }

    template <typename... Args>
    T& emplace(Args&&... args) {
        reset();
        new (&data_) T(std::forward<Args>(args)...);
        is_initialized_ = true;
        return get_value();
    }

   private:
    T& get_value() noexcept {
        return *reinterpret_cast<T*>(&data_);
    }

    const T& get_value() const noexcept {
        return *reinterpret_cast<const T*>(&data_);
    }

    void check_initialized() const {
        if (!is_initialized_) {
            throw bad_optional_access();
        }
    }

    alignas(T) uint8_t data_[sizeof(T)];
    bool is_initialized_ = false;
};

} // namespace bmstu

#endif // BMSTU_OPTIONAL_H
