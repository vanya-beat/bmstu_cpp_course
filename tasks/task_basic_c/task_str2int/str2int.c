#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "str2int.h"

int str2int(const char* str)
{
	if (str == NULL || *str == '\0')
	{
		return 0;
	}

	int sign = 1;
	long long result = 0;
	int i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result = result * 10 + (str[i] - '0');

			if ((sign == 1 && result > INT_MAX) ||
				(sign == -1 && result > (long long)INT_MAX + 1))
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	result *= sign;

	if (result < INT_MIN || result > INT_MAX)
	{
		return 0;
	}

	return (int)result;
}