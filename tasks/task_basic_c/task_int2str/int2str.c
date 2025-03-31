#include <stdlib.h>
#include "int2str.h"

char* int2str(int n)
{
	char* buf = (char*)calloc(13, 1);
	int len = 0;

	unsigned int u = 0;
	if (n < 0)
	{
		len = 1;
		buf[0] = '-';
		u = -n;
	}
	else
	{
		u = n;
	}

	unsigned int tmp = u;
	int cnt = 0;
	do
	{
		tmp /= 10;
		len++;
		cnt++;
	} while (tmp > 0 || cnt < 1);

	int d = 0;
	cnt = 0;
	char c = 0;
	do
	{
		d = u % 10;
		u /= 10;
		c = d + 48;
		buf[len - 1 - cnt] = c;
		cnt++;
	} while (u > 0 || cnt < 1);
	buf[len] = 0;

	return buf;
}