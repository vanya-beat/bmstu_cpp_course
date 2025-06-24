#include <assert.h>
#include "str2int.h"
#include "stdio.h"

int str2int(const char *str) {
  int x = 1;
  int z = 0;
  if (*str == '-'){
    x = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }
  int count = 0;
  while (*str != '\0') {
    z = z * 10 + (*str - '0');
    count += 1;
    str++;
    assert((x == -1 && z == -2147483648) || (x == -1 && z >= 0 && count <= 9) || (x == 1 && z >= 0));
  }
  assert(count > 0);
  return z * x;
}
