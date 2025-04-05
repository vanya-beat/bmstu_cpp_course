#include "base_algo_let.h"
#include <algorithm>
#include <numeric>

// 1. Сохранить в отдельный массив только положительные числа
std::vector<int> positive_numbers(const std::vector<int>& v) {
    std::vector<int> result;
    for (int num : v) {
        if (num > 0) {
            result.push_back(num);
        }
    }
    return result;
}

// 2. Отсортировать положительные числа по возрастанию (в тесте проверка на ascending)
void sort_positive_numbers(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

// 3. Сумма положительных чисел
int sum_positive_numbers(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0,
        [](int sum, int num) { return num > 0 ? sum + num : sum; });
}

// 4. Проверка на наличие чисел кратных 10
bool is_divisible_by_10(const std::vector<int>& v) {
    return std::any_of(v.begin(), v.end(),
        [](int num) { return num % 10 == 0; });
}

// 5. Замена отрицательных чисел нулями
void replace_negative_numbers(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](int num) { return num < 0 ? 0 : num; });
}

// 6. Удвоение всех значений
void double_values(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](int num) { return num * 2; });
}

// 7. Сортировка студентов по возрасту
void sort_students_by_age(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(),
        [](const Student& a, const Student& b) { return a.age < b.age; });
}

// 8. Сортировка студентов по имени
void sort_students_by_name(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(),
        [](const Student& a, const Student& b) { return a.name < b.name; });
}

