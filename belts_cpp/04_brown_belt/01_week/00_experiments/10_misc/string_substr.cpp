#include <iostream>
#include <string>

int main() {
    // std::string raw_name = "<xml_root>";
    // std::cout << "raw_name = " << raw_name << std::endl;

    // std::string name = raw_name.substr(1, raw_name.size() - 2);
    // std::cout << "name = " << name << std::endl;

    std::string name;
    int id;

    std::cout << "Enter id: ";
    std::cin >> id;

    std::cout << "Enter name: ";
    // std::cin >> name;
    std::getline(std::cin, name);

    std::cout << "Id: " << id << std::endl
              << "Name: " << name << std::endl;


    return 0;
}
