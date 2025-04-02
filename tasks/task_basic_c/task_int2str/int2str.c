#include "int2str.h"
#include <assert.h>
#include "stdio.h"

static int count_digits(unsigned n)
{
	int c = !n;
	while (n)
	{
		c++;
		n /= 10;
	}
	return c;
}

char* int2str(int n)
{
	if (n == 0)
	{
		char* z = malloc(2);
		z[0] = '0';
		z[1] = '\0';
		return z;
	}

	int neg = n < 0;
	unsigned num = neg ? -n : n;
	int len = count_digits(num) + neg;
	char* s = malloc(len + 1);

	if (!s)
		return NULL;

	s[len] = '\0';
	int i = len - 1;

	do
	{
		s[i--] = '0' + num % 10;
		num /= 10;
	} while (num);

	if (neg)
		s[0] = '-';

	return s;
}