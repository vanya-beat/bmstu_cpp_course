#include <iostream>
typedef unsigned long long uint64_t;

constexpr uint64_t factorial(uint64_t n) {
    if (n==0 | n==1) {
        return 1;
    } else {
        return n * factorial(n-1);
    }
}
int main(int argc, char *argv[]) {
    constexpr uint64_t a = factorial(19);
    constexpr uint64_t b = factorial(18);
    constexpr uint64_t c = factorial(16);
    constexpr uint64_t d = factorial(14);
    constexpr uint64_t e = factorial(12);
    std::cout << a << "" << b << "" << std::endl;
    return e+d+c+b+a;
}
