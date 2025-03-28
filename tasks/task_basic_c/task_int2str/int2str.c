#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

char* int2str(int number)
{
	if (number == 0)
	{
		char* zero_str = malloc(2 * sizeof(char));
		if (zero_str == NULL)
			return NULL;
		zero_str[0] = '0';
		zero_str[1] = '\0';
		return zero_str;
	}

	// Обработка INT_MIN
	if (number == INT_MIN)
	{
		return strdup("-2147483648");
	}

	int is_negative = number < 0;
	if (is_negative)
	{
		number = -number;
	}

	int num_len = 0;
	int temp = number;
	do
	{
		num_len++;
		temp /= 10;
	} while (temp > 0);

	// Учитываем знак
	if (is_negative)
	{
		num_len++;
	}

	char* str = malloc((num_len + 1) * sizeof(char));
	if (str == NULL)
		return NULL;

	str[num_len] = '\0';
	int idx = num_len - 1;

	while (number > 0)
	{
		str[idx--] = (number % 10) + '0';
		number /= 10;
	}

	if (is_negative)
	{
		str[idx] = '-';
	}

	return str;
}