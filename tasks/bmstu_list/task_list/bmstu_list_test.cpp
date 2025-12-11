#include "bmstu_list.h"

#include <gtest/gtest.h>
#include <algorithm>

TEST(BidirectLinkedListTests, init)
{
	using namespace std;
	{
		const bmstu::list<int> empty_int_list;
		ASSERT_EQ(empty_int_list.size(), 0u);
		ASSERT_TRUE(empty_int_list.empty());
	}

	{
		const bmstu::list<string> empty_string_list;
		ASSERT_EQ(empty_string_list.size(), 0u);
		ASSERT_TRUE(empty_string_list.empty());
	}
}

TEST(BidirectLinkedListTests, push_front)
{
	using namespace std;
	{
		bmstu::list<int> l;
		ASSERT_TRUE(l.empty());
		ASSERT_EQ(l.size(), 0u);

		l.push_front(0);
		l.push_front(1);
		ASSERT_EQ(l.size(), 2);
		ASSERT_TRUE(!l.empty());

		l.clear();
		ASSERT_EQ(l.size(), 0);
		ASSERT_TRUE(l.empty());
	}
}

TEST(BidirectLinkedListTests, push_back)
{
	using namespace std;
	{
		bmstu::list<int> l;
		ASSERT_TRUE(l.empty());
		ASSERT_EQ(l.size(), 0u);

		l.push_back(1);
		l.push_back(1);
		l.push_back(3);
		ASSERT_EQ(l.size(), 3);
		ASSERT_TRUE(!l.empty());

		l.clear();
		ASSERT_EQ(l.size(), 0);
		ASSERT_TRUE(l.empty());
	}
}

TEST(BidirectLinkedListTests, deletion_spy)
{
	struct DeletionSpy
	{
		DeletionSpy() = default;

		explicit DeletionSpy(int& instance_counter) noexcept
			: instance_counter_ptr_(&instance_counter)	//
		{
			OnAddInstance();
		}

		DeletionSpy(const DeletionSpy& other) noexcept
			: instance_counter_ptr_(other.instance_counter_ptr_)  //
		{
			OnAddInstance();
		}

		DeletionSpy& operator=(const DeletionSpy& rhs) noexcept
		{
			if (this != &rhs)
			{
				auto rhs_copy(rhs);
				std::swap(instance_counter_ptr_,
						  rhs_copy.instance_counter_ptr_);
			}
			return *this;
		}

		~DeletionSpy() { OnDeleteInstance(); }

	   private:
		void OnAddInstance() noexcept
		{
			if (instance_counter_ptr_)
			{
				++(*instance_counter_ptr_);
			}
		}

		void OnDeleteInstance() noexcept
		{
			if (instance_counter_ptr_)
			{
				ASSERT_NE(*instance_counter_ptr_, 0);
				--(*instance_counter_ptr_);
			}
		}

		int* instance_counter_ptr_ = nullptr;
	};

	{
		int item0_counter = 0;
		int item1_counter = 0;
		int item2_counter = 0;
		{
			bmstu::list<DeletionSpy> list;
			list.push_front(DeletionSpy{item0_counter});
			list.push_front(DeletionSpy{item1_counter});
			list.push_front(DeletionSpy{item2_counter});

			ASSERT_EQ(item0_counter, 1);
			ASSERT_EQ(item1_counter, 1);
			ASSERT_EQ(item2_counter, 1);
			list.clear();
			ASSERT_EQ(item0_counter, 0);
			ASSERT_EQ(item1_counter, 0);
			ASSERT_EQ(item2_counter, 0);

			list.push_front(DeletionSpy{item0_counter});
			list.push_front(DeletionSpy{item1_counter});
			list.push_front(DeletionSpy{item2_counter});
			ASSERT_EQ(item0_counter, 1);
			ASSERT_EQ(item1_counter, 1);
			ASSERT_EQ(item2_counter, 1);
		}
		ASSERT_EQ(item0_counter, 0);
		ASSERT_EQ(item1_counter, 0);
		ASSERT_EQ(item2_counter, 0);
	}
}

TEST(BidirectLinkedListTests, Throw)
{
	struct ThrowOnCopy
	{
		ThrowOnCopy() = default;

		explicit ThrowOnCopy(int& copy_counter) noexcept
			: countdown_ptr(&copy_counter)
		{
		}

		ThrowOnCopy(const ThrowOnCopy& other)
			: countdown_ptr(other.countdown_ptr)  //
		{
			if (countdown_ptr)
			{
				if (*countdown_ptr == 0)
				{
					throw std::bad_alloc();
				}
				else
				{
					--(*countdown_ptr);
				}
			}
		}

		ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;

		int* countdown_ptr = nullptr;
	};

	{
		bool exception_was_thrown = false;

		for (int max_copy_counter = 5; max_copy_counter >= 0;
			 --max_copy_counter)
		{
			bmstu::list<ThrowOnCopy> list;
			list.push_front(ThrowOnCopy{});
			try
			{
				int copy_counter = max_copy_counter;
				list.push_front(ThrowOnCopy(copy_counter));

				ASSERT_EQ(list.size(), 2);
			}
			catch (const std::bad_alloc&)
			{
				exception_was_thrown = true;

				ASSERT_EQ(list.size(), 1);
				break;
			}
		}
		ASSERT_TRUE(exception_was_thrown);
	}
}

TEST(BidirectLinkedListTests, iterators_empty)
{
	{
		bmstu::list<int> list;

		const auto& const_list = list;

		ASSERT_EQ(list.begin(), list.end());
		ASSERT_EQ(const_list.begin(), const_list.end());
		ASSERT_EQ(list.cbegin(), list.cend());
		ASSERT_EQ(list.cbegin(), const_list.begin());
		ASSERT_EQ(list.cend(), const_list.end());
	}
}

TEST(BidirectLinkedListTests, iterators_non_empty)
{
	{
		bmstu::list<int> list;
		const auto& const_list = list;

		list.push_front(1);
		ASSERT_EQ(list.size(), 1u);
		ASSERT_TRUE(!list.empty());

		ASSERT_TRUE(const_list.begin() != const_list.end());
		ASSERT_TRUE(const_list.cbegin() != const_list.cend());
		ASSERT_TRUE(list.begin() != list.end());

		ASSERT_TRUE(const_list.begin() == const_list.cbegin());

		ASSERT_TRUE(*list.cbegin() == 1);
		*list.begin() = -1;
		ASSERT_TRUE(*list.cbegin() == -1);

		const auto old_begin = list.cbegin();
		list.push_front(2);
		ASSERT_EQ(list.size(), 2);

		const auto new_begin = list.cbegin();
		ASSERT_NE(new_begin, old_begin);

		{
			auto new_begin_copy(new_begin);
			ASSERT_EQ((++(new_begin_copy)), old_begin);
		}

		{
			auto new_begin_copy(new_begin);
			ASSERT_EQ(((new_begin_copy)++), new_begin);
			ASSERT_EQ(new_begin_copy, old_begin);
		}

		{
			auto old_begin_copy(old_begin);
			ASSERT_EQ((++old_begin_copy), list.end());
		}
	}
}

TEST(BidirectLinkedListTests, iterators_decrement)
{
	{
		bmstu::list<int> list;
		const auto& const_list = list;

		list.push_back(100500);
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		ASSERT_EQ(list.size(), 4u);
		ASSERT_TRUE(!list.empty());

		ASSERT_TRUE(const_list.begin() != const_list.end());
		ASSERT_TRUE(const_list.cbegin() != const_list.cend());
		ASSERT_TRUE(list.begin() != list.end());

		ASSERT_TRUE(const_list.begin() == const_list.cbegin());

		ASSERT_EQ(*list.cbegin(), 100500);
		*list.begin() = -1;
		ASSERT_TRUE(*list.cbegin() == -1);
		auto end_list = list.end();
		auto back = --end_list;
		ASSERT_EQ(*(end_list), 3);
	}
}

TEST(BidirectLinkedListTests, iterators_decrement_2)
{
	{
		bmstu::list<int> list;
		const auto& const_list = list;

		list.push_back(100500);
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_front(100501);
		ASSERT_EQ(*(--(--list.end())), 2);
		ASSERT_EQ(*(list.begin()), 100501);
	}
}

TEST(BidirectLinkedListTests, iterators_back)
{
	{
		using namespace std;
		bmstu::list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		std::stringstream out;
		out << list;
		ASSERT_EQ(out.str(), "{1, 2, 3, 4}"s);
	}
	{
		using namespace std;
		bmstu::list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_front(-2);
		list.push_front(-1);
		std::stringstream out;
		out << list;
		ASSERT_EQ(out.str(), "{-1, -2, 1, 2}"s);
	}
}

TEST(BidirectLinkedListTests, arithmetic)
{
	{
		using namespace std;
		bmstu::list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		bmstu::list<int>::iterator it = list.begin();
		ASSERT_EQ(*((it + 1) + 1), 3);
		ASSERT_EQ(*((it + 3) - 1), 3);
	}
	{
		bmstu::list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		bmstu::list<int>::iterator it = list.end();
		ASSERT_EQ(*(it - list.size()), 1);
	}
	{
		using namespace std;
		bmstu::list<int> list;
		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);
		list.push_back(5);
		bmstu::list<int>::iterator it = list.begin();
		ASSERT_EQ(*(it + 1), 2);
	}
}

TEST(BidirectLinkedListTests, distance)
{
	bmstu::list<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_back(5);
	ASSERT_EQ(*list.begin(), 1);
	ASSERT_EQ(*(list.begin() + 1), 2);
	ASSERT_EQ(*(list.begin() + 2), 3);
	ASSERT_EQ((list.end() - list.begin()), 5);
	ASSERT_EQ(std::distance(list.begin(), list.end()), 5);
}

TEST(BidirectLinkedListTests, operators)
{
	bmstu::list<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_back(5);
	ASSERT_EQ(list[0], 1);
	ASSERT_EQ(list[1], 2);
	ASSERT_EQ(list[2], 3);
	list[1] = 100500;
}

TEST(BidirectLinkedListTests, initializer_list)
{
	bmstu::list<int> list{1, 2, 3, 4, 5, 6};
	ASSERT_EQ(list[0], 1);
	ASSERT_EQ(list[list.size() - 1], 6);
}

TEST(BidirectLinkedListTests, initializer_list_strings)
{
	using namespace std;
	bmstu::list<std::string> list{"first1"s, "first2"s, "first3"s,
								  "first4"s, "first5"s, "first6"s};
	ASSERT_EQ(*list.begin(), "first1"s);
}

TEST(BidirectLinkedListTests, iterators_operators)
{
	{
		using namespace std;
		bmstu::list<std::string> string_list;

		string_list.push_front("one"s);
		ASSERT_EQ(string_list.cbegin()->length(), 3u);
		string_list.begin()->push_back('!');
		ASSERT_EQ(*string_list.begin(), std::string("one!"));
	}
}

TEST(BidirectLinkedListTests, equals)
{
	bmstu::list<int> list1{1, 2, 3, 4, 5, 6};
	bmstu::list<int> list1_1{1, 2, 3, 4, 5, 6};
	bmstu::list<int> list3{1, 2, 3, 3, 5, 6};
	bmstu::list<int> list2{666, 667, 668};
	ASSERT_FALSE(list1 == list2);
	ASSERT_FALSE(list1 == list3);
	ASSERT_TRUE(list1 == list1_1);
}

TEST(BidirectLinkedListTests, iterators_poly)
{
	{
		bmstu::list<int> list;
		list.push_front(1);

		bmstu::list<int>::const_iterator const_it(list.begin());
		ASSERT_EQ(const_it, list.cbegin());
		ASSERT_EQ(*const_it, *list.cbegin());

		bmstu::list<int>::const_iterator const_it1;

		const_it1 = list.begin();
		ASSERT_EQ(const_it1, const_it);
	}
}

TEST(BidirectLinkedListTests, equals2)
{
	bmstu::list<int> list_1;
	list_1.push_front(1);
	list_1.push_front(2);

	bmstu::list<int> list_2;
	list_2.push_front(1);
	list_2.push_front(2);
	list_2.push_front(3);

	bmstu::list<int> list_1_copy;
	list_1_copy.push_front(1);
	list_1_copy.push_front(2);

	bmstu::list<int> empty_list;
	bmstu::list<int> another_empty_list;

	ASSERT_TRUE(list_1 == list_1);
	ASSERT_TRUE(empty_list == empty_list);

	ASSERT_TRUE(list_1 == list_1_copy);
	ASSERT_TRUE(list_1 != list_2);
	ASSERT_TRUE(list_2 != list_1);
	ASSERT_TRUE(empty_list == another_empty_list);
}

TEST(BidirectLinkedListTests, swap)
{
	bmstu::list<int> list_1{1, 2, 3};
	bmstu::list<int> list_2{-1, -2};
	list_1.swap(list_2);
	ASSERT_EQ(list_1, (bmstu::list<int>{-1, -2}));
}

TEST(BidirectLinkedListTests, swap2)
{
	bmstu::list<int> first;
	first.push_back(1);
	first.push_back(2);

	bmstu::list<int> second;
	second.push_back(10);
	second.push_back(11);
	second.push_back(15);

	const auto old_first_begin = first.begin();
	const auto old_second_begin = second.begin();
	const auto old_first_size = first.size();
	const auto old_second_size = second.size();

	first.swap(second);

	ASSERT_EQ(second.begin(), old_first_begin);
	ASSERT_EQ(first.begin(), old_second_begin);
	ASSERT_EQ(second.size(), old_first_size);
	ASSERT_EQ(first.size(), old_second_size);

	{
		using std::swap;

		swap(first, second);

		ASSERT_EQ(first.begin(), old_first_begin);
		ASSERT_EQ(second.begin(), old_second_begin);
		ASSERT_EQ(first.size(), old_first_size);
		ASSERT_EQ(second.size(), old_second_size);
	}
}

TEST(BidirectLinkedListTests, from_vector)
{
	using namespace std;
	std::vector<std::string> my_vec = {"string1"s, "string3"s, "string4"s,
									   "string5"s, "string6"s, "string7"s};
	bmstu::list<std::string> my_list(my_vec.begin(), my_vec.end());
	my_list.push_front("begin_string"s);
	my_list.push_back("end_string"s);
	my_vec.clear();
	my_vec.insert(my_vec.begin(), my_list.begin(), my_list.end());

	ASSERT_EQ((std::vector<std::string>{"begin_string"s, "string1"s, "string3"s,
										"string4"s, "string5"s, "string6"s,
										"string7"s, "end_string"s}),
			  my_vec);
}