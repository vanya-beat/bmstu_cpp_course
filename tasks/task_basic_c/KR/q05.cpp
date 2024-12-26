#include<iostream>
using namespace std;
/* 
консольное приложение, которое переводит
аргумент командной строки в формате 16-ричного числа 0xABCD0123 в 10-е число
*/ 
int main(int argc, char *argv[]) 
{
    string hexNumber;
    if (argc != 2) 
        {
            std::cout << "No parameters is  set, use default value = 0xABCD0123" << std::endl; /* если агрумент не задан, то берем значение по умолчанию*/
            hexNumber = "0xABCD0123";    
        } 
        else
        {
            hexNumber = argv[1];
        } ;    
    int base = 16;
    long long rr = stoull(hexNumber, 0, base);
    cout << "hexNumber = " <<  hexNumber << " , Result = " << rr << endl; 
    return 0;
}