#include "gtest/gtest.h"

#include "base_algo_let.h"

TEST(BaseAlgoLet, PositiveNumbers) {
  std::vector<int> v = {1, -2, 3, -4, 5};
  std::vector<int> expected = {1, 3, 5};
  ASSERT_EQ(positive_numbers(v), expected);
}

TEST(BaseAlgoLet, SortPositiveNumbers) {
  std::vector<int> v = {3, 1, 4, 1, 5, 9};
  std::vector<int> expected = {1, 1, 3, 4, 5, 9};
  sort_positive_numbers(v);
  ASSERT_EQ(v, expected);
}

TEST(BaseAlgoLet, SumPositiveNumbers) {
  std::vector<int> v = {1, -2, 3, -4, 5};
  int expected = 9;
  ASSERT_EQ(sum_positive_numbers(v), expected);
}

TEST(BaseAlgoLet, IsDivisibleBy10) {
  std::vector<int> v1 = {10, 20, 30};
  std::vector<int> v2 = {1, 2, 3};
  ASSERT_TRUE(is_divisible_by_10(v1));
  ASSERT_FALSE(is_divisible_by_10(v2));
}

TEST(BaseAlgoLet, ReplaceNegativeNumbers) {
  std::vector<int> v = {1, -2, 3, -4, 5};
  std::vector<int> expected = {1, 0, 3, 0, 5};
  replace_negative_numbers(v);
  ASSERT_EQ(v, expected);
}

TEST(BaseAlgoLet, DoubleValues) {
  std::vector<int> v = {1, 2, 3};
  std::vector<int> expected = {2, 4, 6};
  double_values(v);
  ASSERT_EQ(v, expected);
}

TEST(BaseAlgoLet, SortStudentsByAge) {
  std::vector<Student> v = {{"Alice", 20}, {"Bob", 18}, {"Charlie", 22}};
  std::vector<Student> expected = {{"Bob", 18}, {"Alice", 20}, {"Charlie", 22}};
  sort_students_by_age(v);
  ASSERT_EQ(v, expected);
}

TEST(BaseAlgoLet, SortStudentsByName) {
  std::vector<Student> v = {{"Charlie", 22}, {"Alice", 20}, {"Bob", 18}};
  std::vector<Student> expected = {{"Alice", 20}, {"Bob", 18}, {"Charlie", 22}};
  sort_students_by_name(v);
  ASSERT_EQ(v, expected);
}