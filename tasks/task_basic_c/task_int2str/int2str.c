#include <stdlib.h>

#include "int2str.h"

char* int2str(const int number)
{
	int size = 0;
	unsigned int temp_number = number;
	unsigned int ext_number;

	if (number < 0)
	{
		ext_number = -number;
		temp_number = -temp_number;
		size++;
	}
	else
	{
		ext_number = number;
	}

	do
	{
		size++;
		temp_number /= 10;
	} while (temp_number > 0);

	char* buf = malloc(size + 1);
	buf[size] = '\0';

	for (int i = size - 1; i >= 0; i--)
	{
		buf[i] = (ext_number % 10) + '0';
		ext_number /= 10;
	}

	if (number < 0)
	{
		buf[0] = '-';
	}
	return buf;
}
