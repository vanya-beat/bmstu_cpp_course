#include <assert.h>
#include "str2int.h"
#include "stdio.h"

int str2int(const char *str) {
  int sign = 1;
  int number = 0;
  
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }
  
  int co = 0;
  while (*str != '\0') {
    number = number * 10 + (*str - '0');
    co += 1;
    str++;
    assert(sign == -1 && number == -2147483648 || sign == -1 && number >= 0 && co <= 9 || sign == 1 && number >= 0);
  }
  
  assert(co > 0);
  return number * sign;
}
