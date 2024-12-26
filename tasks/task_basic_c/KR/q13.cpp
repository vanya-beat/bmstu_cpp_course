#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>   
using namespace std;
/*
Необходимо сделать требуемые функции и объявить тип функции, 
чтобы этот код успешно компилировался и работал
*/
double add (double a, double b)           // операция сложения параметров
    {
        return a+b;    
    };
double subtract (double a, double b)      // операция вычитания параметров
    {
        return a-b;
    };
double multiply (double a, double b)      // операция умножения параметров
    {
        return a*b;    
    };
double divide (double a, double b)       // операция деления параметров
    {
        return a/b;
    };

typedef // указатель на функцию
    double // возвращаемое значение 
        (*Operation) 
            (const double, const double); // список параметров 

struct OperationEntry {
    char symbol; 
    Operation operation;
};  

int main () {
    double num1 , num2 ;
    num1 = 2;
    num2 = 5; 
 
 OperationEntry operations [] = {
        {'+', add},
        {'-', subtract },
        {'*', multiply },
        {'/', divide }
    };  
    cout << operations[0].symbol << " " << operations[0].operation(num1, num2) << endl ;  
    return 0;
}