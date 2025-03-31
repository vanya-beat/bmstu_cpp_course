#include "int2str.h"
#include <stdlib.h>

char* int2str(int num)
{
	char* str = (char*)calloc(13, sizeof(char));
	int number_len = 0;

	unsigned int number = 0;
	if (num < 0)
	{
		number_len++;
		str[0] = '-';
		number = num * -1;
	}
	else
	{
		number = num;
	}

	unsigned int number_copy = number;
	int count = 0;
	while ((number_copy == 0 && count < 1) || number_copy > 0)
	{
		number_copy /= 10;
		number_len++;
		count++;
	}

	int digit = 0;
	count = 0;
	char ch = 0;
	while ((number == 0 && count < 1) || number > 0)
	{
		digit = number % 10;
		number = number / 10;
		ch = digit + '0';
		str[number_len - 1 - count] = ch;
		count++;
	}
	str[number_len] = '\0';

	return str;
}
