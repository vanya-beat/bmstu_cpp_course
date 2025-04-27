#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char* int2str(int number)
{
	
	if (number == -2147483648)
	{
		char* result = (char*)malloc(12);
		if (result == NULL)
			return NULL;
		sprintf(result, "-2147483648");
		return result;
	}

	
	if (number == 0)
	{
		char* result = (char*)malloc(2);
		if (result == NULL)
			return NULL;
		sprintf(result, "0");
		return result;
	}


	bool isNegative = false;
	if (number < 0)
	{
		isNegative = true;
		number = -number;
	}

	
	int len = 0;
	int temp = number;
	while (temp != 0)
	{
		temp /= 10;
		len++;
	}

	
	char* str = (char*)malloc(len + isNegative + 1);
	if (str == NULL)
		return NULL;


	int i = len + isNegative;
	str[i--] = '\0';

	
	do
	{
		str[i--] = (number % 10) + '0';
		number /= 10;
	} while (number > 0);

	
	if (isNegative)
	{
		str[0] = '-';
	}

	return str;
}
