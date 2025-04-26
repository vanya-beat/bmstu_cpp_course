#pragma once

#include <vector>
#include <cstdint>    // Добавлено для uint8_t
#include <string>     // Добавлено для std::string

/*
... ваш комментарий с заданиями остается без изменений ...
*/

struct Student {
  // Сначала объявляем поля класса
  uint8_t age;
  std::string name;

  // Конструктор
  Student(const char* name_, int age_)
      : age(static_cast<uint8_t>(age_)), // Добавлено приведение типа
        name(name_) {
  }

  /// operator ==
  bool operator==(const Student& other) const {
    return age == other.age && name == other.name;
  }
};

// Прототипы функций остаются без изменений
std::vector<int> positive_numbers(const std::vector<int>& v);
void sort_positive_numbers(std::vector<int>& v);
int sum_positive_numbers(const std::vector<int>& v);
bool is_divisible_by_10(const std::vector<int>& v);
void replace_negative_numbers(std::vector<int>& v);
void double_values(std::vector<int>& v);
void sort_students_by_age(std::vector<Student>& v);    // Исправлено на передачу по ссылке
void sort_students_by_name(std::vector<Student>& v);   // Исправлено на передачу по ссылке