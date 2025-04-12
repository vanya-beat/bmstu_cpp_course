#include "bmstu_simple_vector.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <sstream>

TEST(SimpleVector, DefaultConstructor)
{
	bmstu::simple_vector<int> v;
	ASSERT_EQ(v.size(), 0u);
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.capacity(), 0u);
}

TEST(SimpleVector, Init)
{
	bmstu::simple_vector<int> v(5);
	ASSERT_EQ(v.size(), 5u);
	ASSERT_EQ(v.capacity(), 5u);
	ASSERT_TRUE(!v.empty());
	for (size_t i = 0; i < v.size(); ++i)
	{
		ASSERT_EQ(v[i], 0);
	}
}

TEST(SimpleVector, Init2)
{
	bmstu::simple_vector<int> v(3, 42);
	ASSERT_EQ(v.size(), 3);
	ASSERT_EQ(v.capacity(), 3);
	for (size_t i = 0; i < v.size(); ++i)
	{
		ASSERT_EQ(v[i], 42);
	}
}

TEST(SimpleVector, InitList)
{
	bmstu::simple_vector<int> v{1, 2, 3};
	ASSERT_EQ(v.size(), 3);
	ASSERT_EQ(v.capacity(), 3);
	ASSERT_EQ(v[2], 3);
}

TEST(SimpleVector, At)
{
	bmstu::simple_vector<int> v(3);
	ASSERT_EQ(&v.at(2), &v[2]);
	try
	{
		v.at(3);
	}
	catch (std::out_of_range const& err)
	{
		EXPECT_EQ(err.what(), std::string("Index out of range"));
	}
}

TEST(SimpleVector, Clear)
{
	bmstu::simple_vector<int> v(10);
	const size_t old_capacity = v.capacity();
	v.clear();
	ASSERT_EQ(v.size(), 0);
	ASSERT_EQ(v.capacity(), old_capacity);
}

TEST(SimpleVector, Resize)
{
	{
		bmstu::simple_vector<int> v(3);
		v[2] = 17;
		v.resize(7);
		ASSERT_EQ(v.size(), 7);
		ASSERT_TRUE(v.capacity() >= v.size());
		ASSERT_EQ(v[2], 17);
		ASSERT_EQ(v[3], 0);
	}
}

TEST(SimpleVector, Resize1)
{
	{
		bmstu::simple_vector<int> v(3);
		v[0] = 42;
		v[1] = 55;
		const size_t old_capacity = v.capacity();
		v.resize(2);
		ASSERT_EQ(v.size(), 2);
		ASSERT_EQ(v.capacity(), old_capacity);
		ASSERT_EQ(v[0], 42);
		ASSERT_EQ(v[1], 55);
	}
}

TEST(SimpleVector, Resize2)
{
	const size_t old_size = 3;
	bmstu::simple_vector<int> v(3);
	v.resize(old_size + 5);
	v[3] = 42;
	v.resize(old_size);
	ASSERT_EQ(v[2], 0);
	v.resize(old_size + 3);
	ASSERT_EQ(v[3], 0);
}

TEST(SimpleVector, Constructors)
{
	{
		bmstu::simple_vector<int> v(5);
		v[0] = 1;
		v[1] = 2;
		v[2] = 3;
		v[3] = 4;
		v[4] = 5;
		bmstu::simple_vector<int> c(v);
		ASSERT_EQ(v[2], v[2]);
	}
	{
		bmstu::simple_vector<int> v(5);
		v[0] = 1;
		v[1] = 2;
		v[2] = 3;
		v[3] = 4;
		v[4] = 5;
		bmstu::simple_vector<int> c(v);
		ASSERT_EQ(v[2], v[2]);
	}
	{
		bmstu::simple_vector<int> v(1);
		v.push_back(1);
		ASSERT_EQ(v[0], 0);
		ASSERT_EQ(*(v.begin()) + 1, 1);
	}
}

TEST(SimpleVector, Push)
{
	bmstu::simple_vector<int> v(1);
	v.push_back(42);
	ASSERT_EQ(v.size(), 2);
	ASSERT_TRUE(v.capacity() >= v.size());
	ASSERT_EQ(v[0], 0);
	ASSERT_EQ(v[1], 42);
}

TEST(SimpleVector, CopyConstruct)
{
	bmstu::simple_vector<int> numbers{1, 2};
	auto numbers_copy(numbers);
	ASSERT_TRUE(&numbers_copy[0] != &numbers[0]);
	ASSERT_EQ(numbers_copy.size(), numbers.size());
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		ASSERT_EQ(numbers_copy[i], numbers[i]);
		ASSERT_TRUE(&numbers_copy[i] != &numbers[i]);
	}
}

TEST(SimpleVector, PopBack)
{
	{
		bmstu::simple_vector<int> v{0, 1, 2, 3};
		const size_t old_capacity = v.capacity();
		const auto old_begin = v.begin();
		v.pop_back();
		ASSERT_EQ(v.capacity(), old_capacity);
		ASSERT_EQ(v.begin(), old_begin);
		ASSERT_EQ(v, (bmstu::simple_vector<int>{0, 1, 2}));
	}
}

TEST(SimpleVector, Capacity)
{
	bmstu::simple_vector<int> v(2);
	v.resize(1);
	const size_t old_capacity = v.capacity();
	v.push_back(123);
	ASSERT_EQ(v.size(), 2);
	ASSERT_EQ(v.capacity(), old_capacity);
}

TEST(SimpleVector, Iterate)
{
	{
		{
			bmstu::simple_vector<int> v;
			ASSERT_EQ(v.begin(), nullptr);
			ASSERT_EQ(v.end(), nullptr);
		}

		{
			bmstu::simple_vector<int> v(10, 42);
			ASSERT_EQ(*v.begin(), 42);
			ASSERT_EQ(v.end(), v.begin() + v.size());
		}
	}
}

TEST(SimpleVector, Compare)
{
	{
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} == bmstu::simple_vector{1, 2, 3}));
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} != bmstu::simple_vector{1, 2, 2}));

		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} < bmstu::simple_vector{1, 2, 3, 1}));
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} > bmstu::simple_vector{1, 2, 2, 1}));

		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} >= bmstu::simple_vector{1, 2, 3}));
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 4} >= bmstu::simple_vector{1, 2, 3}));
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} <= bmstu::simple_vector{1, 2, 3}));
		ASSERT_TRUE(
			(bmstu::simple_vector{1, 2, 3} <= bmstu::simple_vector{1, 2, 4}));
	}
}

TEST(SimpleVector, Pushback2)
{
	bmstu::simple_vector<int> v2;
	v2.push_back(0);
	v2.push_back(1);
	v2.push_back(2);
	ASSERT_EQ(v2, (bmstu::simple_vector<int>{0, 1, 2}));
}

TEST(SimpleVector, Swap)
{
	{
		bmstu::simple_vector<int> v1{42, 666};
		bmstu::simple_vector<int> v2;
		v2.push_back(0);
		v2.push_back(1);
		v2.push_back(2);
		const int* const begin1 = &v1[0];
		const int* const begin2 = &v2[0];

		const size_t capacity1 = v1.capacity();
		const size_t capacity2 = v2.capacity();

		const size_t size1 = v1.size();
		const size_t size2 = v2.size();

		v1.swap(v2);
		ASSERT_TRUE(&v2[0] == begin1);
		ASSERT_TRUE(&v1[0] == begin2);
		ASSERT_TRUE(v1.size() == size2);
		ASSERT_TRUE(v2.size() == size1);
		ASSERT_TRUE(v1.capacity() == capacity2);
		ASSERT_TRUE(v2.capacity() == capacity1);
	}
}

TEST(SimpleVector, Test1)
{
	{
		bmstu::simple_vector<int> src_vector{1, 2, 3, 4};
		bmstu::simple_vector<int> dst_vector{1, 2, 3, 4, 5, 6};
		dst_vector = src_vector;
		ASSERT_EQ(dst_vector, src_vector);
	}
}

TEST(SimpleVector, Insert)
{
	// Вставка элементов
	{
		bmstu::simple_vector<int> v{1, 2, 3, 4};
		auto vit = v.begin() + 3;
		v.insert(v.begin() + 2, 42);
		ASSERT_EQ(v, (bmstu::simple_vector<int>{1, 2, 42, 3, 4}));
	}
}

TEST(SimpleVector, Insert2)
{
	bmstu::simple_vector<int> v;
	v.insert(v.begin(), 42);
	ASSERT_EQ(v, (bmstu::simple_vector<int>{42}));
}

TEST(SimpleVector, Erase)
{
	{
		bmstu::simple_vector<int> v{1, 2, 3, 4};
		v.erase(v.begin() + 2);
		ASSERT_EQ(v, (bmstu::simple_vector<int>{1, 2, 4}));
	}

	{
		bmstu::simple_vector<int> v{1, 2, 3, 4, 5};
		v.erase(v.begin());
		ASSERT_EQ(v, (bmstu::simple_vector<int>{2, 3, 4, 5}));
	}

	{
		bmstu::simple_vector<int> v{1, 2, 3, 4, 5};
		v.erase(v.end());
		ASSERT_EQ(v, (bmstu::simple_vector<int>{1, 2, 3, 4}));
	}
}

TEST(SimpleVector, Reserve)
{
	{
		bmstu::simple_vector<int> v(5);
		ASSERT_EQ(v.capacity(), 5);
		ASSERT_FALSE(v.empty());
	}

	{
		bmstu::simple_vector<int> v;
		v.reserve(5);
		ASSERT_EQ(v.capacity(), 5);
		ASSERT_TRUE(v.empty());

		v.reserve(1);

		ASSERT_EQ(v.capacity(), 5);
		for (int i = 0; i < 10; ++i)
		{
			v.push_back(i);
		}
		ASSERT_EQ(v.size(), 10);

		v.reserve(100);

		ASSERT_EQ(v.size(), 10);
		ASSERT_EQ(v.capacity(), 100);
		for (int i = 0; i < 10; ++i)
		{
			ASSERT_EQ(v[i], i);
		}
	}
}

bmstu::simple_vector<int> GenerateVector(size_t size)
{
	bmstu::simple_vector<int> v(size);
	std::iota(v.begin(), v.end(), 1);
	return v;
}

TEST(SimpleVector, temporary)
{
	const size_t size = 1000000;
	bmstu::simple_vector<int> moved_vector(GenerateVector(size));
	ASSERT_EQ(moved_vector.size(), size);
}

TEST(SimpleVector, moveconstructor)
{
	const size_t size = 1000000;

	bmstu::simple_vector<int> vector_to_move(GenerateVector(size));
	ASSERT_EQ(vector_to_move.size(), size);

	bmstu::simple_vector<int> moved_vector(std::move(vector_to_move));
	ASSERT_EQ(moved_vector.size(), size);
	ASSERT_EQ(vector_to_move.size(), 0);
}

TEST(SimpleVector, moveoperator)
{
	const size_t size = 1000000;
	bmstu::simple_vector<int> vector_to_move(GenerateVector(size));
	ASSERT_EQ(vector_to_move.size(), size);

	bmstu::simple_vector<int> moved_vector = std::move(vector_to_move);
	ASSERT_EQ(moved_vector.size(), size);
	ASSERT_EQ(vector_to_move.size(), 0);
}

TEST(SimpleVector, test_new_push)
{
	bmstu::simple_vector<int> v;
	ASSERT_EQ(v.capacity(), 0);
	ASSERT_EQ(v.size(), 0);
	v.push_back(1);
	ASSERT_EQ(v.capacity(), 1);
	ASSERT_EQ(v.size(), 1);
	v.push_back(2);
	ASSERT_EQ(v.size(), 2);
	ASSERT_EQ(v.capacity(), 2);
}

class CopyTracker
{
   public:
	static int copy_count;
	static int move_count;
	int value;

	CopyTracker(int v = 0) : value(v) {}

	CopyTracker(const CopyTracker& other) : value(other.value) { ++copy_count; }

	CopyTracker(CopyTracker&& other) noexcept : value(other.value)
	{
		++move_count;
		other.value = 0;
	}

	CopyTracker& operator=(const CopyTracker& other)
	{
		if (this != &other)
		{
			value = other.value;
			++copy_count;
		}
		return *this;
	}

	CopyTracker& operator=(CopyTracker&& other) noexcept
	{
		value = other.value;
		other.value = 0;
		++move_count;
		return *this;
	}

	bool operator==(const CopyTracker& other) const
	{
		return value == other.value;
	}

	static void reset()
	{
		copy_count = 0;
		move_count = 0;
	}
};

int CopyTracker::copy_count = 0;
int CopyTracker::move_count = 0;

TEST(SimpleVector, PushBackCopyMove)
{
	CopyTracker::reset();

	bmstu::simple_vector<CopyTracker> v;
	CopyTracker original(42);

	v.push_back(original);

	ASSERT_EQ(CopyTracker::copy_count, 2);
	ASSERT_GE(CopyTracker::move_count, 1);
	ASSERT_EQ(v[0].value, 42);
	ASSERT_EQ(original.value, 42);
}

TEST(SimpleVector, PushBackCopyMove2)
{
	CopyTracker::reset();

	bmstu::simple_vector<CopyTracker> v;
	CopyTracker original(42);

	v.push_back(std::move(original));

	ASSERT_EQ(CopyTracker::copy_count, 1);
	ASSERT_GE(CopyTracker::move_count, 1);
	ASSERT_EQ(v[0].value, 42);
	ASSERT_EQ(original.value, 0);
}

TEST(SimpleVector, PushBackCopyMove3)
{
	bmstu::simple_vector<int> v;
	v.push_back(42);
	auto it = v.begin();
	it = nullptr;
}
