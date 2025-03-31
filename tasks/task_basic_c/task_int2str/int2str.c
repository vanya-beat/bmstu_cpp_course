#include <assert.h>
#include <stdlib.h>
#include "int2str.h"
#include "stdio.h"

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

	int neg = number < 0;
	unsigned num = neg ? -number : number;

	int len = 0;
	for (unsigned t = num; t; t /= 10)
		len++;

	str = malloc(len + neg + 1);
	if (!str)
		return NULL;

	int pos = len + neg;
	str[pos--] = '\0';

	do
	{
		str[pos--] = '0' + num % 10;
		num /= 10;
	} while (num);

	if (neg)
		str[0] = '-';
	return str;
}