#pragma once
#include <vector>
#include <string>
#include <cstdint>

/*
1. Сохранить в отдельный массив только положительные числа из исходного массива.
2. Отсортировать массив положительных чисел в порядке убывания.
3. Найти сумму всех элементов массива положительных чисел.
4. Проверить, есть ли в исходном массиве хотя бы одно число, кратное 10.
5. Заменить все отрицательные числа в исходном массиве нулями.
6. Удвоить все значения оставшихся элементов массива (модифицировать массив
"наместе").
7. Отсортировать массив студентов по имени и по возрасту
 */

struct Student {
  Student(const char* name_, int age_)
      : age(age_),
        name(name_) {
  }
  /// operator ==
  bool operator==(const Student& other) const {
    return other.age == age && other.name == name;
  }
  uint8_t age;
  std::string name;
};
std::vector<int> positive_numbers(const std::vector<int>& v);
void sort_positive_numbers(std::vector<int>& v);
int sum_positive_numbers(const std::vector<int>& v);
bool is_divisible_by_10(const std::vector<int>& v);
void replace_negative_numbers(std::vector<int>& v);
void double_values(std::vector<int>& v);
void sort_students_by_age(std::vector<Student>& v);
void sort_students_by_name(std::vector<Student>& v);
