#include <assert.h>

#include "stdio.h"
#include "str2int.h"

int str2int(const char* str)
{
	int das = 1;
	int result = 0;

	// Обработка знака числа
	if (*str == '-')
	{
		das = -1;
		str++;
	}
	else if (*str == '+')
	{
		str++;
	}

	int digit_count = 0;

	// Обработка каждой цифры числа
	while (*str != '\0')
	{
		result = result * 10 + (*str - '0');
		digit_count++;
		str++;

		// Проверки корректности промежуточного результата
		assert((das == -1 && result == -2147483648) ||
			   (das == -1 && result >= 0 && digit_count <= 9) ||
			   (das == 1 && result >= 0));
	}

	// Проверка что были обработаны цифры
	assert(digit_count > 0);

	return result * das;
}