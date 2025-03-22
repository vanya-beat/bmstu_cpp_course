#include "int2str.h"
#include <assert.h>
#include <stdlib.h>

char* int2str(int number)
{
	char* str = malloc(12);
	int i = 0;
	unsigned int n = number;
	if (number < 0)
	{
		n = -number;
	}
	while (n > 0)
	{
		str[i++] = n % 10 + '0';
		n /= 10;
	}
	if (number < 0)
	{
		str[i++] = '-';
	}
	if (number == 0)
	{
		str[0] = '0';
		i++;
	}
	str[i] = '\0';
	for (int j = 0, k = i - 1; j < k; j++, k--)
	{
		char temp = str[j];
		str[j] = str[k];
		str[k] = temp;
	}
	return str;
}
