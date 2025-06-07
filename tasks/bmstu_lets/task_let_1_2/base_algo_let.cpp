#include "base_algo_let.h"
#include <algorithm>
#include <numeric>

bool compare_numbers(size_t n1, size_t n2) {
  return n1 < n2;
}
std::vector<int> positive_numbers(const std::vector<int>& v) {
  std::vector<int> pn;
  for (auto i : v) if (i > 0) pn.push_back(i);
  return pn;
}
void sort_positive_numbers(std::vector<int>& v) {
  std::sort(v.begin(), v.end(), compare_numbers);
}
int sum_positive_numbers(const std::vector<int>& v) {
	std::vector<int> pos_Student = positive_numbers(v);
	return std::accumulate(pos_Student.begin(), pos_Student.end(), 0);
}
bool is_divisible_by_10(const std::vector<int>& v) {
  for (auto i : v) if (i % 10 == 0 && i != 0) return true;
  return false;
}
void replace_negative_numbers(std::vector<int>& v) {
  for (auto& i: v) {
	  if (i < 0) {
		  i = 0;
	  }
  }
}
void double_values(std::vector<int>& v) {
  for (auto& i : v) {
	  if (i > 0) {
		  i *= 2;
	  }
  }
}
bool compare_students_by_age(const Student& a, const Student& b) {
	return a.age < b.age;
}
bool compare_students_by_name(const Student& a, const Student& b) {
	return a.name < b.name;
}
void sort_students_by_age(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), compare_students_by_age);
}
void sort_students_by_name(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), compare_students_by_name);
}