#include <assert.h>
#include "str2int.h"
#include <stdio.h>
#define int_MAX 2147483647
#define int_MIN (-2147483647-1)

int str2int(const char *str) {
  //assert(str != NULL);
  int result = 0;
  int sign = 1;
  int lenght = 0;

  while(*str == ' ') {
    str++;
  }

  if (*str == '-' || *str == '+') {
    sign = (*str == '-') ? -1 : 1;
    str++;
  }

  while (*str >= '0' && *str <= '9') {
    lenght++;
    int digit = *str - 0x30;
    if (sign == 1 && (result > (int_MAX - digit) / 10)) {
      assert(1 == 0);
    }
    if (sign == -1 && (result > (int_MIN - digit) / 10)) {
      assert(1 == 0);
    }
    result = result * 10 + digit;
    str++;
  }
  assert(lenght > 0);

  return result * sign;
}
