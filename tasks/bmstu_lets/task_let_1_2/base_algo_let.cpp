#include "base_algo_let.h"
#include <algorithm>
#include <numeric>

// Используем алгоритм copy_if для фильтрации положительных чисел
std::vector<int> positive_numbers(const std::vector<int>& v)
{
	std::vector<int> result;
	std::copy_if(v.begin(), v.end(), std::back_inserter(result),
				 [](int num) { return num > 0; });
	return result;
}

// Сортировка по возрастанию (можно опустить std::less<int>(), так как это
// значение по умолчанию)
void sort_positive_numbers(std::vector<int>& v)
{
	std::sort(v.begin(), v.end());
}

// Используем алгоритм accumulate для суммирования
int sum_positive_numbers(const std::vector<int>& v)
{
	return std::accumulate(v.begin(), v.end(), 0, [](int sum, int num)
						   { return num > 0 ? sum + num : sum; });
}

// Используем алгоритм any_of для проверки условия
bool is_divisible_by_10(const std::vector<int>& v)
{
	return std::any_of(v.begin(), v.end(),
					   [](int num) { return num % 10 == 0; });
}

// Заменяем отрицательные числа на 0 с помощью transform
void replace_negative_numbers(std::vector<int>& v)
{
	std::transform(v.begin(), v.end(), v.begin(),
				   [](int num) { return num < 0 ? 0 : num; });
}

// Удваиваем значения с помощью transform
void double_values(std::vector<int>& v)
{
	std::transform(v.begin(), v.end(), v.begin(),
				   [](int num) { return num * 2; });
}

// Сортировка студентов по возрасту (остаётся без изменений)
void sort_students_by_age(std::vector<Student>& v)
{
	std::sort(v.begin(), v.end(),
			  [](const Student& a, const Student& b) { return a.age < b.age; });
}

// Сортировка студентов по имени (добавляем проверку на регистр)
void sort_students_by_name(std::vector<Student>& v)
{
	std::sort(v.begin(), v.end(),
			  [](const Student& a, const Student& b)
			  {
				  return std::lexicographical_compare(
					  a.name.begin(), a.name.end(), b.name.begin(),
					  b.name.end(), [](char c1, char c2)
					  { return tolower(c1) < tolower(c2); });
			  });
}