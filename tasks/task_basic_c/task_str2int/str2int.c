#include "str2int.h"
#include <assert.h>

#define INT_MAX_VALUE 2147483647
#define INT_MIN_VALUE (-2147483647 - 1)

int str2int(const char* s)
{
	if (!s || !*s)
		assert(0 && "Empty string");

	int sign = 1;
	if (*s == '+')
	{
		s++;
	}
	else if (*s == '-')
	{
		sign = -1;
		s++;
	}

	if (!*s)
		assert(0 && "No digits after sign");

	long long res = 0;
	while (*s >= '0' && *s <= '9')
	{
		res = res * 10 + (*s - '0');

		if ((sign == 1 && res > INT_MAX_VALUE) ||
			(sign == -1 && res > (long long)INT_MAX_VALUE + 1))
		{
			assert(0 && "Integer overflow");
		}

		s++;
	}

	if (*s != '\0')
		assert(0 && "Invalid characters");

	return (int)(res * sign);
}
//bmstuu