#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>  
#include <string>

using namespace std;
/*
Написать программу, которая выводит содержимое
файла на экран. Имя файла вводится через аргументы командной строки. 
Обработать исключение, если файл не найден
*/
int main (int argc, char *argv[])
{
    if (argc < 2) 
    {
        std::cerr << "There are no parameters!" << std::endl;
        return 1;
    };
    std::string NameOfFile = argv[1];
    /* int num_lines; */
    std::ifstream input_file(NameOfFile);   // открытие файла для чтения
    if (input_file.is_open() )                   // проверка, что файл открыт
        {
            string s;
            while(getline(input_file, s))     // берем очередную строку в переменную s
            {
                cout << s << endl; // выводим на экран
                /* num_lines = num_lines+1; */
            }
        }
    else
    {
        std::cerr << "File is not found!" << std::endl;
        return 1;   
    };
    input_file.close(); // закрытие файла
    /* std::cout << "Lines in the file: " << num_lines << std::endl; // число строк в файле */ 
    return 0;
}