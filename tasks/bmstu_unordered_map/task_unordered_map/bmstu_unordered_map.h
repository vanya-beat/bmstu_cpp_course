#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <span>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "fast_streebog.h"

namespace bmstu
{

namespace detail
{

template <typename Buffer, typename = void>
struct is_byte_buffer_impl : std::false_type
{
};

template <typename Buffer>
struct is_byte_buffer_impl<
	Buffer,
	std::void_t<decltype(std::declval<const Buffer&>().data()),
				decltype(std::declval<const Buffer&>().size())>>
	: std::bool_constant<
		  std::is_same_v<std::remove_cv_t<std::remove_pointer_t<
							 decltype(std::declval<const Buffer&>().data())>>,
						 uint8_t> &&
		  std::is_convertible_v<decltype(std::declval<const Buffer&>().size()),
								std::size_t>>
{
};

template <typename T, typename = void>
struct has_raw_bytes_impl : std::false_type
{
};

template <typename T>
struct has_raw_bytes_impl<
	T,
	std::void_t<decltype(std::declval<const T&>().rawBytes())>>
	: is_byte_buffer_impl<decltype(std::declval<const T&>().rawBytes())>
{
};

template <typename T, typename = void>
struct has_equality_operator_impl : std::false_type
{
};

template <typename T>
struct has_equality_operator_impl<
	T,
	std::void_t<decltype(std::declval<const T&>() == std::declval<const T&>())>>
	: std::is_convertible<decltype(std::declval<const T&>() ==
								   std::declval<const T&>()),
						  bool>
{
};

}  // namespace detail

template <typename T>
struct has_raw_bytes : detail::has_raw_bytes_impl<T>
{
};

template <typename T>
inline constexpr bool has_raw_bytes_v = has_raw_bytes<T>::value;

template <typename T>
struct has_equality_operator : detail::has_equality_operator_impl<T>
{
};

template <typename T>
inline constexpr bool has_equality_operator_v = has_equality_operator<T>::value;

template <typename K>
struct streebog_hash
{
	static_assert(has_raw_bytes_v<K> || std::is_trivially_copyable_v<K>,
				  "bmstu::streebog_hash: key type must be trivially-copyable "
				  "or provide `rawBytes() const` returning a byte buffer with "
				  "`data()`/`size()` (e.g. std::span<const uint8_t> or "
				  "std::vector<uint8_t>). ");

	std::size_t operator()(const K& key) const noexcept
	{
		uint8_t digest[32];
		if constexpr (has_raw_bytes_v<K>)
		{
			auto bytes = key.rawBytes();
			streebog_hash_256(bytes.data(), bytes.size(), digest);
		}
		else
		{
			streebog_hash_256(reinterpret_cast<const uint8_t*>(&key), sizeof(K),
							  digest);
		}
		std::size_t result = 0;
		std::memcpy(&result, digest, sizeof(std::size_t));
		return result;
	}
};

template <>
struct streebog_hash<std::string>
{
	std::size_t operator()(const std::string& key) const noexcept
	{
		uint8_t digest[32];
		streebog_hash_256(reinterpret_cast<const uint8_t*>(key.data()),
						  key.size(), digest);
		std::size_t result = 0;
		std::memcpy(&result, digest, sizeof(std::size_t));
		return result;
	}
};

template <typename K,
		  typename V,
		  typename Hash = streebog_hash<K>,
		  typename Equal = std::equal_to<K>>
class unordered_map
{
	static_assert(has_equality_operator_v<K>,
				  "bmstu::unordered_map: key type must provide `operator==`.");

   public:
	using key_type = K;
	using mapped_type = V;
	using value_type = std::pair<const K, V>;
	using size_type = std::size_t;

   private:
	static constexpr size_type DEFAULT_BUCKET_COUNT = 16;
	static constexpr double MAX_LOAD_FACTOR = 0.75;

	using bucket_type = std::list<value_type>;

	std::vector<bucket_type> buckets_;
	size_type size_ = 0;
	Hash hasher_;
	Equal equal_;

	size_type bucket_for(const K& key) const { return 0; }

	void rehash(size_type new_count) {}

   public:
	struct iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = unordered_map::value_type;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		std::vector<bucket_type>* buckets_ = nullptr;
		size_type bucket_idx_ = 0;
		typename bucket_type::iterator list_it_;

		iterator() = default;

		iterator(std::vector<bucket_type>* buckets,
				 size_type idx,
				 typename bucket_type::iterator it)
			: buckets_(buckets), bucket_idx_(idx), list_it_(it)
		{
		}

		reference operator*() const { return *list_it_; }
		pointer operator->() const { return &(*list_it_); }

		iterator& operator++() { return *this; }

		iterator operator++(int) { return *this; }

		bool operator==(const iterator& o) const { return false; }

		bool operator!=(const iterator& o) const { return !(*this == o); }
	};

	struct const_iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = const unordered_map::value_type;
		using difference_type = std::ptrdiff_t;
		using pointer = const value_type*;
		using reference = const value_type&;

		const std::vector<bucket_type>* buckets_ = nullptr;
		size_type bucket_idx_ = 0;
		typename bucket_type::const_iterator list_it_;

		const_iterator() = default;

		const_iterator(const std::vector<bucket_type>* buckets,
					   size_type idx,
					   typename bucket_type::const_iterator it)
			: buckets_(buckets), bucket_idx_(idx), list_it_(it)
		{
		}

		const_iterator(const iterator& it)
			: buckets_(it.buckets_),
			  bucket_idx_(it.bucket_idx_),
			  list_it_(it.list_it_)
		{
		}

		reference operator*() const { return *list_it_; }
		pointer operator->() const { return &(*list_it_); }

		const_iterator& operator++() { return *this; }

		const_iterator operator++(int) { return *this; }

		bool operator==(const const_iterator& o) const { return false; }

		bool operator!=(const const_iterator& o) const { return !(*this == o); }
	};

	explicit unordered_map(size_type bucket_count = DEFAULT_BUCKET_COUNT)
		: buckets_(bucket_count)
	{
	}

	unordered_map(const unordered_map&) = default;
	unordered_map(unordered_map&&) = default;
	unordered_map& operator=(const unordered_map&) = default;
	unordered_map& operator=(unordered_map&&) = default;
	~unordered_map() = default;

	iterator begin() { return end(); }

	iterator end() { return iterator(&buckets_, buckets_.size(), {}); }

	const_iterator begin() const { return end(); }

	const_iterator end() const
	{
		return const_iterator(&buckets_, buckets_.size(), {});
	}

	const_iterator cbegin() const { return begin(); }
	const_iterator cend() const { return end(); }

	size_type size() const noexcept { return 0; }

	bool empty() const noexcept { return true; }

	iterator find(const K& key) { return end(); }

	const_iterator find(const K& key) const { return end(); }

	bool contains(const K& key) const { return find(key) != end(); }

	V& at(const K& key)
	{
		throw std::out_of_range("bmstu::unordered_map::at: key not found");
	}

	const V& at(const K& key) const
	{
		throw std::out_of_range("bmstu::unordered_map::at: key not found");
	}

	std::pair<iterator, bool> insert(const value_type& kv)
	{
		return {end(), false};
	}

	V& operator[](const K& key) { return insert({key, V{}}).first->second; }

	bool erase(const K& key) { return false; }

	void clear() {}

	double load_factor() const { return 0.0; }

	size_type bucket_count() const { return 0; }

	void reserve(size_type count) {}
};

}  // namespace bmstu
