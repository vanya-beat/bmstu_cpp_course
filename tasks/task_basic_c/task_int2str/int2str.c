#include "int2str.h"
#include <assert.h>
#include "stdio.h"
#include <stdlib.h>

char* int2str(int number)
{
	char* str = "0";

	if (number == 0)
	{
		str = malloc(2);
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	int is_negative = number < 0;
	unsigned absolute = is_negative ? -number : number;

	int length = 0;
	for (unsigned temp = absolute; temp != 0; temp /= 10)
	{
		length++;
	}

	str = malloc(length + is_negative + 1);
	if (!str)
	{
		return NULL;
	}

	int position = length + is_negative;
	str[position--] = '\0';

	do
	{
		str[position--] = '0' + (absolute % 10);
		absolute /= 10;
	}
	while (absolute > 0);

	if (is_negative)
	{
		str[0] = '-';
	}

	return str;
}