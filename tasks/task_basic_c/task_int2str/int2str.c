#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char* int2str(int number) {
    // Специальный случай для минимального значения int
    if (number == INT_MIN) {
        char* result = malloc(12); // "-2147483648" + '\0'
        if (result == NULL) return NULL;
        sprintf(result, "%d", INT_MIN);
        return result;
    }

    // Определяем знак числа
    bool is_negative = number < 0;
    if (is_negative) {
        number = -number;
    }

    // Вычисляем длину числа
    int length = 0;
    int temp = number;
    do {
        temp /= 10;
        length++;
    } while (temp != 0);

    // Выделяем память (знак + цифры + нуль-терминатор)
    char* str = malloc(length + is_negative + 1);
    if (str == NULL) return NULL;

    // Заполняем строку с конца
    int pos = length + is_negative;
    str[pos--] = '\0';
    
    do {
        str[pos--] = (number % 10) + '0';
        number /= 10;
    } while (number != 0);

    // Добавляем знак минус если нужно
    if (is_negative) {
        str[0] = '-';
    }

    return str;
}

// Тесты
void test_int2str() {
    assert(strcmp(int2str(0), "0") == 0);
    assert(strcmp(int2str(123), "123") == 0);
    assert(strcmp(int2str(-456), "-456") == 0);
    assert(strcmp(int2str(INT_MAX), "2147483647") == 0);
    assert(strcmp(int2str(INT_MIN), "-2147483648") == 0);
}

int main() {
    test_int2str();
    return 0;
}