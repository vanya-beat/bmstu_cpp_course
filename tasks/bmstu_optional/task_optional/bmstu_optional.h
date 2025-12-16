#pragma once
#include <cstdint>
#include <exception>
#include <iostream>	 // äëÿ îòëàäêè
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
	// Êîíñòðóêòîðû
	optional() noexcept = default;

	optional(nullopt_t) noexcept {}

	optional(const T& value)
	{
		new (&data_) T(value);
		is_initialized_ = true;
	}

	optional(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		new (&data_) T(std::move(value));
		is_initialized_ = true;
	}

	optional(const optional& other)
	{
		if (other.is_initialized_)
		{
			new (&data_) T(other.get_value());
			is_initialized_ = true;
		}
	}

	optional(optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
		: is_initialized_(other.is_initialized_)
	{
		if (other.is_initialized_)
		{
			new (&data_) T(std::move(other.get_value()));
			other.is_initialized_ =
				false;	// Âàæíî: ÍÅ âûçûâàåì other.reset() çäåñü!
		}
	}

	// Îïåðàòîðû ïðèñâàèâàíèÿ
	optional& operator=(nullopt_t) noexcept
	{
		reset();
		return *this;
	}

	optional& operator=(const T& value)
	{
		if (is_initialized_)
		{
			get_value() = value;
		}
		else
		{
			new (&data_) T(value);
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(T&& value) noexcept(
		std::is_nothrow_move_assignable_v<T>)
	{
		if (is_initialized_)
		{
			get_value() = std::move(value);
		}
		else
		{
			new (&data_) T(std::move(value));
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(const optional& other)
	{
		if (this == &other)
			return *this;

		if (!other.is_initialized_)
		{
			reset();
		}
		else if (is_initialized_)
		{
			get_value() = other.get_value();
		}
		else
		{
			new (&data_) T(other.get_value());
			is_initialized_ = true;
		}
		return *this;
	}

	optional& operator=(optional&& other) noexcept(
		std::is_nothrow_move_constructible_v<T> &&
		std::is_nothrow_move_assignable_v<T>)
	{
		if (this != &other)
		{
			if (!other.is_initialized_)
			{
				reset();
			}
			else if (is_initialized_)
			{
				get_value() = std::move(other.get_value());
			}
			else
			{
				// Êëþ÷åâîé ìîìåíò: åñëè this íå èíèöèàëèçèðîâàí, èñïîëüçóåì
				// move construction
				new (&data_) T(std::move(other.get_value()));
				is_initialized_ = true;
			}
			// ÍÅ ñáðàñûâàåì other çäåñü - ïóñòü ýòî ñäåëàåò äåñòðóêòîð
			// åñëè íóæíî ñáðîñèòü, òî òîëüêî óñòàíîâèòü ôëàã:
			other.is_initialized_ = false;
		}
		return *this;
	}

	// Äåñòðóêòîð
	~optional() { reset(); }

	// Äîñòóï ê çíà÷åíèþ
	T& operator*() & noexcept { return get_value(); }

	const T& operator*() const& noexcept { return get_value(); }

	T&& operator*() && noexcept { return std::move(get_value()); }

	const T&& operator*() const&& noexcept { return std::move(get_value()); }

	T* operator->() noexcept { return &get_value(); }

	const T* operator->() const noexcept { return &get_value(); }

	// value() ñ ïðîâåðêîé
	T& value() &
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return get_value();
	}

	const T& value() const&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return get_value();
	}

	T&& value() &&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return std::move(get_value());
	}

	const T&& value() const&&
	{
		if (!is_initialized_)
			throw bad_optional_access();
		return std::move(get_value());
	}

	// Ìåòîäû
	template <typename... Args>
	T& emplace(Args&&... args)
	{
		reset();
		new (&data_) T(std::forward<Args>(args)...);
		is_initialized_ = true;
		return get_value();
	}

	void reset() noexcept
	{
		if (is_initialized_)
		{
			get_value().~T();
			is_initialized_ = false;
		}
	}

	// Ïðîâåðêè
	bool has_value() const noexcept { return is_initialized_; }

	explicit operator bool() const noexcept { return is_initialized_; }

   private:
	// Âñïîìîãàòåëüíûå ìåòîäû äëÿ äîñòóïà ê äàííûì
	T& get_value() noexcept { return *reinterpret_cast<T*>(&data_); }

	const T& get_value() const noexcept
	{
		return *reinterpret_cast<const T*>(&data_);
	}

	alignas(T) std::byte data_[sizeof(T)];
	bool is_initialized_ = false;
};
}  // namespace bmstu
