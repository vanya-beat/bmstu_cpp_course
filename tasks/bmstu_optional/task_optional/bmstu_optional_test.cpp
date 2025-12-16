#include "bmstu_optional.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

struct Tracker
{
	Tracker() noexcept { ++def_ctor; }
	Tracker(int v) noexcept : value(v) { ++param_ctor; }
	Tracker(const Tracker& other) noexcept : value(other.value) { ++copy_ctor; }
	Tracker(Tracker&& other) noexcept : value(other.value)
	{
		++move_ctor;
		other.value = -1;
	}

	Tracker& operator=(const Tracker& other) noexcept
	{
		if (this != &other)
		{
			value = other.value;
			++copy_assign;
		}
		return *this;
	}

	Tracker& operator=(Tracker&& other) noexcept
	{
		value = other.value;
		other.value = -1;
		++move_assign;
		return *this;
	}

	~Tracker() { ++dtor; }

	static void reset()
	{
		def_ctor = 0;
		param_ctor = 0;
		copy_ctor = 0;
		move_ctor = 0;
		copy_assign = 0;
		move_assign = 0;
		dtor = 0;
	}

	int value = 0;
	inline static int def_ctor = 0;
	inline static int param_ctor = 0;
	inline static int copy_ctor = 0;
	inline static int move_ctor = 0;
	inline static int copy_assign = 0;
	inline static int move_assign = 0;
	inline static int dtor = 0;
};

struct sample_class
{
	sample_class() = default;
	~sample_class() { std::cout << "SAMPLE CLASS DELETE" << std::endl; }
	size_t a;
	bool b;
	char data[1000];
};

struct Z
{
	Z() noexcept { ++def_ctor; }
	Z(const Z&) noexcept { ++copy_ctor; }
	Z(Z&&) noexcept { ++move_ctor; }

	Z& operator=(const Z& other) noexcept
	{
		if (this != &other)
		{
			++copy_assign;
		}
		return *this;
	}

	Z& operator=(Z&& other) noexcept
	{
		++move_assign;
		return *this;
	}

	~Z() { ++dtor; }

	void update() const& { ++const_lvalue_call_count; }
	void update() & { ++lvalue_call_count; }
	void update() && { ++rvalue_call_count; }

	static size_t instance_count()
	{
		return def_ctor + copy_ctor + move_ctor - dtor;
	}

	static void reset()
	{
		def_ctor = 0;
		copy_ctor = 0;
		move_ctor = 0;
		copy_assign = 0;
		move_assign = 0;
		dtor = 0;
		lvalue_call_count = 0;
		rvalue_call_count = 0;
		const_lvalue_call_count = 0;
	}

	static void print_state()
	{
		std::cout << " " << std::endl;
		std::cout << "def_ctor: => " << def_ctor << " " << std::endl;
		std::cout << "copy_ctor: => " << copy_ctor << " " << std::endl;
		std::cout << "move_ctor: => " << move_ctor << " " << std::endl;
		std::cout << "copy_assign: => " << copy_assign << " " << std::endl;
		std::cout << "move_assign: => " << move_assign << " " << std::endl;
		std::cout << "dtor: => " << dtor << " " << std::endl;
		std::cout << "lvalue_call_count: => " << lvalue_call_count << " "
				  << std::endl;
		std::cout << "rvalue_call_count: => " << rvalue_call_count << " "
				  << std::endl;
		std::cout << "const_lvalue_call_count: => " << const_lvalue_call_count
				  << " " << std::endl;
		std::cout << " " << std::endl;
	}

	inline static size_t def_ctor = 0;
	inline static size_t copy_ctor = 0;
	inline static size_t move_ctor = 0;
	inline static size_t copy_assign = 0;
	inline static size_t move_assign = 0;
	inline static size_t dtor = 0;
	inline static size_t lvalue_call_count = 0;
	inline static size_t rvalue_call_count = 0;
	inline static size_t const_lvalue_call_count = 0;
};

TEST(Optional, Test)
{
	int a = 6;
	bmstu::optional<int> b;
	ASSERT_FALSE(b.has_value());
	b = a;
	ASSERT_TRUE(b.has_value());
	ASSERT_EQ(b.value(), 6);
}

TEST(Optional, Test2)
{
	std::string str = "123456700000090-00000030912-93123123123123";
	bmstu::optional<std::string> b;
	ASSERT_FALSE(b.has_value());
	b = str;
	ASSERT_TRUE(b.has_value());
	ASSERT_STREQ(b.value().c_str(),
				 "123456700000090-00000030912-93123123123123");
}

TEST(Optional, Test3)
{
	std::string str = "123456700000090-00000030912-93123123123123";
	bmstu::optional<std::string> b;
	ASSERT_FALSE(b.has_value());
	b = std::move(str);
	ASSERT_TRUE(b.has_value());
	ASSERT_STREQ(b.value().c_str(),
				 "123456700000090-00000030912-93123123123123");
}

TEST(Optional, Init)
{
	Z::reset();
	{
		bmstu::optional<Z> o;
		ASSERT_FALSE(o.has_value());
		ASSERT_EQ(Z::instance_count(), 0);
	}
	ASSERT_EQ(Z::instance_count(), 0);
	Z::reset();
	{
		Z z;
		bmstu::optional<Z> o(z);
		ASSERT_TRUE(o.has_value());
		ASSERT_TRUE(Z::def_ctor == 1 && Z::copy_ctor == 1);
		ASSERT_EQ(Z::instance_count(), 2);
	}
	ASSERT_EQ(Z::instance_count(), 0);
	Z::reset();
	{
		Z z;
		bmstu::optional<Z> o(std::move(z));
		ASSERT_TRUE(o.has_value());
		ASSERT_EQ(Z::def_ctor, 1);
		ASSERT_EQ(Z::move_ctor, 1);
		ASSERT_EQ(Z::copy_ctor, 0);
		ASSERT_EQ(Z::copy_assign, 0);
		ASSERT_EQ(Z::move_assign, 0);
		ASSERT_EQ(Z::instance_count(), 2);
	}
	ASSERT_EQ(Z::instance_count(), 0);
}

TEST(Optional, Bug)
{
	Z::reset();
	{
		bmstu::optional<Z> o1;
		bmstu::optional<Z> o2{Z{}};
		Z::reset();
		o1 = std::move(o2);
		Z::print_state();
		ASSERT_EQ(Z::def_ctor, 0);
		ASSERT_EQ(Z::move_ctor, 1);
		ASSERT_EQ(Z::copy_ctor, 0);
		ASSERT_EQ(Z::copy_assign, 0);
		ASSERT_EQ(Z::move_assign, 0);
		ASSERT_EQ(Z::instance_count(), 1);
	}
}

TEST(Optional, DefaultConstructor)
{
	bmstu::optional<int> opt;
	ASSERT_FALSE(opt.has_value());
}

TEST(Optional, ValueConstructorCopy)
{
	Tracker::reset();
	{
		Tracker t(42);
		bmstu::optional<Tracker> opt(t);
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::param_ctor, 1);
		ASSERT_EQ(Tracker::copy_ctor, 1);
	}
	ASSERT_EQ(Tracker::dtor, 2);
}

TEST(Optional, ValueConstructorMove)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(42));
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::param_ctor, 1);
		ASSERT_EQ(Tracker::move_ctor, 1);
	}
	ASSERT_EQ(Tracker::dtor, 2);
}

TEST(Optional, CopyConstructorFromEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1;
		bmstu::optional<Tracker> opt2(opt1);
		ASSERT_FALSE(opt1.has_value());
		ASSERT_FALSE(opt2.has_value());
		ASSERT_EQ(Tracker::copy_ctor, 0);
	}
	ASSERT_EQ(Tracker::dtor, 0);
}

TEST(Optional, CopyConstructorFromValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		Tracker::reset();
		bmstu::optional<Tracker> opt2(opt1);
		ASSERT_TRUE(opt1.has_value());
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::copy_ctor, 1);
	}
}

TEST(Optional, MoveConstructorFromEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1;
		bmstu::optional<Tracker> opt2(std::move(opt1));
		ASSERT_FALSE(opt2.has_value());
		ASSERT_EQ(Tracker::move_ctor, 0);
	}
}

TEST(Optional, MoveConstructorFromValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		Tracker::reset();
		bmstu::optional<Tracker> opt2(std::move(opt1));
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::move_ctor, 1);
	}
}

TEST(Optional, AssignValueToEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt;
		Tracker t(42);
		Tracker::reset();
		opt = t;
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::copy_ctor, 1);
		ASSERT_EQ(Tracker::copy_assign, 0);
	}
}

TEST(Optional, AssignValueToValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(10));
		Tracker t(42);
		Tracker::reset();
		opt = t;
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::copy_assign, 1);
		ASSERT_EQ(Tracker::copy_ctor, 0);
	}
}

TEST(Optional, AssignRvalueToEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt;
		opt = Tracker(42);
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::param_ctor, 1);
		ASSERT_EQ(Tracker::move_ctor, 1);
		ASSERT_EQ(Tracker::move_assign, 0);
	}
}

TEST(Optional, AssignRvalueToValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(10));
		Tracker::reset();
		opt = Tracker(42);
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::move_assign, 1);
		ASSERT_EQ(Tracker::move_ctor, 0);
	}
}

TEST(Optional, AssignOptionalToEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		bmstu::optional<Tracker> opt2;
		Tracker::reset();
		opt2 = opt1;
		ASSERT_TRUE(opt1.has_value());
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::copy_ctor, 1);
	}
}

TEST(Optional, AssignOptionalToValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		bmstu::optional<Tracker> opt2(Tracker(10));
		Tracker::reset();
		opt2 = opt1;
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::copy_assign, 1);
		ASSERT_EQ(Tracker::copy_ctor, 0);
	}
}

TEST(Optional, AssignEmptyOptionalToValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1;
		bmstu::optional<Tracker> opt2(Tracker(42));
		Tracker::reset();
		opt2 = opt1;
		ASSERT_FALSE(opt2.has_value());
		ASSERT_EQ(Tracker::dtor, 1);
	}
}

TEST(Optional, MoveAssignOptionalToEmpty)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		bmstu::optional<Tracker> opt2;
		Tracker::reset();
		opt2 = std::move(opt1);
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::move_ctor, 1);
		ASSERT_EQ(Tracker::copy_ctor, 0);
	}
}

TEST(Optional, MoveAssignOptionalToValue)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(42));
		bmstu::optional<Tracker> opt2(Tracker(10));
		Tracker::reset();
		opt2 = std::move(opt1);
		ASSERT_TRUE(opt2.has_value());
		ASSERT_EQ(opt2.value().value, 42);
		ASSERT_EQ(Tracker::move_assign, 1);
		ASSERT_EQ(Tracker::move_ctor, 0);
	}
}

TEST(Optional, DereferenceOperator)
{
	bmstu::optional<int> opt(42);
	ASSERT_EQ(*opt, 42);
	*opt = 100;
	ASSERT_EQ(*opt, 100);
}

TEST(Optional, ArrowOperator)
{
	struct Point
	{
		int x, y;
	};
	bmstu::optional<Point> opt(Point{10, 20});
	ASSERT_EQ(opt->x, 10);
	ASSERT_EQ(opt->y, 20);
	opt->x = 100;
	ASSERT_EQ(opt->x, 100);
}

TEST(Optional, ValueMethod)
{
	bmstu::optional<int> opt(42);
	ASSERT_EQ(opt.value(), 42);
	opt.value() = 100;
	ASSERT_EQ(opt.value(), 100);
}

TEST(Optional, ValueMethodThrows)
{
	bmstu::optional<int> opt;
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ConstAccess)
{
	const bmstu::optional<int> opt(42);
	ASSERT_EQ(*opt, 42);
	ASSERT_EQ(opt.value(), 42);
}

TEST(Optional, Emplace)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt;
		opt.emplace(42);
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::param_ctor, 1);
	}
}

TEST(Optional, EmplaceOverwrite)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(10));
		Tracker::reset();
		opt.emplace(42);
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value().value, 42);
		ASSERT_EQ(Tracker::dtor, 1);
		ASSERT_EQ(Tracker::param_ctor, 1);
	}
}

TEST(Optional, Reset)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(42));
		Tracker::reset();
		opt.reset();
		ASSERT_FALSE(opt.has_value());
		ASSERT_EQ(Tracker::dtor, 1);
	}
}

TEST(Optional, ResetEmpty)
{
	bmstu::optional<int> opt;
	opt.reset();
	ASSERT_FALSE(opt.has_value());
}

TEST(Optional, HasValue)
{
	bmstu::optional<int> opt1;
	ASSERT_FALSE(opt1.has_value());

	bmstu::optional<int> opt2(42);
	ASSERT_TRUE(opt2.has_value());

	opt2.reset();
	ASSERT_FALSE(opt2.has_value());
}

TEST(Optional, SelfAssignment)
{
	bmstu::optional<int> opt(42);
	opt = opt;
	ASSERT_TRUE(opt.has_value());
	ASSERT_EQ(opt.value(), 42);
}

TEST(Optional, StringOptional)
{
	bmstu::optional<std::string> opt;
	ASSERT_FALSE(opt.has_value());

	opt = std::string("Hello, World!");
	ASSERT_TRUE(opt.has_value());
	ASSERT_EQ(opt.value(), "Hello, World!");

	opt.emplace("New String");
	ASSERT_EQ(opt.value(), "New String");

	opt.reset();
	ASSERT_FALSE(opt.has_value());
}

TEST(Optional, ChainedOperations)
{
	bmstu::optional<int> opt;
	opt = 10;
	opt = 20;
	opt = 30;
	ASSERT_EQ(opt.value(), 30);

	opt.reset();
	opt = 40;
	ASSERT_EQ(opt.value(), 40);
}

TEST(Optional, MultipleResets)
{
	bmstu::optional<int> opt(42);
	opt.reset();
	opt.reset();
	opt.reset();
	ASSERT_FALSE(opt.has_value());
}

TEST(Optional, ComplexType)
{
	struct Complex
	{
		std::string name;
		int value;
		std::vector<int> data;
	};

	bmstu::optional<Complex> opt;
	opt.emplace(Complex{"test", 42, {1, 2, 3, 4, 5}});
	ASSERT_TRUE(opt.has_value());
	ASSERT_EQ(opt->name, "test");
	ASSERT_EQ(opt->value, 42);
	ASSERT_EQ(opt->data.size(), 5u);
}

TEST(Optional, DestructorCalls)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt1(Tracker(1));
		bmstu::optional<Tracker> opt2(Tracker(2));
		bmstu::optional<Tracker> opt3;
	}
	ASSERT_EQ(Tracker::dtor, 4);
}

TEST(Optional, NoLeaksOnReassignment)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt(Tracker(1));
		Tracker::reset();
		opt = Tracker(2);
		opt = Tracker(3);
		opt = Tracker(4);
	}
}

TEST(Optional, ExceptionOnValueEmpty)
{
	bmstu::optional<int> opt;
	ASSERT_FALSE(opt.has_value());
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionOnConstValueEmpty)
{
	bmstu::optional<int> temp;
	const bmstu::optional<int>& opt = temp;
	ASSERT_FALSE(opt.has_value());
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionMessage)
{
	bmstu::optional<int> opt;
	try
	{
		opt.value();
		FAIL() << "Expected bad_optional_access exception";
	}
	catch (const bmstu::bad_optional_access& e)
	{
		ASSERT_STREQ(e.what(), "Bad optional access");
	}
	catch (...)
	{
		FAIL() << "Expected bad_optional_access exception";
	}
}

TEST(Optional, NoExceptionWhenHasValue)
{
	bmstu::optional<int> opt(42);
	ASSERT_NO_THROW(opt.value());
	ASSERT_EQ(opt.value(), 42);
}

TEST(Optional, ExceptionAfterReset)
{
	bmstu::optional<int> opt(42);
	ASSERT_NO_THROW(opt.value());
	opt.reset();
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionWithComplexType)
{
	bmstu::optional<std::string> opt;
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
	opt = "test";
	ASSERT_NO_THROW(opt.value());
	opt.reset();
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionAfterMove)
{
	bmstu::optional<int> opt1(42);
	bmstu::optional<int> opt2;

	opt2 = std::move(opt1);
	ASSERT_NO_THROW(opt2.value());

	opt2.reset();
	ASSERT_THROW(opt2.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionWithEmptyOptionalAssignment)
{
	bmstu::optional<int> opt1;
	bmstu::optional<int> opt2(42);

	ASSERT_NO_THROW(opt2.value());
	opt2 = opt1;
	ASSERT_THROW(opt2.value(), bmstu::bad_optional_access);
}

TEST(Optional, MultipleExceptionChecks)
{
	bmstu::optional<int> opt;

	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);

	opt = 42;
	ASSERT_NO_THROW(opt.value());

	opt.reset();
	ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
}

TEST(Optional, ExceptionSafety)
{
	bmstu::optional<int> opt;

	try
	{
		opt.value();
	}
	catch (const bmstu::bad_optional_access&)
	{
		ASSERT_FALSE(opt.has_value());
		opt = 100;
		ASSERT_TRUE(opt.has_value());
		ASSERT_EQ(opt.value(), 100);
	}
}

TEST(Optional, BadOptionalAccessInheritance)
{
	bmstu::optional<int> opt;

	try
	{
		opt.value();
		FAIL() << "Expected exception";
	}
	catch (const std::exception& e)
	{
		ASSERT_NE(std::string(e.what()).find("Bad optional access"),
				  std::string::npos);
	}
}

TEST(Optional, ConstOptionalExceptionSafety)
{
	bmstu::optional<std::string> temp1;
	const bmstu::optional<std::string>& opt1 = temp1;
	ASSERT_THROW(opt1.value(), bmstu::bad_optional_access);

	bmstu::optional<std::string> temp2("Hello");
	const bmstu::optional<std::string>& opt2 = temp2;
	ASSERT_NO_THROW(opt2.value());
	ASSERT_EQ(opt2.value(), "Hello");
}
TEST(Optional, ExceptionWithTrackedObject)
{
	Tracker::reset();
	{
		bmstu::optional<Tracker> opt;
		ASSERT_THROW(opt.value(), bmstu::bad_optional_access);

		opt.emplace(42);
		ASSERT_NO_THROW(opt.value());
		ASSERT_EQ(opt.value().value, 42);

		opt.reset();
		ASSERT_THROW(opt.value(), bmstu::bad_optional_access);
	}
	ASSERT_EQ(Tracker::param_ctor, 1);
	ASSERT_EQ(Tracker::dtor, 1);
}