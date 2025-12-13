#pragma once
#include <cstdint>
#include <exception>
#include <type_traits>

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

	const char* what() const noexcept override { return "WRONG TEXT"; }
};

template <typename T>
class optional
{
   public:
	optional() = default;

	optional(const T& value) {}

	optional(T&& value) {}

	optional(const optional& other) {}

	optional(optional&& other) noexcept {}

	optional& operator=(const T& value) { return *this; }

	optional& operator=(T&& value) { return *this; }

	optional& operator=(const optional& value) { return *this; }

	optional& operator=(optional&& value) { return *this; }

	T& operator*() &
	{
		T* ptr = nullptr;
		return *ptr;
	}

	const T& operator*() const&
	{
		T* ptr = nullptr;
		return *ptr;
	}

	T* operator->() { return nullptr; }

	const T* operator->() const { return nullptr; }

	T&& operator*() &&
	{
		T* ptr = nullptr;
		return std::move(*ptr);
	}

	T& value() &
	{
		T* ptr = nullptr;
		return *ptr;
	}

	const T& value() const&
	{
		T* ptr = nullptr;
		return *ptr;
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
	}

	void reset() {}

	~optional() {}

	bool has_value() const { return false; };

   private:
	alignas(T) uint8_t data_[sizeof(T)];
	bool is_initialized_ = false;
};
}  // namespace bmstu