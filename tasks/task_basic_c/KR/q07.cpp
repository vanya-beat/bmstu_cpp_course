#include<iostream>
using namespace std;
 /* 
Создать консольное приложение, которое выводит на экран дни недели в
зависимости от введенного числа (ввод параметра через аргументы командной
строки)
*/ 

int main (int argc, char *argv[])
{
     if (argc != 2) 
     {
            std::cerr << "Day Number parameter is not set " << std::endl;
            return 1;
     }
    int q = std::stoi(argv[1]);
    if (q<1 or q>7)
    {
        std::cerr << "Uncorrect Day Number!" << std::endl;
        return 1;   
    };
    switch (q) {
        case 1:
              std::cout << "Monday" << std::endl;
              break;
        case 2:
              std::cout << "Tuesday" << std::endl;
              break;
        case 3:
              std::cout << "Wednesday" << std::endl;
              break;
        case 4:
              std::cout << "Thursday" << std::endl;
              break;
        case 5:
              std::cout << "Friday" << std::endl;
              break;
        case 6:
              std::cout << "Saturday" << std::endl;
              break;
        case 7:
              std::cout << "Sunday" << std::endl;
              break;
        default:
              std::cout << " " << std::endl;
              break;
      };
    return 0;
}