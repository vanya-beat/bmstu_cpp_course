#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include "base_algo_let.h"


std::vector<int> positive_numbers(const std::vector<int>& v) {
    std::vector<int> positives;
    std::copy_if(v.begin(), v.end(), std::back_inserter(positives), [](int num) {
        return num > 0;
    });
    return positives;
}


void sort_positive_numbers(std::vector<int>& v) {
    std::vector<int> positives = positive_numbers(v);
    std::sort(positives.begin(), positives.end());
    size_t pos_index = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] > 0) {
            v[i] = positives[pos_index++];
        }
    }
}


int sum_positive_numbers(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0, [](int sum, int num) {
        return num > 0 ? sum + num : sum;
    });
}


bool is_divisible_by_10(const std::vector<int>& v) {
    int sum = sum_positive_numbers(v);
    return sum % 10 == 0;
}


void replace_negative_numbers(std::vector<int>& v) {
    std::replace_if(v.begin(), v.end(), [](int num) {
        return num < 0;
    }, 0);
}


void double_values(std::vector<int>& v){
    std::transform(v.begin(), v.end(), v.begin(), [](int num) {
        return num * 2;
    });
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
