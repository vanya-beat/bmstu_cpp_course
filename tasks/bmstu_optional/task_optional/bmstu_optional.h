#pragma once
#include <cstdint>
#include <exception>
#include <new>
#include <type_traits>
#include <utility>

namespace bmstu
{
struct nullopt_t
{
	constexpr explicit nullopt_t(int) {}
};
inline constexpr nullopt_t nullopt{0};

class bad_optional_access : public std::exception
{
   public:
	using exception::exception;

	const char* what() const noexcept override { return "Bad optional access"; }
};

template <typename T>
class optional
{
   public:
	optional() = default;

	optional(const T& value)
	{
		new (get_ptr()) T(value);
		is_initialized_ = true;
	}

	optional(T&& value)
	{
		new (get_ptr()) T(std::move(value));
		is_initialized_ = true;
	}

	optional(const optional& other)
	{
		if (other.is_initialized_)
		{
			new (get_ptr()) T(*other.get_ptr());
			is_initialized_ = true;
		}
	}

	optional(optional&& other) noexcept
	{
		if (other.is_initialized_)
		{
			new (get_ptr()) T(std::move(*other.get_ptr()));
			is_initialized_ = true;
		}
	}

	optional& operator=(const T& value)
	{
		if (is_initialized_)
		{
			*get_ptr() = value;
		}
		else
		{
			new (get_ptr()) T(value);
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(T&& value)
	{
		if (is_initialized_)
		{
			*get_ptr() = std::move(value);
		}
		else
		{
			new (get_ptr()) T(std::move(value));
			is_initialized_ = true;
		}

		return *this;
	}

	optional& operator=(const optional& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (is_initialized_ && other.is_initialized_)
		{
			*get_ptr() = *other.get_ptr();
		}
		else if (!is_initialized_ && other.is_initialized_)
		{
			new (get_ptr()) T(*other.get_ptr());
			is_initialized_ = true;
		}
		else if (is_initialized_ && !other.is_initialized_)
		{
			reset();
		}
		return *this;
	}

	optional& operator=(optional&& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (is_initialized_ && other.is_initialized_)
		{
			*get_ptr() = std::move(*other.get_ptr());
		}
		else if (!is_initialized_ && other.is_initialized_)
		{
			new (get_ptr()) T(std::move(*other.get_ptr()));
			is_initialized_ = true;
		}
		else if (is_initialized_ && !other.is_initialized_)
		{
			reset();
		}
		return *this;
	}

	T& operator*() & { return *get_ptr(); }

	const T& operator*() const& { return *get_ptr(); }

	T* operator->() { return get_ptr(); }

	const T* operator->() const { return get_ptr(); }

	T&& operator*() && { return std::move(*get_ptr()); }

	T& value() &
	{
		if (!is_initialized_)
		{
			throw bad_optional_access();
		}
		return *get_ptr();
	}

	const T& value() const&
	{
		if (!is_initialized_)
		{
			throw bad_optional_access();
		}
		return *get_ptr();
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		reset();
		new (get_ptr()) T(std::forward<Args>(args)...);
		is_initialized_ = true;
	}

	void reset()
	{
		if (is_initialized_)
		{
			get_ptr()->~T();
			is_initialized_ = false;
		}
	}

	~optional() { reset(); }

	bool has_value() const { return is_initialized_; };

   private:
	T* get_ptr() { return reinterpret_cast<T*>(data_); }
	const T* get_ptr() const { return reinterpret_cast<const T*>(data_); }
	alignas(T) uint8_t data_[sizeof(T)];
	bool is_initialized_ = false;
};
}  // namespace bmstu