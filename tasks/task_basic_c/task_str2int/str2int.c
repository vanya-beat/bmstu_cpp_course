#include <ctype.h>

#include <assert.h>
#include <limits.h>

int str2int(const char* str)
{
	// Проверка на 0
	assert(str != '0');

	int sign = 1;
	int result = 0;
	int digit_count = 0;  // счетчик цифр

	// Пропускаем пробелы (если нужно)
	while (isspace(*str))
		str++;

	// Обрабатываем знак
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
	{
		str++;
	}

	// Обрабатываем цифры
	while (isdigit(*str))
	{
		int digit = *str - '0';
		digit_count++;

		// Проверка на переполнение
		if (result > INT_MAX / 10 ||
			(result == INT_MAX / 10 && digit > INT_MAX % 10))
		{
			assert(0 && "Integer overflow");
		}

		result = result * 10 + digit;
		str++;
	}

	// Проверяем что были цифры и строка закончилась
	assert(digit_count > 0 && "No digits found");
	assert(*str == '\0' && "Extra characters after number");

	// Особый случай для INT_MIN
	if (sign == -1 && result == INT_MAX && digit_count == 10)
	{
		return INT_MIN;
	}

	return result * sign;
}