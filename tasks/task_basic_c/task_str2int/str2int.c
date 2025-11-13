#include <assert.h>
#include "str2int.h"

int str2int(const char *str) {
    int sign = 1;
    int result = 0;

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    if (*str == '\0') {
        assert(0);
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            assert(0);
        }

        if (result > 214748364) {
            assert(0);
        }

        result = result * 10 + (*str - '0');

        if (sign == 1 && result < 0) {
            assert(0);
        }
        if (sign == -1 && result < 0 && result != -2147483648) {
            assert(0);
        }

        str++;
    }

    result *= sign;

    return result;
}