#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Student {
    std::string name;
    int age;
};

// Функция для сортировки студентов по имени и возрасту
bool compareStudents(const Student &a, const Student &b) {
    if (a.name == b.name) {
        return a.age < b.age; // Сортировка по возрасту, если имена одинаковые
    }
    return a.name < b.name; // Сортировка по имени
}

int main() {
    std::vector<int> originalArray = { -5, 3, 0, 7, -2, 10, 15, -1, 20 };
    
    // 1. Сохранить в отдельный массив только положительные числа
    std::vector<int> positiveNumbers;
    for (int num : originalArray) {
        if (num > 0) {
            positiveNumbers.push_back(num);
        }
    }

    // 2. Отсортировать массив положительных чисел в порядке убывания
    std::sort(positiveNumbers.begin(), positiveNumbers.end(), std::greater<int>());

    // 3. Найти сумму всех элементов массива положительных чисел
    int sumPositive = 0;
    for (int num : positiveNumbers) {
        sumPositive += num;
    }

    // 4. Проверить, есть ли в исходном массиве хотя бы одно число, кратное 10
    bool hasMultipleOfTen = false;
    for (int num : originalArray) {
        if (num % 10 == 0 && num != 0) {
            hasMultipleOfTen = true;
            break;
        }
    }

    // 5. Заменить все отрицательные числа в исходном массиве нулями
    for (int &num : originalArray) {
        if (num < 0) {
            num = 0;
        }
    }

    // 6. Удвоить все значения оставшихся элементов массива
    for (int &num : originalArray) {
        num *= 2;
    }

    // 7. Удостоверимся, что у нас есть массив студентов
    std::vector<Student> students = {
        {"Alice", 20},
        {"Bob", 22},
        {"Charlie", 20},
        {"Alice", 22},
        {"Bob", 20}
    };

    // Сортировка массива студентов
    std::sort(students.begin(), students.end(), compareStudents);

    // Вывод результатов
    std::cout << "Положительные числа в порядке убывания: ";
    for (int num : positiveNumbers) {
        std::cout << num << " ";
    }
    std::cout << "\nСумма положительных чисел: " << sumPositive << std::endl;
    std::cout << "Есть ли число, кратное 10: " << (hasMultipleOfTen ? "Да" : "Нет") << std::endl;

    std::cout << "Измененный массив: ";
    for (int num : originalArray) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "Отсортированные студенты:\n";
    for (const auto &student : students) {
        std::cout << "Имя: " << student.name << ", Возраст: " << student.age << std::endl;
    }

    return 0;
}