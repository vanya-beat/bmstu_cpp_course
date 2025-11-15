#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& v)
{
	auto r = std::vector<int>{};
	std::for_each(v.cbegin(), v.cend(),
				  [&r](int n)
				  {
					  if (n > 0)
						  r.emplace_back(n);
				  });
	return r;
}

void sort_positive_numbers(std::vector<int>& v)
{
	std::sort(v.begin(), v.end());
}

int sum_positive_numbers(const std::vector<int>& v)
{
	auto s = 0;
	std::for_each(v.cbegin(), v.cend(), [&s](int n) { s += n > 0 ? n : 0; });
	return s;
}

bool is_divisible_by_10(const std::vector<int>& v)
{
	auto found =
		std::find_if(v.cbegin(), v.cend(), [](int n) { return n % 10 == 0; });
	return found != v.cend();
}

void replace_negative_numbers(std::vector<int>& v)
{
	std::for_each(v.begin(), v.end(), [](int& n) { n = n < 0 ? 0 : n; });
}

void double_values(std::vector<int>& v)
{
	std::for_each(v.begin(), v.end(), [](int& n) { n = n * 2; });
}

void sort_students_by_age(std::vector<Student>& v)
{
	auto cmp = [](const Student& x, const Student& y) { return x.age < y.age; };
	std::sort(v.begin(), v.end(), cmp);
}

void sort_students_by_name(std::vector<Student>& v)
{
	auto cmp = [](const Student& x, const Student& y)
	{ return x.name < y.name; };
	std::sort(v.begin(), v.end(), cmp);
}