#pragma once

#include <iostream>

namespace bmstu {
template<typename T>
class basic_string;  // объявление класс шаблонной строки

typedef basic_string<char> string;       // тип для обычных строк
typedef basic_string<wchar_t> wstring;   // тип для широких строк
typedef basic_string<char8_t> u8string; // тип для UTF-8 строк
typedef basic_string<char16_t> u16string; // тип для UTF-16 строк
typedef basic_string<char32_t> u32string; // тип для UTF-32 строк

template<typename T>
class basic_string {
  public: // NOLINT
    /// Конструктор по умолчанию
    basic_string() {
      size_ = 0;
      data_ = new T[1];
      data_[0] = 0; // инициализация нулевым символом
    }

    /// Конструктор с заданным размером
    explicit basic_string(size_t size) {
      size_ = size;
      data_ = new T[size_ + 1];
      for (size_t i = 0; i < size_; ++i) {
        data_[i] = ' '; // заполнение пробелами
      }
      data_[size_] = 0; // добавление нулевого символа в конец
    }

    /// Конструктор с инициализатором списка
    basic_string(std::initializer_list<T> list) {
      size_ = list.size();
      data_ = new T[size_ + 1];
      size_t i = 0;
      for (auto& item : list) {
        data_[i] = item; // копируем элементы из списка
        ++i;
      }
      data_[size_] = 0; // добавление нулевого символа
    }

    /// Конструктор из C-строки
    basic_string(const T* c_str) { // NOLINT
      size_ = len_(c_str);
      data_ = new T[size_ + 1];
      copy_data_(c_str); // копируем данные из C-строки
    }

    /// Копирующий конструктор
    basic_string(const basic_string& other) {
      size_ = other.size_;
      data_ = new T[size_ + 1];
      copy_data_(other.c_str()); // копируем данные из другой строки
    }

    /// Конструктор перемещения
    basic_string(basic_string&& dying) noexcept {
      data_ = dying.data_; // передаем управление ресурсами
      size_ = dying.size_;
      dying.data_ = nullptr; // обнуляем указатель
      dying.size_ = 0; // сбрасываем размер
    }

    /// Деструктор
    ~basic_string() {
      clean_(); // освобождаем ресурсы
    }

    /// Геттер для получения C-строки
    [[nodiscard]] const T* c_str() const {
      return (data_ != nullptr) ? data_ : reinterpret_cast<const T *>("");
    }

    /// Геттер для получения размера строки
    [[nodiscard]] size_t size() const {
      return size_;
    }

    /// Оператор копирующего присваивания
    basic_string& operator=(const basic_string& other) {
      if (*this == other) {
        return *this; // если строки идентичны, ничего не делаем
      } else {
        clean_(); // освобождаем старые ресурсы
        size_ = other.size_;
        data_ = new T[size_ + 1];
        copy_data_(other.c_str()); // копируем данные
        data_[size_] = 0; // добавление нулевого символа
        return *this;
      }
    }

    /// Оператор перемещающего присваивания
    basic_string& operator=(basic_string&& other) noexcept {
      if (this == &other) {
        return *this; // предотвращаем самоприсваивание
      } else {
        delete[] data_; // освобождаем старые ресурсы
        size_ = other.size_;
        data_ = other.data_;
        other.data_ = nullptr; // обнуляем указатель
        other.size_ = 0; // сбрасываем размер
        return *this;
      }
    }

    /// Оператор присваивания из C-строки
    basic_string& operator=(const T* c_str) {
      clean_(); // освобождаем старые ресурсы
      size_ = len_(c_str);
      data_ = new T[size_ + 1];
      for (size_t i = 0; i < size_; ++i) {
        data_[i] = c_str[i]; // копируем данные из C-строки
      }
      data_[size_] = 0; // добавление нулевого символа
      return *this;
    }

    /// Оператор конкатенации строк
    friend bmstu::basic_string<T> operator+(const basic_string& left,
                                            const basic_string& right) {
      basic_string result;
      result.size_ = left.size_ + right.size_;
      result.data_ = new T[result.size_ + 1];
      result.copy_data_(left.c_str()); // копируем данные первой строки
      for (size_t i = 0; i < right.size_; ++i) {
        result.data_[i + left.size_] = right.data_[i]; // добавляем данные второй строки
      }
      return result;
    }

    /// Оператор вывода в поток
    friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os,
                                             const basic_string& obj) {
      os << obj.c_str(); // выводим C-строку
      return os;
    }

    /// Оператор ввода из потока
    friend std::basic_istream<T>& operator>>(std::basic_istream<T>& is,
                                             basic_string& obj) {
      is >> std::noskipws;  // отключаем игнорирование пробелов
      T symbol = 1;
      for (size_t i = 0; symbol != 0; ++i) {
        symbol = 0; // сбрасываем символ
        is >> symbol; // читаем символ из потока
        if (symbol != 0) {
          obj += symbol; // добавляем символ в строку
        }
      }
      return is;
    }

    /// Оператор конкатенации с другой строкой
    basic_string& operator+=(const basic_string& other) {
      *this = (*this + other); // используем оператор +
      return *this;
    }

    /// Оператор конкатенации с символом
    basic_string& operator+=(const T symbol) {
      T* prev_data = data_;
      ++size_; // увеличиваем размер
      data_ = new T[size_ + 1]; // выделяем новую память
      copy_data_(prev_data); // копируем старые данные
      delete[] prev_data; // освобождаем старую память
      data_[size_ - 1] = symbol; // добавляем новый символ
      data_[size_] = 0; // добавление нулевого символа
      return *this;
    }

    // Оператор равенства двух строк
    bool operator==(const basic_string& input) const {
      if (input.size_ != size_) return false; // сравниваем размеры
      if (input.data_ == data_) return true; // если указатели равны
      for (size_t i = 0; i < input.size_; ++i) {
        if (data_[i] != input[i]) { return false; } // сравниваем символы
      }
      return true; // строки равны
    }

    /// Значение по индексу
    T& operator[](const size_t index) const {
      if (index <= size_) {
        return data_[index]; // возвращаем символ по индексу
      } else {
        throw std::out_of_range("Index out of range"); // выбрасываем исключение
      }
    }
    
    /// Приватные члены, не все могут их видеть...
  private: // NOLINT
    size_t size_ = 0; // размер строки
    T* data_ = nullptr; // указатель на данные строки

    /// Получаем длину C-строки
    static size_t len_(const T* str) {
      size_t len = 0;
      while (str[len] != 0) { ++len; } // считаем длину до нулевого символа
      return len;
    }

    /// Получаем длину другой строки
    static size_t len_(const basic_string& input) {
      return input.size_; // возвращаем размер строки
    }

    /// Освобождаем ресурсы строки
    void clean_() {
      size_ = 0; // сбрасываем размер
      delete[] data_; // освобождаем память
    }

    /// Копируем данные из C-строки
    void copy_data_(const T* str) {
      for (size_t i = 0; i < len_(str); ++i) {
        data_[i] = str[i]; // копируем данные
      }
      data_[size_] = 0; // добавление нулевого символа
    }
};
}  // namespace bmstu
