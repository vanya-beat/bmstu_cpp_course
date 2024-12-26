#include <iostream>
#include <fstream>
#include <cstdlib>  
#include <string>
#include<sstream>

using namespace std;
/*
Напишите программу, которая принимает через аргументы
командной строки путь до файла и выводит количество строк в файле и
количество слов.
*/
int main (int argc, char *argv[])
{
    if (argc < 2) 
    {
        std::cerr << "There are no parameters!" << std::endl;
        return 1;
    };
    std::string NameOfFile = argv[1];
    int num_lines =0;  // количество строк 
    int num_words=0;  // количество слов  
    std::ifstream input_file(NameOfFile);   // открытие файла для чтения 
    if (input_file.is_open() )              // проверка, что файл открыт
        {
            string s;       // строка файла
            while(getline(input_file, s))   // берем очередную стоку в переменную s
            {
                std::string word;           // слово
                istringstream ss(s);
                while(ss >> word)            // цикл по извлечепнию слов из строки
                {               
                    num_words++;            // счетчик слов
                    /* std::cout << " " << word ;*/ 
                }
                num_lines++;                // счетчик строк 
            }
        }
    else
    {
        std::cerr << "File is not found!" << std::endl;
        return 1;   
    };
    input_file.close(); // закрытие файла
    std::cout << "Words in the file: " << num_words << ", number of lines: " << num_lines << ::endl; // число слов и строк в файле 
    return 0;
}