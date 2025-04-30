#include <algorithm>
#include <numeric>
#include "base_algo_let.h"

std::vector<int> positive_numbers(const std::vector<int>& v)
{
    std::vector<int> result;
    std::remove_copy_if(v.begin(), v.end(), std::back_inserter(result),
        [](int num) { return num <= 0; });
    return result;
}

void sort_positive_numbers(std::vector<int>& v)
{
    std::sort(v.begin(), v.end(), std::less<int>());
}

int sum_positive_numbers(const std::vector<int>& v)
{
    int sum = 0;
    std::for_each(v.begin(), v.end(), 
        [&sum](int num) { if (num > 0) sum += num; });
    return sum;
}

bool is_divisible_by_10(const std::vector<int>& v)
{
    return std::find_if(v.begin(), v.end(),
        [](int num) { return num % 10 == 0; }) != v.end();
}

void replace_negative_numbers(std::vector<int>& v)
{
    std::for_each(v.begin(), v.end(),
        [](int& num) { if (num < 0) num = 0; });
}

void double_values(std::vector<int>& v)
{
    for (auto& num : v) {
        num *= 2;
    }
}

void sort_students_by_age(std::vector<Student>& v)
{
    std::stable_sort(v.begin(), v.end(),
        [](const Student& a, const Student& b) { return a.age < b.age; });
}

void sort_students_by_name(std::vector<Student>& v)
{
    auto compare = [](const Student& a, const Student& b) {
        std::string a_name, b_name;
        std::transform(a.name.begin(), a.name.end(), std::back_inserter(a_name), ::tolower);
        std::transform(b.name.begin(), b.name.end(), std::back_inserter(b_name), ::tolower);
        return a_name < b_name;
    };
    std::sort(v.begin(), v.end(), compare);
}
