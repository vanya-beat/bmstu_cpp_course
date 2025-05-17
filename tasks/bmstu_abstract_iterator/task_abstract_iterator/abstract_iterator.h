#pragma once
#include <cstddef>	// for std::ptrdiff_t

namespace bmstu
{
template <typename Z>
struct Identity
{
	using element_type = Z;
};

template <typename Derived,
		  typename Type,
		  typename Tag,
		  template <typename> class Wrapper = Identity>
struct abstract_iterator
{
   public:
	using iterator_category = Tag;
	using value_type = Type;
	using pointer = Type*;
	using reference = Type&;
	using difference_type = std::ptrdiff_t;
	using element_type = typename Wrapper<Type>::element_type;

	virtual ~abstract_iterator() = default;

	virtual Derived& operator++() = 0;
	virtual Derived& operator--() = 0;

	virtual Derived operator++(int) = 0;
	virtual Derived operator--(int) = 0;

	virtual Derived& operator+=(const difference_type& n) = 0;
	virtual Derived& operator-=(const difference_type& n) = 0;
	virtual Derived operator+(const difference_type& n) const = 0;
	virtual Derived operator-(const difference_type& n) const = 0;

	virtual reference operator*() const = 0;
	virtual pointer operator->() const = 0;

	virtual bool operator==(const Derived& other) const = 0;
	virtual bool operator!=(const Derived& other) const = 0;

	virtual explicit operator bool() const = 0;

	virtual difference_type operator-(const Derived& other) const = 0;
};
}  // namespace bmstu