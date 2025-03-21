<<<<<<< HEAD
// Copyright 2025 <ShuranovArtem>
#include <algorithm>
#include <numeric>
#include "base_algo_let.h"

std::vector<int> positive_numbers(const std::vector<int>& v) {
  std::vector<int> positives;
    std::copy_if(v.begin(), v.end(), std::back_inserter(positives),
      [](int i) { return i > 0; });
    return positives;
}
void sort_positive_numbers(std::vector<int>& v) {
  std::sort(v.begin(), v.end());
}
int sum_positive_numbers(const std::vector<int>& v) {
  return std::accumulate(v.begin(), v.end(), 0,
    [](int sum, int i) { return i > 0 ? sum + i : sum; });
}
bool is_divisible_by_10(const std::vector<int>& v) {
  return std::any_of(v.begin(), v.end(), [](int i) { return i % 10 == 0; });
}
void replace_negative_numbers(std::vector<int>& v) {
  std::replace_if(v.begin(), v.end(), [](int i) { return i < 0; }, 0);
}
void double_values(std::vector<int>& v) {
  std::transform(v.begin(), v.end(), v.begin(), [](int i) { return i * 2; });
}
void sort_students_by_age(std::vector<Student>& v) {
     std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.age < b.age;
    });
}
void sort_students_by_name(std::vector<Student>& v) {
      std::sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
}
=======
#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& v)
{
	return {};
}
void sort_positive_numbers(std::vector<int>& v) {}
int sum_positive_numbers(const std::vector<int>& v)
{
	return 0;
}
bool is_divisible_by_10(const std::vector<int>& v)
{
	return false;
}
void replace_negative_numbers(std::vector<int>& v)
{
	return;
}
void double_values(std::vector<int>& v)
{
	return;
}
void sort_students_by_age(std::vector<Student>& v)
{
	return;
}
void sort_students_by_name(std::vector<Student>& v) {}
>>>>>>> f11ac894e72424eea42e968c1821c9aaf87062ce
