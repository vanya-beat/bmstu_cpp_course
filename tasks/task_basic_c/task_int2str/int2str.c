#include "int2str.h"
#include <stdlib.h>

char* int2str(int num)
{
	char* str = (char*)calloc(13, sizeof(char));

	int number_len = 0;

	unsigned int number = abs(num);

	if (num < 0)
	{
		str[number_len++] = '-';
	}
	else if (num == 0)
	{
		str[number_len++] = '0';
	}

	unsigned int temp = number;
	if (number == 0)
	{
		number_len++;
	}
	else
	{
		while (temp > 0)
		{
			number_len++;
			temp /= 10;
		}
	}

	str[number_len] = '\0';
	while (number > 0)
	{
		str[--number_len] = (number % 10) + '0';
		number /= 10;
	}

	return str;
}