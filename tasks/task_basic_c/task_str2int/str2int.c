#include <assert.h>
#include "stdio.h"
#include "str2int.h"

int str2int(const char* s)
{
	int m = 1;

	int r = 0;

	if (*s == '-')
	{
		m = -1;
		s++;
	}
	else if (*s == '+')
	{
		s++;
	}

	int n = 0;
	while (*s)
	{
		r = r * 10 + (*s - '0');
		n++;
		s++;
		assert(m == -1 && r == -2147483648 || m == -1 && r >= 0 && n <= 9 ||
			   m == 1 && r >= 0);
	}
	assert(n > 0);
	return r * m;
}