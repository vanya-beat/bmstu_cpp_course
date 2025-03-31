#include "base_algo_let.h"
#include <algorithm>
#include <numeric>


std::vector<int> positive_numbers(const std::vector<int>& v)
{
	std::vector<int> result(v.size());
	auto end = std::copy_if(v.begin(), v.end(), result.begin(),
							[](int x) { return x > 0; });
	result.resize(std::distance(result.begin(), end));
	return result;
}

void sort_positive_numbers(std::vector<int>& v)
{
	std::sort(v.begin(), v.end(), std::less<>());
}

int sum_positive_numbers(const std::vector<int>& v)
{
	return std::accumulate(v.begin(), v.end(), 0,
						   [](int a, int b) { return b > 0 ? a + b : a; });
}

bool is_divisible_by_10(const std::vector<int>& v)
{
	return v.end() !=
		   std::find_if(v.begin(), v.end(), [](int x) { return x % 10 == 0; });
}

void replace_negative_numbers(std::vector<int>& v)
{
	std::for_each(v.begin(), v.end(),
				  [](int& x)
				  {
					  if (x < 0)
						  x = 0;
				  });
}

void double_values(std::vector<int>& v)
{
	std::transform(v.begin(), v.end(), v.begin(), [](int x) { return x * 2; });
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