#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	if (number == INT_MIN)
	{
		char* min_str = malloc(12 * sizeof(char));
		if (min_str == NULL)
			return NULL;
		strcpy(min_str, "-2147483648");
		return min_str;
	}

	int is_negative = number < 0;
	if (is_negative)
	{
		number = -number;
	}

	int num_len = 0;
	int temp = number;
	while (temp > 0)
	{
		num_len++;
		temp /= 10;
	}

	if (is_negative)
	{
		num_len++;
	}

	char* str = malloc((num_len + 1) * sizeof(char));
	if (str == NULL)
		return NULL;

	str[num_len] = '\0';
	int idx = num_len - 1;

	do
	{
		str[idx--] = (number % 10) + '0';
		number /= 10;
	} while (number > 0);

	if (is_negative)
	{
		str[0] = '-';
	}

	return str;
}