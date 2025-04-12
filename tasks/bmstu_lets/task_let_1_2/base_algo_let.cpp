#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <functional>
#include "base_algo_let.h"

std::vector<int> positive_numbers(const std::vector<int>& v) {
    std::vector<int> result;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] > 0) {
            result.push_back(v[i]);
        }
    }
    return result;
}

void sort_positive_numbers(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

int sum_positive_numbers(const std::vector<int>& v) {
    int sum = 0;
    for (const auto& num : v) {
        if (num > 0) {
            sum += num;
        }
    }
    return sum;
}

bool is_divisible_by_10(const std::vector<int>& v) {
    for (const int& num : v) {
        if (num % 10 == 0) {
            return true;
        }
    }
    return false;
}

void replace_negative_numbers(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
                   [](int n) { return (n < 0) ? 0 : n; });
}

void double_values(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
                   std::bind(std::multiplies<int>(), std::placeholders::_1, 2));
}

void sort_students_by_age(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(),
              [](const Student& a, const Student& b) { return a.age < b.age; });
}

void sort_students_by_name(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(),
              [](const Student& a, const Student& b) { return a.name < b.name; });
}
