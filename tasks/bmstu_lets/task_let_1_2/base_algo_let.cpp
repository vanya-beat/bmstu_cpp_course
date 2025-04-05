#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& v)
{
	std::vector<int> expected;
	for (int i : v)
	{
		if (i > 0)
		{
			expected.push_back(i);
		}
	}
	return {expected};
}
void sort_positive_numbers(std::vector<int>& v)
{
	std::sort(v.begin(), v.end());
}
int sum_positive_numbers(const std::vector<int>& v)
{
	int sum = 0;
	for (int i : v)
	{
		if (i > 0)
		{
			sum += i;
		}
	}
	return sum;
}
bool is_divisible_by_10(const std::vector<int>& v)
{
	return std::any_of(v.begin(), v.end(),
					   [](int num) { return num % 10 == 0; });
}
void replace_negative_numbers(std::vector<int>& v)
{
	for (int& i : v)
	{
		if (i < 0)
		{
			i = 0;
		}
	}
	return;
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
