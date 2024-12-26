#include <iostream>
#include <string>
#include <math.h>
using namespace std;
/*
Написать шаблонную функцию sum , которая принимает два параметра
и возвращает их сумму. Продемонстрировать работу шаблонной функции на
примере сложения двух чисел разных типов и типа DummyMetric 
*/
struct DummyMetric {
    double a;
    double b;
};

int sum(const int x, const int y) {
    return x+y;
}
float sum(const float x, const float y) {
    return x+y;
}
double sum(const double x, const double y) {
    return x+y;
}
DummyMetric sum(const DummyMetric x, const DummyMetric y)
{
    return {x.a - y.a, x.b - y.b};
}

int main()
{
    int x1=3;
    int y1=4;
    float x2 = 1.11;
    float y2 = 2.22;
    double x3 = 100000;
    double y3 =2222222222;
    DummyMetric x4 = {1, 2};
    DummyMetric y4 = {3, 4};
    DummyMetric res = sum(x4, y4);
    cout << "int x = " << x1 << ", y = " << y1 << ", sum = " << sum(x1 ,y1) << std::endl; 
    cout << "float x = " << x2 << ", y = " << y2 << ", sum = " << sum(x2 ,y2) << std::endl;
    cout << "double x = " << x3 << ", y = " << y3 << ", sum = " << sum(x3 ,y3) << std::endl;
    cout << "DummyMetric x = {" << x4.a << ", " << x4.b << "}, y = {" << y4.a << ", " << y4.b << "}, sum = {" << res.a << ", " << res.b << "}" << std::endl;
    return 0;
}