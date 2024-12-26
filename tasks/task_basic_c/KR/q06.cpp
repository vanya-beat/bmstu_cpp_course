#include "string"
#include "locale.h"
#include<iostream>
using namespace std;
/* 
Создать стуктуру комлексного числа и реализовать
сложение, умножение, деление, вычитание этих чисел
*/ 
struct Complex {
    double real ;
    double imag ;
};
int main()
{
    Complex a = {1, 2};
    Complex b = {3, 4};
    Complex c, d , e , f;
    cout << "complex a = {" <<  a.real << " , " << a.imag << "}" << std::endl;
    cout << "complex b = {" <<  b.real << " , " << b.imag << "}" << std::endl;
    /* сложение */
    c.real=a.real+b.real;
    c.imag=a.imag+b.imag;
    cout << "complex c = (a + b) = {" <<  c.real << " , " << c.imag << "}" << std::endl;
    /* вычитание */
    d.real=a.real-b.real;
    d.imag=a.imag-b.imag;
    cout << "complex d = (a - b) = {" <<  d.real << " , " << d.imag << "}" << std::endl;
    /* умножение */
    e.real= (a.real*b.real)-(a.imag*b.imag);
    e.imag= (a.real*b.imag)+(a.imag*b.real); 
    cout << "complex e = (a * b) = {" <<  e.real << " , " << e.imag << "}" << std::endl;
    /* деление */
    f.real=(a.real*b.real + a.imag*b.imag) / (b.real*b.real + b.imag*b.imag);
    f.imag=(a.imag*b.real-a.real*b.imag) / (b.real*b.real + b.imag*b.imag);   
    cout << "complex f = (a / b) = {" <<  f.real << " , " << f.imag << "}" << std::endl;    
    return 0;
}