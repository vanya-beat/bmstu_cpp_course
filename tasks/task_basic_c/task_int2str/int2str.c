#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char* int2str(int number)
{
	if (number == -2147483648)
	{
		return "-2147483648";
	}
	bool isNegative = false;
	if (number == 0)
	{
		return "0";
	}
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
	str[len + isNegative] = '\0';
	for (int i = len + isNegative - 1; i >= isNegative; i--)
	{
		str[i] = (number % 10) + '0';
		number /= 10;
	}
	if (isNegative)
	{
		str[0] = '-';
	}
	return str;
}