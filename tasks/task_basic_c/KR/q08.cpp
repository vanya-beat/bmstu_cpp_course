#include <iostream>
#include <fstream>
#include <string>
using namespace std;
 /* 
Создать консольное приложение, которое выводит
на экран все аргументы командной строки в обратном порядке
*/ 
int main (int argc, char *argv[])
{
     if (argc < 2) 
     {
            std::cerr << "There are no parameters!" << std::endl;
            return 1;
     };
    std::cout << "Arguments Quantity = " << argc-1 << std::endl; 
    for (int i=argc-1;i>0; i--){
        std::string s=argv[i];
        std::cout << s << " ";
    };
    return 0;
}