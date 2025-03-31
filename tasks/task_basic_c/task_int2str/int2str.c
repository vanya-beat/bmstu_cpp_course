#include <stdlib.h>
#include "int2str.h"

static int calculate_number_len(unsigned int number)
{
	int length = 0;
	do
	{
		length++;
		number /= 10;
	} while (number > 0);
	return length;
}

char* int2str(int num)
{
	int is_negative = num < 0;
	unsigned int number = is_negative ? -num : num;
	int number_len = calculate_number_len(number);
	int str_len = number_len + (is_negative ? 1 : 0) + 1;
	char* str = (char*)calloc(str_len, sizeof(char));

	if (str == NULL)
	{
		return NULL;
	}

	if (is_negative)
	{
		str[0] = '-';
	}

	for (int i = number_len - 1; i >= 0; i--)
	{
		str[is_negative ? i + 1 : i] = (number % 10) + '0';
		number /= 10;
	}

	str[str_len - 1] = '\0';

	return str;
}