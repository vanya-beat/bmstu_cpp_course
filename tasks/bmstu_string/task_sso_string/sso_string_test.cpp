#include <gtest/gtest.h>

#include <sstream>
#include "bmstu_sso_string.h"

TEST(SSOStringTest, DefaultConstructor)
{
	bmstu::string str;
	ASSERT_STREQ(str.c_str(), "");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, DefaultConstructorW)
{
	bmstu::wstring str;
	ASSERT_STREQ(str.c_str(), L"");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, InitializerListConstructorW)
{
	bmstu::wstring str{};
	ASSERT_STREQ(str.c_str(), L"");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, InitializerListConstructor)
{
	bmstu::wstring str{};
	ASSERT_STREQ(str.c_str(), L"");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, InitializerListConstructorNonEmpty)
{
	bmstu::wstring str{L'H', L'E', L'L', L'L', L'O'};
	ASSERT_STREQ(str.c_str(), L"HELLO");
	ASSERT_EQ(str.size(), 5);
}

TEST(SSOStringTest, ConstructorSizeW)
{
	bmstu::wstring str(10);
	ASSERT_STREQ(str.c_str(), L"          ");
	ASSERT_EQ(str.size(), 10);
}

TEST(SSOStringTest, ConstructorSize)
{
	bmstu::string str(10);
	ASSERT_STREQ(str.c_str(), "          ");
	ASSERT_EQ(str.size(), 10);
}

TEST(SSOStringTest, ConstructorSizeOne)
{
	bmstu::wstring str(1);
	ASSERT_STREQ(str.c_str(), L" ");
	ASSERT_EQ(str.size(), 1);
}

TEST(SSOStringTest, FromCStr)
{
	bmstu::string str("str\n");
	ASSERT_STREQ(str.c_str(), "str\n");
	ASSERT_EQ(str.size(), 4);
}

TEST(SSOStringTest, FromWCStr)
{
	bmstu::wstring str(L"test");
	ASSERT_STREQ(str.c_str(), L"test");
	ASSERT_EQ(str.size(), 4);
}

TEST(SSOStringTest, FromWCStrSymbol)
{
	bmstu::wstring str(L"test");
	ASSERT_STREQ(str.c_str(), L"test");
	wchar_t c = L't';
	ASSERT_EQ(str[0], c);
}

TEST(SSOStringTest, USymbolsW)
{
	bmstu::wstring str(L"a");
	ASSERT_STREQ(str.c_str(), L"a");
	wchar_t c = L'a';
	ASSERT_EQ(str.size(), 1);
}

TEST(SSOStringTest, USymbol)
{
	bmstu::string str("\xe3\x81\x8a\x00");
	ASSERT_STREQ(str.c_str(), "お");
	ASSERT_EQ(str.size(), 3);
}

TEST(SSOStringTest, USymbols)
{
	bmstu::string str("おはよう");
	ASSERT_STREQ(str.c_str(), "おはよう");
	ASSERT_EQ(str.size() + 1, sizeof("おはよう"));
}

TEST(SSOStringTest, Empty)
{
	bmstu::string empty;
	ASSERT_EQ(empty.size(), 0);
	ASSERT_STREQ(empty.c_str(), "");
}

TEST(SSOStringTest, EmptyW)
{
	bmstu::wstring empty;
	ASSERT_EQ(empty.size(), 0);
	ASSERT_STREQ(empty.c_str(), L"");
}

TEST(SSOStringTest, CopyAssignW)
{
	bmstu::wstring empty(L"Non Empty string");
	bmstu::wstring nonempty = empty;
	ASSERT_STREQ(nonempty.c_str(), L"Non Empty string");
	ASSERT_EQ(nonempty.size(), 16);
}

TEST(SSOStringTest, CopyConstructor)
{
	bmstu::string empty("Non Empty string");
	bmstu::string nonempty = empty;
	ASSERT_STREQ(nonempty.c_str(), "Non Empty string");
	ASSERT_EQ(nonempty.size(), sizeof("Non Empty string") - 1);
}

TEST(SSOStringTest, CopyAssignConstructor)
{
	bmstu::string* str = new bmstu::string("str");
	bmstu::string* nonempty = new bmstu::string;
	*nonempty = *str;
	ASSERT_STREQ(nonempty->c_str(), "str");
	ASSERT_EQ(nonempty->size(), sizeof("str") - 1);
	delete str;
	delete nonempty;
}

TEST(SSOStringTest, MoveAssignConstructor)
{
	bmstu::string str = "str";
	bmstu::string str2 = "other";
	str2 = std::move(str);
	ASSERT_STREQ(str2.c_str(), "str");
	ASSERT_EQ(str2.size(), sizeof("str") - 1);

	ASSERT_STREQ(str.c_str(), "");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, MoveAssignConstructor2)
{
	bmstu::string str = "str";
	bmstu::string str2 = "other";
	str2 = std::move(str);
	ASSERT_STREQ(str2.c_str(), "str");
	ASSERT_EQ(str2.size(), sizeof("str") - 1);

	ASSERT_STREQ(str.c_str(), "");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, CopyAssignConstructor2)
{
	bmstu::string str = "str";
	bmstu::string str2(bmstu::string("other"));
	str2 = std::move(str);
	ASSERT_STREQ(str2.c_str(), "str");
	ASSERT_EQ(str2.size(), sizeof("str") - 1);

	ASSERT_STREQ(str.c_str(), "");
	ASSERT_EQ(str.size(), 0);
}

TEST(SSOStringTest, MoveAssignConstructorW)
{
	bmstu::wstring str2(std::move(bmstu::wstring(L"other")));
	ASSERT_STREQ(str2.c_str(), L"other");
	ASSERT_EQ(str2.size(), (sizeof(L"other") / sizeof(wchar_t)) - 1);
}

TEST(SSOStringTest, CStrCopyAssign)
{
	const char* str = "Simple Str";
	bmstu::string b_str;
	b_str = str;
	ASSERT_STREQ(b_str.c_str(), str);
}

TEST(SSOStringTest, CStrCopyAssignW)
{
	const wchar_t* str = L"Simple string";
	bmstu::wstring b_str;
	b_str = str;
	ASSERT_STREQ(b_str.c_str(), str);
}

TEST(SSOStringTest, AssignCStr)
{
	bmstu::wstring str = L"123456789";
	ASSERT_STREQ(str.c_str(), L"123456789");
}

TEST(SSOStringTest, ConcatOpW)
{
	bmstu::wstring a_str(L"right");
	bmstu::wstring b_str(L"_left");
	auto c_str = a_str + b_str;
	ASSERT_STREQ(c_str.c_str(), L"right_left");
}

TEST(SSOStringTest, ConcatOp)
{
	bmstu::string a_str("right");
	bmstu::string b_str("_left");
	auto c_str = a_str + b_str;
	ASSERT_STREQ(c_str.c_str(), "right_left");
}

TEST(SSOStringTest, OStream)
{
	bmstu::string a_str("String value");
	std::stringstream ss;
	ss << a_str;
	ASSERT_STREQ(ss.str().c_str(), "String value");
}

TEST(SSOStringTest, OStreamW)
{
	bmstu::wstring a_str(L"String value");
	std::wstringstream ss;
	ss << a_str;
	ASSERT_STREQ(ss.str().c_str(), L"String value");
}

TEST(SSOStringTest, IStream)
{
	std::stringstream ss;
	ss.str("Value of\nstring");
	bmstu::string a_str;
	std::string a;
	ss >> a_str;
	ASSERT_STREQ(a_str.c_str(), "Value of\nstring");
}

TEST(SSOStringTest, IStreamW)
{
	std::wstringstream ss;
	ss.str(L"Value of\nstring");
	bmstu::wstring a_str;
	ss >> a_str;
	ASSERT_STREQ(a_str.c_str(), L"Value of\nstring");
}

TEST(SSOStringTest, IStreamWNewLine)
{
	std::wstringstream ss;
	ss.str(L"Value of\nstring");
	bmstu::wstring a_str;
	ss >> a_str;
	ASSERT_STREQ(ss.str().c_str(), L"Value of\nstring");
}

TEST(SSOStringTest, ConcatSymbol)
{
	bmstu::wstring a_str;
	a_str += L'S';
	ASSERT_STREQ(a_str.c_str(), L"S");
	a_str += L'a';
	ASSERT_STREQ(a_str.c_str(), L"Sa");
	a_str += L'b';
	ASSERT_STREQ(a_str.c_str(), L"Sab");
	ASSERT_EQ(a_str.size(), 3);
}

TEST(SSOStringTest, Item)
{
	bmstu::wstring a_str(L"TESTWORD");
	ASSERT_EQ(a_str[0], L'T');
	ASSERT_EQ(a_str[1], L'E');
	ASSERT_EQ(a_str[a_str.size() - 1], L'D');
}

TEST(SSOStringTest, SSOShortString)
{
	bmstu::string short_str("short");
	ASSERT_TRUE(short_str.is_using_sso());
	ASSERT_EQ(short_str.size(), 5);
}

TEST(SSOStringTest, SSOEmptyString)
{
	bmstu::string empty_str;
	ASSERT_TRUE(empty_str.is_using_sso());
	ASSERT_EQ(empty_str.size(), 0);
}

TEST(SSOStringTest, SSOLongString)
{
	bmstu::string long_str(
		"This is a very long string that definitely exceeds SSO capacity");
	ASSERT_FALSE(long_str.is_using_sso());
	ASSERT_GT(long_str.size(), 23);
}

TEST(SSOStringTest, SSOBoundaryString)
{
	bmstu::string boundary_str("12345678901234567890123");
	ASSERT_TRUE(boundary_str.is_using_sso());
	ASSERT_EQ(boundary_str.size(), 23);
}

TEST(SSOStringTest, SSOBeyondBoundary)
{
	bmstu::string beyond_str("123456789012345678901234");
	ASSERT_FALSE(beyond_str.is_using_sso());
	ASSERT_EQ(beyond_str.size(), 24);
}

TEST(SSOStringTest, SSOTransitionOnConcat)
{
	bmstu::string str1("short");
	ASSERT_TRUE(str1.is_using_sso());

	str1 += " string that becomes very long";
	ASSERT_FALSE(str1.is_using_sso());
}

TEST(SSOStringTest, SSOWideShortString)
{
	bmstu::wstring short_wstr(L"short");
	ASSERT_TRUE(short_wstr.is_using_sso());
}

TEST(SSOStringTest, SSOWideLongString)
{
	bmstu::wstring long_wstr(L"This is a very long wide string");
	ASSERT_FALSE(long_wstr.is_using_sso());
}

TEST(SSOStringTest, SSOAfterMove)
{
	bmstu::string src("short");
	ASSERT_TRUE(src.is_using_sso());

	bmstu::string dest = std::move(src);
	ASSERT_TRUE(dest.is_using_sso());
	ASSERT_STREQ(dest.c_str(), "short");
}

TEST(SSOStringTest, SSOAfterCopy)
{
	bmstu::string src("short");
	ASSERT_TRUE(src.is_using_sso());

	bmstu::string copy = src;
	ASSERT_TRUE(copy.is_using_sso());
	ASSERT_TRUE(src.is_using_sso());
}

TEST(SSOStringTest, SSOCapacity)
{
	bmstu::string short_str("test");
	ASSERT_TRUE(short_str.is_using_sso());
	ASSERT_GE(short_str.capacity(), short_str.size());

	bmstu::string long_str("This is a very long string");
	ASSERT_FALSE(long_str.is_using_sso());
	ASSERT_GE(long_str.capacity(), long_str.size());
}
