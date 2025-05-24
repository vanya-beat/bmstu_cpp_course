#include "base_algo_let.h"
#include <algorithm>

std::vector<int> positive_numbers(const std::vector<int>& v)
// сохранение положительных чисел в другой массив
{
    std::vector<int> pos_numbers (v.size());
    auto end_pos_iter = std::copy_if(begin(v), end(v), begin(pos_numbers), [] (int x) {return x>0;} );
    pos_numbers.erase(end_pos_iter, end(pos_numbers));
    return pos_numbers;
};


void sort_positive_numbers(std::vector<int>& v) 
// Отсортировать массив положительных чисел в порядке убывания.
{
    std::sort(v.begin(), v.end());
};

int sum_positive_numbers(const std::vector<int>& v)
// Найти сумму всех элементов массива положительных чисел.
{
	int rez=0;
    for (const int& item :v)
    {
        if (item > 0) rez= rez + item;
    }
    return rez;
}

bool is_divisible_by_10(const std::vector<int>& v)
// Проверить, есть ли в исходном массиве хотя бы одно число, кратное 10.
{
	bool rez=false;
    for (const int& item :v)
    {
        if (item % 10 == 0) rez = true;
    }
    return rez;
}

void replace_negative_numbers(std::vector<int>& v)
// Заменить все отрицательные числа в исходном массиве нулями.
{
    std::replace_if (v.begin(), v.end(), [] (int x) { return x < 0; }, 0);
    return;
}

void double_values(std::vector<int>& v)
// Удвоить все значения оставшихся элементов массива (модифицировать массив "на месте")
{
    std::transform(v.begin(), v.end(), v.begin(), [] (int c) {return c*2;});
    return;
}

void sort_students_by_age(std::vector<Student>& v)
//  Отсортировать массив студентов по возрасту
{
    std::sort(v.begin(), v.end(), [](Student a, Student b) { return (a.age < b.age); });
	return;
}

void sort_students_by_name(std::vector<Student>& v) 
// Отсортировать массив студентов по имени
{
    std::sort(v.begin(), v.end(), [](Student a, Student b) { return (a.name < b.name); });
    return;
}
