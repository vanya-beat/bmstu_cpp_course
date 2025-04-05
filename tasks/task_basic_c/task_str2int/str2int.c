#include <assert.h>

#include "str2int.h"

int str2int(const char* str)
{
	const int int_rng_mx = 2147483647;
	const int int_rng_mn = -2147483647;
	int result = 0;
	int sign = 1;
	int lenght = 0;
	if (*str == '+')
	{
		str++;
	}
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}

	while ((*str >= '0' && *str <= '9') && *str != '\0')
	{
		int number = *str - '0';
		if (sign == 1 && (result > (int_rng_mx - number) / 10))
		{
			assert(1 == 0);
		}
		if (sign == -1 && (result > (int_rng_mn - 1 - number) / 10))
		{
			assert(1 == 0);
		}
		result = result * 10 + number;
		str++;
		lenght++;
	}
	assert(lenght > 0);
	return result * sign;
}