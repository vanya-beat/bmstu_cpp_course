#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

int str2int(const char* str) 
{
    // Проверка на NULL указатель
    assert(str != '0');
    
    // Пропускаем ведущие пробелы (если нужно)
    while (isspace(*str)) {
        str++;
    }

    // Определяем знак числа
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } 
    else if (*str == '+') {
        str++;
    }

    // Проверяем, что после знака есть цифры
    assert(isdigit(*str));

    int result = 0;
    bool overflow = false;
    const int max_safe = INT_MAX / 10;
    const int last_digit_max = INT_MAX % 10;

    while (isdigit(*str)) {
        int digit = *str - '0';
        
        // Проверка на переполнение
        if (result > max_safe || 
            (result == max_safe && digit > last_digit_max)) {
            overflow = true;
            break;
        }

        result = result * 10 + digit;
        str++;
    }

    // Проверка на корректное завершение строки
    assert(*str == '\0');
    assert(!overflow);

    // Применяем знак с проверкой на INT_MIN
    if (sign == -1) {
        assert(result <= INT_MAX); // INT_MIN обрабатывается особо
        return (result == INT_MAX) ? INT_MIN : -result;
    }

    return result;
}