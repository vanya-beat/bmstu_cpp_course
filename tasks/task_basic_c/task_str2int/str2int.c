#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include "str2int.h"

int str2int(const char* str)
{
	if (str == NULL || *str == '\0')
	{
		return 0;
	}

	int sign = 1;
	int i = 0;
	int number = 0;
	bool negative_overflow = false;
	bool positive_overflow = false;

	if (str[0] == '-')
	{
		sign = -1;
		i = 1;
	}

	for (; str[i] != '\0'; i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return 0;
		}

		int digit = str[i] - '0';

		if (sign == 1)
		{
			if (number > INT_MAX / 10 ||
				(number == INT_MAX / 10 && digit > INT_MAX % 10))
			{
				return 0;
			}
		}
		else
		{
			if (number > INT_MAX / 10 ||
				(number == INT_MAX / 10 && digit > (INT_MAX % 10 + 1)))
			{
				return 0;
			}
		}

		number = number * 10 + digit;
	}

	number = number * sign;

	return number;
}