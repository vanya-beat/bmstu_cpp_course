#include "int2str.h"
#include <stdbool.h>
#include <stdlib.h>
#define int_MIN (-2147483647 - 1)
#define int_MAX 2147483647
char* int2str(int number)
{
	char* str = (char*)malloc(12 * sizeof(char));

	bool is_negative = false;
	int i = 0;
	long long int remainder = number;

	if (remainder < 0)
	{
		is_negative = true;
		remainder = -remainder;
	}
	if (remainder == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	while (remainder != 0)
	{
		str[i++] = (remainder % 10) + '0';
		remainder /= 10;
	}
	if (is_negative)
	{
		str[i++] = '-';
	}
	str[i] = '\0';
	for (int j = 0; j < i / 2; j++)
	{
		char temp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = temp;
	}

	return str;
}