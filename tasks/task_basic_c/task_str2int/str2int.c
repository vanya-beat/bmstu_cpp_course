#include "str2int.h"
#include <assert.h>
#include "stdio.h"

int str2int(const char* str)
{
	int sign = 1;

	int result = 0;

	int count = 0;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
	{
		str++;
	}

	while (*str != '\0')
	{
		result = result * 10 + (*str - '0');
		count++;
		str++;
		assert(sign == -1 && result == -2147483648 ||
			   sign == -1 && result >= 0 && count <= 9 ||
			   sign == 1 && result >= 0);
	}
	assert(count > 0);
	return result * sign;
}