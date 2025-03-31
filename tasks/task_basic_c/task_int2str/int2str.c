#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

char* int2str(int number) {
    // Специальный случай для минимального значения int
    if (number == INT_MIN) {
        char* result = malloc(12);
        if (result) snprintf(result, 12, "%d", INT_MIN);
        return result;
    }

    // Определяем знак и абсолютное значение
    bool is_negative = number < 0;
    unsigned absolute = is_negative ? -number : number;

    // Вычисляем длину числа
    int length = 0;
    for (unsigned n = absolute; n > 0; n /= 10) {
        length++;
    }
    if (absolute == 0) length = 1;  // Обработка нуля

    // Выделяем память (знак + цифры + нуль-терминатор)
    char* str = malloc(length + is_negative + 1);
    if (!str) return NULL;

    // Заполняем строку с конца
    int pos = length + is_negative;
    str[pos--] = '\0';
    
    do {
        str[pos--] = '0' + (absolute % 10);
        absolute /= 10;
    } while (absolute > 0);

    // Добавляем знак минус если нужно
    if (is_negative) {
        str[0] = '-';
    }

    return str;
}