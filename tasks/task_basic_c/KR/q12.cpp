#include <iostream>
#include <string>
using namespace std;
/* 
Определить, является ли строка палиндромом. 
Строка вводится через аргументы командной строки
*/
int main (int argc, char *argv[])
{
    if (argc < 2) 
    {
        std::cerr << "There are no parameters!" << std::endl;
        return 1;
    };
    std::string Stroka = argv[1];
    int Len=Stroka.length();  /* длина строки */
    bool IsPalindrom = true;
    for (int i=0; i<Len/2; i++)
    {
        if (Stroka[i]!=Stroka[Len-i-1]) /* если симметричные символы не совпадают, то строка не палиндром */
        {
            IsPalindrom = false;
        };
    };
    if (IsPalindrom)
    {
        std::cout << "String is palindrom." << std::endl;
    }
    else
    {
        std::cout << "String is not palindrom." << std::endl;
    };
    return 0;
}