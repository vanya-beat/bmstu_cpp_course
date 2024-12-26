#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

/*namespace bmstu {*/
    class String {
    public:
        /// Конструктор по умолчанию
        String() = default;
        /// Конструктор с параметром "cи строкой"
        String(const char *c_str) {
            ptr_ = new char[size_ + 1];
            std::copy(c_str, c_str + size_, ptr_);
            ptr_[size_] = '\0';
        }
        /// Копирующий конструктор
        String(const String& other) : ptr_(new char[other.size_ + 1]), size_(other.size_) {
            std::copy(other.ptr_, other.ptr_ + other.size_, ptr_);
            ptr_[size_] = '\0';
        }
        /// Конструктор перемещения
        String(String&& dying) noexcept {
            *this = std::move(dying);
        }
        /// Деструктор
        ~String() {
            delete[] ptr_;
        }
        /// Геттер на си-строку
        const char *c_str() const { 
            return ptr_;
        }
        /// Геттер на размер
        size_t size() const {
            return size_;
        }
        /// Оператор копирующего присваивания
         String& operator=(const String& other) {
                if (this == &other) {
                return *this;
                }
            String s = other;
            std::swap(*this, s);
            return  *this;
        }
        /// Оператор перемещающего присваивания
        String& operator=(String&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            std::swap(ptr_, other.ptr_);
            std::swap(size_, other.size_);
            return *this;
        }
        /// Оператор присваивания си строки
        String& operator=(const char* c_str) {
            return *this;
        }
        /*Все все понимают*/
        friend String operator+(const String& left, const String& right) {
            String ts = left;
            return ts += right;
        }
        friend std::ostream &operator<<(std::ostream& os, const String& obj) {
            return os << obj.ptr_;
        }
        friend std::istream &operator>>(std::istream& is, const String& obj) {
            return is >> obj.ptr_;
        
        /* const char *o;
            String stri;
            std::cin >> stri;
            o = stri.c_str(); 
            std::copy(obj.ptr_, o);
            return is;*/
        }
        String& operator+=(const String& other) {
            char* new_str = new char[size_ + other.size_ + 1];
            std::copy(ptr_, ptr_ + size_, new_str);
            std::copy(other.ptr_, other.ptr_ + other.size_ + 1, new_str + size_);
            delete[] ptr_;
            ptr_ = new_str;
            size_ += other.size_;
            return *this;
        }
        String& operator+=(const char symbol) {
            char* new_str = new char[size_ + 2];
            new_str[size_+1] = symbol;
            ptr_ = new_str;
            size_ += 1;
            return *this;      
        }
        char &operator[](size_t index) {
        
            return ptr_[index];
        }



    static    
     /// приватные хелперы
        int strlen_(const char* str) {
            int result = 0;
            while (*str != '\0') {
                str++;
                result++;
            }
            std::cout << "Длина строки**" << result  << std::endl;
            return result;
        }  
        void clean_() {
            String obj;
        /*
            std::cout << " " << std::endl;*/
            return; 
        }      

        /// Поля класса
        char *ptr_ = nullptr;
        int size_;

    };


int main ()
{
    String obj;

    String a, b, c;  
    a="Tokmakov";
    b="Andrey";
    /*std::cout << "Input c:";
    std::cin >> c;*/


    int lenstr1, lenstr2;
    /*lenstr1 = String::strlen_(a);*/
    /*lenstr2 = b.strlen_;*/
    /*std::cout << "Длина строки: "  <<  a.strlen_ << std::endl;*/
    /*std::cout << a+b << std::endl;*/
    return 0;
}