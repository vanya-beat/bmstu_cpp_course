#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
dr#include <cmath>

struct Student {
    uint8_t age;
    bool in_vacation;
    bool is_fired;
    bool has_depts;
    uint8_t group_number;
    uint8_t rating;
    std::string name;

    friend std::ostream &operator<<(std::ostream &os, const Student &student) {
        os << std::boolalpha << "Student(age=" << int(student.age) << ", "
                << "in_vacation=" << student.in_vacation << ", "
                << "is_fired=" << student.is_fired << ", "
                << "has_depts=" << student.has_depts << ", "
                << "group_number=" << int(student.group_number) << ", "
                << "name=" << student.name << ")";
        return os;
    }

    static Student fill_from_specific_stream(std::istream &is) {
        Student student;
        is.read(reinterpret_cast<char *>(&student.age), sizeof(uint8_t));
        uint8_t composite_byte;
        is.read(reinterpret_cast<char *>(&composite_byte), sizeof(uint8_t));
        /// [in_vacation][is_fired][has_depts] [][][][][] group number
        student.in_vacation = (composite_byte & 0b1000000) != 0;
        student.is_fired = (composite_byte & 0b01000000) != 0;
        student.has_depts = (composite_byte & 0b00100000) != 0;
        student.group_number = (composite_byte & 0b00011111) != 0;
        is.read(reinterpret_cast<char *>(&student.rating), sizeof(uint8_t));
        //read bytes to std::string while not 0x00
        char ch;
        while (is.get(ch) && ch != 0x00) {
            student.name.push_back(ch);
        }
        return student;
    }
};

bool less_age_student(const Student &left, const Student &right) {
    return left.age < right.age && left.name < right.name;
}

bool greater_student(const Student &left, const Student &right) {
    return left.rating > right.rating;
}

bool rating(const Student &left, const Student &right) {
    return left.rating > right.rating;
}
template<typename Comp>
void sort_by_comp(std::vector<Student> &students, Comp comp) {
    std::sort(students.begin(), students.end(), comp);
}

int main(int argc, char *argv[]) {
    // usage <path_to_file> <mode>
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file " << argv[1] << std::endl;
        return 2;
    }
    std::vector<Student> students;
    while (!input_file.eof()) {
        students.push_back(Student::fill_from_specific_stream(input_file));
    }

    input_file.close();
    sort_by_comp(students, rating);
    /// print top 15

    for (int i = 0; i < std::min(students.size(), size_t(15)); i++) {
        std::cout << students[i] << std::endl;
    }


    return 0;
}