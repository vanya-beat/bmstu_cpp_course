#include <assert.h>
#include "stdio.h"
#include "str2int.h"

int str2int(const char* str)
{
	int sign = 1;
	int res = 0;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
	{
		str++;
	}

	int count = 0;
	while (*str != '\0')
	{
		res = res * 10 + (*str - '0');
		count += 1;
		str++;
		assert(sign == -1 && res == -2147483648 ||
			   sign == -1 && res >= 0 && count <= 9 || sign == 1 && res >= 0);
	}
	assert(count > 0);
	return res * sign;
}