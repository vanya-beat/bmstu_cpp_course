#include <algorithm>
#include <numeric>
#include "base_algo_let.h"

std::vector<int> positive_numbers(const std::vector<int>& v)
{
	std::vector<int> result;
	std::copy_if(v.begin(), v.end(), std::back_inserter(result),
				 [](int num) { return num > 0; });
	return result;
}

void sort_positive_numbers(std::vector<int>& v)
{
	std::sort(v.begin(), v.end());
}

int sum_positive_numbers(const std::vector<int>& v)
{
	return std::accumulate(v.begin(), v.end(), 0, [](int sum, int num)
						   { return num > 0 ? sum + num : sum; });
}

bool is_divisible_by_10(const std::vector<int>& v)
{
	return std::all_of(v.begin(), v.end(),
					   [](int num) { return num % 10 == 0; });
}

void replace_negative_numbers(std::vector<int>& v)
{
	std::transform(v.begin(), v.end(), v.begin(),
				   [](int num) { return num < 0 ? 0 : num; });
}

void double_values(std::vector<int>& v)
{
	std::transform(v.begin(), v.end(), v.begin(),
				   [](int num) { return num * 2; });
}

void sort_students_by_age(std::vector<Student>& v)
{
	std::sort(v.begin(), v.end(),
			  [](const Student& a, const Student& b) { return a.age < b.age; });
}

void sort_students_by_name(std::vector<Student>& v)
{
	std::sort(v.begin(), v.end(), [](const Student& a, const Student& b)
			  { return a.name < b.name; });
}