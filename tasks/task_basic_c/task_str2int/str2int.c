#include <assert.h>
#include "str2int.h"
#include "stdio.h"

int str2int(const char *str) {
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }

    if (*str == '+') {
        str++;
    }

    int intgr = 0;
    int count = 0;

    while (*str != '\0') {
        intgr = intgr * 10 + (*str - *"0");
        count += 1;
        str++;
        assert(sign == -1 && intgr == -2147483648 || sign == -1 && intgr >= 0 && count <= 9 || sign == 1 && intgr >= 0);
    }
    assert(count > 0);
    return sign * intgr;
}
