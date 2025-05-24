#include "int2str.h"
#include <assert.h>
#include "stdio.h"
#include <iostream>
#include <cstdlib>

static int length_number (const unsigned int number) {
	int length = 0;
    unsigned int nn = number;
	do {
		length++;
		nn /= 10;
	} while (nn > 0);
	return length;
}

char* int2str(int number)
{
    if (number == 0) {
        char* str = "0";
        return str;
    }
    bool is_negative = false;
    if (number < 0)
    {
        is_negative=true;   
    } 
    unsigned int newnumber = is_negative ? number*(-1) : number;
    int num_length = length_number(newnumber);
    int str_length = num_length + (is_negative ? 1 : 0) + 1;
    char* str = (char*)calloc(str_length, sizeof(char));
    if (str == NULL) {
		return NULL;
	}
	
  unsigned int k;
    for (int i = num_length; i >= 0; i--) {
        k = newnumber % 10;
        switch (k)
        {
        case 1:
            str[is_negative ? i  : i-1] = '1';
            break;
        case 2:
            str[is_negative ? i  : i-1] = '2';
            break;
        case 3:
            str[is_negative ? i  : i-1] = '3';
            break;
        case 4:
            str[is_negative ? i  : i-1] = '4';
            break;
        case 5:
            str[is_negative ? i  : i-1] = '5';
            break;
        case 6:
            str[is_negative ? i  : i-1] = '6';
            break;
        case 7:
            str[is_negative ? i  : i-1] = '7';
            break;
        case 8:
            str[is_negative ? i  : i-1] = '8';
            break;
        case 9:
            str[is_negative ? i  : i-1] = '9';
            break;
        case 0:
            str[is_negative ? i  : i-1] = '0';
            break;
        }
		newnumber /= 10;
	}
	str[str_length - 1] = '\0';
  	if (is_negative) {
		str[0] = '-';
	}
    return str;
}
