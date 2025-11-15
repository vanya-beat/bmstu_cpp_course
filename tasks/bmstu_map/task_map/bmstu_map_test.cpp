#include "bmstu_map.h"

#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>

TEST(MapTest, BasicInsertAndAccess)
{
	bmstu::map<int, std::string> map;

	map[1] = "one";
	map[2] = "two";
	map[3] = "three";

	EXPECT_EQ(map[1], "one");
	EXPECT_EQ(map[2], "two");
	EXPECT_EQ(map[3], "three");
	EXPECT_EQ(map.size(), 3);
}

TEST(MapTest, InsertAndFind)
{
	bmstu::map<std::string, int> map;

	map["apple"] = 5;
	map["banana"] = 10;
	map["cherry"] = 15;

	auto result1 = map.find("apple");
	EXPECT_NE(result1, nullptr);
	EXPECT_EQ(*result1, 5);

	auto result2 = map.find("banana");
	EXPECT_NE(result2, nullptr);
	EXPECT_EQ(*result2, 10);

	auto result3 = map.find("grape");
	EXPECT_EQ(result3, nullptr);
}

TEST(MapTest, Contains)
{
	bmstu::map<int, double> map;

	map[1] = 1.5;
	map[2] = 2.5;
	map[3] = 3.5;

	EXPECT_TRUE(map.contains(1));
	EXPECT_TRUE(map.contains(2));
	EXPECT_TRUE(map.contains(3));
	EXPECT_FALSE(map.contains(4));
	EXPECT_FALSE(map.contains(0));
}

TEST(MapTest, Erase)
{
	bmstu::map<int, std::string> map;

	map[5] = "five";
	map[6] = "six";
	map[7] = "seven";
	map[8] = "eight";

	EXPECT_EQ(map.size(), 4);
	EXPECT_TRUE(map.contains(6));

	map.erase(6);

	EXPECT_EQ(map.size(), 3);
	EXPECT_FALSE(map.contains(6));
	EXPECT_TRUE(map.contains(5));
	EXPECT_TRUE(map.contains(7));
	EXPECT_TRUE(map.contains(8));
}

TEST(MapTest, AtMethod)
{
	bmstu::map<int, std::string> map;

	map[1] = "one";
	map[2] = "two";

	EXPECT_EQ(map.at(1), "one");
	EXPECT_EQ(map.at(2), "two");

	EXPECT_THROW(map.at(3), std::out_of_range);
}

TEST(MapTest, EmptyMap)
{
	bmstu::map<int, int> map;

	EXPECT_TRUE(map.empty());
	EXPECT_EQ(map.size(), 0);

	map[1] = 100;

	EXPECT_FALSE(map.empty());
	EXPECT_EQ(map.size(), 1);
}

TEST(MapTest, Clear)
{
	bmstu::map<int, int> map;

	map[1] = 10;
	map[2] = 20;
	map[3] = 30;

	EXPECT_EQ(map.size(), 3);
	EXPECT_FALSE(map.empty());

	map.clear();

	EXPECT_EQ(map.size(), 0);
	EXPECT_TRUE(map.empty());
	EXPECT_FALSE(map.contains(1));
}

TEST(MapTest, UpdateValues)
{
	bmstu::map<std::string, int> map;

	map["counter"] = 0;
	EXPECT_EQ(map["counter"], 0);

	map["counter"] = 5;
	EXPECT_EQ(map["counter"], 5);

	map["counter"] = 10;
	EXPECT_EQ(map["counter"], 10);
}

TEST(MapTest, StringKeyValue)
{
	bmstu::map<std::string, std::string> map;

	map["watermelon"] = "fruit";
	map["potato"] = "vegetable";
	map["tomato"] = "vegetable";
	map["apple"] = "fruit";
	map["cherry"] = "berry";

	EXPECT_EQ(map["watermelon"], "fruit");
	EXPECT_EQ(map["potato"], "vegetable");
	EXPECT_EQ(map["tomato"], "vegetable");
	EXPECT_EQ(map["apple"], "fruit");
	EXPECT_EQ(map["cherry"], "berry");

	EXPECT_EQ(map.size(), 5);
}

TEST(MapTest, AVLBalancing)
{
	bmstu::map<int, int> map;

	for (int i = 1; i <= 10; ++i)
	{
		map[i] = i * 10;
	}

	EXPECT_EQ(map.size(), 10);

	for (int i = 1; i <= 10; ++i)
	{
		EXPECT_TRUE(map.contains(i));
		EXPECT_EQ(map[i], i * 10);
	}
}

TEST(MapTest, EraseWithBalancing)
{
	bmstu::map<double, std::string> map;

	map[5.0] = "five";
	map[6.0] = "six";
	map[7.0] = "seven";
	map[8.0] = "eight";
	map[7.5] = "seven-half";
	map[4.0] = "four";

	std::cout << "After inserting all elements:\n";
	map.print();

	map.erase(6.0);
	std::cout << "After removing 6:\n";
	map.print();

	map.erase(7.0);
	std::cout << "After removing 7:\n";
	map.print();

	EXPECT_EQ(map.size(), 4);
	EXPECT_FALSE(map.contains(6.0));
	EXPECT_FALSE(map.contains(7.0));
	EXPECT_TRUE(map.contains(5.0));
	EXPECT_TRUE(map.contains(8.0));
	EXPECT_TRUE(map.contains(7.5));
	EXPECT_TRUE(map.contains(4.0));
}

TEST(MapTest, IteratorBasic)
{
	bmstu::map<int, std::string> map;

	map[3] = "three";
	map[1] = "one";
	map[4] = "four";
	map[2] = "two";
	map[5] = "five";

	std::vector<int> keys;
	std::vector<std::string> values;

	for (auto it = map.begin(); it != map.end(); ++it)
	{
		keys.push_back((*it).first);
		values.push_back((*it).second);
	}

	EXPECT_EQ(keys.size(), 5);
	EXPECT_EQ(keys[0], 1);
	EXPECT_EQ(keys[1], 2);
	EXPECT_EQ(keys[2], 3);
	EXPECT_EQ(keys[3], 4);
	EXPECT_EQ(keys[4], 5);

	EXPECT_EQ(values[0], "one");
	EXPECT_EQ(values[1], "two");
	EXPECT_EQ(values[2], "three");
	EXPECT_EQ(values[3], "four");
	EXPECT_EQ(values[4], "five");
}

TEST(MapTest, RangeBasedFor)
{
	bmstu::map<std::string, int> map;

	map["apple"] = 5;
	map["banana"] = 10;
	map["cherry"] = 15;
	map["date"] = 20;

	std::vector<std::string> keys;
	std::vector<int> values;

	for (const auto& [key, value] : map)
	{
		keys.push_back(key);
		values.push_back(value);
	}

	EXPECT_EQ(keys.size(), 4);
	EXPECT_EQ(keys[0], "apple");
	EXPECT_EQ(keys[1], "banana");
	EXPECT_EQ(keys[2], "cherry");
	EXPECT_EQ(keys[3], "date");

	EXPECT_EQ(values[0], 5);
	EXPECT_EQ(values[1], 10);
	EXPECT_EQ(values[2], 15);
	EXPECT_EQ(values[3], 20);
}

TEST(MapTest, EmptyRangeIteration)
{
	bmstu::map<int, int> map;

	int count = 0;
	for (const auto& [key, value] : map)
	{
		++count;
	}

	EXPECT_EQ(count, 0);
}

TEST(MapTest, SingleElementIteration)
{
	bmstu::map<int, std::string> map;
	map[42] = "answer";

	int count = 0;
	for (const auto& [key, value] : map)
	{
		EXPECT_EQ(key, 42);
		EXPECT_EQ(value, "answer");
		++count;
	}

	EXPECT_EQ(count, 1);
}

TEST(MapTest, IteratorNumericKeys)
{
	bmstu::map<double, int> map;

	map[3.14] = 314;
	map[2.71] = 271;
	map[1.41] = 141;
	map[1.73] = 173;

	std::vector<double> keys;
	for (const auto& [key, value] : map)
	{
		keys.push_back(key);
	}

	EXPECT_EQ(keys.size(), 4);
	EXPECT_DOUBLE_EQ(keys[0], 1.41);
	EXPECT_DOUBLE_EQ(keys[1], 1.73);
	EXPECT_DOUBLE_EQ(keys[2], 2.71);
	EXPECT_DOUBLE_EQ(keys[3], 3.14);
}

TEST(MapTest, IteratorArrowOperator)
{
	bmstu::map<int, std::string> map;

	map[1] = "hello";
	map[2] = "world";

	for (auto it = map.begin(); it != map.end(); ++it)
	{
		EXPECT_TRUE(it->first == 1 || it->first == 2);
		EXPECT_FALSE(it->second.empty());
	}
}

TEST(MapTest, RangeBasedForDemo)
{
	bmstu::map<std::string, std::string> catalog;

	catalog["watermelon"] = "fruit";
	catalog["potato"] = "vegetable";
	catalog["tomato"] = "vegetable";
	catalog["apple"] = "fruit";
	catalog["cherry"] = "berry";

	std::cout << "\n=== Product Catalog (sorted by name) ===\n";
	for (const auto& [product, category] : catalog)
	{
		std::cout << "  " << product << " -> " << category << "\n";
	}
	std::cout << "=========================================\n";

	EXPECT_EQ(catalog.size(), 5);
	EXPECT_EQ(catalog["apple"], "fruit");
}