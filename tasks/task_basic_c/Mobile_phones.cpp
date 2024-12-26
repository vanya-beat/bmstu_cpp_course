#include <iostream>
#include <vector>

struct MobilePhone {
    double weight;
    bool is_broken;
    double price;
};

int main() {
    int num_phones;
    std::cout <<"Enter the number of mobile phones";
    std::cin >> num_phones;

    std::vector<MobilePhone> phones(num_phones);

    for (int i = 0; i <num_phones; i++) {
        std::cout <<"enter the weight" <<i<<":";
        std::cin >> phones[i].weight >> phones[i].is_broken >> phones[i].price;
    }
    std::cout <<"MobilePhones" << std::endl;
    for (int i=0; i<num_phones; i++) {
        std::cout <<"MobilePhone id=\"" <<i<<"\" weight=\"" <<phones[i].weight << "\" is_broken=\"" << (phones[i].is_broken ? "true" : "false") << "\" price=\"" << phones[i].price << "\"/>" << std::endl;
    }
    std::cout << "</MobilePhones>" << std::endl;
    return 0;
}