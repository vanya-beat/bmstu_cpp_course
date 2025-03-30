#include <stdlib.h>
#include "int2str.h"

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
	unsigned n = neg ? -number : number;

	int len = 0;
	unsigned tmp = n;
	while (tmp)
	{
		len++;
		tmp /= 10;
	}

	str = malloc(len + neg + 1);
	if (!str)
		return NULL;

	str[len + neg] = '\0';
	int i = len + neg - 1;

	while (n)
	{
		str[i--] = '0' + (n % 10);
		n /= 10;
	}

	if (neg)
		str[0] = '-';
	return str;
}