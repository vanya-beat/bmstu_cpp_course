#include <iostream>
#include <cstring>

class String {
private:
    char* string_;
    int size;

public:
    // Конструктор по умолчанию
    String() {
        size = 1;
        string_ = new char[1];
        string_[0] = 0;
    }

    // Конструктор с произвольным размером
    String(int length) {
        size = length;
        string_ = new char[size + 1];
        std::fill(string_, string_ + size, '\0');
        //std::memset(string_, 0, sizeof(char) * (length + 1));   
    }

    // Конструктор с инициализацией от пользователя
    String(const char* input) {
        size = strlen(input);
        string_ = new char[size + 1];
        for (int i = 0; i < size; i++)
        {
            string_[i] = input[i];
        }
        string_[size] = '\0';
    }

    // Метод для ввода строки с клавиатуры
    void input() {
        std::cout << "Введите строку: ";
        std::cin.getline(string_, size + 1);
    }

    // Метод для вывода строки на экран
    void output() {
        std::cout << string_ << std::endl;
    }

    // Деструктор
    ~String() {
        delete[] string_;
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    String defaultString; 
    defaultString.input();
    defaultString.output();

    String customString(10); 
    customString.input(); // Ввод строки с клавиатуры
    customString.output();
    

    String userString("Привет, мир!"); // Создание строки с инициализацией от пользователя  
    userString.output();     

    return 0;
}